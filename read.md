�����Ҫ��ϸ�˽���ֺ���Э��Ĺ���ģʽ����ֲ�����ȣ����Է�������������ַ
IRMPЭ���Ӧ����ַ��https://www.mikrocontroller.net/articles/IRMP
IRSNDЭ���Ӧ����ַ��https://www.mikrocontroller.net/articles/IRSND

�����̵�����оƬ��STM32F103ZET6 �������ͷ��HS0038
��UCOSII����������ֲ��IRMP-�����Э�������
�����ӵ���Ĺ����п���GPIO�ӿڲ�̫һ�£���Ҫ��irmpconfig.h�ļ����޸ģ�

����Э��ĳ�ʼ��
irmp_init();    // initialize irmp  

��ʱ���ĳ�ʼ��
irmp_timer_init(); // initialize timer ��Ҫ�Լ��޸�

��ʱ���ж�
(void) irmp_ISR(); // call irmp ISR  �Ѵ˺����ŵ���ʱ���жϷ�������

���ܵ���������
typedef struct IRMP_PACKED_STRUCT
{
    uint8_t        protocol;                                   // protocol, e.g. NEC_PROTOCOL
    uint16_t       address;                                    // address
    uint16_t       command;                                    // command
    uint8_t        flags;                                      // flags, e.g. repetition
} IRMP_DATA;

��ȡ���ݺ���
IRMP_DATA  irmp_data;
irmp_get_data (&irmp_data);
