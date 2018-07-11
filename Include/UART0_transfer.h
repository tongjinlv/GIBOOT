void TM0_ini(void);
void UART0_ini_115200(void);
void Send_64byte_To_UART0(void);
void Send_Error_To_UART0(void);
#define UartBufSize 150
extern  xdata volatile uint8_t uart_rcvbuf[]; 
extern  xdata volatile uint8_t uart_txbuf[8];
extern data volatile uint8_t bufhead;
extern  data volatile uint16_t flash_address; 
extern  data volatile uint16_t AP_size;
extern  data volatile uint8_t g_timer1Counter;
extern  data volatile uint8_t count; 
extern  data volatile uint16_t g_timer0Counter;
extern  bit volatile bUartDataReady;
extern  bit volatile g_timer0Over;
extern  bit volatile g_timer1Over;
extern  bit volatile g_progarmflag;
extern bit BIT_TMP;

