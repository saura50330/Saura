/*******************************************************************************
* File Name: TMR_Os_Tmr_Co_Op.c
* Version 2.70
*
* Description:
*  The Timer component consists of a 8, 16, 24 or 32-bit timer with
*  a selectable period between 2 and 2^Width - 1.  The timer may free run
*  or be used as a capture timer as well.  The capture can be initiated
*  by a positive or negative edge signal as well as via software.
*  A trigger input can be programmed to enable the timer on rising edge
*  falling edge, either edge or continous run.
*  Interrupts may be generated due to a terminal count condition
*  or a capture event.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "TMR_Os_Tmr_Co_Op.h"

uint8 TMR_Os_Tmr_Co_Op_initVar = 0u;


/*******************************************************************************
* Function Name: TMR_Os_Tmr_Co_Op_Init
********************************************************************************
*
* Summary:
*  Initialize to the schematic state
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void TMR_Os_Tmr_Co_Op_Init(void) 
{
    #if(!TMR_Os_Tmr_Co_Op_UsingFixedFunction)
            /* Interrupt State Backup for Critical Region*/
            uint8 TMR_Os_Tmr_Co_Op_interruptState;
    #endif /* Interrupt state back up for Fixed Function only */

    #if (TMR_Os_Tmr_Co_Op_UsingFixedFunction)
        /* Clear all bits but the enable bit (if it's already set) for Timer operation */
        TMR_Os_Tmr_Co_Op_CONTROL &= TMR_Os_Tmr_Co_Op_CTRL_ENABLE;

        /* Clear the mode bits for continuous run mode */
        #if (CY_PSOC5A)
            TMR_Os_Tmr_Co_Op_CONTROL2 &= ((uint8)(~TMR_Os_Tmr_Co_Op_CTRL_MODE_MASK));
        #endif /* Clear bits in CONTROL2 only in PSOC5A */

        #if (CY_PSOC3 || CY_PSOC5LP)
            TMR_Os_Tmr_Co_Op_CONTROL3 &= ((uint8)(~TMR_Os_Tmr_Co_Op_CTRL_MODE_MASK));
        #endif /* CONTROL3 register exists only in PSoC3 OR PSoC5LP */

        /* Check if One Shot mode is enabled i.e. RunMode !=0*/
        #if (TMR_Os_Tmr_Co_Op_RunModeUsed != 0x0u)
            /* Set 3rd bit of Control register to enable one shot mode */
            TMR_Os_Tmr_Co_Op_CONTROL |= 0x04u;
        #endif /* One Shot enabled only when RunModeUsed is not Continuous*/

        #if (TMR_Os_Tmr_Co_Op_RunModeUsed == 2)
            #if (CY_PSOC5A)
                /* Set last 2 bits of control2 register if one shot(halt on
                interrupt) is enabled*/
                TMR_Os_Tmr_Co_Op_CONTROL2 |= 0x03u;
            #endif /* Set One-Shot Halt on Interrupt bit in CONTROL2 for PSoC5A */

            #if (CY_PSOC3 || CY_PSOC5LP)
                /* Set last 2 bits of control3 register if one shot(halt on
                interrupt) is enabled*/
                TMR_Os_Tmr_Co_Op_CONTROL3 |= 0x03u;
            #endif /* Set One-Shot Halt on Interrupt bit in CONTROL3 for PSoC3 or PSoC5LP */

        #endif /* Remove section if One Shot Halt on Interrupt is not enabled */

        #if (TMR_Os_Tmr_Co_Op_UsingHWEnable != 0)
            #if (CY_PSOC5A)
                /* Set the default Run Mode of the Timer to Continuous */
                TMR_Os_Tmr_Co_Op_CONTROL2 |= TMR_Os_Tmr_Co_Op_CTRL_MODE_PULSEWIDTH;
            #endif /* Set Continuous Run Mode in CONTROL2 for PSoC5A */

            #if (CY_PSOC3 || CY_PSOC5LP)
                /* Clear and Set ROD and COD bits of CFG2 register */
                TMR_Os_Tmr_Co_Op_CONTROL3 &= ((uint8)(~TMR_Os_Tmr_Co_Op_CTRL_RCOD_MASK));
                TMR_Os_Tmr_Co_Op_CONTROL3 |= TMR_Os_Tmr_Co_Op_CTRL_RCOD;

                /* Clear and Enable the HW enable bit in CFG2 register */
                TMR_Os_Tmr_Co_Op_CONTROL3 &= ((uint8)(~TMR_Os_Tmr_Co_Op_CTRL_ENBL_MASK));
                TMR_Os_Tmr_Co_Op_CONTROL3 |= TMR_Os_Tmr_Co_Op_CTRL_ENBL;

                /* Set the default Run Mode of the Timer to Continuous */
                TMR_Os_Tmr_Co_Op_CONTROL3 |= TMR_Os_Tmr_Co_Op_CTRL_MODE_CONTINUOUS;
            #endif /* Set Continuous Run Mode in CONTROL3 for PSoC3ES3 or PSoC5A */

        #endif /* Configure Run Mode with hardware enable */

        /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
        TMR_Os_Tmr_Co_Op_RT1 &= ((uint8)(~TMR_Os_Tmr_Co_Op_RT1_MASK));
        TMR_Os_Tmr_Co_Op_RT1 |= TMR_Os_Tmr_Co_Op_SYNC;

        /*Enable DSI Sync all all inputs of the Timer*/
        TMR_Os_Tmr_Co_Op_RT1 &= ((uint8)(~TMR_Os_Tmr_Co_Op_SYNCDSI_MASK));
        TMR_Os_Tmr_Co_Op_RT1 |= TMR_Os_Tmr_Co_Op_SYNCDSI_EN;

        /* Set the IRQ to use the status register interrupts */
        TMR_Os_Tmr_Co_Op_CONTROL2 |= TMR_Os_Tmr_Co_Op_CTRL2_IRQ_SEL;
    #endif /* Configuring registers of fixed function implementation */

    /* Set Initial values from Configuration */
    TMR_Os_Tmr_Co_Op_WritePeriod(TMR_Os_Tmr_Co_Op_INIT_PERIOD);
    TMR_Os_Tmr_Co_Op_WriteCounter(TMR_Os_Tmr_Co_Op_INIT_PERIOD);

    #if (TMR_Os_Tmr_Co_Op_UsingHWCaptureCounter)/* Capture counter is enabled */
        TMR_Os_Tmr_Co_Op_CAPTURE_COUNT_CTRL |= TMR_Os_Tmr_Co_Op_CNTR_ENABLE;
        TMR_Os_Tmr_Co_Op_SetCaptureCount(TMR_Os_Tmr_Co_Op_INIT_CAPTURE_COUNT);
    #endif /* Configure capture counter value */

    #if (!TMR_Os_Tmr_Co_Op_UsingFixedFunction)
        #if (TMR_Os_Tmr_Co_Op_SoftwareCaptureMode)
            TMR_Os_Tmr_Co_Op_SetCaptureMode(TMR_Os_Tmr_Co_Op_INIT_CAPTURE_MODE);
        #endif /* Set Capture Mode for UDB implementation if capture mode is software controlled */

        #if (TMR_Os_Tmr_Co_Op_SoftwareTriggerMode)
            #if (!TMR_Os_Tmr_Co_Op_UDB_CONTROL_REG_REMOVED)
                if (0u == (TMR_Os_Tmr_Co_Op_CONTROL & TMR_Os_Tmr_Co_Op__B_TIMER__TM_SOFTWARE))
                {
                    TMR_Os_Tmr_Co_Op_SetTriggerMode(TMR_Os_Tmr_Co_Op_INIT_TRIGGER_MODE);
                }
            #endif /* (!TMR_Os_Tmr_Co_Op_UDB_CONTROL_REG_REMOVED) */
        #endif /* Set trigger mode for UDB Implementation if trigger mode is software controlled */

        /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
        /* Enter Critical Region*/
        TMR_Os_Tmr_Co_Op_interruptState = CyEnterCriticalSection();

        /* Use the interrupt output of the status register for IRQ output */
        TMR_Os_Tmr_Co_Op_STATUS_AUX_CTRL |= TMR_Os_Tmr_Co_Op_STATUS_ACTL_INT_EN_MASK;

        /* Exit Critical Region*/
        CyExitCriticalSection(TMR_Os_Tmr_Co_Op_interruptState);

        #if (TMR_Os_Tmr_Co_Op_EnableTriggerMode)
            TMR_Os_Tmr_Co_Op_EnableTrigger();
        #endif /* Set Trigger enable bit for UDB implementation in the control register*/
		
		
        #if (TMR_Os_Tmr_Co_Op_InterruptOnCaptureCount && !TMR_Os_Tmr_Co_Op_UDB_CONTROL_REG_REMOVED)
            TMR_Os_Tmr_Co_Op_SetInterruptCount(TMR_Os_Tmr_Co_Op_INIT_INT_CAPTURE_COUNT);
        #endif /* Set interrupt count in UDB implementation if interrupt count feature is checked.*/

        TMR_Os_Tmr_Co_Op_ClearFIFO();
    #endif /* Configure additional features of UDB implementation */

    TMR_Os_Tmr_Co_Op_SetInterruptMode(TMR_Os_Tmr_Co_Op_INIT_INTERRUPT_MODE);
}


