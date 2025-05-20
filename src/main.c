// Wokwi Custom UART Chip Example
//
// This chip implements a simple ROT13 letter substitution cipher.
// It receives a string over UART, and returns the same string with
// each alphabetic character replaced with its ROT13 substitution.
//
// For information and examples see:
// https://link.wokwi.com/custom-chips-alpha
//
// SPDX-License-Identifier: MIT
// Copyright (C) 2022 Uri Shaked / wokwi.com

#include "wokwi-api.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  uart_dev_t uart0;
} chip_state_t;

static void on_uart_rx_data(void *user_data, uint8_t byte);
static void on_uart_write_done(void *user_data);
static void chip_timer_event(void *user_data);

void chip_init(void) {
  chip_state_t *chip = malloc(sizeof(chip_state_t));

  const uart_config_t uart_config = {
    .tx = pin_init("TX", INPUT_PULLUP),
    .rx = pin_init("RX", INPUT),
    .baud_rate = 9600,
    .rx_data = on_uart_rx_data,
    .write_done = on_uart_write_done,
    .user_data = chip,
  };
  chip->uart0 = uart_init(&uart_config);

  const timer_config_t timer_config = {
    .callback = chip_timer_event,
    .user_data = chip,
  };

  timer_t timer = timer_init(&timer_config);

  timer_start(timer, 300000, true);

  // printf("UART Chip initialized!\n");
}

static void on_uart_rx_data(void *user_data, uint8_t byte) {
  chip_state_t *chip = (chip_state_t*)user_data;
  printf("Incoming UART data: 0x%02X\n", byte);
  // uint8_t data_out = byte;
  // uart_write(chip->uart0, &data_out, sizeof(data_out));
}

static void on_uart_write_done(void *user_data) {
  chip_state_t *chip = (chip_state_t*)user_data;
  // printf("UART done\n");
}

void chip_timer_event(void *user_data) {
  chip_state_t *chip = (chip_state_t*)user_data;
  unsigned char alive_str[] = {0x02, 0x00, 0x00, 0x0C, 0x07, 0x15, 0x4E, 0x6F, 0x72, 0x6D, 0x61, 0x6C, 0x20, 0x41, 0x69, 0x72, 0x66, 0x6C, 0x6F, 0x77, 0x20, 0x20, 0x16, 0x4E, 0x6F, 0x72, 0x6D, 0x61, 0x6C, 0x20, 0x41, 0x69, 0x72, 0x66, 0x6C, 0x6F, 0x77, 0x20, 0x20, 0xF4, 0x85};
    uint8_t data_out = alive_str[0];
    uart_write(chip->uart0, &data_out, sizeof(alive_str)+1);
}