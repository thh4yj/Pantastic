/*
 * Timer.c
 *
 *  Created on: Oct 20, 2021
 *      Author: tyler
 */

#include <Timer.h>
#include <ti/drivers/timer/TimerCC32XX.h>
#include <ti/drivers/timer/TimerSupport.h>

const TimerCC32XX_HWAttrs timerCC32XXHWAttrs[] =
  {
      {
          .baseAddress = TIMERA0_BASE,
          .subTimer = TimerCC32XX_timer32,
          .intNum = INT_TIMERA0A,
          .intPriority = ~0
      },
      {
          .baseAddress = TIMERA1_BASE,
          .subTimer = TimerCC32XX_timer16A,
          .intNum = INT_TIMERA1A,
          .intPriority = ~0
      },
      {
          .baseAddress = TIMERA1_BASE,
          .subTimer = TimerCC32XX_timer16B,
          .intNum = INT_TIMERA1B,
          .intPriority = ~0
      }
};



void Timer_Init(){

}