/*******************************************************************************
* Function Name: TMR_Os_Tmr_Co_Op_Enable
********************************************************************************
*
* Summary:
*  Enable the Timer
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void TMR_Os_Tmr_Co_Op_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (TMR_Os_Tmr_Co_Op_UsingFixedFunction)
        TMR_Os_Tmr_Co_Op_GLOBAL_ENABLE |= TMR_Os_Tmr_Co_Op_BLOCK_EN_MASK;
        TMR_Os_Tmr_Co_Op_GLOBAL_STBY_ENABLE |= TMR_Os_Tmr_Co_Op_BLOCK_STBY_EN_MASK;
    #endif /* Set Enable bit for enabling Fixed function timer*/

    /* Remove assignment if control register is removed */
    #if (!TMR_Os_Tmr_Co_Op_UDB_CONTROL_REG_REMOVED || TMR_Os_Tmr_Co_Op_UsingFixedFunction)
        TMR_Os_Tmr_Co_Op_CONTROL |= TMR_Os_Tmr_Co_Op_CTRL_ENABLE;
    #endif /* Remove assignment if control register is removed */
}


/*******************************************************************************
* Function Name: TMR_Os_Tmr_Co_Op_Start
********************************************************************************
*
* Summary:
*  The start function initializes the timer with the default values, the
*  enables the timerto begin counting.  It does not enable interrupts,
*  the EnableInt command should be called if interrupt generation is required.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  TMR_Os_Tmr_Co_Op_initVar: Is modified when this function is called for the
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void TMR_Os_Tmr_Co_Op_Start(void) 
{
    if(TMR_Os_Tmr_Co_Op_initVar == 0u)
    {
        TMR_Os_Tmr_Co_Op_Init();

        TMR_Os_Tmr_Co_Op_initVar = 1u;   /* Clear this bit for Initialization */
    }

    /* Enable the Timer */
    TMR_Os_Tmr_Co_Op_Enable();
}


