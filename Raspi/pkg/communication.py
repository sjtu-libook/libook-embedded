import requests
from urllib.parse import quote
from datetime import datetime, timedelta
import serial
import asyncio


class CommunicateServer():
    ''' API of HTTP GET/POST Communication between Raspi and Server '''

    def __init__(self, url_base, api_key, device_id):
        self.url_base = url_base
        self.api_key = api_key
        self.device_id = device_id

    def get(self):
        ''' Get reservation data of the current hour and device'''

        from_time = datetime.now() - timedelta(hours=1)
        to_time = datetime.now() + timedelta(hours=1)

        # from_time = quote(from_time.astimezone().isoformat(), 'utf-8')
        # to_time = quote(to_time.astimezone().isoformat(), 'utf-8')
        # url = f'{self.url_base}/api/devices/{self.device_id}?api_key={self.api_key}&fake=false&from_time={from_time}&to_time={to_time}'

        url = f'{self.url_base}/api/devices/{self.device_id}'
        params = {'api_key': self.api_key, 'fake': False,
                  'from_time': from_time, 'to_time': to_time}

        try:
            # response = requests.get(url)

            response = requests.get(url, params=params)
            data = response.json()
            length = len(data)
            if length == 0:
                return {'message': 'No Reservation!'}
            elif length != 1:
                # Comment from @skyzh:
                # length 可能不为 1。同一个区域可能有多个人预约。但我们可以暂时不处理这种情况，或者只按照预约 ID 最小的人（首个预约的人）进行之后的流程。
                return {'message': 'Get Wrong Data From Server!'}
            reservation_id = data[0]['id']
            user_id = data[0]['user']['id']
            return {'message': 'Success', 'reservation_id': reservation_id, 'user_id': user_id}

        except requests.exceptions.ConnectionError:
            return {'message': 'HTTP Connection Error!'}

    def post(self, fingerprint_id, token, reservation_id, user_id):
        ''' Post user input to the server '''

        # Comment from @skyzh:
        # 这里建议分成两个函数：一个用来更新用户信息，一个用来确认用户的预约。前者需要 user_id, token, fingerprint_id，后者只需要 reservation_id。
        url = f'{self.url_base}/api/devices/{self.device_id}'
        if token:
            params = {'fingerprint_id': fingerprint_id, 'token': token,
                      'reservation_id': reservation_id, 'user_id': user_id, 'api_key': self.api_key}
        else:
            params = {'fingerprint_id': fingerprint_id, 'reservation_id': reservation_id,
                      'user_id': user_id, 'api_key': self.api_key}

        try:
            response = requests.post(url, data=params)
            if response.status_code == 200:
                return True
            else:
                return False
        except requests.exceptions.ConnectionError:
            return False


class CommunicateSTM32():
    ''' API of Serial Communication between Raspi and STM32 '''

    # TODO @yangco-le 郦洋
    # Comment from @skyzh:
    # pyserial 支持 async / await，可以评估一下有没有可能把整个程序的架构都换成 async / await 的模式，可以避免多线程产生的各种问题（
    # https://github.com/pyserial/pyserial-asyncio

    def __init__(self):
        self.PORT = 'com3'  # 串口号
        self.BAUDRATE = 115200  # 波特率
        self.open_ser()

    def open_ser(self):
        ''' Open the serial '''
        try:
            global ser
            ser = serial.Serial(self.PORT, self.BAUDRATE, timeout=2)
            if(ser.isOpen()==True):
                print("串口打开成功")
        except Exception as exc:
            print("串口打开异常",exc)

    def send_msg(self, send_data):
        ''' Send message '''
        try:
            ser.write(send_data.encode("gbk"))
            print("已发送数据:",send_data)
        except Exception as exc:
            print("发送异常", exc)
            
    def read_msg(self):
        ''' Recieve message '''
        try:
            print("等待接收数据")
            while True:
                data = ser.read(ser.in_waiting).decode('gbk')
                if data != '':
                    break
            print("已接受到数据:", data)
            return data
        except Exception as exc:
            print("读取异常",exc)
            return
            
    def close_ser(self):
        ''' Close the serial '''
        try:
            ser.close()
            if ser.isOpen():
                print("串口未关闭")
            else:
                print("串口已关闭")
        except Exception as exc:
            print("串口关闭异常", exc)

    async def get_token(self):
        ''' Return (True, token) if the user is seated for the first time, and return (False, '') if not '''
        # 注意
        # 1. 当用户没有落座（STM32上没有操作时），这个函数应该保持阻塞，直到用户有所操作（比如输入 token 或验证指纹后）
        # 2. 如果用户首次落座，返回一个二元组，例如 (True, '123456')
        # 3. 如果用户非首次落座，也返回一个二元组，例如 (False, '')，元组第二项为空字符串
        # 4. 树莓派主控程序将直接调用该函数获取用户的输入
        while True:
            self.send_msg('req00')
            msg = self.read_msg()
            # message格式： 1. "True{tocken}" 首次落座   2. "False" 非首次落座  3. "Null" 未落座
            if msg == 'Null':
                await asyncio.sleep(3)
            elif msg == 'False':
                return False, ''
            elif msg[0: 5] == 'True':
                return True, msg[5:]

    async def get_fingerprint_id(self):
        ''' Return fingerprint id created by STM32 '''
        # 注意：
        # 1. 如果用户首次落座，需要等待 STM32 录入指纹，然后返回新创建的指纹 ID
        # 2. 如果用户非首次落座，需要返回验证成功的指纹 ID
        # 3. 此函数也需要阻塞，直到用户完成操作
        while True:
            self.send_msg('req01')
            msg = self.read_msg()
            # message格式： 1. "Suc{id}" 返回ID  2. "Null" ID尚未准备
            if msg == 'Null':
                await asyncio.sleep(3)
            elif msg[0: 3] == 'Suc':
                return int(msg[3:])

    async def get_temperature(self):
        ''' Return temperature collected by STM32 '''
        # 注意：
        # 1. 获取 STM32 发来的温度数据（浮点数）
        # 2. 树莓派总控程序会以一定频率调用该函数
        self.send_msg('req10')
        msg = self.read_msg()
        # message格式: "Temp{t}"
        if msg[0: 5] != 'Temp':
            print("返回温度异常")
            return
        else:
            return float(msg[5:])

    async def is_leave(self):
        ''' Return True if the user is going to leave '''
        # 注意：
        # 1. 如果用户主动希望离开（点击 STM32 某个按键），此函数返回 True
        # 2. 否则应该始终返回 False
        self.send_msg('req11')
        msg = self.read_msg()
        # message格式: "True" "False"
        if msg == 'True':
            return True
        elif msg == 'False':
            return False
        else:
            print("返回状态异常")
            return 
