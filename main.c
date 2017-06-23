/******************************************************************************
* Project Name		: PRoC_BLE_CapSense_Slider_LED
* File Name			: main.c
* Version 			: 1.0
* Device Used		: CYBL10563-56LQXI
* Software Used		: PSoC Creator 3.1
* Compiler    		: ARM GCC 4.8.4, ARM RVDS Generic, ARM MDK Generic
* Related Hardware	: CY8CKIT-042-BLE Bluetooth Low Energy Pioneer Kit 
* Owner             : ROIT
*
********************************************************************************
* Copyright (2014-15), Cypress Semiconductor Corporation. All Rights Reserved.
********************************************************************************
* This software is owned by Cypress Semiconductor Corporation (Cypress)
* and is protected by and subject to worldwide patent protection (United
* States and foreign), United States copyright laws and international treaty
* provisions. Cypress hereby grants to licensee a personal, non-exclusive,
* non-transferable license to copy, use, modify, create derivative works of,
* and compile the Cypress Source Code and derivative works for the sole
* purpose of creating custom software in support of licensee product to be
* used only in conjunction with a Cypress integrated circuit as specified in
* the applicable agreement. Any reproduction, modification, translation,
* compilation, or representation of this software except as specified above 
* is prohibited without the express written permission of Cypress.
*
* Disclaimer: CYPRESS MAKES NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, WITH 
* REGARD TO THIS MATERIAL, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
* Cypress reserves the right to make changes without further notice to the 
* materials described herein. Cypress does not assume any liability arising out 
* of the application or use of any product or circuit described herein. Cypress 
* does not authorize its products for use as critical components in life-support 
* systems where a malfunction or failure may reasonably be expected to result in 
* significant injury to the user. The inclusion of Cypress' product in a life-
* support systems application implies that the manufacturer assumes all risk of 
* such use and in doing so indemnifies Cypress against all charges. 
*
* Use of this Software may be limited by and subject to the applicable Cypress
* software license agreement. 
*******************************************************************************/

/******************************************************************************
*                           THEORY OF OPERATION
*******************************************************************************
* This project will showcase the capability of PRoC BLE to communicate 
* bi-directionally with a BLE Central device. The centroid value of CapSense
* slider is sent from PRoC BLE to central device using a custom service. Also, 
* RGB LED control data is sent from central device to PRoC BLE over another 
* custom service. The CapSense custom service allows notifications to be sent to
* central device when notifications are enabled. On the other hand, the RGB LED 
* custom service allows read and write of attributes under the RGB characteristics.
* This project utilizes CapSense component to check finger position on slider 
* and report this to central device over BLE. Also, the control values sent to 
* PRoC BLE is converted to respective color and intensity on the onboard RGB LED
* The BLE central device can be any BLE central device, including CySmart mobile
* app or CySmart PC tool. 
* This project also inludes low power mode operation, idle for battery operated 
* devices. The project utlizes Deep Sleep feature of both BLESS and CPU to remain 
* in low power mode as much as possible, while maintaining the BLE connection and  
* data transfer. This allows the device to run on coin cell battery for long time.
*
* Note:
* The programming pins have been configured as GPIO, and not SWD. This is because 
* when programming pins are configured for SWD, then the silicon consumes extra
* power through the pins. To prevent the leakage of power, the pins have been set 
* to GPIO. With this setting, the kit can still be acquired by PSoC Creator or
* PSoC Programmer software tools for programming, but the project cannot be 
* externally debugged. To re-enable debugging, go to PRoC_BLE_CapSense_Slider_LED.cydwr 
* from Workspace Explorer, go to Systems tab, and set the Debug Select option to 'SWD'.
* Build and program this project to enable external Debugging.
*
* Refer to BLE Pioneer Kit user guide for details.
************************************************************************************
* Hardware connection required for testing -
* Slider pins 	- P2[1]-P2[5] (hard-wired in the BLE Pioneer Kit)
* Cmod pin		- P4[0] (hard-wired in the PRoC BLE module)
* R-G-B LED 	- P2[6], P3[6] and P3[7] (hard-wired on the BLE Pioneer Kit)
* User Switch	- P2[7] (hard-wired on the BLE Pioneer Kit)
***********************************************************************************/
#include <main.h>
#include <stdio.h>

/* Array to store the present RGB LED control data. The 4 bytes 
* of the array represents {R,G,B,Intensity} */
extern uint8 DelightData[DELIGHT_CHAR_DATA_LEN];						

/* This flag is used by application to know whether a Central 
* device has been connected. This is updated in BLE event callback 
* function*/
extern uint8 deviceConnected;