/*******************************************************************************
* Function Name: TMR_Os_Tmr_Co_Op_Stop
********************************************************************************
*
* Summary:
*  The stop function halts the timer, but does not change any modes or disable
*  interrupts.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Side Effects: If the Enable mode is set to Hardware only then this function
*               has no effect on the operation of the timer.
*
*******************************************************************************/
void TMR_Os_Tmr_Co_Op_Stop(void) 
{
    /* Disable Timer */
    #if(!TMR_Os_Tmr_Co_Op_UDB_CONTROL_REG_REMOVED || TMR_Os_Tmr_Co_Op_UsingFixedFunction)
        TMR_Os_Tmr_Co_Op_CONTROL &= ((uint8)(~TMR_Os_Tmr_Co_Op_CTRL_ENABLE));
    #endif /* Remove assignment if control register is removed */

    /* Globally disable the Fixed Function Block chosen */
    #if (TMR_Os_Tmr_Co_Op_UsingFixedFunction)
        TMR_Os_Tmr_Co_Op_GLOBAL_ENABLE &= ((uint8)(~TMR_Os_Tmr_Co_Op_BLOCK_EN_MASK));
        TMR_Os_Tmr_Co_Op_GLOBAL_STBY_ENABLE &= ((uint8)(~TMR_Os_Tmr_Co_Op_BLOCK_STBY_EN_MASK));
    #endif /* Disable global enable for the Timer Fixed function block to stop the Timer*/
}


