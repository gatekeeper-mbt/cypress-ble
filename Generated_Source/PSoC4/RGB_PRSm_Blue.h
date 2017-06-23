/*******************************************************************************
* File Name: RGB_PRSm_Blue.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_RGB_PRSm_Blue_H) /* Pins RGB_PRSm_Blue_H */
#define CY_PINS_RGB_PRSm_Blue_H

#include "cytypes.h"
#include "cyfitter.h"
#include "RGB_PRSm_Blue_aliases.h"


/***************************************
*     Data Struct Definitions
***************************************/

/**
* \addtogroup group_structures
* @{
*/
    
/* Structure for sleep mode support */
typedef struct
{
    uint32 pcState; /**< State of the port control register */
    uint32 sioState; /**< State of the SIO configuration */
    uint32 usbState; /**< State of the USBIO regulator */
} RGB_PRSm_Blue_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   RGB_PRSm_Blue_Read(void);
void    RGB_PRSm_Blue_Write(uint8 value);
uint8   RGB_PRSm_Blue_ReadDataReg(void);
#if defined(RGB_PRSm_Blue__PC) || (CY_PSOC4_4200L) 
    void    RGB_PRSm_Blue_SetDriveMode(uint8 mode);
#endif
void    RGB_PRSm_Blue_SetInterruptMode(uint16 position, uint16 mode);
uint8   RGB_PRSm_Blue_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void RGB_PRSm_Blue_Sleep(void); 
void RGB_PRSm_Blue_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(RGB_PRSm_Blue__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define RGB_PRSm_Blue_DRIVE_MODE_BITS        (3)
    #define RGB_PRSm_Blue_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - RGB_PRSm_Blue_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the RGB_PRSm_Blue_SetDriveMode() function.
         *  @{
         */
        #define RGB_PRSm_Blue_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define RGB_PRSm_Blue_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define RGB_PRSm_Blue_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define RGB_PRSm_Blue_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define RGB_PRSm_Blue_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define RGB_PRSm_Blue_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define RGB_PRSm_Blue_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define RGB_PRSm_Blue_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define RGB_PRSm_Blue_MASK               RGB_PRSm_Blue__MASK
#define RGB_PRSm_Blue_SHIFT              RGB_PRSm_Blue__SHIFT
#define RGB_PRSm_Blue_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in RGB_PRSm_Blue_SetInterruptMode() function.
     *  @{
     */
        #define RGB_PRSm_Blue_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define RGB_PRSm_Blue_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define RGB_PRSm_Blue_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define RGB_PRSm_Blue_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(RGB_PRSm_Blue__SIO)
    #define RGB_PRSm_Blue_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(RGB_PRSm_Blue__PC) && (CY_PSOC4_4200L)
    #define RGB_PRSm_Blue_USBIO_ENABLE               ((uint32)0x80000000u)
    #define RGB_PRSm_Blue_USBIO_DISABLE              ((uint32)(~RGB_PRSm_Blue_USBIO_ENABLE))
    #define RGB_PRSm_Blue_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define RGB_PRSm_Blue_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define RGB_PRSm_Blue_USBIO_ENTER_SLEEP          ((uint32)((1u << RGB_PRSm_Blue_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << RGB_PRSm_Blue_USBIO_SUSPEND_DEL_SHIFT)))
    #define RGB_PRSm_Blue_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << RGB_PRSm_Blue_USBIO_SUSPEND_SHIFT)))
    #define RGB_PRSm_Blue_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << RGB_PRSm_Blue_USBIO_SUSPEND_DEL_SHIFT)))
    #define RGB_PRSm_Blue_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(RGB_PRSm_Blue__PC)
    /* Port Configuration */
    #define RGB_PRSm_Blue_PC                 (* (reg32 *) RGB_PRSm_Blue__PC)
#endif
/* Pin State */
#define RGB_PRSm_Blue_PS                     (* (reg32 *) RGB_PRSm_Blue__PS)
/* Data Register */
#define RGB_PRSm_Blue_DR                     (* (reg32 *) RGB_PRSm_Blue__DR)
/* Input Buffer Disable Override */
#define RGB_PRSm_Blue_INP_DIS                (* (reg32 *) RGB_PRSm_Blue__PC2)

/* Interrupt configuration Registers */
#define RGB_PRSm_Blue_INTCFG                 (* (reg32 *) RGB_PRSm_Blue__INTCFG)
#define RGB_PRSm_Blue_INTSTAT                (* (reg32 *) RGB_PRSm_Blue__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define RGB_PRSm_Blue_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(RGB_PRSm_Blue__SIO)
    #define RGB_PRSm_Blue_SIO_REG            (* (reg32 *) RGB_PRSm_Blue__SIO)
#endif /* (RGB_PRSm_Blue__SIO_CFG) */

/* USBIO registers */
#if !defined(RGB_PRSm_Blue__PC) && (CY_PSOC4_4200L)
    #define RGB_PRSm_Blue_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define RGB_PRSm_Blue_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define RGB_PRSm_Blue_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define RGB_PRSm_Blue_DRIVE_MODE_SHIFT       (0x00u)
#define RGB_PRSm_Blue_DRIVE_MODE_MASK        (0x07u << RGB_PRSm_Blue_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins RGB_PRSm_Blue_H */


/* [] END OF FILE */
