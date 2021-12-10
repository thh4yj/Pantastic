/*
 *  ======== ti_drivers_config.c ========
 *  Configured TI-Drivers module definitions
 *
 *  DO NOT EDIT - This file is generated for the CC3220S_LAUNCHXL
 *  by the SysConfig tool.
 */

#include <stddef.h>
#include <stdint.h>

#ifndef DeviceFamily_CC3220
#define DeviceFamily_CC3220
#endif

#include <ti/devices/DeviceFamily.h>

#include "ti_drivers_config.h"

/*
 *  =============================== GPIO ===============================
 */

#include <ti/drivers/GPIO.h>
#include <ti/drivers/gpio/GPIOCC32XX.h>

#define CONFIG_GPIO_COUNT 13

/*
 *  ======== gpioPinConfigs ========
 *  Array of Pin configurations
 */
GPIO_PinConfig gpioPinConfigs[] = {
    /* LED1 */
    GPIOCC32XX_GPIO_12 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW,
    /* LED2 */
    GPIOCC32XX_GPIO_14 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW,
    /* LED3 */
    GPIOCC32XX_GPIO_15 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW,
    /* LED4 */
    GPIOCC32XX_GPIO_16 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW,
    /* LED5 */
    GPIOCC32XX_GPIO_17 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW,
    /* LED6 */
    GPIOCC32XX_GPIO_28 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW,
    /* AlarmButton */
    GPIOCC32XX_GPIO_22 | GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_NONE,
    /* LaserButton */
    GPIOCC32XX_GPIO_06 | GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_NONE,
    /* LaserDiode */
    GPIOCC32XX_GPIO_00 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW,
    /* SDA2 */
    GPIOCC32XX_GPIO_08 | GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_NONE,
    /* SCL2 */
    GPIOCC32XX_GPIO_30 | GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_NONE,
    /* Alarm */
    GPIOCC32XX_GPIO_07 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW,
    /* IR_EN */
    GPIOCC32XX_GPIO_09 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW,
};

/*
 *  ======== gpioCallbackFunctions ========
 *  Array of callback function pointers
 *
 *  NOTE: Unused callback entries can be omitted from the callbacks array to
 *  reduce memory usage by enabling callback table optimization
 *  (GPIO.optimizeCallbackTableSize = true)
 */
GPIO_CallbackFxn gpioCallbackFunctions[] = {
    /* LED1 */
    NULL,
    /* LED2 */
    NULL,
    /* LED3 */
    NULL,
    /* LED4 */
    NULL,
    /* LED5 */
    NULL,
    /* LED6 */
    NULL,
    /* AlarmButton */
    NULL,
    /* LaserButton */
    NULL,
    /* LaserDiode */
    NULL,
    /* SDA2 */
    NULL,
    /* SCL2 */
    NULL,
    /* Alarm */
    NULL,
    /* IR_EN */
    NULL,
};

const uint_least8_t LED1_CONST = LED1;
const uint_least8_t LED2_CONST = LED2;
const uint_least8_t LED3_CONST = LED3;
const uint_least8_t LED4_CONST = LED4;
const uint_least8_t LED5_CONST = LED5;
const uint_least8_t LED6_CONST = LED6;
const uint_least8_t AlarmButton_CONST = AlarmButton;
const uint_least8_t LaserButton_CONST = LaserButton;
const uint_least8_t LaserDiode_CONST = LaserDiode;
const uint_least8_t SDA2_CONST = SDA2;
const uint_least8_t SCL2_CONST = SCL2;
const uint_least8_t Alarm_CONST = Alarm;
const uint_least8_t IR_EN_CONST = IR_EN;

/*
 *  ======== GPIOCC32XX_config ========
 */
const GPIOCC32XX_Config GPIOCC32XX_config = {
    .pinConfigs = (GPIO_PinConfig *)gpioPinConfigs,
    .callbacks = (GPIO_CallbackFxn *)gpioCallbackFunctions,
    .numberOfPinConfigs = 13,
    .numberOfCallbacks = 13,
    .intPriority = (~0)
};

/*
 *  =============================== Power ===============================
 */
#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerCC32XX.h>
#include <ti/devices/cc32xx/driverlib/prcm.h>

extern void PowerCC32XX_initPolicy(void);
extern void PowerCC32XX_sleepPolicy(void);
PowerCC32XX_ParkInfo parkInfo[];
/*
 *  This structure defines the configuration for the Power Manager.
 */