/*******************************************************************************
* Function Name: TMR_Os_Tmr_Co_Op_SetInterruptMode
********************************************************************************
*
* Summary:
*  This function selects which of the interrupt inputs may cause an interrupt.
*  The twosources are caputure and terminal.  One, both or neither may
*  be selected.
*
* Parameters:
*  interruptMode:   This parameter is used to enable interrups on either/or
*                   terminal count or capture.
*
* Return:
*  void
*
*******************************************************************************/
void TMR_Os_Tmr_Co_Op_SetInterruptMode(uint8 interruptMode) 
{
    TMR_Os_Tmr_Co_Op_STATUS_MASK = interruptMode;
}


/*******************************************************************************
* Function Name: TMR_Os_Tmr_Co_Op_SoftwareCapture
********************************************************************************
*
* Summary:
*  This function forces a capture independent of the capture signal.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Side Effects:
*  An existing hardware capture could be overwritten.
*
*******************************************************************************/
void TMR_Os_Tmr_Co_Op_SoftwareCapture(void) 
{
    /* Generate a software capture by reading the counter register */
    #if(TMR_Os_Tmr_Co_Op_UsingFixedFunction)
        (void)CY_GET_REG16(TMR_Os_Tmr_Co_Op_COUNTER_LSB_PTR);
    #else
        (void)CY_GET_REG8(TMR_Os_Tmr_Co_Op_COUNTER_LSB_PTR_8BIT);
    #endif/* (TMR_Os_Tmr_Co_Op_UsingFixedFunction) */
    /* Capture Data is now in the FIFO */
}


/*******************************************************************************
* Function Name: TMR_Os_Tmr_Co_Op_ReadStatusRegister
********************************************************************************
*
* Summary:
*  Reads the status register and returns it's state. This function should use
*  defined types for the bit-field information as the bits in this register may
*  be permuteable.
*
* Parameters:
*  void
*
* Return:
*  The contents of the status register
*
* Side Effects:
*  Status register bits may be clear on read.
*
*******************************************************************************/
uint8   TMR_Os_Tmr_Co_Op_ReadStatusRegister(void) 
{
    return (TMR_Os_Tmr_Co_Op_STATUS);
}


#if (!TMR_Os_Tmr_Co_Op_UDB_CONTROL_REG_REMOVED) /* Remove API if control register is unused */


/*******************************************************************************
* Function Name: TMR_Os_Tmr_Co_Op_ReadControlRegister
********************************************************************************
*
* Summary:
*  Reads the control register and returns it's value.
*
* Parameters:
*  void
*
* Return:
*  The contents of the control register
*
*******************************************************************************/
uint8 TMR_Os_Tmr_Co_Op_ReadControlRegister(void) 
{
    #if (!TMR_Os_Tmr_Co_Op_UDB_CONTROL_REG_REMOVED) 
        return ((uint8)TMR_Os_Tmr_Co_Op_CONTROL);
    #else
        return (0);
    #endif /* (!TMR_Os_Tmr_Co_Op_UDB_CONTROL_REG_REMOVED) */
}


