# STM32

### 串口通信

路径：[/Serial_Communication](https://github.com/zhliuworks/libook-embedded/STM32/Serial_Communication)

解释：[@yangco-le](https://github.com/yangco-le)

目前可以做到由`Raspi`的python端发送消息到`STM32`，接收后回送任何字符串消息，`Raspi`可以正常接收，双方可以互传数据。功能通过中断完成，串口接收消息产生中断，在中断处理时将其他信息返回。

加入了LED显示功能，作为正常运行提示，如果加载正常，蓝灯将闪烁，停止闪烁提示出现问题。