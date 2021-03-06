/*******************************************************************************
* File Name: TMR_Os_Tmr_Pre_Emp.h
* Version 2.70
*
*  Description:
*     Contains the function prototypes and constants available to the timer
*     user module.
*
*   Note:
*     None
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_Timer_v2_60_TMR_Os_Tmr_Pre_Emp_H)
#define CY_Timer_v2_60_TMR_Os_Tmr_Pre_Emp_H

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 TMR_Os_Tmr_Pre_Emp_initVar;

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component Timer_v2_70 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */


/**************************************
*           Parameter Defaults
**************************************/

#define TMR_Os_Tmr_Pre_Emp_Resolution                 16u
#define TMR_Os_Tmr_Pre_Emp_UsingFixedFunction         1u
#define TMR_Os_Tmr_Pre_Emp_UsingHWCaptureCounter      0u
#define TMR_Os_Tmr_Pre_Emp_SoftwareCaptureMode        0u
#define TMR_Os_Tmr_Pre_Emp_SoftwareTriggerMode        0u
#define TMR_Os_Tmr_Pre_Emp_UsingHWEnable              0u
#define TMR_Os_Tmr_Pre_Emp_EnableTriggerMode          0u
#define TMR_Os_Tmr_Pre_Emp_InterruptOnCaptureCount    0u
#define TMR_Os_Tmr_Pre_Emp_RunModeUsed                0u
#define TMR_Os_Tmr_Pre_Emp_ControlRegRemoved          0u

#if defined(TMR_Os_Tmr_Pre_Emp_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG)
    #define TMR_Os_Tmr_Pre_Emp_UDB_CONTROL_REG_REMOVED            (0u)
#elif  (TMR_Os_Tmr_Pre_Emp_UsingFixedFunction)
    #define TMR_Os_Tmr_Pre_Emp_UDB_CONTROL_REG_REMOVED            (0u)
#else 
    #define TMR_Os_Tmr_Pre_Emp_UDB_CONTROL_REG_REMOVED            (1u)
#endif /* End TMR_Os_Tmr_Pre_Emp_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG */


/***************************************
*       Type defines
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for Timer Component
 *************************************************************************/
typedef struct
{
    uint8 TimerEnableState;
    #if(!TMR_Os_Tmr_Pre_Emp_UsingFixedFunction)

        uint16 TimerUdb;
        uint8 InterruptMaskValue;
        #if (TMR_Os_Tmr_Pre_Emp_UsingHWCaptureCounter)
            uint8 TimerCaptureCounter;
        #endif /* variable declarations for backing up non retention registers in CY_UDB_V1 */

        #if (!TMR_Os_Tmr_Pre_Emp_UDB_CONTROL_REG_REMOVED)
            uint8 TimerControlRegister;
        #endif /* variable declaration for backing up enable state of the Timer */
    #endif /* define backup variables only for UDB implementation. Fixed function registers are all retention */

}TMR_Os_Tmr_Pre_Emp_backupStruct;


/***************************************
*       Function Prototypes
***************************************/

void    TMR_Os_Tmr_Pre_Emp_Start(void) ;
void    TMR_Os_Tmr_Pre_Emp_Stop(void) ;

void    TMR_Os_Tmr_Pre_Emp_SetInterruptMode(uint8 interruptMode) ;
uint8   TMR_Os_Tmr_Pre_Emp_ReadStatusRegister(void) ;
/* Deprecated function. Do not use this in future. Retained for backward compatibility */
#define TMR_Os_Tmr_Pre_Emp_GetInterruptSource() TMR_Os_Tmr_Pre_Emp_ReadStatusRegister()

#if(!TMR_Os_Tmr_Pre_Emp_UDB_CONTROL_REG_REMOVED)
    uint8   TMR_Os_Tmr_Pre_Emp_ReadControlRegister(void) ;
    void    TMR_Os_Tmr_Pre_Emp_WriteControlRegister(uint8 control) ;
#endif /* (!TMR_Os_Tmr_Pre_Emp_UDB_CONTROL_REG_REMOVED) */

uint16  TMR_Os_Tmr_Pre_Emp_ReadPeriod(void) ;
void    TMR_Os_Tmr_Pre_Emp_WritePeriod(uint16 period) ;
uint16  TMR_Os_Tmr_Pre_Emp_ReadCounter(void) ;
void    TMR_Os_Tmr_Pre_Emp_WriteCounter(uint16 counter) ;
uint16  TMR_Os_Tmr_Pre_Emp_ReadCapture(void) ;
void    TMR_Os_Tmr_Pre_Emp_SoftwareCapture(void) ;

