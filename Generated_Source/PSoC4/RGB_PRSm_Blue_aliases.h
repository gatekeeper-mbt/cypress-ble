/*******************************************************************************
* File Name: RGB_PRSm_Blue.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_RGB_PRSm_Blue_ALIASES_H) /* Pins RGB_PRSm_Blue_ALIASES_H */
#define CY_PINS_RGB_PRSm_Blue_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define RGB_PRSm_Blue_0			(RGB_PRSm_Blue__0__PC)
#define RGB_PRSm_Blue_0_PS		(RGB_PRSm_Blue__0__PS)
#define RGB_PRSm_Blue_0_PC		(RGB_PRSm_Blue__0__PC)
#define RGB_PRSm_Blue_0_DR		(RGB_PRSm_Blue__0__DR)
#define RGB_PRSm_Blue_0_SHIFT	(RGB_PRSm_Blue__0__SHIFT)
#define RGB_PRSm_Blue_0_INTR	((uint16)((uint16)0x0003u << (RGB_PRSm_Blue__0__SHIFT*2u)))

#define RGB_PRSm_Blue_INTR_ALL	 ((uint16)(RGB_PRSm_Blue_0_INTR))


#endif /* End Pins RGB_PRSm_Blue_ALIASES_H */


/* [] END OF FILE */
