# 8QM_RTSP
使用H.264硬件编码器压缩编码四路720P NV12数据使用Gstreamer搭建RTSP 服务器并推流

程序默认已经调好一版低延迟的参数

其中需要注意悦享魔改的系统包里无Gstreamer服务器的运行库

硬件编码器 VPU 为Video13 ，可用V4L2-Utils查看具体支持的参数。

## 支持调节的参数

- 码率方式： 支持恒定码率CBR ，可变码率VBR
- I帧间隔
- b帧数量
- gop size
- 码率 ，不建议超过2M
- 峰值码率，这个可以拉高

## 易出现的问题

改为恒定码率，虽然推流成功，但是硬件编码器可能不工作

**码率过高，超过2M。延迟可能会变高，且会花屏**


