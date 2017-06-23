/*******************************************************************************
* File Name: CLK_1M.h
* Version 2.20
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CLOCK_CLK_1M_H)
#define CY_CLOCK_CLK_1M_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void CLK_1M_StartEx(uint32 alignClkDiv);
#define CLK_1M_Start() \
    CLK_1M_StartEx(CLK_1M__PA_DIV_ID)

#else

void CLK_1M_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void CLK_1M_Stop(void);

void CLK_1M_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 CLK_1M_GetDividerRegister(void);
uint8  CLK_1M_GetFractionalDividerRegister(void);

#define CLK_1M_Enable()                         CLK_1M_Start()
#define CLK_1M_Disable()                        CLK_1M_Stop()
#define CLK_1M_SetDividerRegister(clkDivider, reset)  \
    CLK_1M_SetFractionalDividerRegister((clkDivider), 0u)
#define CLK_1M_SetDivider(clkDivider)           CLK_1M_SetDividerRegister((clkDivider), 1u)
#define CLK_1M_SetDividerValue(clkDivider)      CLK_1M_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define CLK_1M_DIV_ID     CLK_1M__DIV_ID

#define CLK_1M_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define CLK_1M_CTRL_REG   (*(reg32 *)CLK_1M__CTRL_REGISTER)
#define CLK_1M_DIV_REG    (*(reg32 *)CLK_1M__DIV_REGISTER)

#define CLK_1M_CMD_DIV_SHIFT          (0u)
#define CLK_1M_CMD_PA_DIV_SHIFT       (8u)
#define CLK_1M_CMD_DISABLE_SHIFT      (30u)
#define CLK_1M_CMD_ENABLE_SHIFT       (31u)

#define CLK_1M_CMD_DISABLE_MASK       ((uint32)((uint32)1u << CLK_1M_CMD_DISABLE_SHIFT))
#define CLK_1M_CMD_ENABLE_MASK        ((uint32)((uint32)1u << CLK_1M_CMD_ENABLE_SHIFT))

#define CLK_1M_DIV_FRAC_MASK  (0x000000F8u)
#define CLK_1M_DIV_FRAC_SHIFT (3u)
#define CLK_1M_DIV_INT_MASK   (0xFFFFFF00u)
#define CLK_1M_DIV_INT_SHIFT  (8u)

#else 

#define CLK_1M_DIV_REG        (*(reg32 *)CLK_1M__REGISTER)
#define CLK_1M_ENABLE_REG     CLK_1M_DIV_REG
#define CLK_1M_DIV_FRAC_MASK  CLK_1M__FRAC_MASK
#define CLK_1M_DIV_FRAC_SHIFT (16u)
#define CLK_1M_DIV_INT_MASK   CLK_1M__DIVIDER_MASK
#define CLK_1M_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_CLK_1M_H) */

/* [] END OF FILE */