/*******************************************************************************
* Function Name: TMR_Os_Tmr_Co_Op_WriteControlRegister
********************************************************************************
*
* Summary:
*  Sets the bit-field of the control register.
*
* Parameters:
*  control: The contents of the control register
*
* Return:
*
*******************************************************************************/
void TMR_Os_Tmr_Co_Op_WriteControlRegister(uint8 control) 
{
    #if (!TMR_Os_Tmr_Co_Op_UDB_CONTROL_REG_REMOVED) 
        TMR_Os_Tmr_Co_Op_CONTROL = control;
    #else
        control = 0u;
    #endif /* (!TMR_Os_Tmr_Co_Op_UDB_CONTROL_REG_REMOVED) */
}

#endif /* Remove API if control register is unused */


/*******************************************************************************
* Function Name: TMR_Os_Tmr_Co_Op_ReadPeriod
********************************************************************************
*
* Summary:
*  This function returns the current value of the Period.
*
* Parameters:
*  void
*
* Return:
*  The present value of the counter.
*
*******************************************************************************/
uint16 TMR_Os_Tmr_Co_Op_ReadPeriod(void) 
{
   #if(TMR_Os_Tmr_Co_Op_UsingFixedFunction)
       return ((uint16)CY_GET_REG16(TMR_Os_Tmr_Co_Op_PERIOD_LSB_PTR));
   #else
       return (CY_GET_REG16(TMR_Os_Tmr_Co_Op_PERIOD_LSB_PTR));
   #endif /* (TMR_Os_Tmr_Co_Op_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: TMR_Os_Tmr_Co_Op_WritePeriod
********************************************************************************
*
* Summary:
*  This function is used to change the period of the counter.  The new period
*  will be loaded the next time terminal count is detected.
*
* Parameters:
*  period: This value may be between 1 and (2^Resolution)-1.  A value of 0 will
*          result in the counter remaining at zero.
*
* Return:
*  void
*
*******************************************************************************/
void TMR_Os_Tmr_Co_Op_WritePeriod(uint16 period) 
{
    #if(TMR_Os_Tmr_Co_Op_UsingFixedFunction)
        uint16 period_temp = (uint16)period;
        CY_SET_REG16(TMR_Os_Tmr_Co_Op_PERIOD_LSB_PTR, period_temp);
    #else
        CY_SET_REG16(TMR_Os_Tmr_Co_Op_PERIOD_LSB_PTR, period);
    #endif /*Write Period value with appropriate resolution suffix depending on UDB or fixed function implementation */
}


/*******************************************************************************
* Function Name: TMR_Os_Tmr_Co_Op_ReadCapture
********************************************************************************
*
* Summary:
*  This function returns the last value captured.
*
* Parameters:
*  void
*
* Return:
*  Present Capture value.
*
*******************************************************************************/
uint16 TMR_Os_Tmr_Co_Op_ReadCapture(void) 
{
   #if(TMR_Os_Tmr_Co_Op_UsingFixedFunction)
       return ((uint16)CY_GET_REG16(TMR_Os_Tmr_Co_Op_CAPTURE_LSB_PTR));
   #else
       return (CY_GET_REG16(TMR_Os_Tmr_Co_Op_CAPTURE_LSB_PTR));
   #endif /* (TMR_Os_Tmr_Co_Op_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: TMR_Os_Tmr_Co_Op_WriteCounter
********************************************************************************
*
* Summary:
*  This funtion is used to set the counter to a specific value
*
* Parameters:
*  counter:  New counter value.
*
* Return:
*  void
*
*******************************************************************************/
void TMR_Os_Tmr_Co_Op_WriteCounter(uint16 counter) 
{
   #if(TMR_Os_Tmr_Co_Op_UsingFixedFunction)
        /* This functionality is removed until a FixedFunction HW update to
         * allow this register to be written
         */
        CY_SET_REG16(TMR_Os_Tmr_Co_Op_COUNTER_LSB_PTR, (uint16)counter);
        
    #else
        CY_SET_REG16(TMR_Os_Tmr_Co_Op_COUNTER_LSB_PTR, counter);
    #endif /* Set Write Counter only for the UDB implementation (Write Counter not available in fixed function Timer */
}


/*******************************************************************************
* Function Name: TMR_Os_Tmr_Co_Op_ReadCounter
********************************************************************************
*
* Summary:
*  This function returns the current counter value.
*
* Parameters:
*  void
*
* Return:
*  Present compare value.
*
*******************************************************************************/
uint16 TMR_Os_Tmr_Co_Op_ReadCounter(void) 
{
    /* Force capture by reading Accumulator */
    /* Must first do a software capture to be able to read the counter */
    /* It is up to the user code to make sure there isn't already captured data in the FIFO */
    #if(TMR_Os_Tmr_Co_Op_UsingFixedFunction)
        (void)CY_GET_REG16(TMR_Os_Tmr_Co_Op_COUNTER_LSB_PTR);
    #else
        (void)CY_GET_REG8(TMR_Os_Tmr_Co_Op_COUNTER_LSB_PTR_8BIT);
    #endif/* (TMR_Os_Tmr_Co_Op_UsingFixedFunction) */

    /* Read the data from the FIFO (or capture register for Fixed Function)*/
    #if(TMR_Os_Tmr_Co_Op_UsingFixedFunction)
        return ((uint16)CY_GET_REG16(TMR_Os_Tmr_Co_Op_CAPTURE_LSB_PTR));
    #else
        return (CY_GET_REG16(TMR_Os_Tmr_Co_Op_CAPTURE_LSB_PTR));
    #endif /* (TMR_Os_Tmr_Co_Op_UsingFixedFunction) */
}


#if(!TMR_Os_Tmr_Co_Op_UsingFixedFunction) /* UDB Specific Functions */

    
/*******************************************************************************
 * The functions below this point are only available using the UDB
 * implementation.  If a feature is selected, then the API is enabled.
 ******************************************************************************/


#if (TMR_Os_Tmr_Co_Op_SoftwareCaptureMode)


/*******************************************************************************
* Function Name: TMR_Os_Tmr_Co_Op_SetCaptureMode
********************************************************************************
*
* Summary:
*  This function sets the capture mode to either rising or falling edge.
*
* Parameters:
*  captureMode: This parameter sets the capture mode of the UDB capture feature
*  The parameter values are defined using the
*  #define TMR_Os_Tmr_Co_Op__B_TIMER__CM_NONE 0
#define TMR_Os_Tmr_Co_Op__B_TIMER__CM_RISINGEDGE 1
#define TMR_Os_Tmr_Co_Op__B_TIMER__CM_FALLINGEDGE 2
#define TMR_Os_Tmr_Co_Op__B_TIMER__CM_EITHEREDGE 3
#define TMR_Os_Tmr_Co_Op__B_TIMER__CM_SOFTWARE 4
 identifiers
*  The following are the possible values of the parameter
*  TMR_Os_Tmr_Co_Op__B_TIMER__CM_NONE        - Set Capture mode to None
*  TMR_Os_Tmr_Co_Op__B_TIMER__CM_RISINGEDGE  - Rising edge of Capture input
*  TMR_Os_Tmr_Co_Op__B_TIMER__CM_FALLINGEDGE - Falling edge of Capture input
*  TMR_Os_Tmr_Co_Op__B_TIMER__CM_EITHEREDGE  - Either edge of Capture input
*
* Return:
*  void
*
*******************************************************************************/
void TMR_Os_Tmr_Co_Op_SetCaptureMode(uint8 captureMode) 
{
    /* This must only set to two bits of the control register associated */
    captureMode = ((uint8)((uint8)captureMode << TMR_Os_Tmr_Co_Op_CTRL_CAP_MODE_SHIFT));
    captureMode &= (TMR_Os_Tmr_Co_Op_CTRL_CAP_MODE_MASK);

    #if (!TMR_Os_Tmr_Co_Op_UDB_CONTROL_REG_REMOVED)
        /* Clear the Current Setting */
        TMR_Os_Tmr_Co_Op_CONTROL &= ((uint8)(~TMR_Os_Tmr_Co_Op_CTRL_CAP_MODE_MASK));

        /* Write The New Setting */
        TMR_Os_Tmr_Co_Op_CONTROL |= captureMode;
    #endif /* (!TMR_Os_Tmr_Co_Op_UDB_CONTROL_REG_REMOVED) */
}
#endif /* Remove API if Capture Mode is not Software Controlled */


#if (TMR_Os_Tmr_Co_Op_SoftwareTriggerMode)


/*******************************************************************************
* Function Name: TMR_Os_Tmr_Co_Op_SetTriggerMode
********************************************************************************
*
* Summary:
*  This function sets the trigger input mode
*
* Parameters:
*  triggerMode: Pass one of the pre-defined Trigger Modes (except Software)
    #define TMR_Os_Tmr_Co_Op__B_TIMER__TM_NONE 0x00u
    #define TMR_Os_Tmr_Co_Op__B_TIMER__TM_RISINGEDGE 0x04u
    #define TMR_Os_Tmr_Co_Op__B_TIMER__TM_FALLINGEDGE 0x08u
    #define TMR_Os_Tmr_Co_Op__B_TIMER__TM_EITHEREDGE 0x0Cu
    #define TMR_Os_Tmr_Co_Op__B_TIMER__TM_SOFTWARE 0x10u
*
* Return:
*  void
*
*******************************************************************************/
void TMR_Os_Tmr_Co_Op_SetTriggerMode(uint8 triggerMode) 
{
    /* This must only set to two bits of the control register associated */
    triggerMode &= TMR_Os_Tmr_Co_Op_CTRL_TRIG_MODE_MASK;

    #if (!TMR_Os_Tmr_Co_Op_UDB_CONTROL_REG_REMOVED)   /* Remove assignment if control register is removed */
    
        /* Clear the Current Setting */
        TMR_Os_Tmr_Co_Op_CONTROL &= ((uint8)(~TMR_Os_Tmr_Co_Op_CTRL_TRIG_MODE_MASK));

        /* Write The New Setting */
        TMR_Os_Tmr_Co_Op_CONTROL |= (triggerMode | TMR_Os_Tmr_Co_Op__B_TIMER__TM_SOFTWARE);
    #endif /* Remove code section if control register is not used */
}
#endif /* Remove API if Trigger Mode is not Software Controlled */

#if (TMR_Os_Tmr_Co_Op_EnableTriggerMode)


/*******************************************************************************
* Function Name: TMR_Os_Tmr_Co_Op_EnableTrigger
********************************************************************************
*
* Summary:
*  Sets the control bit enabling Hardware Trigger mode
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void TMR_Os_Tmr_Co_Op_EnableTrigger(void) 
{
    #if (!TMR_Os_Tmr_Co_Op_UDB_CONTROL_REG_REMOVED)   /* Remove assignment if control register is removed */
        TMR_Os_Tmr_Co_Op_CONTROL |= TMR_Os_Tmr_Co_Op_CTRL_TRIG_EN;
    #endif /* Remove code section if control register is not used */
}


/*******************************************************************************
* Function Name: TMR_Os_Tmr_Co_Op_DisableTrigger
********************************************************************************
*
* Summary:
*  Clears the control bit enabling Hardware Trigger mode
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void TMR_Os_Tmr_Co_Op_DisableTrigger(void) 
{
    #if (!TMR_Os_Tmr_Co_Op_UDB_CONTROL_REG_REMOVED )   /* Remove assignment if control register is removed */
        TMR_Os_Tmr_Co_Op_CONTROL &= ((uint8)(~TMR_Os_Tmr_Co_Op_CTRL_TRIG_EN));
    #endif /* Remove code section if control register is not used */
}
#endif /* Remove API is Trigger Mode is set to None */