const PowerCC32XX_ConfigV1 PowerCC32XX_config = {
    .policyInitFxn             = PowerCC32XX_initPolicy,
    .policyFxn                 = PowerCC32XX_sleepPolicy,
    .enterLPDSHookFxn          = NULL,
    .resumeLPDSHookFxn         = NULL,
    .enablePolicy              = false,
    .enableGPIOWakeupLPDS      = true,
    .enableGPIOWakeupShutdown  = true,
    .enableNetworkWakeupLPDS   = true,
    .wakeupGPIOSourceLPDS      = PRCM_LPDS_GPIO13,
    .wakeupGPIOTypeLPDS        = PRCM_LPDS_FALL_EDGE,
    .wakeupGPIOFxnLPDS         = NULL,
    .wakeupGPIOFxnLPDSArg      = 0,
    .wakeupGPIOSourceShutdown  = PRCM_HIB_GPIO13,
    .wakeupGPIOTypeShutdown    = PRCM_HIB_RISE_EDGE,
    .ramRetentionMaskLPDS      = PRCM_SRAM_COL_1|PRCM_SRAM_COL_2|PRCM_SRAM_COL_3|PRCM_SRAM_COL_4,
    .latencyForLPDS            = 20000,
    .keepDebugActiveDuringLPDS = false,
    .ioRetentionShutdown       = PRCM_IO_RET_GRP_0|PRCM_IO_RET_GRP_1|PRCM_IO_RET_GRP_2|PRCM_IO_RET_GRP_3,
    .pinParkDefs               = parkInfo,
    .numPins                   = 31
};

/*
 *  =============================== Timer ===============================
 */

#include <ti/drivers/Timer.h>
#include <ti/devices/cc32xx/inc/hw_memmap.h>
#include <ti/devices/cc32xx/inc/hw_ints.h>
#include <ti/drivers/timer/TimerCC32XX.h>

#define CONFIG_TIMER_COUNT 3

/*
 *  ======== timerCC32XXObjects ========
 */
TimerCC32XX_Object timerCC32XXObjects[CONFIG_TIMER_COUNT];

/*
 *  ======== timerCC32XXHWAttrs ========
 */
const TimerCC32XX_HWAttrs timerCC32XXHWAttrs[CONFIG_TIMER_COUNT] = {
    /* DebounceTimer */
    {
        .baseAddress = TIMERA0_BASE,
        .subTimer    = TimerCC32XX_timer32,
        .intNum      = INT_TIMERA0A,
        .intPriority = 0x40
    },
    /* AlarmTimer */
    {
        .baseAddress = TIMERA1_BASE,
        .subTimer    = TimerCC32XX_timer32,
        .intNum      = INT_TIMERA1A,
        .intPriority = 0x20
    },
    /* ButtonDebounceConifg */
    {
        .baseAddress = TIMERA2_BASE,
        .subTimer    = TimerCC32XX_timer32,
        .intNum      = INT_TIMERA2A,
        .intPriority = 0x20
    },
};

/*
 *  ======== Timer_config ========
 */
const Timer_Config Timer_config[CONFIG_TIMER_COUNT] = {
    /* DebounceTimer */
    {
        .object      = &timerCC32XXObjects[DebounceTimer],
        .hwAttrs     = &timerCC32XXHWAttrs[DebounceTimer]
    },
    /* AlarmTimer */
    {
        .object      = &timerCC32XXObjects[AlarmTimer],
        .hwAttrs     = &timerCC32XXHWAttrs[AlarmTimer]
    },
    /* ButtonDebounceConifg */
    {
        .object      = &timerCC32XXObjects[ButtonDebounceConifg],
        .hwAttrs     = &timerCC32XXHWAttrs[ButtonDebounceConifg]
    },
};

const uint_least8_t DebounceTimer_CONST = DebounceTimer;
const uint_least8_t AlarmTimer_CONST = AlarmTimer;
const uint_least8_t ButtonDebounceConifg_CONST = ButtonDebounceConifg;
const uint_least8_t Timer_count = CONFIG_TIMER_COUNT;

#include <ti/drivers/power/PowerCC32XX.h>

/*
 * This table defines the parking state to be set for each parkable pin
 * during LPDS. (Device resources must be parked during LPDS to achieve maximum
 * power savings.)  If the pin should be left unparked, specify the state
 * PowerCC32XX_DONT_PARK.  For example, for a UART TX pin, the device
 * will automatically park the pin in a high state during transition to LPDS,
 * so the Power Manager does not need to explictly park the pin.  So the
 * corresponding entries in this table should indicate PowerCC32XX_DONT_PARK.
 */