#if(!TMR_Os_Tmr_Pre_Emp_UsingFixedFunction) /* UDB Prototypes */
    #if (TMR_Os_Tmr_Pre_Emp_SoftwareCaptureMode)
        void    TMR_Os_Tmr_Pre_Emp_SetCaptureMode(uint8 captureMode) ;
    #endif /* (!TMR_Os_Tmr_Pre_Emp_UsingFixedFunction) */

    #if (TMR_Os_Tmr_Pre_Emp_SoftwareTriggerMode)
        void    TMR_Os_Tmr_Pre_Emp_SetTriggerMode(uint8 triggerMode) ;
    #endif /* (TMR_Os_Tmr_Pre_Emp_SoftwareTriggerMode) */

    #if (TMR_Os_Tmr_Pre_Emp_EnableTriggerMode)
        void    TMR_Os_Tmr_Pre_Emp_EnableTrigger(void) ;
        void    TMR_Os_Tmr_Pre_Emp_DisableTrigger(void) ;
    #endif /* (TMR_Os_Tmr_Pre_Emp_EnableTriggerMode) */


    #if(TMR_Os_Tmr_Pre_Emp_InterruptOnCaptureCount)
        void    TMR_Os_Tmr_Pre_Emp_SetInterruptCount(uint8 interruptCount) ;
    #endif /* (TMR_Os_Tmr_Pre_Emp_InterruptOnCaptureCount) */

    #if (TMR_Os_Tmr_Pre_Emp_UsingHWCaptureCounter)
        void    TMR_Os_Tmr_Pre_Emp_SetCaptureCount(uint8 captureCount) ;
        uint8   TMR_Os_Tmr_Pre_Emp_ReadCaptureCount(void) ;
    #endif /* (TMR_Os_Tmr_Pre_Emp_UsingHWCaptureCounter) */

    void TMR_Os_Tmr_Pre_Emp_ClearFIFO(void) ;
#endif /* UDB Prototypes */

/* Sleep Retention APIs */
void TMR_Os_Tmr_Pre_Emp_Init(void)          ;
void TMR_Os_Tmr_Pre_Emp_Enable(void)        ;
void TMR_Os_Tmr_Pre_Emp_SaveConfig(void)    ;
void TMR_Os_Tmr_Pre_Emp_RestoreConfig(void) ;
void TMR_Os_Tmr_Pre_Emp_Sleep(void)         ;
void TMR_Os_Tmr_Pre_Emp_Wakeup(void)        ;


/***************************************
*   Enumerated Types and Parameters
***************************************/

/* Enumerated Type B_Timer__CaptureModes, Used in Capture Mode */
#define TMR_Os_Tmr_Pre_Emp__B_TIMER__CM_NONE 0
#define TMR_Os_Tmr_Pre_Emp__B_TIMER__CM_RISINGEDGE 1
#define TMR_Os_Tmr_Pre_Emp__B_TIMER__CM_FALLINGEDGE 2
#define TMR_Os_Tmr_Pre_Emp__B_TIMER__CM_EITHEREDGE 3
#define TMR_Os_Tmr_Pre_Emp__B_TIMER__CM_SOFTWARE 4



/* Enumerated Type B_Timer__TriggerModes, Used in Trigger Mode */
#define TMR_Os_Tmr_Pre_Emp__B_TIMER__TM_NONE 0x00u
#define TMR_Os_Tmr_Pre_Emp__B_TIMER__TM_RISINGEDGE 0x04u
#define TMR_Os_Tmr_Pre_Emp__B_TIMER__TM_FALLINGEDGE 0x08u
#define TMR_Os_Tmr_Pre_Emp__B_TIMER__TM_EITHEREDGE 0x0Cu
#define TMR_Os_Tmr_Pre_Emp__B_TIMER__TM_SOFTWARE 0x10u


/***************************************
*    Initialial Parameter Constants
***************************************/

#define TMR_Os_Tmr_Pre_Emp_INIT_PERIOD             99u
#define TMR_Os_Tmr_Pre_Emp_INIT_CAPTURE_MODE       ((uint8)((uint8)0u << TMR_Os_Tmr_Pre_Emp_CTRL_CAP_MODE_SHIFT))
#define TMR_Os_Tmr_Pre_Emp_INIT_TRIGGER_MODE       ((uint8)((uint8)0u << TMR_Os_Tmr_Pre_Emp_CTRL_TRIG_MODE_SHIFT))
#if (TMR_Os_Tmr_Pre_Emp_UsingFixedFunction)
    #define TMR_Os_Tmr_Pre_Emp_INIT_INTERRUPT_MODE (((uint8)((uint8)1u << TMR_Os_Tmr_Pre_Emp_STATUS_TC_INT_MASK_SHIFT)) | \
                                                  ((uint8)((uint8)0 << TMR_Os_Tmr_Pre_Emp_STATUS_CAPTURE_INT_MASK_SHIFT)))
#else
    #define TMR_Os_Tmr_Pre_Emp_INIT_INTERRUPT_MODE (((uint8)((uint8)1u << TMR_Os_Tmr_Pre_Emp_STATUS_TC_INT_MASK_SHIFT)) | \
                                                 ((uint8)((uint8)0 << TMR_Os_Tmr_Pre_Emp_STATUS_CAPTURE_INT_MASK_SHIFT)) | \
                                                 ((uint8)((uint8)0 << TMR_Os_Tmr_Pre_Emp_STATUS_FIFOFULL_INT_MASK_SHIFT)))
