import cv2
import numpy as np
import keras
import keras.backend as k
from keras.layers import Conv2D, MaxPooling2D, SpatialDropout2D, Flatten, Dropout, Dense
from keras.models import Sequential, load_model
from keras.optimizers import Adam
from keras.preprocessing import image


class MaskDetector():
    ''' mask detection and temperature display '''

    def __init__(self, stm32):
        '''
            stm32: CommunicateSTM32()
        '''
        self.stm32 = stm32
        self.face_cascade = cv2.CascadeClassifier(
            'model/haarcascade_frontalface_default.xml')
        self.cap = cv2.VideoCapture(0)
        self.mymodel = load_model('model/mask_detection_model.h5')
        self.on = False

    def seton(self):
        '''
            set the switch on before start()
        '''
        self.on = True

    def start(self):
        ''' Start mask detection and temperature display, which serves as the main function of this class '''
        while self.on and self.cap.isOpened():
            temperature = self.stm32.get_temperature()

            # photos, my_face_encodings = faceencoding()
            _, frame = self.cap.read()
            frame = cv2.flip(frame, 1)
            gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
            face = self.face_cascade.detectMultiScale(
                frame, scaleFactor=1.1, minNeighbors=4)
            for (x, y, w, h) in face:
                face_img = frame[y:y + h, x:x + w]
                #target_size = (150, 150)
                #test_image = cv2.resize(face_img, target_size)
                cv2.imwrite('temp.jpg', face_img)
                test_image = image.load_img(
                    'temp.jpg', target_size=(150, 150, 3))
                test_image = image.img_to_array(test_image)
                test_image = np.expand_dims(test_image, axis=0)
                pred = self.mymodel.predict_classes(test_image)[0][0]
                if pred == 1:
                    cv2.putText(frame, 'NO MASK', ((x + w) // 2, y + h + 20),
                                cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 3)
                else:
                    cv2.rectangle(
                        frame, (x, y), (x + w, y + h), (0, 255, 0), 3)
                    cv2.putText(frame, 'MASK', ((x + w) // 2, y + h + 20),
                                cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 3)
            cv2.putText(frame, 'Temperature: ' + str(temperature),
                        ((x + w) // 2, y + h + 50), cv2.FONT_HERSHEY_SIMPLEX,
                        0.7, (128, 128, 0), 2)
            cv2.imshow("Recognition", frame)
            key = cv2.waitKey(6) & 0xFF
            if key == 27:
                break
        if self.cap.isOpened():
            self.cap.release()
            cv2.destroyAllWindows()

    def end(self):
        ''' End mask detection and temperature display '''
        self.on = False

    # 注意：
    # 1. 主控程序分配一个线程用于 MaskDetector.start() 函数
    # 2. 获取温度可以调用 self.stm32.get_temperature() 返回一个浮点数，详见 communication.py - CommunicateSTM32.get_temperature() by Yang Li
    # 3. 可以在这个类中再定义其它函数
