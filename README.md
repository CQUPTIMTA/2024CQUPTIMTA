# 2024年中国大学生机械工程创新创意大赛物流技术（起重机）创意赛
# 重庆邮电大学参赛作品"多即是好"代码仓库
硬件开源链接：https://oshwhub.com/qingmeijiupiao/2024-wu-liu-qi-zhong-ji
## 设计思路
通过运行在固定框架上的多个并行的抓取模块来达到极致的速度，并且通过模块的通用化，精简化，小型化设计来压缩成本，代码也做到了模块化，通用化。
## 控制层级
![image](https://github.com/user-attachments/assets/78652727-fb97-488c-9aab-ff25033e6de5)
## 代码架构
![image](https://github.com/user-attachments/assets/650994c0-90d9-4dff-b0b5-e295c588f871)
## 开发环境
抓取单元和横梁单元主控为ESP32S3，识别单元为ESP32-C3，主控未使用引脚，可以为任意版本ESP32

vscode+platformio插件

arduino框架混合esp-idf函数

对于代码复用的问题，我们采用Esp32的NVS文件系统来存储需要修改的参数，使得同一份工程可以烧录在不同位置的单元上
## 目录简介
## Grab_unit
抓取单元代码
## crossbeam_control
横梁代码
## recognition_unit
识别单元代码
## wirelesstester
无线主控代码
## 测试工具
一些比赛准备过程中写的脚本，例如随机生成砝码顺序等
## 通信协议.md
本作品ESPNOW的数据包格式，以及规定的数据包指令
