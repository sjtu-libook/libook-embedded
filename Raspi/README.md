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

## Log 2021/4/5

目前，仅简单搭建了框架，主要写了主控程序 `manager.py` 的逻辑，在一个主循环内，实现**身份认证**、**就座时逻辑**。工作流程基于 https://shimo.im/docs/GHGrrrpkXqrdQwHC 。口罩检测控制器的调用，使用了多线程。目前尚未在 Raspi 上进行测试，程序中 bug 可能较多 （仅仅实现了框架，很多地方还未实现

- [x] `manager.py` （基本实现，最终整合起来之后可能 bug 较多，多线程处理也有待考量）
- [x] `pkg/communication.py CommunicateServer` （基本实现，还需要更多测试）
- [ ] `pkg/communication.py CommunicateSTM32` [@yangco-le](https://github.com/yangco-le)
- [ ] `pkg/mask_detection.py`   [@initializer-z](https://github.com/initializer-z)
- [ ] `pkg/simulate_stm32.py`   [@initializer-z](https://github.com/initializer-z)

解释：[@zhliuworks](https://github.com/zhliuworks)