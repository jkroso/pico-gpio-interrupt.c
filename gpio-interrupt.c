#ifndef PICO_GPIO_INTERRUPT_C
#define PICO_GPIO_INTERRUPT_C

#include "pico/stdlib.h"
#include <stdlib.h>

typedef void (*handler)(void *argument);

typedef struct {
  void * argument;
  handler fn;
} closure_t;

closure_t handlers[NUM_BANK0_GPIOS] = {NULL};

void handle_interupt(uint gpio, uint32_t events) {
  closure_t handler = handlers[gpio];
  // The SDK sends every GPIO interrupt here, including pins we never listened to
  if (handler.fn) handler.fn(handler.argument);
}

void listen(uint pin, int condition, handler fn, void *arg) {
  // Store the handler first so an interrupt that fires on enable can find it
  handlers[pin] = (closure_t){ arg, fn };
  gpio_init(pin);
  gpio_pull_up(pin);
  gpio_set_irq_enabled_with_callback(pin, condition, true, handle_interupt);
}

#endif
