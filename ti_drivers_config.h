/*
 *  ======== ti_drivers_config.h ========
 *  Configured TI-Drivers module declarations
 *
 *  The macros defines herein are intended for use by applications which
 *  directly include this header. These macros should NOT be hard coded or
 *  copied into library source code.
 *
 *  Symbols declared as const are intended for use with libraries.
 *  Library source code must extern the correct symbol--which is resolved
 *  when the application is linked.
 *
 *  DO NOT EDIT - This file is generated for the CC3220S_LAUNCHXL
 *  by the SysConfig tool.
 */
#ifndef ti_drivers_config_h
#define ti_drivers_config_h

#define CONFIG_SYSCONFIG_PREVIEW

#define CONFIG_CC3220S_LAUNCHXL
#ifndef DeviceFamily_CC3220
#define DeviceFamily_CC3220
#endif

#include <ti/devices/DeviceFamily.h>

#include <stdint.h>

/* support C++ sources */
#ifdef __cplusplus
extern "C" {
#endif


/*
 *  ======== Crypto ========
 */

extern const uint_least8_t                  CONFIG_Crypto_0_CONST;
#define CONFIG_Crypto_0                     0
#define CONFIG_TI_DRIVERS_CRYPTO_COUNT      1

/*
 *  ======== GPIO ========
 */

/* P03 */
extern const uint_least8_t              LED1_CONST;
#define LED1                            0
/* P05 */
extern const uint_least8_t              LED2_CONST;
#define LED2                            1
/* P06 */
extern const uint_least8_t              LED3_CONST;
#define LED3                            2
/* P07 */
extern const uint_least8_t              LED4_CONST;
#define LED4                            3
/* P08 */
extern const uint_least8_t              LED5_CONST;
#define LED5                            4
/* P18 */
extern const uint_least8_t              LED6_CONST;
#define LED6                            5
/* P15 */
extern const uint_least8_t              AlarmButton_CONST;
#define AlarmButton                     6
/* P61 */
extern const uint_least8_t              LaserButton_CONST;
#define LaserButton                     7
/* P50 */
extern const uint_least8_t              LaserDiode_CONST;
#define LaserDiode                      8
/* P63 */
extern const uint_least8_t              SDA2_CONST;
#define SDA2                            9
/* P53 */
extern const uint_least8_t              SCL2_CONST;
#define SCL2                            10
/* P62 */
extern const uint_least8_t              Alarm_CONST;
#define Alarm                           11
/* P64 */
extern const uint_least8_t              IR_EN_CONST;
#define IR_EN                           12
#define CONFIG_TI_DRIVERS_GPIO_COUNT    13

/* LEDs are active high */
#define CONFIG_GPIO_LED_ON  (1)
#define CONFIG_GPIO_LED_OFF (0)

#define CONFIG_LED_ON  (CONFIG_GPIO_LED_ON)
#define CONFIG_LED_OFF (CONFIG_GPIO_LED_OFF)


/*
 *  ======== SPI ========
 */

extern const uint_least8_t              CONFIG_NWP_SPI_CONST;
#define CONFIG_NWP_SPI                  0
/*
 *  MOSI: P52
 *  MISO: P53
 *  SCLK: P45
 */
extern const uint_least8_t              CONFIG_SPI_0_CONST;
#define CONFIG_SPI_0                    1
#define CONFIG_TI_DRIVERS_SPI_COUNT     2


/*
 *  ======== Timer ========
 */

extern const uint_least8_t                  DebounceTimer_CONST;
#define DebounceTimer                       0
extern const uint_least8_t                  AlarmTimer_CONST;
#define AlarmTimer                          1
extern const uint_least8_t                  ButtonDebounceConifg_CONST;
#define ButtonDebounceConifg                2
#define CONFIG_TI_DRIVERS_TIMER_COUNT       3

/*
 *  ======== UART ========
 */

/*
 *  TX: P55
 *  RX: P57
 *  XDS110 UART
 */
extern const uint_least8_t              CONFIG_UART_0_CONST;
#define CONFIG_UART_0                   0
#define CONFIG_TI_DRIVERS_UART_COUNT    1


/*
 *  ======== Watchdog ========
 */

extern const uint_least8_t                  CONFIG_WATCHDOG_0_CONST;
#define CONFIG_WATCHDOG_0                   0
#define CONFIG_TI_DRIVERS_WATCHDOG_COUNT    1


/*
 *  ======== Board_init ========
 *  Perform all required TI-Drivers initialization
 *
 *  This function should be called once at a point before any use of
 *  TI-Drivers.
 */
extern void Board_init(void);

/*
 *  ======== Board_initGeneral ========
 *  (deprecated)
 *
 *  Board_initGeneral() is defined purely for backward compatibility.
 *
 *  All new code should use Board_init() to do any required TI-Drivers
 *  initialization _and_ use <Driver>_init() for only where specific drivers
 *  are explicitly referenced by the application.  <Driver>_init() functions
 *  are idempotent.
 */
#define Board_initGeneral Board_init

#ifdef __cplusplus
}
#endif

#endif /* include guard */