/* 'restartAdvertisement' flag provided the present state of power mode in firmware */
extern uint8 restartAdvertisement;

/* 'connectionHandle' stores connection parameters */
extern CYBLE_CONN_HANDLE_T  connectionHandle;

/* These flags are used to let application update the respective CCCD value of the 
* custom characteristics for correct read operation by connected Central device */
extern uint8 updateDelightNotificationCCCDAttribute;


uint8 timer = FALSE;
uint8 TimerSTART = FALSE;
uint8 TimePause = FALSE;
uint8 onPower = FALSE;
uint32 time = 0;
volatile uint16 CurrentTime = 0;

static volatile uint32 _millisCounter;

volatile uint32 previousMillis = 0;   // will store last time LED was updated


/**
 * @brief Returns the system uptime, in milliseconds.
 */
static inline uint32 systick_uptime(void) {
    return _millisCounter;
}

//回傳ms
volatile uint32 millis(void)
{
	return systick_uptime();
}

void __cs3_isr_systick( void )
{
    _millisCounter++;
}

/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
*        System entrance point. This calls the initializing function and
* continuously process BLE and CapSense events.
*
* Parameters:
*  void
*
* Return:
*  int
*

*******************************************************************************/
void int2str(uint32 i, char *s) {
  sprintf(s,"%lu",i);
}

void print(uint32 num){
    char s[8] = {};
    int i =0; 
    int2str(num, s);
    
    for(i = 0; i<8; i++) 
       UART_1_UartPutChar(s[i]);
}

int main()
{
    uint32 currentMillis;
 
	/* This function will initilize the system resources such as BLE and CapSense */
    InitializeSystem();
	previousMillis = millis();
    DelightData[LAMP_INTENSITY_INDEX] = DELIGHT_LAMP_FULL; //開機亮度最大
    Update_LAMP_INTENSITY();
    
    for(;;)
    {
        if (onPower == TRUE && TimerSTART == TRUE){
            currentMillis = millis();
        
            if(currentMillis - previousMillis >= 1000){ //每計數1000(1秒)減1000
                CurrentTime--;
               /* print(currentMillis);
                    UART_1_UartPutString(" - ");
                print(previousMillis);
                    UART_1_UartPutString(" - ");
                    print(CurrentTime);
                    UART_1_UartPutCRLF(255);
                */    
                //Pin_SW_PWR_Write( ! Pin_SW_PWR_Read() );

                    
                DelightData[TIME1_INDEX] = CurrentTime / 256;
                DelightData[TIME2_INDEX] = CurrentTime % 256;
				/* Set flag to allow CCCD to be updated for next read operation */
				updateDelightNotificationCCCDAttribute = TRUE;
				
				/* Update the RGB LED Notification attribute with new color coordinates */
				SendDataOverDelightNotification(DelightData, DELIGHT_CHAR_DATA_LEN);
                
                previousMillis = currentMillis;
            }
           
            if (CurrentTime == 0){
            	Pin_SW_PWR_Write(1);
                DelightData[LAMP_INDEX] = 1;
                DelightData[TIMER_INDEX] = 0;
                DelightData[LAMP_INTENSITY_INDEX] = 0;
                Update_LAMP_INTENSITY();
                timer = FALSE;
                onPower = FALSE;
                TimerSTART = FALSE;
                
				/* Set flag to allow CCCD to be updated for next read operation */
				updateDelightNotificationCCCDAttribute = TRUE;
				
				/* Update the RGB LED Notification attribute with new color coordinates */
				SendDataOverDelightNotification(DelightData, DELIGHT_CHAR_DATA_LEN);
            }

       }
        
        /*Process event callback to handle BLE events. The events generated and 
		* used for this application are inside the 'CustomEventHandler' routine*/
        CyBle_ProcessEvents();
		
		if(TRUE == deviceConnected)
		{
			/* After the connection, send new connection parameter to the Client device 
			* to run the BLE communication on desired interval. This affects the data rate 
			* and power consumption. High connection interval will have lower data rate but 
			* lower power consumption. Low connection interval will have higher data rate at
			* expense of higher power. This function is called only once per connection. */
			UpdateConnectionParam();
			
			/* When the Client Characteristic Configuration descriptor (CCCD) is written
			* by Central device for enabling/disabling notifications, then the same
			* descriptor value has to be explicitly updated in application so that
			* it reflects the correct value when the descriptor is read */
			UpdateNotificationCCCD();

		}
		
		//#ifdef ENABLE_LOW_POWER_MODE
			/* Put system to Deep sleep, including BLESS, and wakeup on interrupt. 
			* The source of the interrupt can be either BLESS Link Layer in case of 
			* BLE advertisement and connection or by User Button press during BLE 
			* disconnection */
		//	HandleLowPowerMode();
		//#endif
		
		if(restartAdvertisement)
		{
			/* Reset 'restartAdvertisement' flag*/
			restartAdvertisement = FALSE;

			/* Start Advertisement and enter Discoverable mode*/
			CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);	
		}
    }	/* End of for(;;) */
}

