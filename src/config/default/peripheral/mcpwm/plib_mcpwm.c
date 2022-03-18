/*******************************************************************************
  Motor Control PWM (MCPWM) Peripheral Library (PLIB)

  Company:
    Microchip Technology Inc.

  File Name:
    plib_mcpwm.c

  Summary:
    MCPWM Source File

  Description:
    None

*******************************************************************************/

/*******************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/
#include "device.h"
#include "plib_mcpwm.h"

// *****************************************************************************

// *****************************************************************************
// Section: MCPWM Implementation
// *****************************************************************************
// *****************************************************************************

void MCPWM_Initialize (void)
{
    /* PTCON register  */
    /*  SEVTPS  = 0 */
    /*  PCLKDIV = 0 */
    /*  SEIEN   = false */
    PTCON = 0x0;

    /* STCON register  */
    /*  SEVTPS  = 0 */
    /*  SCLKDIV = 0 */
    /*  SSEIEN = false */
    STCON = 0x0;

    PTPER = 2000;
    STPER = 2000;
    SEVTCMP = 10;
    SSEVTCMP = 10;

    /*********** Channel 5 Configurations **********/
    /* PWMCON5 register  */
    /*  MTBS   = 0 */
    /*  PTDIR  =  0 */
    /*  ECAM   =  0 */
    /*  DTCP   =  0 */
    /*  DTC    =  0 */
    /*  ITB    = 0 */
    /*  PWMHIEN =  false */
    /*  PWMLIEN = false */
    /*  TRGIEN = false */
    /*  CLIEN = false */
    /*  FLTIEN = false */
    PWMCON5 = 0x0;

    /* IOCON5 register  */
    /*  SWAP    = 0*/
    /*  PMOD    = 3*/
    /*  POLH    = 0*/
    /*  POLL    = 0*/
    /*  FLTDAT  = 0b00 */
    /*  FLTMOD  = 3 */
    /*  FLTPOL  = 0  */
    /*  FLTSRC  = 0  */
    /*  CLDAT  = 0b00 */
    /*  CLMOD  = 0 */
    /*  CLPOL  = 0  */
    /*  CLSRC  = 0  */
    IOCON5 = 0x3cc00;

    PDC5 = 1000;
    SDC5 = 500;
    PHASE5 = 0;

    /* Dead Time */
    DTR5 = 75;
    ALTDTR5 = 75;

    /* Trigger Generator */
    TRGCON5 = 0x0;
    TRIG5 = 0;
    STRIG5 = 0;

    /* leading edge blanking */
    /* LEBCON5 register  */
    /*  CLLEBEN    = false  */
    /*  FLTLEBEN   = false */
    /*  PLF        = 0  */
    /*  PLR        = 0  */
    /*  PHF        = 0  */
    /*  PHR        = 0  */
    LEBCON5 = 0x0;
    LEBDLY5 = 10;


    /*********** Channel 6 Configurations **********/
    /* PWMCON6 register  */
    /*  MTBS   = 0 */
    /*  PTDIR  =  0 */
    /*  ECAM   =  0 */
    /*  DTCP   =  0 */
    /*  DTC    =  0 */
    /*  ITB    = 0 */
    /*  PWMHIEN =  false */
    /*  PWMLIEN = false */
    /*  TRGIEN = false */
    /*  CLIEN = false */
    /*  FLTIEN = false */
    PWMCON6 = 0x0;

    /* IOCON6 register  */
    /*  SWAP    = 0*/
    /*  PMOD    = 0*/
    /*  POLH    = 0*/
    /*  POLL    = 0*/
    /*  FLTDAT  = 0b00 */
    /*  FLTMOD  = 3 */
    /*  FLTPOL  = 0  */
    /*  FLTSRC  = 0  */
    /*  CLDAT  = 0b00 */
    /*  CLMOD  = 0 */
    /*  CLPOL  = 0  */
    /*  CLSRC  = 0  */
    IOCON6 = 0xc000;

    PDC6 = 1000;
    SDC6 = 500;
    PHASE6 = 0;

    /* Dead Time */
    DTR6 = 75;
    ALTDTR6 = 75;

    /* Trigger Generator */
    TRGCON6 = 0x0;
    TRIG6 = 0;
    STRIG6 = 0;

    /* leading edge blanking */
    /* LEBCON6 register  */
    /*  CLLEBEN    = false  */
    /*  FLTLEBEN   = false */
    /*  PLF        = 0  */
    /*  PLR        = 0  */
    /*  PHF        = 0  */
    /*  PHR        = 0  */
    LEBCON6 = 0x0;
    LEBDLY6 = 10;


}


void MCPWM_Start(void)
{
    /* Enable MCPWM module */
    PTCONbits.PTEN = 0x1;
}

void MCPWM_Stop(void)
{
    /* Disable MCPWM module */
    PTCONbits.PTEN = 0x0;
}

void MCPWM_PrimaryPeriodSet(uint16_t period)
{
    PTPER = period;
}

uint16_t MCPWM_PrimaryPeriodGet(void)
{
    return PTPER;
}


void MCPWM_SecondaryPeriodSet(uint16_t period)
{
    STPER = period;
}

uint16_t MCPWM_SecondaryPeriodGet(void)
{
    return STPER;
}

void MCPWM_ChannelPrimaryDutySet(MCPWM_CH_NUM channel, uint16_t duty)
{
    *(&PDC1 + (0x40 * (channel))) = duty;
}

void MCPWM_ChannelSecondaryDutySet(MCPWM_CH_NUM channel, uint16_t duty)
{
    *(&SDC1 + (0x40 * (channel))) = duty;
}

void MCPWM_ChannelDeadTimeSet(MCPWM_CH_NUM channel, uint16_t high_deadtime, uint16_t low_deadtime)
{
    *(&DTR1 + (0x40 * (channel))) = (high_deadtime & 0x3FFF);
    *(&ALTDTR1 + (0x40 * (channel))) = (low_deadtime & 0x3FFF);
}

void MCPWM_ChannelPrimaryTriggerSet(MCPWM_CH_NUM channel, uint16_t trigger)
{
    *(&TRIG1 + (0x40 * (channel))) = trigger;
}

void MCPWM_ChannelSecondaryTriggerSet(MCPWM_CH_NUM channel, uint16_t trigger)
{
    *(&STRIG1 + (0x40 * (channel))) = trigger;
}

void MCPWM_ChannelLeadingEdgeBlankingDelaySet(MCPWM_CH_NUM channel, uint16_t delay)
{
    *(&LEBDLY1 + (0x40 * (channel))) = delay;
}

void MCPWM_ChannelPinsOverrideEnable(MCPWM_CH_NUM channel)
{
    *(&IOCON1 + (0x40 * (channel))) |= _IOCON1_OVRENL_MASK | _IOCON1_OVRENH_MASK;
}

void MCPWM_ChannelPinsOverrideDisable(MCPWM_CH_NUM channel)
{
    *(&IOCON1 + (0x40 * (channel))) &= ~(_IOCON1_OVRENL_MASK | _IOCON1_OVRENH_MASK);
}

void MCPWM_ChannelPinsOwnershipEnable(MCPWM_CH_NUM channel)
{
    *(&IOCON1 + (0x40 * (channel))) |= _IOCON1_PENH_MASK | _IOCON1_PENL_MASK;
}

void MCPWM_ChannelPinsOwnershipDisable(MCPWM_CH_NUM channel)
{
    *(&IOCON1 + (0x40 * (channel))) &= ~(_IOCON1_PENH_MASK | _IOCON1_PENL_MASK);
}