#endif /* (TMR_Os_Tmr_Pre_Emp_UsingFixedFunction) */
#define TMR_Os_Tmr_Pre_Emp_INIT_CAPTURE_COUNT      (2u)
#define TMR_Os_Tmr_Pre_Emp_INIT_INT_CAPTURE_COUNT  ((uint8)((uint8)(1u - 1u) << TMR_Os_Tmr_Pre_Emp_CTRL_INTCNT_SHIFT))


/***************************************
*           Registers
***************************************/

#if (TMR_Os_Tmr_Pre_Emp_UsingFixedFunction) /* Implementation Specific Registers and Register Constants */


    /***************************************
    *    Fixed Function Registers
    ***************************************/

    #define TMR_Os_Tmr_Pre_Emp_STATUS         (*(reg8 *) TMR_Os_Tmr_Pre_Emp_TimerHW__SR0 )
    /* In Fixed Function Block Status and Mask are the same register */
    #define TMR_Os_Tmr_Pre_Emp_STATUS_MASK    (*(reg8 *) TMR_Os_Tmr_Pre_Emp_TimerHW__SR0 )
    #define TMR_Os_Tmr_Pre_Emp_CONTROL        (*(reg8 *) TMR_Os_Tmr_Pre_Emp_TimerHW__CFG0)
    #define TMR_Os_Tmr_Pre_Emp_CONTROL2       (*(reg8 *) TMR_Os_Tmr_Pre_Emp_TimerHW__CFG1)
    #define TMR_Os_Tmr_Pre_Emp_CONTROL2_PTR   ( (reg8 *) TMR_Os_Tmr_Pre_Emp_TimerHW__CFG1)
    #define TMR_Os_Tmr_Pre_Emp_RT1            (*(reg8 *) TMR_Os_Tmr_Pre_Emp_TimerHW__RT1)
    #define TMR_Os_Tmr_Pre_Emp_RT1_PTR        ( (reg8 *) TMR_Os_Tmr_Pre_Emp_TimerHW__RT1)

    #if (CY_PSOC3 || CY_PSOC5LP)
        #define TMR_Os_Tmr_Pre_Emp_CONTROL3       (*(reg8 *) TMR_Os_Tmr_Pre_Emp_TimerHW__CFG2)
        #define TMR_Os_Tmr_Pre_Emp_CONTROL3_PTR   ( (reg8 *) TMR_Os_Tmr_Pre_Emp_TimerHW__CFG2)
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */
    #define TMR_Os_Tmr_Pre_Emp_GLOBAL_ENABLE  (*(reg8 *) TMR_Os_Tmr_Pre_Emp_TimerHW__PM_ACT_CFG)
    #define TMR_Os_Tmr_Pre_Emp_GLOBAL_STBY_ENABLE  (*(reg8 *) TMR_Os_Tmr_Pre_Emp_TimerHW__PM_STBY_CFG)

    #define TMR_Os_Tmr_Pre_Emp_CAPTURE_LSB         (* (reg16 *) TMR_Os_Tmr_Pre_Emp_TimerHW__CAP0 )
    #define TMR_Os_Tmr_Pre_Emp_CAPTURE_LSB_PTR       ((reg16 *) TMR_Os_Tmr_Pre_Emp_TimerHW__CAP0 )
    #define TMR_Os_Tmr_Pre_Emp_PERIOD_LSB          (* (reg16 *) TMR_Os_Tmr_Pre_Emp_TimerHW__PER0 )
    #define TMR_Os_Tmr_Pre_Emp_PERIOD_LSB_PTR        ((reg16 *) TMR_Os_Tmr_Pre_Emp_TimerHW__PER0 )
    #define TMR_Os_Tmr_Pre_Emp_COUNTER_LSB         (* (reg16 *) TMR_Os_Tmr_Pre_Emp_TimerHW__CNT_CMP0 )
    #define TMR_Os_Tmr_Pre_Emp_COUNTER_LSB_PTR       ((reg16 *) TMR_Os_Tmr_Pre_Emp_TimerHW__CNT_CMP0 )


    /***************************************
    *    Register Constants
    ***************************************/

    /* Fixed Function Block Chosen */
    #define TMR_Os_Tmr_Pre_Emp_BLOCK_EN_MASK                     TMR_Os_Tmr_Pre_Emp_TimerHW__PM_ACT_MSK
    #define TMR_Os_Tmr_Pre_Emp_BLOCK_STBY_EN_MASK                TMR_Os_Tmr_Pre_Emp_TimerHW__PM_STBY_MSK

    /* Control Register Bit Locations */
    /* Interrupt Count - Not valid for Fixed Function Block */
    #define TMR_Os_Tmr_Pre_Emp_CTRL_INTCNT_SHIFT                  0x00u
    /* Trigger Polarity - Not valid for Fixed Function Block */
    #define TMR_Os_Tmr_Pre_Emp_CTRL_TRIG_MODE_SHIFT               0x00u
    /* Trigger Enable - Not valid for Fixed Function Block */
    #define TMR_Os_Tmr_Pre_Emp_CTRL_TRIG_EN_SHIFT                 0x00u
    /* Capture Polarity - Not valid for Fixed Function Block */
    #define TMR_Os_Tmr_Pre_Emp_CTRL_CAP_MODE_SHIFT                0x00u
    /* Timer Enable - As defined in Register Map, part of TMRX_CFG0 register */
    #define TMR_Os_Tmr_Pre_Emp_CTRL_ENABLE_SHIFT                  0x00u

    /* Control Register Bit Masks */
    #define TMR_Os_Tmr_Pre_Emp_CTRL_ENABLE                        ((uint8)((uint8)0x01u << TMR_Os_Tmr_Pre_Emp_CTRL_ENABLE_SHIFT))

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define TMR_Os_Tmr_Pre_Emp_CTRL2_IRQ_SEL_SHIFT                 0x00u
    #define TMR_Os_Tmr_Pre_Emp_CTRL2_IRQ_SEL                      ((uint8)((uint8)0x01u << TMR_Os_Tmr_Pre_Emp_CTRL2_IRQ_SEL_SHIFT))

    #if (CY_PSOC5A)
        /* Use CFG1 Mode bits to set run mode */
        /* As defined by Verilog Implementation */
        #define TMR_Os_Tmr_Pre_Emp_CTRL_MODE_SHIFT                 0x01u
        #define TMR_Os_Tmr_Pre_Emp_CTRL_MODE_MASK                 ((uint8)((uint8)0x07u << TMR_Os_Tmr_Pre_Emp_CTRL_MODE_SHIFT))
    #endif /* (CY_PSOC5A) */
    #if (CY_PSOC3 || CY_PSOC5LP)
        /* Control3 Register Bit Locations */
        #define TMR_Os_Tmr_Pre_Emp_CTRL_RCOD_SHIFT        0x02u
        #define TMR_Os_Tmr_Pre_Emp_CTRL_ENBL_SHIFT        0x00u
        #define TMR_Os_Tmr_Pre_Emp_CTRL_MODE_SHIFT        0x00u

        /* Control3 Register Bit Masks */
        #define TMR_Os_Tmr_Pre_Emp_CTRL_RCOD_MASK  ((uint8)((uint8)0x03u << TMR_Os_Tmr_Pre_Emp_CTRL_RCOD_SHIFT)) /* ROD and COD bit masks */
        #define TMR_Os_Tmr_Pre_Emp_CTRL_ENBL_MASK  ((uint8)((uint8)0x80u << TMR_Os_Tmr_Pre_Emp_CTRL_ENBL_SHIFT)) /* HW_EN bit mask */
        #define TMR_Os_Tmr_Pre_Emp_CTRL_MODE_MASK  ((uint8)((uint8)0x03u << TMR_Os_Tmr_Pre_Emp_CTRL_MODE_SHIFT)) /* Run mode bit mask */

        #define TMR_Os_Tmr_Pre_Emp_CTRL_RCOD       ((uint8)((uint8)0x03u << TMR_Os_Tmr_Pre_Emp_CTRL_RCOD_SHIFT))
        #define TMR_Os_Tmr_Pre_Emp_CTRL_ENBL       ((uint8)((uint8)0x80u << TMR_Os_Tmr_Pre_Emp_CTRL_ENBL_SHIFT))
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */

    /*RT1 Synch Constants: Applicable for PSoC3 and PSoC5LP */
    #define TMR_Os_Tmr_Pre_Emp_RT1_SHIFT                       0x04u
    /* Sync TC and CMP bit masks */
    #define TMR_Os_Tmr_Pre_Emp_RT1_MASK                        ((uint8)((uint8)0x03u << TMR_Os_Tmr_Pre_Emp_RT1_SHIFT))
    #define TMR_Os_Tmr_Pre_Emp_SYNC                            ((uint8)((uint8)0x03u << TMR_Os_Tmr_Pre_Emp_RT1_SHIFT))
    #define TMR_Os_Tmr_Pre_Emp_SYNCDSI_SHIFT                   0x00u
    /* Sync all DSI inputs with Mask  */
    #define TMR_Os_Tmr_Pre_Emp_SYNCDSI_MASK                    ((uint8)((uint8)0x0Fu << TMR_Os_Tmr_Pre_Emp_SYNCDSI_SHIFT))
    /* Sync all DSI inputs */
    #define TMR_Os_Tmr_Pre_Emp_SYNCDSI_EN                      ((uint8)((uint8)0x0Fu << TMR_Os_Tmr_Pre_Emp_SYNCDSI_SHIFT))

    #define TMR_Os_Tmr_Pre_Emp_CTRL_MODE_PULSEWIDTH            ((uint8)((uint8)0x01u << TMR_Os_Tmr_Pre_Emp_CTRL_MODE_SHIFT))
    #define TMR_Os_Tmr_Pre_Emp_CTRL_MODE_PERIOD                ((uint8)((uint8)0x02u << TMR_Os_Tmr_Pre_Emp_CTRL_MODE_SHIFT))
    #define TMR_Os_Tmr_Pre_Emp_CTRL_MODE_CONTINUOUS            ((uint8)((uint8)0x00u << TMR_Os_Tmr_Pre_Emp_CTRL_MODE_SHIFT))

    /* Status Register Bit Locations */
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define TMR_Os_Tmr_Pre_Emp_STATUS_TC_SHIFT                 0x07u
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define TMR_Os_Tmr_Pre_Emp_STATUS_CAPTURE_SHIFT            0x06u
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define TMR_Os_Tmr_Pre_Emp_STATUS_TC_INT_MASK_SHIFT        (TMR_Os_Tmr_Pre_Emp_STATUS_TC_SHIFT - 0x04u)
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define TMR_Os_Tmr_Pre_Emp_STATUS_CAPTURE_INT_MASK_SHIFT   (TMR_Os_Tmr_Pre_Emp_STATUS_CAPTURE_SHIFT - 0x04u)

    /* Status Register Bit Masks */
    #define TMR_Os_Tmr_Pre_Emp_STATUS_TC                       ((uint8)((uint8)0x01u << TMR_Os_Tmr_Pre_Emp_STATUS_TC_SHIFT))
    #define TMR_Os_Tmr_Pre_Emp_STATUS_CAPTURE                  ((uint8)((uint8)0x01u << TMR_Os_Tmr_Pre_Emp_STATUS_CAPTURE_SHIFT))
    /* Interrupt Enable Bit-Mask for interrupt on TC */
    #define TMR_Os_Tmr_Pre_Emp_STATUS_TC_INT_MASK              ((uint8)((uint8)0x01u << TMR_Os_Tmr_Pre_Emp_STATUS_TC_INT_MASK_SHIFT))
    /* Interrupt Enable Bit-Mask for interrupt on Capture */
    #define TMR_Os_Tmr_Pre_Emp_STATUS_CAPTURE_INT_MASK         ((uint8)((uint8)0x01u << TMR_Os_Tmr_Pre_Emp_STATUS_CAPTURE_INT_MASK_SHIFT))

