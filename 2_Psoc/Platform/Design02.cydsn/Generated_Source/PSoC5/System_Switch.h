/*******************************************************************************
* File Name: System_Switch.h  
* Version 2.10
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_System_Switch_H) /* Pins System_Switch_H */
#define CY_PINS_System_Switch_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "System_Switch_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_10 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 System_Switch__PORT == 15 && ((System_Switch__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    System_Switch_Write(uint8 value) ;
void    System_Switch_SetDriveMode(uint8 mode) ;
uint8   System_Switch_ReadDataReg(void) ;
uint8   System_Switch_Read(void) ;
uint8   System_Switch_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define System_Switch_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define System_Switch_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define System_Switch_DM_RES_UP          PIN_DM_RES_UP
#define System_Switch_DM_RES_DWN         PIN_DM_RES_DWN
#define System_Switch_DM_OD_LO           PIN_DM_OD_LO
#define System_Switch_DM_OD_HI           PIN_DM_OD_HI
#define System_Switch_DM_STRONG          PIN_DM_STRONG
#define System_Switch_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define System_Switch_MASK               System_Switch__MASK
#define System_Switch_SHIFT              System_Switch__SHIFT
#define System_Switch_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define System_Switch_PS                     (* (reg8 *) System_Switch__PS)
/* Data Register */
#define System_Switch_DR                     (* (reg8 *) System_Switch__DR)
/* Port Number */
#define System_Switch_PRT_NUM                (* (reg8 *) System_Switch__PRT) 
/* Connect to Analog Globals */                                                  
#define System_Switch_AG                     (* (reg8 *) System_Switch__AG)                       
/* Analog MUX bux enable */
#define System_Switch_AMUX                   (* (reg8 *) System_Switch__AMUX) 
/* Bidirectional Enable */                                                        
#define System_Switch_BIE                    (* (reg8 *) System_Switch__BIE)
/* Bit-mask for Aliased Register Access */
#define System_Switch_BIT_MASK               (* (reg8 *) System_Switch__BIT_MASK)
/* Bypass Enable */
#define System_Switch_BYP                    (* (reg8 *) System_Switch__BYP)
/* Port wide control signals */                                                   
#define System_Switch_CTL                    (* (reg8 *) System_Switch__CTL)
/* Drive Modes */
#define System_Switch_DM0                    (* (reg8 *) System_Switch__DM0) 
#define System_Switch_DM1                    (* (reg8 *) System_Switch__DM1)
#define System_Switch_DM2                    (* (reg8 *) System_Switch__DM2) 
/* Input Buffer Disable Override */
#define System_Switch_INP_DIS                (* (reg8 *) System_Switch__INP_DIS)
/* LCD Common or Segment Drive */
#define System_Switch_LCD_COM_SEG            (* (reg8 *) System_Switch__LCD_COM_SEG)
/* Enable Segment LCD */
#define System_Switch_LCD_EN                 (* (reg8 *) System_Switch__LCD_EN)
/* Slew Rate Control */
#define System_Switch_SLW                    (* (reg8 *) System_Switch__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define System_Switch_PRTDSI__CAPS_SEL       (* (reg8 *) System_Switch__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define System_Switch_PRTDSI__DBL_SYNC_IN    (* (reg8 *) System_Switch__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define System_Switch_PRTDSI__OE_SEL0        (* (reg8 *) System_Switch__PRTDSI__OE_SEL0) 
#define System_Switch_PRTDSI__OE_SEL1        (* (reg8 *) System_Switch__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define System_Switch_PRTDSI__OUT_SEL0       (* (reg8 *) System_Switch__PRTDSI__OUT_SEL0) 
#define System_Switch_PRTDSI__OUT_SEL1       (* (reg8 *) System_Switch__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define System_Switch_PRTDSI__SYNC_OUT       (* (reg8 *) System_Switch__PRTDSI__SYNC_OUT) 


#if defined(System_Switch__INTSTAT)  /* Interrupt Registers */

    #define System_Switch_INTSTAT                (* (reg8 *) System_Switch__INTSTAT)
    #define System_Switch_SNAP                   (* (reg8 *) System_Switch__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_System_Switch_H */


/* [] END OF FILE */
