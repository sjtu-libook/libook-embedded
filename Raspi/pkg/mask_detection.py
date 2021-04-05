# Your imports

class MaskDetector():
    ''' mask detection and temperature display '''

    # TODO @initializer-z 朱祥辉

    def __init__(self, stm32):
        self.stm32 = stm32

    def start(self):
        ''' Start mask detection and temperature display, which serves as the main function of this class '''
        pass

    def end(self):
        ''' End mask detection and temperature display '''
        pass

    # 注意：
    # 1. 主控程序分配一个线程用于 MaskDetector.start() 函数
    # 2. 获取温度可以调用 self.stm32.get_temperature() 返回一个浮点数，详见 communication.py - CommunicateSTM32.get_temperature() by Yang Li
    # 3. 可以在这个类中再定义其它函数
