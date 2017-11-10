#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#include "esp8266/gpio_register.h"

#define GPIO_PIN_1              (BIT(1))  /// Pin 1 selected.
#define GPIO_PIN_0              (BIT(0))  /// Pin 0 selected.
#define GPIO_PIN_2              (BIT(2))  /// Pin 2 selected.
#define GPIO_PIN_3              (BIT(3))  /// Pin 3 selected.
#define GPIO_PIN_4              (BIT(4))  /// Pin 4 selected.
#define GPIO_PIN_5              (BIT(5))  /// Pin 5 selected.
#define GPIO_PIN_6              (BIT(6))  /// Pin 6 selected.
#define GPIO_PIN_7              (BIT(7))  /// Pin 7 selected.
#define GPIO_PIN_8              (BIT(8))  /// Pin 8 selected.
#define GPIO_PIN_9              (BIT(9))  /// Pin 9 selected.
#define GPIO_PIN_10             (BIT(10)) /// Pin 10 selected.
#define GPIO_PIN_11             (BIT(11)) /// Pin 11 selected.
#define GPIO_PIN_12             (BIT(12)) /// Pin 12 selected.
#define GPIO_PIN_13             (BIT(13)) /// Pin 13 selected.
#define GPIO_PIN_14             (BIT(14)) /// Pin 14 selected.
#define GPIO_PIN_15             (BIT(15)) /// Pin 15 selected.
#define GPIO_PIN_All            (0xFFFF)  /// All pins selected.
#define GPIO_PIN_NONE           (0x0000)  /// No pin selected.

#define GPIO_PIN_REG_0          PERIPHS_IO_MUX_GPIO0_U
#define GPIO_PIN_REG_1          PERIPHS_IO_MUX_U0TXD_U
#define GPIO_PIN_REG_2          PERIPHS_IO_MUX_GPIO2_U
#define GPIO_PIN_REG_3          PERIPHS_IO_MUX_U0RXD_U
#define GPIO_PIN_REG_4          PERIPHS_IO_MUX_GPIO4_U
#define GPIO_PIN_REG_5          PERIPHS_IO_MUX_GPIO5_U
#define GPIO_PIN_REG_6          PERIPHS_IO_MUX_SD_CLK_U
#define GPIO_PIN_REG_7          PERIPHS_IO_MUX_SD_DATA0_U
#define GPIO_PIN_REG_8          PERIPHS_IO_MUX_SD_DATA1_U
#define GPIO_PIN_REG_9          PERIPHS_IO_MUX_SD_DATA2_U
#define GPIO_PIN_REG_10         PERIPHS_IO_MUX_SD_DATA3_U
#define GPIO_PIN_REG_11         PERIPHS_IO_MUX_SD_CMD_U
#define GPIO_PIN_REG_12         PERIPHS_IO_MUX_MTDI_U
#define GPIO_PIN_REG_13         PERIPHS_IO_MUX_MTCK_U
#define GPIO_PIN_REG_14         PERIPHS_IO_MUX_MTMS_U
#define GPIO_PIN_REG_15         PERIPHS_IO_MUX_MTDO_U

#define GPIO_PIN_FUNCTION_1     0
#define GPIO_PIN_FUNCTION_2     1
#define GPIO_PIN_FUNCTION_3     2
#define GPIO_PIN_FUNCTION_4     3
#define GPIO_PIN_FUNCTION_5     4

#define GPIO_PIN_REG(i) \
    (i==0) ? GPIO_PIN_REG_0:  \
    (i==1) ? GPIO_PIN_REG_1:  \
    (i==2) ? GPIO_PIN_REG_2:  \
    (i==3) ? GPIO_PIN_REG_3:  \
    (i==4) ? GPIO_PIN_REG_4:  \
    (i==5) ? GPIO_PIN_REG_5:  \
    (i==6) ? GPIO_PIN_REG_6:  \
    (i==7) ? GPIO_PIN_REG_7:  \
    (i==8) ? GPIO_PIN_REG_8:  \
    (i==9) ? GPIO_PIN_REG_9:  \
    (i==10)? GPIO_PIN_REG_10: \
    (i==11)? GPIO_PIN_REG_11: \
    (i==12)? GPIO_PIN_REG_12: \
    (i==13)? GPIO_PIN_REG_13: \
    (i==14)? GPIO_PIN_REG_14: \
    GPIO_PIN_REG_15

#define GPIO_PIN_ADDR(i)        (GPIO_PIN0_ADDRESS + i*4)

#define GPIO_ID_IS_PIN_REGISTER(reg_id) \
    ((reg_id >= GPIO_ID_PIN0) && (reg_id <= GPIO_ID_PIN(GPIO_PIN_COUNT-1)))

#define GPIO_REGID_TO_PINIDX(reg_id) ((reg_id) - GPIO_ID_PIN0)

typedef enum {
  GPIO_INT_DISABLE = 0, /// disable GPIO interrupt.
  GPIO_INT_RISING_EDGE = 1, /// GPIO interrupt at the rising edge.
  GPIO_INT_FALLING_EDGE = 2, /// GPIO interrupt at the falling edge.
  GPIO_INT_BOTH = 3, /// GPIO interrupt at both, rising and falling, edges.
  GPIO_INT_LOW = 4, /// GPIO interrupt at low level.
  GPIO_INT_HIGH = 5 /// GPIO interrupt at high level.
} GPIO_IntType;

typedef enum {
  GPIO_TYPE_INPUT = 0x0, /// Pin configured as Input.
  GPIO_TYPE_OPEN_DRAIN, /// Pin configured as an Open Drain Output.
  GPIO_TYPE_OUTPUT, /// Pin configured as a Push-pull Output.
  GPIO_TYPE_SIGMA_DELTA, /// Pin configured as sigma delta (???);
} GPIO_PinType;

typedef uint16_t GPIO_Pin;

void GPIO_Init(GPIO_Pin pin);

void GPIO_SetPinType(GPIO_Pin pin, GPIO_PinType type);

void GPIO_PullUpEnable(GPIO_Pin pin);

void GPIO_PullUpDisable(GPIO_Pin pin);

void GPIO_SetPinLow(GPIO_Pin pin);

void GPIO_SetPinHigh(GPIO_Pin pin);

void GPIO_SetPinToggle(GPIO_Pin pin);

GPIO_Pin GPIO_GetPin(GPIO_Pin pin);

void GPIO_IntSetType(GPIO_Pin pin, GPIO_IntType type);

void GPIO_IntClear();

void GPIO_IntEnable();

void GPIO_IntDisable();

void GPIO_IntRegisterIsr(void *pfunc, void* arg);

GPIO_Pin GPIO_IntGetStatus();

void GPIO_WakeUpEnable(GPIO_Pin pin, GPIO_IntType type);

void GPIO_WakeUpDisable(GPIO_Pin pin, GPIO_IntType type);

void GPIO_16_SetType(GPIO_PinType type);

void GPIO_16_SetHigh();

void GPIO_16_SetLow();

void GPIO_16_SetToggle();

GPIO_Pin GPIO_16_Get();

#ifdef __cplusplus
}
#endif

#endif
