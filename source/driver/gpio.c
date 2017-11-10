#include "esp_common.h"
#include "portmacro.h"

#include "gpio.h"

LOCAL void gpio_set_pin_open_drain(GPIO_Pin pin) {
  uint32_t pin_number = 0;

  while (pin > 0) {

    if (pin & 0x0001) {
      portENTER_CRITICAL();

      uint32_t pin_reg = GPIO_REG_READ(GPIO_PIN_ADDR(pin_number));
      pin_reg &= (~GPIO_PIN_DRIVER_MASK);
      pin_reg |= (GPIO_PAD_DRIVER_ENABLE << GPIO_PIN_DRIVER_LSB);
      GPIO_REG_WRITE(GPIO_PIN_ADDR(pin_number), pin_reg);

      portEXIT_CRITICAL();
    }

    pin_number++;
    pin >>= 1;
  }
}

LOCAL void gpio_set_pin_sigma_delta(GPIO_Pin pin) {
  uint32_t pin_number = 0;

  while (pin > 0) {

    if (pin & 0x0001) {
      portENTER_CRITICAL();

      uint32_t pin_reg = GPIO_REG_READ(GPIO_PIN_ADDR(pin_number));
      pin_reg &= (~GPIO_PIN_SOURCE_MASK);
      pin_reg |= (0x1 << GPIO_PIN_SOURCE_LSB);
      GPIO_REG_WRITE(GPIO_PIN_ADDR(pin_number), pin_reg);
      GPIO_REG_WRITE(GPIO_SIGMA_DELTA_ADDRESS, SIGMA_DELTA_ENABLE);

      portEXIT_CRITICAL();
    }

    pin_number++;
    pin >>= 1;
  }
}

void GPIO_Init(GPIO_Pin pin) {

  uint8_t pin_number = 0;

  while (pin > 0) {
    if (pin & 0x0001) {
      uint32_t pin_reg = GPIO_PIN_REG(pin_number);

      if ((0x1 << pin_number) & (GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5)) {
        PIN_FUNC_SELECT(pin_reg, GPIO_PIN_FUNCTION_1);
      } else {
        PIN_FUNC_SELECT(pin_reg, GPIO_PIN_FUNCTION_4);
      }
    }

    pin_number++;
    pin >>= 1;
  }
}

void GPIO_SetPinType(GPIO_Pin pin, GPIO_PinType type) {

  switch (type) {
  case GPIO_TYPE_INPUT:
    GPIO_REG_WRITE(GPIO_OUT_W1TS_ADDRESS, 0);
    GPIO_REG_WRITE(GPIO_OUT_W1TC_ADDRESS, 0);
    GPIO_REG_WRITE(GPIO_ENABLE_W1TS_ADDRESS, 0);
    GPIO_REG_WRITE(GPIO_ENABLE_W1TC_ADDRESS, pin);
    break;

  case GPIO_TYPE_OUTPUT:
    GPIO_REG_WRITE(GPIO_ENABLE_W1TS_ADDRESS, pin);
    break;

  case GPIO_TYPE_OPEN_DRAIN:
    gpio_set_pin_open_drain(pin);
    break;

  case GPIO_TYPE_SIGMA_DELTA:
    gpio_set_pin_sigma_delta(pin);
    break;

  default:
    GPIO_REG_WRITE(GPIO_ENABLE_W1TC_ADDRESS, pin);
    break;
  }
}

void GPIO_PullUpEnable(GPIO_Pin pin) {
  uint32_t pin_number = 0;

  while (pin > 0) {

    if (pin & 0x0001) {
      uint32_t pin_reg = GPIO_PIN_REG(pin_number);
      PIN_PULLUP_EN(pin_reg);
    }

    pin_number++;
    pin >>= 1;
  }
}

void GPIO_PullUpDisable(GPIO_Pin pin) {
  uint32_t pin_number = 0;

  while (pin > 0) {

    if (pin & 0x0001) {
      uint32_t pin_reg = GPIO_PIN_REG(pin_number);
      PIN_PULLUP_DIS(pin_reg);
    }

    pin_number++;
    pin >>= 1;
  }
}

void GPIO_SetPinLow(GPIO_Pin pin) {
  GPIO_REG_WRITE(GPIO_OUT_W1TC_ADDRESS, pin);
  GPIO_REG_WRITE(GPIO_ENABLE_W1TS_ADDRESS, pin);
}

void GPIO_SetPinHigh(GPIO_Pin pin) {
  GPIO_REG_WRITE(GPIO_OUT_W1TS_ADDRESS, pin);
  GPIO_REG_WRITE(GPIO_ENABLE_W1TS_ADDRESS, pin);
}

void GPIO_SetPinToggle(GPIO_Pin pin) {
  GPIO_Pin state = GPIO_GetPin(pin);

  GPIO_Pin set_mask = pin & (~state);
  GPIO_Pin clear_mask = pin & state;

  GPIO_REG_WRITE(GPIO_OUT_W1TS_ADDRESS, set_mask);
  GPIO_REG_WRITE(GPIO_OUT_W1TC_ADDRESS, clear_mask);
  GPIO_REG_WRITE(GPIO_ENABLE_W1TS_ADDRESS, pin);
}

GPIO_Pin GPIO_GetPin(GPIO_Pin pin) {
  GPIO_Pin state = GPIO_REG_READ(GPIO_IN_ADDRESS) & pin;
  return state;
}

