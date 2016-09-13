/*******************************************************************************
* File Name: TMR_Os_Tmr_Co_Op_PM.c
* Version 2.70
*
*  Description:
*     This file provides the power management source code to API for the
*     Timer.
*
*   Note:
*     None
*
*******************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "TMR_Os_Tmr_Co_Op.h"

static TMR_Os_Tmr_Co_Op_backupStruct TMR_Os_Tmr_Co_Op_backup;


/*******************************************************************************
* Function Name: TMR_Os_Tmr_Co_Op_SaveConfig
********************************************************************************
*
* Summary:
*     Save the current user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  TMR_Os_Tmr_Co_Op_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void TMR_Os_Tmr_Co_Op_SaveConfig(void) 
{
    #if (!TMR_Os_Tmr_Co_Op_UsingFixedFunction)
        TMR_Os_Tmr_Co_Op_backup.TimerUdb = TMR_Os_Tmr_Co_Op_ReadCounter();
        TMR_Os_Tmr_Co_Op_backup.InterruptMaskValue = TMR_Os_Tmr_Co_Op_STATUS_MASK;
        #if (TMR_Os_Tmr_Co_Op_UsingHWCaptureCounter)
            TMR_Os_Tmr_Co_Op_backup.TimerCaptureCounter = TMR_Os_Tmr_Co_Op_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!TMR_Os_Tmr_Co_Op_UDB_CONTROL_REG_REMOVED)
            TMR_Os_Tmr_Co_Op_backup.TimerControlRegister = TMR_Os_Tmr_Co_Op_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: TMR_Os_Tmr_Co_Op_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  TMR_Os_Tmr_Co_Op_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void TMR_Os_Tmr_Co_Op_RestoreConfig(void) 
{   
    #if (!TMR_Os_Tmr_Co_Op_UsingFixedFunction)

        TMR_Os_Tmr_Co_Op_WriteCounter(TMR_Os_Tmr_Co_Op_backup.TimerUdb);
        TMR_Os_Tmr_Co_Op_STATUS_MASK =TMR_Os_Tmr_Co_Op_backup.InterruptMaskValue;
        #if (TMR_Os_Tmr_Co_Op_UsingHWCaptureCounter)
            TMR_Os_Tmr_Co_Op_SetCaptureCount(TMR_Os_Tmr_Co_Op_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!TMR_Os_Tmr_Co_Op_UDB_CONTROL_REG_REMOVED)
            TMR_Os_Tmr_Co_Op_WriteControlRegister(TMR_Os_Tmr_Co_Op_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: TMR_Os_Tmr_Co_Op_Sleep
********************************************************************************
*
* Summary:
*     Stop and Save the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  TMR_Os_Tmr_Co_Op_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void TMR_Os_Tmr_Co_Op_Sleep(void) 
{
    #if(!TMR_Os_Tmr_Co_Op_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(TMR_Os_Tmr_Co_Op_CTRL_ENABLE == (TMR_Os_Tmr_Co_Op_CONTROL & TMR_Os_Tmr_Co_Op_CTRL_ENABLE))
        {
            /* Timer is enabled */
            TMR_Os_Tmr_Co_Op_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            TMR_Os_Tmr_Co_Op_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    TMR_Os_Tmr_Co_Op_Stop();
    TMR_Os_Tmr_Co_Op_SaveConfig();
}


/*******************************************************************************
* Function Name: TMR_Os_Tmr_Co_Op_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  TMR_Os_Tmr_Co_Op_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void TMR_Os_Tmr_Co_Op_Wakeup(void) 
{
    TMR_Os_Tmr_Co_Op_RestoreConfig();
    #if(!TMR_Os_Tmr_Co_Op_UDB_CONTROL_REG_REMOVED)
        if(TMR_Os_Tmr_Co_Op_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                TMR_Os_Tmr_Co_Op_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