#if(TMR_Os_Tmr_Co_Op_InterruptOnCaptureCount)


/*******************************************************************************
* Function Name: TMR_Os_Tmr_Co_Op_SetInterruptCount
********************************************************************************
*
* Summary:
*  This function sets the capture count before an interrupt is triggered.
*
* Parameters:
*  interruptCount:  A value between 0 and 3 is valid.  If the value is 0, then
*                   an interrupt will occur each time a capture occurs.
*                   A value of 1 to 3 will cause the interrupt
*                   to delay by the same number of captures.
*
* Return:
*  void
*
*******************************************************************************/
void TMR_Os_Tmr_Co_Op_SetInterruptCount(uint8 interruptCount) 
{
    /* This must only set to two bits of the control register associated */
    interruptCount &= TMR_Os_Tmr_Co_Op_CTRL_INTCNT_MASK;

    #if (!TMR_Os_Tmr_Co_Op_UDB_CONTROL_REG_REMOVED)
        /* Clear the Current Setting */
        TMR_Os_Tmr_Co_Op_CONTROL &= ((uint8)(~TMR_Os_Tmr_Co_Op_CTRL_INTCNT_MASK));
        /* Write The New Setting */
        TMR_Os_Tmr_Co_Op_CONTROL |= interruptCount;
    #endif /* (!TMR_Os_Tmr_Co_Op_UDB_CONTROL_REG_REMOVED) */
}
#endif /* TMR_Os_Tmr_Co_Op_InterruptOnCaptureCount */


#if (TMR_Os_Tmr_Co_Op_UsingHWCaptureCounter)


/*******************************************************************************
* Function Name: TMR_Os_Tmr_Co_Op_SetCaptureCount
********************************************************************************
*
* Summary:
*  This function sets the capture count
*
* Parameters:
*  captureCount: A value between 2 and 127 inclusive is valid.  A value of 1
*                to 127 will cause the interrupt to delay by the same number of
*                captures.
*
* Return:
*  void
*
*******************************************************************************/
void TMR_Os_Tmr_Co_Op_SetCaptureCount(uint8 captureCount) 
{
    TMR_Os_Tmr_Co_Op_CAP_COUNT = captureCount;
}


/*******************************************************************************
* Function Name: TMR_Os_Tmr_Co_Op_ReadCaptureCount
********************************************************************************
*
* Summary:
*  This function reads the capture count setting
*
* Parameters:
*  void
*
* Return:
*  Returns the Capture Count Setting
*
*******************************************************************************/
uint8 TMR_Os_Tmr_Co_Op_ReadCaptureCount(void) 
{
    return ((uint8)TMR_Os_Tmr_Co_Op_CAP_COUNT);
}
#endif /* TMR_Os_Tmr_Co_Op_UsingHWCaptureCounter */


/*******************************************************************************
* Function Name: TMR_Os_Tmr_Co_Op_ClearFIFO
********************************************************************************
*
* Summary:
*  This function clears all capture data from the capture FIFO
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void TMR_Os_Tmr_Co_Op_ClearFIFO(void) 
{
    while(0u != (TMR_Os_Tmr_Co_Op_ReadStatusRegister() & TMR_Os_Tmr_Co_Op_STATUS_FIFONEMP))
    {
        (void)TMR_Os_Tmr_Co_Op_ReadCapture();
    }
}

#endif /* UDB Specific Functions */


/* [] END OF FILE */
