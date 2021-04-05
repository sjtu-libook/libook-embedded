import requests
from urllib.parse import quote
from datetime import datetime, timedelta


class CommunicateServer():
    ''' API of HTTP GET/POST Communication between Raspi and Server '''

    def __init__(self, url_base, api_key, device_id):
        self.url_base = url_base
        self.api_key = api_key
        self.device_id = device_id
        self.headers = {'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; WOW64) '
                        'AppleWebKit/537.36 (KHTML, like Gecko) '
                        'Chrome/53.0.2785.104 '
                        'Safari/537.36 Core/1.53.4843.400 '
                        'QQBrowser/9.7.13021.400'}

    def get(self):
        ''' Get reservation data of the current hour and device'''

        from_time = datetime.now() - timedelta(hours=1)
        to_time = datetime.now() + timedelta(hours=1)

        from_time = quote(from_time.astimezone().isoformat(), 'utf-8')
        to_time = quote(to_time.astimezone().isoformat(), 'utf-8')
        url = f'{self.url_base}/api/devices/{self.device_id}?api_key={self.api_key}&fake=false&from_time={from_time}&to_time={to_time}'

        try:
            response = requests.get(url, headers=self.headers)
            length = len(response.json())
            if length == 0:
                return {'message': 'No Reservation!'}
            elif length != 1:
                return {'message': 'Get Wrong Data From Server!'}
            reservation_id = response.json()[0]['id']
            user_id = response.json()[0]['user']['id']
            return {'message': 'Success', 'reservation_id': reservation_id, 'user_id': user_id}

        except requests.exceptions.ConnectionError:
            return {'message': 'HTTP Connection Error!'}

    def post(self, fingerprint_id, token, reservation_id, user_id):
        ''' Post user input to the server '''

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
            else
            return False
        except:
            return False


class CommunicateSTM32():
    ''' API of Serial Communication between Raspi and STM32 '''

    # TODO @yangco-le 郦洋

    def __init__(self):
        pass

    def get_token(self):
        ''' Return (True, token) if the user is seated for the first time, and return (False, '') if not '''
        # 注意
        # 1. 当用户没有落座（STM32上没有操作时），这个函数应该保持阻塞，直到用户有所操作（比如输入 token 或验证指纹后）
        # 2. 如果用户首次落座，返回一个二元组，例如 (True, '123456')
        # 3. 如果用户非首次落座，也返回一个二元组，例如 (False, '')，元组第二项为空字符串
        # 4. 树莓派主控程序将直接调用该函数获取用户的输入
        pass
        return True, '123456'

    def get_fingerprint_id(self):
        ''' Return fingerprint id created by STM32 '''
        # 注意：
        # 1. 如果用户首次落座，需要等待 STM32 录入指纹，然后返回新创建的指纹 ID
        # 2. 如果用户非首次落座，需要返回验证成功的指纹 ID
        # 3. 此函数也需要阻塞，直到用户完成操作
        pass
        return 1

    def get_temperature(self):
        ''' Return temperature collected by STM32 '''
        # 注意：
        # 1. 获取 STM32 发来的温度数据（浮点数）
        # 2. 树莓派总控程序会以一定频率调用该函数
        pass
        return 36.0

    def is_leave(self):
        ''' Return True if the user is going to leave '''
        # 注意：
        # 1. 如果用户主动希望离开（点击 STM32 某个按键），此函数返回 True
        # 2. 否则应该始终返回 False
        pass
        return False
