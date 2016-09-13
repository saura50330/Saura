/*******************************************************************************
* File Name: System_Switch.c  
* Version 2.10
*
* Description:
*  This file contains API to enable firmware control of a Pins component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "System_Switch.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 System_Switch__PORT == 15 && ((System_Switch__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: System_Switch_Write
********************************************************************************
*
* Summary:
*  Assign a new value to the digital port's data output register.  
*
* Parameters:  
*  prtValue:  The value to be assigned to the Digital Port. 
*
* Return: 
*  None
*  
*******************************************************************************/
void System_Switch_Write(uint8 value) 
{
    uint8 staticBits = (System_Switch_DR & (uint8)(~System_Switch_MASK));
    System_Switch_DR = staticBits | ((uint8)(value << System_Switch_SHIFT) & System_Switch_MASK);
}


/*******************************************************************************
* Function Name: System_Switch_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  System_Switch_DM_STRONG     Strong Drive 
*  System_Switch_DM_OD_HI      Open Drain, Drives High 
*  System_Switch_DM_OD_LO      Open Drain, Drives Low 
*  System_Switch_DM_RES_UP     Resistive Pull Up 
*  System_Switch_DM_RES_DWN    Resistive Pull Down 
*  System_Switch_DM_RES_UPDWN  Resistive Pull Up/Down 
*  System_Switch_DM_DIG_HIZ    High Impedance Digital 
*  System_Switch_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void System_Switch_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(System_Switch_0, mode);
}


/*******************************************************************************
* Function Name: System_Switch_Read
********************************************************************************
*
* Summary:
*  Read the current value on the pins of the Digital Port in right justified 
*  form.
*
* Parameters:  
*  None
*
* Return: 
*  Returns the current value of the Digital Port as a right justified number
*  
* Note:
*  Macro System_Switch_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 System_Switch_Read(void) 
{
    return (System_Switch_PS & System_Switch_MASK) >> System_Switch_SHIFT;
}


/*******************************************************************************
* Function Name: System_Switch_ReadDataReg
********************************************************************************
*
* Summary:
*  Read the current value assigned to a Digital Port's data output register
*
* Parameters:  
*  None 
*
* Return: 
*  Returns the current value assigned to the Digital Port's data output register
*  
*******************************************************************************/
uint8 System_Switch_ReadDataReg(void) 
{
    return (System_Switch_DR & System_Switch_MASK) >> System_Switch_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(System_Switch_INTSTAT) 

    /*******************************************************************************
    * Function Name: System_Switch_ClearInterrupt
    ********************************************************************************
    * Summary:
    *  Clears any active interrupts attached to port and returns the value of the 
    *  interrupt status register.
    *
    * Parameters:  
    *  None 
    *
    * Return: 
    *  Returns the value of the interrupt status register
    *  
    *******************************************************************************/
    uint8 System_Switch_ClearInterrupt(void) 
    {
        return (System_Switch_INTSTAT & System_Switch_MASK) >> System_Switch_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */
