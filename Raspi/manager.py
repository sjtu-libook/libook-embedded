import argparse
from datetime import datetime, timedelta
from threading import Thread

from pkg.communication import CommunicateSTM32, CommunicateServer
from pkg.mask_detection import MaskDetector

parser = argparse.ArgumentParser(description='Initialize manager.')
parser.add_argument('--device_id', type=int, help='Device ID')
parser.add_argument('--api_key', type=str, help='API Key')
args = parser.parse_args()

URL_BASE = 'https://api.libook.skyzh.dev'
BEGIN_HOUR = 7
END_HOUR = 23
BEGIN_HOUR_WEEKEND = 8
END_HOUR_WEEKEND = 22
DEVICE_ID = args.device_id
API_KEY = args.api_key


while True:
    now = datetime.now()

    # Set BEGIN/END_TIME from Monday to Friday
    if now.weekday() <= 4:
        BEGIN_TIME = now - timedelta(hours=now.hour, minutes=now.minute, seconds=now.second,
                                     microseconds=now.microsecond) + timedelta(hours=BEGIN_HOUR)
        END_TIME = BEGIN_TIME + timedelta(hours=END_HOUR-BEGIN_HOUR)
    # Set BEGIN/END_TIME on weekend
    else:
        BEGIN_TIME = now - timedelta(hours=now.hour, minutes=now.minute, seconds=now.second,
                                     microseconds=now.microsecond) + timedelta(hours=BEGIN_HOUR_WEEKEND)
        END_TIME = BEGIN_TIME + \
            timedelta(hours=END_HOUR_WEEKEND-BEGIN_HOUR_WEEKEND)

    # The device works only at regular hours
    if datetime.now() >= BEGIN_TIME and datetime.now() <= END_TIME:

        # Initialize communication with Server and STM32
        comServer = CommunicateServer(URL_BASE, API_KEY, DEVICE_ID)
        comSTM32 = CommunicateSTM32()

        ''' Authentication Process '''

        # Get reservation data of the current hour and device
        reserve_data = comServer.get()
        if reserve_data['message'] != 'Success':
            print(reserve_data['message'])
            continue

        # Wait for user response (first seated or not)
        is_first_seated, token = comSTM32.get_token()

        # Wait for the fingerprint (first seated or not)
        fingerprint_id = comSTM32.get_fingerprint_id()

        # Post user input to the server (token == '' if not first seated)
        flag_post = comServer.post(
            fingerprint_id, token, reserve_data['reservation_id'], reserve_data['user_id'])

        if not flag_post:
            print('Fail to post!')
            continue

        # Start mask detection and temperature display
        maskDetector = MaskDetector(comSTM32)
        maskDetector.seton()
        t = Thread(target=maskDetector.start)
        t.start()

        ''' While Seated '''
        while True:

            # Check whether the user is going to leave (actively)
            if comSTM32.is_leave():
                print('This user is leaving.')
                maskDetector.end()
                t.join()
                break

            # Check whether the reservation of this user has ended (passively)
            last_user = reserve_data['user_id']
            reserve_data = comServer.get()
            if reserve_data['message'] != 'Success':
                # No more reservation or any error
                print(reserve_data['message'])
                maskDetector.end()
                t.join()
                break
            if reserve_data['user_id'] != last_user:
                # Next user's reservation
                print('Next user is coming, and you have to leave.')
                maskDetector.end()
                t.join()
                break
