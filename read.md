如果想要详细了解此种红外协议的工作模式和移植方法等，可以访问下面两个网址
IRMP协议对应的网址：https://www.mikrocontroller.net/articles/IRMP
IRSND协议对应的网址：https://www.mikrocontroller.net/articles/IRSND

本工程的主控芯片是STM32F103ZET6 红外接收头是HS0038
在UCOSII的任务中移植了IRMP-红外多协议解码器
如果添加到你的过程中可能GPIO接口不太一致，需要在irmpconfig.h文件中修改，

红外协议的初始化
irmp_init();    // initialize irmp  

定时器的初始化
irmp_timer_init(); // initialize timer 需要自己修改

定时器中断
(void) irmp_ISR(); // call irmp ISR  把此函数放到定时器中断服务函数中

接受的数据类型
typedef struct IRMP_PACKED_STRUCT
{
    uint8_t        protocol;                                   // protocol, e.g. NEC_PROTOCOL
    uint16_t       address;                                    // address
    uint16_t       command;                                    // command
    uint8_t        flags;                                      // flags, e.g. repetition
} IRMP_DATA;

获取数据函数
IRMP_DATA  irmp_data;
irmp_get_data (&irmp_data);
