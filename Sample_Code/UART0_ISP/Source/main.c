/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2015 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

//***********************************************************************************************************
//  Nuvoton Technoledge Corp. 
//  Website: http://www.nuvoton.com
//  E-Mail : MicroC-8bit@nuvoton.com
//  Date   : Apr/21/2015
//***********************************************************************************************************

//***********************************************************************************************************
//  File Function: N76E885 UART-0 Mode2 demo code
//***********************************************************************************************************

#include <stdio.h>
#include "N76E003.h"
#include "Typedef.h"
#include "Define.h"
#include "SFR_Macro.h"
#include "UART0_transfer.h"
#include "ISP_USER.h"

sfr RCTRIM0		= 0x84;
sfr RCTRIM1		= 0x85;	


uint8_t Xor()
{
    uint16_t value=0,value2=0,count;
    for(count=8;count<UartBufSize;count++)
	{
    value+=uart_rcvbuf[count];    
	}
    value2=uart_rcvbuf[2];
    value2|=(uart_rcvbuf[3]<<8);	
    if(value2==value)return 1;
    return 0;
}


void main (void)
{
    P0M1 = 0x00;
    P0M2 = 0x00;
    UART0_ini_115200();
    TM0_ini();
    g_timer0Over=0;
    g_timer0Counter=5000;
    g_progarmflag=0;
while(1)
{
	    	if(bUartDataReady == 1)
				{
					EA=0; 
                    if(Xor())
                    {		
					switch(uart_rcvbuf[0])
					{								
						case CMD_SYNC_PACKNO:
						{
							Send_64byte_To_UART0();		
							g_timer0Counter=0; 
							g_timer0Over=0;
						
						break;
						}
		
						case PAGE_ERASE_AP:						
						{
						
							TA=0xAA;	//set_IAPEN;		
							TA=0x55;
							CHPCON |= SET_BIT0 ;	
							TA=0xAA;	//set_APUEN;
							TA=0x55;
							IAPUEN |= SET_BIT0;
							IAPFD = 0xFF;    			//Erase must set IAPFD = 0xFF
							IAPCN = PAGE_ERASE_AP;
							
							for(flash_address=0x0000;flash_address<APROM_SIZE/PAGE_SIZE;flash_address++)
							{        
								IAPAL = LOBYTE(flash_address*PAGE_SIZE);
								IAPAH = HIBYTE(flash_address*PAGE_SIZE);
								TA=0xAA;			//trigger IAP
								TA=0x55;
								IAPTRG |= SET_BIT0 ;
							}						     			
							Send_64byte_To_UART0();	
							break;
						}
                        case CMD_UPDATE_APROM:
                        {
                            flash_address=uart_rcvbuf[4];
							flash_address|=(uart_rcvbuf[5]<<8);
                            for(count=8;count<UartBufSize;count++)
						{
							IAPCN = BYTE_PROGRAM_AP;					//program byte
							IAPAL =flash_address&0xff;
							IAPAH =flash_address>>8;
							IAPFD=uart_rcvbuf[count];
							TA=0xAA;		//trigger IAP
							TA=0x55;
							IAPTRG |= SET_BIT0 ;   			
							IAPCN = BYTE_READ_AP;							//program byte verify
							TA=0xAA;			//trigger IAP
							TA=0x55;
							IAPTRG |= SET_BIT0 ;  
							if(IAPFD!=uart_rcvbuf[count])
							while(1);													
							if (CHPCON==0x43)							//if error flag set, program error stop ISP
							while(1);
                            flash_address++;
						} 
    	        		Send_64byte_To_UART0();
                        break;
						
                        }
                        case CMD_RUN_APROM:
                        {
                            Send_64byte_To_UART0();
                            goto _APROM;
                        }
						
					}	
                }else Send_Error_To_UART0();
			        bUartDataReady = 0;
				    bufhead = 0;						
					EA=1;
			}
             if(g_timer0Over==1)
			{			 
			 goto _APROM;
			}
     	if(g_timer1Over==1)
			{			 
			 if((bufhead<UartBufSize)&&(bufhead>0)||(bufhead>UartBufSize))
			   {
				 bufhead=0;				 
			   }
			}	

}   



_APROM:
    TA = 0xAA;
    TA = 0x55;
    CHPCON &= 0xFD;                 
    TA = 0xAA;
    TA = 0x55;
    CHPCON |= 0x80;                  
    while(1);	
}


