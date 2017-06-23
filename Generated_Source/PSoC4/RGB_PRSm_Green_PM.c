/*******************************************************************************
* File Name: RGB_PRSm_Green.c  
* Version 2.20
*
* Description:
*  This file contains APIs to set up the Pins component for low power modes.
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "RGB_PRSm_Green.h"

static RGB_PRSm_Green_BACKUP_STRUCT  RGB_PRSm_Green_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: RGB_PRSm_Green_Sleep
****************************************************************************//**
*
* \brief Stores the pin configuration and prepares the pin for entering chip 
*  deep-sleep/hibernate modes. This function must be called for SIO and USBIO
*  pins. It is not essential if using GPIO or GPIO_OVT pins.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None 
*  
* \sideeffect
*  For SIO pins, this function configures the pin input threshold to CMOS and
*  drive level to Vddio. This is needed for SIO pins when in device 
*  deep-sleep/hibernate modes.
*
* \funcusage
*  \snippet RGB_PRSm_Green_SUT.c usage_RGB_PRSm_Green_Sleep_Wakeup
*******************************************************************************/
void RGB_PRSm_Green_Sleep(void)
{
    #if defined(RGB_PRSm_Green__PC)
        RGB_PRSm_Green_backup.pcState = RGB_PRSm_Green_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            RGB_PRSm_Green_backup.usbState = RGB_PRSm_Green_CR1_REG;
            RGB_PRSm_Green_USB_POWER_REG |= RGB_PRSm_Green_USBIO_ENTER_SLEEP;
            RGB_PRSm_Green_CR1_REG &= RGB_PRSm_Green_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(RGB_PRSm_Green__SIO)
        RGB_PRSm_Green_backup.sioState = RGB_PRSm_Green_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        RGB_PRSm_Green_SIO_REG &= (uint32)(~RGB_PRSm_Green_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: RGB_PRSm_Green_Wakeup
****************************************************************************//**
*
* \brief Restores the pin configuration that was saved during Pin_Sleep().
*
* For USBIO pins, the wakeup is only triggered for falling edge interrupts.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None
*  
* \funcusage
*  Refer to RGB_PRSm_Green_Sleep() for an example usage.
*******************************************************************************/
void RGB_PRSm_Green_Wakeup(void)
{
    #if defined(RGB_PRSm_Green__PC)
        RGB_PRSm_Green_PC = RGB_PRSm_Green_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            RGB_PRSm_Green_USB_POWER_REG &= RGB_PRSm_Green_USBIO_EXIT_SLEEP_PH1;
            RGB_PRSm_Green_CR1_REG = RGB_PRSm_Green_backup.usbState;
            RGB_PRSm_Green_USB_POWER_REG &= RGB_PRSm_Green_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(RGB_PRSm_Green__SIO)
        RGB_PRSm_Green_SIO_REG = RGB_PRSm_Green_backup.sioState;
    #endif
}


/* [] END OF FILE */