#else   /* UDB Registers and Register Constants */


    /***************************************
    *           UDB Registers
    ***************************************/

    #define TMR_Os_Tmr_Pre_Emp_STATUS              (* (reg8 *) TMR_Os_Tmr_Pre_Emp_TimerUDB_rstSts_stsreg__STATUS_REG )
    #define TMR_Os_Tmr_Pre_Emp_STATUS_MASK         (* (reg8 *) TMR_Os_Tmr_Pre_Emp_TimerUDB_rstSts_stsreg__MASK_REG)
    #define TMR_Os_Tmr_Pre_Emp_STATUS_AUX_CTRL     (* (reg8 *) TMR_Os_Tmr_Pre_Emp_TimerUDB_rstSts_stsreg__STATUS_AUX_CTL_REG)
    #define TMR_Os_Tmr_Pre_Emp_CONTROL             (* (reg8 *) TMR_Os_Tmr_Pre_Emp_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG )
    
    #if(TMR_Os_Tmr_Pre_Emp_Resolution <= 8u) /* 8-bit Timer */
        #define TMR_Os_Tmr_Pre_Emp_CAPTURE_LSB         (* (reg8 *) TMR_Os_Tmr_Pre_Emp_TimerUDB_sT16_timerdp_u0__F0_REG )
        #define TMR_Os_Tmr_Pre_Emp_CAPTURE_LSB_PTR       ((reg8 *) TMR_Os_Tmr_Pre_Emp_TimerUDB_sT16_timerdp_u0__F0_REG )
        #define TMR_Os_Tmr_Pre_Emp_PERIOD_LSB          (* (reg8 *) TMR_Os_Tmr_Pre_Emp_TimerUDB_sT16_timerdp_u0__D0_REG )
        #define TMR_Os_Tmr_Pre_Emp_PERIOD_LSB_PTR        ((reg8 *) TMR_Os_Tmr_Pre_Emp_TimerUDB_sT16_timerdp_u0__D0_REG )
        #define TMR_Os_Tmr_Pre_Emp_COUNTER_LSB         (* (reg8 *) TMR_Os_Tmr_Pre_Emp_TimerUDB_sT16_timerdp_u0__A0_REG )
        #define TMR_Os_Tmr_Pre_Emp_COUNTER_LSB_PTR       ((reg8 *) TMR_Os_Tmr_Pre_Emp_TimerUDB_sT16_timerdp_u0__A0_REG )
    #elif(TMR_Os_Tmr_Pre_Emp_Resolution <= 16u) /* 8-bit Timer */
        #if(CY_PSOC3) /* 8-bit addres space */
            #define TMR_Os_Tmr_Pre_Emp_CAPTURE_LSB         (* (reg16 *) TMR_Os_Tmr_Pre_Emp_TimerUDB_sT16_timerdp_u0__F0_REG )
            #define TMR_Os_Tmr_Pre_Emp_CAPTURE_LSB_PTR       ((reg16 *) TMR_Os_Tmr_Pre_Emp_TimerUDB_sT16_timerdp_u0__F0_REG )
            #define TMR_Os_Tmr_Pre_Emp_PERIOD_LSB          (* (reg16 *) TMR_Os_Tmr_Pre_Emp_TimerUDB_sT16_timerdp_u0__D0_REG )
            #define TMR_Os_Tmr_Pre_Emp_PERIOD_LSB_PTR        ((reg16 *) TMR_Os_Tmr_Pre_Emp_TimerUDB_sT16_timerdp_u0__D0_REG )
            #define TMR_Os_Tmr_Pre_Emp_COUNTER_LSB         (* (reg16 *) TMR_Os_Tmr_Pre_Emp_TimerUDB_sT16_timerdp_u0__A0_REG )
            #define TMR_Os_Tmr_Pre_Emp_COUNTER_LSB_PTR       ((reg16 *) TMR_Os_Tmr_Pre_Emp_TimerUDB_sT16_timerdp_u0__A0_REG )
        #else /* 16-bit address space */
            #define TMR_Os_Tmr_Pre_Emp_CAPTURE_LSB         (* (reg16 *) TMR_Os_Tmr_Pre_Emp_TimerUDB_sT16_timerdp_u0__16BIT_F0_REG )
            #define TMR_Os_Tmr_Pre_Emp_CAPTURE_LSB_PTR       ((reg16 *) TMR_Os_Tmr_Pre_Emp_TimerUDB_sT16_timerdp_u0__16BIT_F0_REG )
            #define TMR_Os_Tmr_Pre_Emp_PERIOD_LSB          (* (reg16 *) TMR_Os_Tmr_Pre_Emp_TimerUDB_sT16_timerdp_u0__16BIT_D0_REG )
            #define TMR_Os_Tmr_Pre_Emp_PERIOD_LSB_PTR        ((reg16 *) TMR_Os_Tmr_Pre_Emp_TimerUDB_sT16_timerdp_u0__16BIT_D0_REG )
            #define TMR_Os_Tmr_Pre_Emp_COUNTER_LSB         (* (reg16 *) TMR_Os_Tmr_Pre_Emp_TimerUDB_sT16_timerdp_u0__16BIT_A0_REG )
            #define TMR_Os_Tmr_Pre_Emp_COUNTER_LSB_PTR       ((reg16 *) TMR_Os_Tmr_Pre_Emp_TimerUDB_sT16_timerdp_u0__16BIT_A0_REG )
        #endif /* CY_PSOC3 */
    #elif(TMR_Os_Tmr_Pre_Emp_Resolution <= 24u)/* 24-bit Timer */
        #define TMR_Os_Tmr_Pre_Emp_CAPTURE_LSB         (* (reg32 *) TMR_Os_Tmr_Pre_Emp_TimerUDB_sT16_timerdp_u0__F0_REG )
        #define TMR_Os_Tmr_Pre_Emp_CAPTURE_LSB_PTR       ((reg32 *) TMR_Os_Tmr_Pre_Emp_TimerUDB_sT16_timerdp_u0__F0_REG )
        #define TMR_Os_Tmr_Pre_Emp_PERIOD_LSB          (* (reg32 *) TMR_Os_Tmr_Pre_Emp_TimerUDB_sT16_timerdp_u0__D0_REG )
        #define TMR_Os_Tmr_Pre_Emp_PERIOD_LSB_PTR        ((reg32 *) TMR_Os_Tmr_Pre_Emp_TimerUDB_sT16_timerdp_u0__D0_REG )
        #define TMR_Os_Tmr_Pre_Emp_COUNTER_LSB         (* (reg32 *) TMR_Os_Tmr_Pre_Emp_TimerUDB_sT16_timerdp_u0__A0_REG )
        #define TMR_Os_Tmr_Pre_Emp_COUNTER_LSB_PTR       ((reg32 *) TMR_Os_Tmr_Pre_Emp_TimerUDB_sT16_timerdp_u0__A0_REG )
    #else /* 32-bit Timer */
        #if(CY_PSOC3 || CY_PSOC5) /* 8-bit address space */
            #define TMR_Os_Tmr_Pre_Emp_CAPTURE_LSB         (* (reg32 *) TMR_Os_Tmr_Pre_Emp_TimerUDB_sT16_timerdp_u0__F0_REG )
            #define TMR_Os_Tmr_Pre_Emp_CAPTURE_LSB_PTR       ((reg32 *) TMR_Os_Tmr_Pre_Emp_TimerUDB_sT16_timerdp_u0__F0_REG )
            #define TMR_Os_Tmr_Pre_Emp_PERIOD_LSB          (* (reg32 *) TMR_Os_Tmr_Pre_Emp_TimerUDB_sT16_timerdp_u0__D0_REG )
            #define TMR_Os_Tmr_Pre_Emp_PERIOD_LSB_PTR        ((reg32 *) TMR_Os_Tmr_Pre_Emp_TimerUDB_sT16_timerdp_u0__D0_REG )
            #define TMR_Os_Tmr_Pre_Emp_COUNTER_LSB         (* (reg32 *) TMR_Os_Tmr_Pre_Emp_TimerUDB_sT16_timerdp_u0__A0_REG )
            #define TMR_Os_Tmr_Pre_Emp_COUNTER_LSB_PTR       ((reg32 *) TMR_Os_Tmr_Pre_Emp_TimerUDB_sT16_timerdp_u0__A0_REG )
        #else /* 32-bit address space */
            #define TMR_Os_Tmr_Pre_Emp_CAPTURE_LSB         (* (reg32 *) TMR_Os_Tmr_Pre_Emp_TimerUDB_sT16_timerdp_u0__32BIT_F0_REG )
            #define TMR_Os_Tmr_Pre_Emp_CAPTURE_LSB_PTR       ((reg32 *) TMR_Os_Tmr_Pre_Emp_TimerUDB_sT16_timerdp_u0__32BIT_F0_REG )
            #define TMR_Os_Tmr_Pre_Emp_PERIOD_LSB          (* (reg32 *) TMR_Os_Tmr_Pre_Emp_TimerUDB_sT16_timerdp_u0__32BIT_D0_REG )
            #define TMR_Os_Tmr_Pre_Emp_PERIOD_LSB_PTR        ((reg32 *) TMR_Os_Tmr_Pre_Emp_TimerUDB_sT16_timerdp_u0__32BIT_D0_REG )
            #define TMR_Os_Tmr_Pre_Emp_COUNTER_LSB         (* (reg32 *) TMR_Os_Tmr_Pre_Emp_TimerUDB_sT16_timerdp_u0__32BIT_A0_REG )
            #define TMR_Os_Tmr_Pre_Emp_COUNTER_LSB_PTR       ((reg32 *) TMR_Os_Tmr_Pre_Emp_TimerUDB_sT16_timerdp_u0__32BIT_A0_REG )
        #endif /* CY_PSOC3 || CY_PSOC5 */ 
    #endif

    #define TMR_Os_Tmr_Pre_Emp_COUNTER_LSB_PTR_8BIT       ((reg8 *) TMR_Os_Tmr_Pre_Emp_TimerUDB_sT16_timerdp_u0__A0_REG )
    
    #if (TMR_Os_Tmr_Pre_Emp_UsingHWCaptureCounter)
        #define TMR_Os_Tmr_Pre_Emp_CAP_COUNT              (*(reg8 *) TMR_Os_Tmr_Pre_Emp_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define TMR_Os_Tmr_Pre_Emp_CAP_COUNT_PTR          ( (reg8 *) TMR_Os_Tmr_Pre_Emp_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define TMR_Os_Tmr_Pre_Emp_CAPTURE_COUNT_CTRL     (*(reg8 *) TMR_Os_Tmr_Pre_Emp_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
        #define TMR_Os_Tmr_Pre_Emp_CAPTURE_COUNT_CTRL_PTR ( (reg8 *) TMR_Os_Tmr_Pre_Emp_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
    #endif /* (TMR_Os_Tmr_Pre_Emp_UsingHWCaptureCounter) */


    /***************************************
    *       Register Constants
    ***************************************/

    /* Control Register Bit Locations */
    #define TMR_Os_Tmr_Pre_Emp_CTRL_INTCNT_SHIFT              0x00u       /* As defined by Verilog Implementation */
    #define TMR_Os_Tmr_Pre_Emp_CTRL_TRIG_MODE_SHIFT           0x02u       /* As defined by Verilog Implementation */
    #define TMR_Os_Tmr_Pre_Emp_CTRL_TRIG_EN_SHIFT             0x04u       /* As defined by Verilog Implementation */
    #define TMR_Os_Tmr_Pre_Emp_CTRL_CAP_MODE_SHIFT            0x05u       /* As defined by Verilog Implementation */
    #define TMR_Os_Tmr_Pre_Emp_CTRL_ENABLE_SHIFT              0x07u       /* As defined by Verilog Implementation */

    /* Control Register Bit Masks */
    #define TMR_Os_Tmr_Pre_Emp_CTRL_INTCNT_MASK               ((uint8)((uint8)0x03u << TMR_Os_Tmr_Pre_Emp_CTRL_INTCNT_SHIFT))
    #define TMR_Os_Tmr_Pre_Emp_CTRL_TRIG_MODE_MASK            ((uint8)((uint8)0x03u << TMR_Os_Tmr_Pre_Emp_CTRL_TRIG_MODE_SHIFT))
    #define TMR_Os_Tmr_Pre_Emp_CTRL_TRIG_EN                   ((uint8)((uint8)0x01u << TMR_Os_Tmr_Pre_Emp_CTRL_TRIG_EN_SHIFT))
    #define TMR_Os_Tmr_Pre_Emp_CTRL_CAP_MODE_MASK             ((uint8)((uint8)0x03u << TMR_Os_Tmr_Pre_Emp_CTRL_CAP_MODE_SHIFT))
    #define TMR_Os_Tmr_Pre_Emp_CTRL_ENABLE                    ((uint8)((uint8)0x01u << TMR_Os_Tmr_Pre_Emp_CTRL_ENABLE_SHIFT))

    /* Bit Counter (7-bit) Control Register Bit Definitions */
    /* As defined by the Register map for the AUX Control Register */
    #define TMR_Os_Tmr_Pre_Emp_CNTR_ENABLE                    0x20u

    /* Status Register Bit Locations */
    #define TMR_Os_Tmr_Pre_Emp_STATUS_TC_SHIFT                0x00u  /* As defined by Verilog Implementation */
    #define TMR_Os_Tmr_Pre_Emp_STATUS_CAPTURE_SHIFT           0x01u  /* As defined by Verilog Implementation */
    #define TMR_Os_Tmr_Pre_Emp_STATUS_TC_INT_MASK_SHIFT       TMR_Os_Tmr_Pre_Emp_STATUS_TC_SHIFT
    #define TMR_Os_Tmr_Pre_Emp_STATUS_CAPTURE_INT_MASK_SHIFT  TMR_Os_Tmr_Pre_Emp_STATUS_CAPTURE_SHIFT
    #define TMR_Os_Tmr_Pre_Emp_STATUS_FIFOFULL_SHIFT          0x02u  /* As defined by Verilog Implementation */
    #define TMR_Os_Tmr_Pre_Emp_STATUS_FIFONEMP_SHIFT          0x03u  /* As defined by Verilog Implementation */
    #define TMR_Os_Tmr_Pre_Emp_STATUS_FIFOFULL_INT_MASK_SHIFT TMR_Os_Tmr_Pre_Emp_STATUS_FIFOFULL_SHIFT

    /* Status Register Bit Masks */
    /* Sticky TC Event Bit-Mask */
    #define TMR_Os_Tmr_Pre_Emp_STATUS_TC                      ((uint8)((uint8)0x01u << TMR_Os_Tmr_Pre_Emp_STATUS_TC_SHIFT))
    /* Sticky Capture Event Bit-Mask */
    #define TMR_Os_Tmr_Pre_Emp_STATUS_CAPTURE                 ((uint8)((uint8)0x01u << TMR_Os_Tmr_Pre_Emp_STATUS_CAPTURE_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define TMR_Os_Tmr_Pre_Emp_STATUS_TC_INT_MASK             ((uint8)((uint8)0x01u << TMR_Os_Tmr_Pre_Emp_STATUS_TC_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define TMR_Os_Tmr_Pre_Emp_STATUS_CAPTURE_INT_MASK        ((uint8)((uint8)0x01u << TMR_Os_Tmr_Pre_Emp_STATUS_CAPTURE_SHIFT))
    /* NOT-Sticky FIFO Full Bit-Mask */
    #define TMR_Os_Tmr_Pre_Emp_STATUS_FIFOFULL                ((uint8)((uint8)0x01u << TMR_Os_Tmr_Pre_Emp_STATUS_FIFOFULL_SHIFT))
    /* NOT-Sticky FIFO Not Empty Bit-Mask */
    #define TMR_Os_Tmr_Pre_Emp_STATUS_FIFONEMP                ((uint8)((uint8)0x01u << TMR_Os_Tmr_Pre_Emp_STATUS_FIFONEMP_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define TMR_Os_Tmr_Pre_Emp_STATUS_FIFOFULL_INT_MASK       ((uint8)((uint8)0x01u << TMR_Os_Tmr_Pre_Emp_STATUS_FIFOFULL_SHIFT))

    #define TMR_Os_Tmr_Pre_Emp_STATUS_ACTL_INT_EN             0x10u   /* As defined for the ACTL Register */

    /* Datapath Auxillary Control Register definitions */
    #define TMR_Os_Tmr_Pre_Emp_AUX_CTRL_FIFO0_CLR             0x01u   /* As defined by Register map */
    #define TMR_Os_Tmr_Pre_Emp_AUX_CTRL_FIFO1_CLR             0x02u   /* As defined by Register map */
    #define TMR_Os_Tmr_Pre_Emp_AUX_CTRL_FIFO0_LVL             0x04u   /* As defined by Register map */
    #define TMR_Os_Tmr_Pre_Emp_AUX_CTRL_FIFO1_LVL             0x08u   /* As defined by Register map */
    #define TMR_Os_Tmr_Pre_Emp_STATUS_ACTL_INT_EN_MASK        0x10u   /* As defined for the ACTL Register */

#endif /* Implementation Specific Registers and Register Constants */

#endif  /* CY_Timer_v2_30_TMR_Os_Tmr_Pre_Emp_H */


/* [] END OF FILE */
