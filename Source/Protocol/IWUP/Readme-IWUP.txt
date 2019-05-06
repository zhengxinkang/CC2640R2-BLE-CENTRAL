要是用本模块需要实现以下接口：
1、需要实现接口
void Uart_adapter_send(uint8_t *buf, uint8_t len)
{
    UartFrame_send(buf, len);
}

2、在串口接收接口中调用
void Uart_adapter_recieve(uint8_t* data, uint16_t len)