/*******************************************************************************
* Function Name: InitializeSystem
********************************************************************************
* Summary:
*        Start the components and initialize system 
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void InitializeSystem(void)
{
    UART_1_Start(); //使用串列埠

    /* 2Hz flash rate at 12MHz CPU clock */
	/* 12000000/12000 = 1000 */
	if( SysTick_Config( 11750 ) ) //數字越大計數越慢
	    Pin_SW_PWR_Write( 1 ); /* Solid LED indicates an error */

	CyIntSetSysVector (15, __cs3_isr_systick);
	CyGlobalIntEnable; /* Enable global interrupts */

    /* Enable global interrupt mask */
	CyGlobalIntEnable; 

	/* Start BLE component and register the CustomEventHandler function. This 
	* function exposes the events from BLE component for application use */
    CyBle_Start(CustomEventHandler);	

	
	/* Set the Watchdog Interrupt vector to the address of Interrupt routine 
	* WDT_INT_Handler. This routine counts the 3 seconds for LED ON state during
	* connection. */
	CyIntSetVector(WATCHDOG_INT_VEC_NUM, &WDT_INT_Handler);
    
    CLK_1M_Start();
    PWM_Start();
}

/*******************************************************************************
* Function Name: UpdateRGBled
********************************************************************************
* Summary:
*        Receive the new RGB data and modify PrISM parameters. Also, update the
* read characteristic handle so that the next read from the BLE central device
* gives present RGB color and intensity data.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void UpdateDelightLamp(void)
{

    Pin_SW_PWR_Write(DelightData[LAMP_INDEX]);   //LAMP ON/OFF
    
    if(onPower == FALSE && DelightData[LAMP_INDEX] == 0){
        DelightData[LAMP_INTENSITY_INDEX] = DELIGHT_LAMP_FULL;   //開燈最大亮度
        onPower = TRUE;
    }else if (DelightData[LAMP_INDEX] == 1){
        onPower = FALSE;
    }
    
    Update_LAMP_INTENSITY();

    timer = DelightData[TIMER_INDEX];

    if(timer == TIME_START && TimerSTART == FALSE && TimePause == FALSE){   //開始
        time = ((DelightData[TIME1_INDEX] * 256) + DelightData[TIME2_INDEX]) * 1000 ;
    	previousMillis = millis();
        CurrentTime = time / 1000;
        TimerSTART = TRUE;
    }else if(timer == TIME_START && TimerSTART == FALSE && TimePause == TRUE){  //暫停後開始
        time = time - (CurrentTime * 1000);
     	previousMillis = millis();
        TimerSTART = TRUE;
        TimePause = FALSE;
    }else if(timer == TIME_PAUSE){  //暫停
        TimerSTART = FALSE;
        TimePause = TRUE;
    }else if(timer == TIME_STOP){   //停止
        TimerSTART = FALSE;
        TimePause = FALSE;
    }

	/* 'rgbHandle' stores RGB control data parameters */
	CYBLE_GATT_HANDLE_VALUE_PAIR_T		delightHandle;
	
	/* Update RGB control handle with new values */
	delightHandle.attrHandle = CYBLE_DELIGHT_LAMP_DELIGHT_LAMP_CONTROL_CHAR_HANDLE;
	delightHandle.value.val = DelightData;
	delightHandle.value.len = DELIGHT_CHAR_DATA_LEN;
	
	/* Send updated RGB control handle as attribute for read by central device */
	CyBle_GattsWriteAttributeValue(&delightHandle, FALSE, &connectionHandle,FALSE);
}

void Update_LAMP_INTENSITY(void)
{
    /* Update the density value of thr PrISM module for color control*/
	uint8 intensity_divide_value = DelightData[LAMP_INTENSITY_INDEX];
    
    // -------- Update PWM output setting
    uint16 intensity_pwm = (intensity_divide_value*1000)/255;
    //intensity_pwm = (254*1000)/255;
  
    PWM_WriteCompare(intensity_pwm);    //亮度
  
}
/* [] END OF FILE */
