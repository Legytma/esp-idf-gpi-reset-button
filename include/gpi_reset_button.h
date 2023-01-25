/**
 * Copyright 2023 Legytma Soluções Inteligentes LTDA
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * gpi_button.h
 *
 *  Created on: 15 de ago de 2022
 *      Author: Alex Manoel Ferreira Silva
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>

#include <sys/types.h>

#include "esp_bit_defs.h"

#include "gpi.h"

#include "sdkconfig.h"

#define GPI_RESET_BUTTON_CONFIG_DEFAULT                                         \
	{                                                                           \
		.mask                 = BIT64(CONFIG_GPI_RESET_BUTTON_GPIO),            \
		.reset_delay          = CONFIG_GPI_RESET_BUTTON_THRESHOLD_RESET_DELAY,  \
		.reboot_delay         = CONFIG_GPI_RESET_BUTTON_THRESHOLD_REBOOT_DELAY, \
		.threshold_reset_wifi = CONFIG_GPI_RESET_BUTTON_THRESHOLD_RESET_WIFI,   \
		.pressed_logic_level  = CONFIG_GPI_RESET_BUTTON_PRESSED_LOGIC_LEVEL,    \
		.press_time = 0, .release_time = 0, .press_state = false,               \
		.threshold_reset_factory =                                              \
			CONFIG_GPI_RESET_BUTTON_THRESHOLD_RESET_FACTORY,                    \
		.gpi_config = {                                                         \
			.threshold    = 10,                                                 \
			.sample_count = 3,                                                  \
			.output_value = 0,                                                  \
			.input_config =                                                     \
				{                                                               \
					.intr_type    = GPIO_INTR_DISABLE,                          \
					.pin_bit_mask = BIT64(CONFIG_GPI_RESET_BUTTON_GPIO),        \
					.mode         = GPIO_MODE_INPUT,                            \
					.pull_down_en = GPIO_PULLDOWN_DISABLE,                      \
					.pull_up_en   = GPIO_PULLUP_ENABLE,                         \
				},                                                              \
			.output_config =                                                    \
				{                                                               \
					.intr_type    = GPIO_INTR_DISABLE,                          \
					.pin_bit_mask = 0,                                          \
					.mode         = GPIO_MODE_OUTPUT,                           \
					.pull_down_en = GPIO_PULLDOWN_DISABLE,                      \
					.pull_up_en   = GPIO_PULLUP_ENABLE,                         \
				},                                                              \
		},                                                                      \
	}

typedef struct gpi_reset_button_config_s {
	gpi_config_t gpi_config;
	uint64_t     mask;
	uint64_t     threshold_reset_wifi;
	uint64_t     threshold_reset_factory;
	uint8_t      reset_delay;
	uint8_t      reboot_delay;
	clock_t      press_time;
	clock_t      release_time;
	uint64_t     pressed_logic_level;
	bool         press_state;
} gpi_reset_button_config_t;

#ifdef __cplusplus
extern "C" {
#endif
void gpi_reset_button_init(gpi_reset_button_config_t* config);
void gpi_reset_button_deinit();
#ifdef __cplusplus
}
#endif
