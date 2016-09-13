/*******************************************************************************
* File Name: SERVO_2.h  
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

#if !defined(CY_PINS_SERVO_2_H) /* Pins SERVO_2_H */
#define CY_PINS_SERVO_2_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "SERVO_2_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_10 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 SERVO_2__PORT == 15 && ((SERVO_2__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    SERVO_2_Write(uint8 value) ;
void    SERVO_2_SetDriveMode(uint8 mode) ;
uint8   SERVO_2_ReadDataReg(void) ;
uint8   SERVO_2_Read(void) ;
uint8   SERVO_2_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define SERVO_2_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define SERVO_2_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define SERVO_2_DM_RES_UP          PIN_DM_RES_UP
#define SERVO_2_DM_RES_DWN         PIN_DM_RES_DWN
#define SERVO_2_DM_OD_LO           PIN_DM_OD_LO
#define SERVO_2_DM_OD_HI           PIN_DM_OD_HI
#define SERVO_2_DM_STRONG          PIN_DM_STRONG
#define SERVO_2_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define SERVO_2_MASK               SERVO_2__MASK
#define SERVO_2_SHIFT              SERVO_2__SHIFT
#define SERVO_2_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define SERVO_2_PS                     (* (reg8 *) SERVO_2__PS)
/* Data Register */
#define SERVO_2_DR                     (* (reg8 *) SERVO_2__DR)
/* Port Number */
#define SERVO_2_PRT_NUM                (* (reg8 *) SERVO_2__PRT) 
/* Connect to Analog Globals */                                                  
#define SERVO_2_AG                     (* (reg8 *) SERVO_2__AG)                       
/* Analog MUX bux enable */
#define SERVO_2_AMUX                   (* (reg8 *) SERVO_2__AMUX) 
/* Bidirectional Enable */                                                        
#define SERVO_2_BIE                    (* (reg8 *) SERVO_2__BIE)
/* Bit-mask for Aliased Register Access */
#define SERVO_2_BIT_MASK               (* (reg8 *) SERVO_2__BIT_MASK)
/* Bypass Enable */
#define SERVO_2_BYP                    (* (reg8 *) SERVO_2__BYP)
/* Port wide control signals */                                                   
#define SERVO_2_CTL                    (* (reg8 *) SERVO_2__CTL)
/* Drive Modes */
#define SERVO_2_DM0                    (* (reg8 *) SERVO_2__DM0) 
#define SERVO_2_DM1                    (* (reg8 *) SERVO_2__DM1)
#define SERVO_2_DM2                    (* (reg8 *) SERVO_2__DM2) 
/* Input Buffer Disable Override */
#define SERVO_2_INP_DIS                (* (reg8 *) SERVO_2__INP_DIS)
/* LCD Common or Segment Drive */
#define SERVO_2_LCD_COM_SEG            (* (reg8 *) SERVO_2__LCD_COM_SEG)
/* Enable Segment LCD */
#define SERVO_2_LCD_EN                 (* (reg8 *) SERVO_2__LCD_EN)
/* Slew Rate Control */
#define SERVO_2_SLW                    (* (reg8 *) SERVO_2__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define SERVO_2_PRTDSI__CAPS_SEL       (* (reg8 *) SERVO_2__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define SERVO_2_PRTDSI__DBL_SYNC_IN    (* (reg8 *) SERVO_2__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define SERVO_2_PRTDSI__OE_SEL0        (* (reg8 *) SERVO_2__PRTDSI__OE_SEL0) 
#define SERVO_2_PRTDSI__OE_SEL1        (* (reg8 *) SERVO_2__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define SERVO_2_PRTDSI__OUT_SEL0       (* (reg8 *) SERVO_2__PRTDSI__OUT_SEL0) 
#define SERVO_2_PRTDSI__OUT_SEL1       (* (reg8 *) SERVO_2__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define SERVO_2_PRTDSI__SYNC_OUT       (* (reg8 *) SERVO_2__PRTDSI__SYNC_OUT) 


#if defined(SERVO_2__INTSTAT)  /* Interrupt Registers */

    #define SERVO_2_INTSTAT                (* (reg8 *) SERVO_2__INTSTAT)
    #define SERVO_2_SNAP                   (* (reg8 *) SERVO_2__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_SERVO_2_H */


/* [] END OF FILE */