void GPIO_IntSetType(GPIO_Pin pin, GPIO_IntType type) {
  uint32 pin_number = 0;

  while (pin > 0) {
    if (pin & 0x0001) {
      portENTER_CRITICAL();

      uint32_t pin_reg = GPIO_REG_READ(GPIO_PIN_ADDR(pin_number));
      pin_reg &= (~GPIO_PIN_INT_TYPE_MASK);
      pin_reg |= (type << GPIO_PIN_INT_TYPE_LSB);
      GPIO_REG_WRITE(GPIO_PIN_ADDR(pin_number), pin_reg);

      portEXIT_CRITICAL();
    }

    pin_number++;
    pin >>= 1;
  }
}

void GPIO_IntClear() {
  uint32_t gpio_status = GPIO_REG_READ(GPIO_STATUS_ADDRESS);
  GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpio_status);
}

void GPIO_IntEnable() {
  _xt_isr_unmask((1 << ETS_GPIO_INUM));
}

void GPIO_IntDisable() {
  _xt_isr_mask((1 << ETS_GPIO_INUM));
}

void GPIO_IntRegisterIsr(void *pfunc, void* arg) {
  _xt_isr_attach(ETS_GPIO_INUM, pfunc, arg);
}

GPIO_Pin GPIO_IntGetStatus() {
  return GPIO_REG_READ(GPIO_STATUS_ADDRESS);
}

void GPIO_WakeUpEnable(GPIO_Pin pin, GPIO_IntType type) {
  // TODO: Add GPIO Wake Up Enable.
  //    uint32 pin_reg;
  //
  //    if ((intr_state == GPIO_PIN_INTR_LOLEVEL) || (intr_state == GPIO_PIN_INTR_HILEVEL)) {
  //        portENTER_CRITICAL();
  //
  //        pin_reg = GPIO_REG_READ(GPIO_PIN_ADDR(i));
  //        pin_reg &= (~GPIO_PIN_INT_TYPE_MASK);
  //        pin_reg |= (intr_state << GPIO_PIN_INT_TYPE_LSB);
  //        pin_reg |= GPIO_PIN_WAKEUP_ENABLE_SET(GPIO_WAKEUP_ENABLE);
  //        GPIO_REG_WRITE(GPIO_PIN_ADDR(i), pin_reg);
  //
  //        portEXIT_CRITICAL();
  //    }
}

void GPIO_WakeUpDisable(GPIO_Pin pin, GPIO_IntType type) {
  // TODO: Add GPIO Wake Up Disable.
  //    uint8  i;
  //    uint32 pin_reg;
  //
  //    for (i = 0; i < GPIO_PIN_COUNT; i++) {
  //        pin_reg = GPIO_REG_READ(GPIO_PIN_ADDR(i));
  //
  //        if (pin_reg & GPIO_PIN_WAKEUP_ENABLE_MASK) {
  //            pin_reg &= (~GPIO_PIN_INT_TYPE_MASK);
  //            pin_reg |= (GPIO_PIN_INTR_DISABLE << GPIO_PIN_INT_TYPE_LSB);
  //            pin_reg &= ~(GPIO_PIN_WAKEUP_ENABLE_SET(GPIO_WAKEUP_ENABLE));
  //            GPIO_REG_WRITE(GPIO_PIN_ADDR(i), pin_reg);
  //        }
  //    }
}

void GPIO_16_SetType(GPIO_PinType type) {
  switch (type) {
  case GPIO_TYPE_INPUT:
    WRITE_PERI_REG(PAD_XPD_DCDC_CONF,
        (READ_PERI_REG(PAD_XPD_DCDC_CONF) & 0xffffffbc) | (uint32)0x1); // mux configuration for XPD_DCDC and rtc_gpio0 connection

    WRITE_PERI_REG(RTC_GPIO_CONF,
        (READ_PERI_REG(RTC_GPIO_CONF) & (uint32)0xfffffffe) | (uint32)0x0); //mux configuration for out enable

    WRITE_PERI_REG(RTC_GPIO_ENABLE, READ_PERI_REG(RTC_GPIO_ENABLE) & (uint32)0xfffffffe); //out disable
    break;

  case GPIO_TYPE_OUTPUT:
    WRITE_PERI_REG(PAD_XPD_DCDC_CONF,
        (READ_PERI_REG(PAD_XPD_DCDC_CONF) & 0xffffffbc) | (uint32)0x1); // mux configuration for XPD_DCDC to output rtc_gpio0

    WRITE_PERI_REG(RTC_GPIO_CONF,
        (READ_PERI_REG(RTC_GPIO_CONF) & (uint32)0xfffffffe) | (uint32)0x0); //mux configuration for out enable

    WRITE_PERI_REG(RTC_GPIO_ENABLE,
        (READ_PERI_REG(RTC_GPIO_ENABLE) & (uint32)0xfffffffe) | (uint32)0x1); //out enable
    break;
  default:
    break;
  }
}

void GPIO_16_SetHigh() {
  WRITE_PERI_REG(RTC_GPIO_OUT, (READ_PERI_REG(RTC_GPIO_OUT) & (uint32)0xffffffff));
}

void GPIO_16_SetLow() {
  WRITE_PERI_REG(RTC_GPIO_OUT, (READ_PERI_REG(RTC_GPIO_OUT) & (uint32)0xfffffffe));
}

void GPIO_16_SetToggle() {
  if (GPIO_16_Get()) {
    GPIO_16_SetLow();
  } else {
    GPIO_16_SetHigh();
  }
}

GPIO_Pin GPIO_16_Get() {
  return READ_PERI_REG(RTC_GPIO_IN_DATA) & 1;
}