PowerCC32XX_ParkInfo parkInfo[] = {
/*        PIN                    PARK STATE              Pin Alias
   -----------------  ------------------------------     ---------------*/

  {PowerCC32XX_PIN01, PowerCC32XX_WEAK_PULL_DOWN_STD},   /* GP10 */
  {PowerCC32XX_PIN02, PowerCC32XX_WEAK_PULL_DOWN_STD},   /* GP11 */
  {PowerCC32XX_PIN03, PowerCC32XX_WEAK_PULL_DOWN_STD},   /* GP12 */
  {PowerCC32XX_PIN04, PowerCC32XX_WEAK_PULL_DOWN_STD},   /* GP13 */
  {PowerCC32XX_PIN05, PowerCC32XX_WEAK_PULL_DOWN_STD},   /* GP14 */
  {PowerCC32XX_PIN06, PowerCC32XX_WEAK_PULL_DOWN_STD},   /* GP15 */
  {PowerCC32XX_PIN07, PowerCC32XX_WEAK_PULL_DOWN_STD},   /* GP16 */
  {PowerCC32XX_PIN08, PowerCC32XX_WEAK_PULL_DOWN_STD},   /* GP17 */
  {PowerCC32XX_PIN13, PowerCC32XX_WEAK_PULL_DOWN_STD},
  {PowerCC32XX_PIN15, PowerCC32XX_WEAK_PULL_DOWN_STD},   /* GP22 */
  {PowerCC32XX_PIN16, PowerCC32XX_WEAK_PULL_DOWN_STD},   /* TDI */
  {PowerCC32XX_PIN17, PowerCC32XX_WEAK_PULL_DOWN_STD},   /* TDO */
  {PowerCC32XX_PIN18, PowerCC32XX_WEAK_PULL_DOWN_STD},   /* GP28 */
  {PowerCC32XX_PIN19, PowerCC32XX_WEAK_PULL_DOWN_STD},   /* TCK */
  {PowerCC32XX_PIN20, PowerCC32XX_WEAK_PULL_DOWN_STD},   /* TMS */
  {PowerCC32XX_PIN21, PowerCC32XX_WEAK_PULL_DOWN_STD},   /* SOP2 */
  {PowerCC32XX_PIN29, PowerCC32XX_WEAK_PULL_DOWN_STD},   /* GP26 */
  {PowerCC32XX_PIN30, PowerCC32XX_WEAK_PULL_DOWN_STD},   /* GP27 */
  {PowerCC32XX_PIN45, PowerCC32XX_WEAK_PULL_DOWN_STD},   /* GP31 */
  {PowerCC32XX_PIN50, PowerCC32XX_WEAK_PULL_DOWN_STD},   /* GP00 */
  {PowerCC32XX_PIN52, PowerCC32XX_WEAK_PULL_DOWN_STD},   /* GP32 */
  {PowerCC32XX_PIN53, PowerCC32XX_WEAK_PULL_DOWN_STD},   /* GP30 */
  {PowerCC32XX_PIN55, PowerCC32XX_WEAK_PULL_UP_STD},   /* GP01 */
  {PowerCC32XX_PIN57, PowerCC32XX_WEAK_PULL_UP_STD},   /* GP02 */
  {PowerCC32XX_PIN58, PowerCC32XX_WEAK_PULL_DOWN_STD},   /* GP03 */
  {PowerCC32XX_PIN59, PowerCC32XX_WEAK_PULL_DOWN_STD},   /* GP04 */
  {PowerCC32XX_PIN60, PowerCC32XX_WEAK_PULL_DOWN_STD},   /* GP05 */
  {PowerCC32XX_PIN61, PowerCC32XX_WEAK_PULL_DOWN_STD},   /* GP06 */
  {PowerCC32XX_PIN62, PowerCC32XX_WEAK_PULL_DOWN_STD},   /* GP07 */
  {PowerCC32XX_PIN63, PowerCC32XX_WEAK_PULL_DOWN_STD},   /* GP08 */
  {PowerCC32XX_PIN64, PowerCC32XX_WEAK_PULL_DOWN_STD},   /* GP09 */
};

#include <ti/drivers/Board.h>

/*
 *  ======== Board_initHook ========
 *  Perform any board-specific initialization needed at startup.  This
 *  function is declared weak to allow applications to override it if needed.
 */
void __attribute__((weak)) Board_initHook(void)
{
}

/*
 *  ======== Board_init ========
 *  Perform any initialization needed before using any board APIs
 */
void Board_init(void)
{
    /* ==== /ti/drivers/Power initialization ==== */
    PRCMCC3200MCUInit();
    Power_init();

    Board_initHook();
}

