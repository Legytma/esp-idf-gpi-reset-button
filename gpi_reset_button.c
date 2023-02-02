/*
 * gpi_button.c
 *
 *  Created on: 15 de ago de 2022
 *      Author: Alex Manoel Ferreira Silva
 */

#include <stdio.h>
#include <stdint.h>

#include "esp_event.h"
#include "esp_err.h"
// #include "esp_bit_defs.h"

#include "esp_rmaker_utils.h"

#include "log_utils.h"
#include "gpi_event.h"
#include "gpi.h"

#include "sdkconfig.h"

#include "gpi_reset_button.h"

LOG_TAG("gpi_reset_button");

static inline clock_t get_time_ms() {
	return (clock() * 1000 / CLOCKS_PER_SEC);
}

static void gpi_event_change_handler(void *handler_args, esp_event_base_t base,
									 int32_t id, void *event_data) {
	LOG_FUN_START_V;
	LOGD("gpi_event_change_handler(%s:GPI_AR_EVENT_CHANGE)", base);

	gpi_reset_button_config_t *config =
		(gpi_reset_button_config_t *)handler_args;
	uint64_t event_value = *((uint64_t *)event_data);

	LOGD(U64_STR "  %08X%08X  %llu", PRINT_U64(event_value),
		 ((unsigned int)(event_value >> 32)), ((unsigned int)event_value), event_value);

	bool press_state =
		(event_value & config->mask) == config->pressed_logic_level;

	if (press_state) {
		config->press_time  = get_time_ms();
		config->press_state = true;

		LOGD("press_time: %lu", config->press_time);
	} else if (config->press_state) {
		config->release_time = get_time_ms();
		config->press_state  = false;

		LOGD("release_time: %lu", config->release_time);

		clock_t diff = config->release_time - config->press_time;

		LOGD("diff: %lu", diff);

		if (diff > config->threshold_reset_factory) {
			ESP_ERROR_CHECK(esp_rmaker_factory_reset(config->reset_delay,
													 config->reboot_delay));
		} else if (diff > config->threshold_reset_wifi) {
			ESP_ERROR_CHECK(esp_rmaker_wifi_reset(config->reset_delay,
												  config->reboot_delay));
		}
	}

	LOG_FUN_END_V;
}

void gpi_reset_button_init(gpi_reset_button_config_t *config) {
	if (!gpi_is_initialized()) {
		ESP_ERROR_CHECK(gpi_init(&config->gpi_config));
	}

	// Register the specific event handlers.
	ESP_ERROR_CHECK(esp_event_handler_register(
		GPI_EVENT, GPI_EVENT_CHANGE, gpi_event_change_handler, config));
}

void gpi_reset_button_deinit() {
	ESP_ERROR_CHECK(esp_event_handler_unregister(GPI_EVENT, GPI_EVENT_CHANGE,
												 gpi_event_change_handler));
}
