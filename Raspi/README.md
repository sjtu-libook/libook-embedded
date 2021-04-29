# Raspi

## Tips

* 格式化所有 Python 文件：

  ```shell
  make format
  ```

* 安装依赖

  ```shell
  pip install -r requirements.txt
  ```

* 更新依赖

  ```shell
  pip install pipreqs
  pipreqs . --encoding=utf8
  ```

## Directory Structure

* `manager.py` ：Raspi 主控程序

  ```shell
  python manager.py [device_id] [api_key]
  # e.g. python manager.py 1 abCdEf
  ```

* `pkg/communication.py` ：Raspi 与 Server 和 STM32 的通信 API
  * `CommunicateServer` ：Raspi 和 Server 之间的 HTTP 通信 API
  * `CommunicateSTM32` ：Raspi 和 STM32 之间的串口通信 API
  
* `pkg/mask_detection.py` ：口罩检测控制器，负责拍摄照片、口罩检测、显示照片和温度至显示屏

* `pkg/simulate_stm32.py`：模拟 STM32 发来的数据，在 debug 时可代替 CommunicateSTM32 类
