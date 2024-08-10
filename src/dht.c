#include "dht.h"
#include <pico/stdlib.h>
#include <memory.h>

void dht_begin(unsigned int pin) {
	gpio_init(pin);
	gpio_set_dir(pin, GPIO_OUT);
	gpio_put(pin, 1);
}

struct dht_data dht_read(unsigned int pin, unsigned int timeout_cycles) {
	// Request
	gpio_set_dir(pin, GPIO_OUT);
	gpio_put(pin, 0);
	sleep_ms(18);
	gpio_put(pin, 1);
	sleep_us(40);
	gpio_set_dir(pin, GPIO_IN);

	// At this point (after our sleep above), HDT should have us pulled down
	// for 80us to acknowledge the request. Wait it out.
	while(!gpio_get(pin)) {
		if (!timeout_cycles--) return DHT_DATA_TIMEOUT;
		tight_loop_contents();
	}
	// Now DHT pulls up for another 80us. Wait it out too.
	while(gpio_get(pin)) {
		if (!timeout_cycles--) return DHT_DATA_TIMEOUT;
		tight_loop_contents();
	}

	uint32_t values[5] = {0};
	for (int i = 0; i < 5; i++) {
		for (int j = 7; j >= 0; j--) {
			// DHT pulls down for 50us to indicate bit transmission, wait it out
			while(!gpio_get(pin)) {
				if (!timeout_cycles--) return DHT_DATA_TIMEOUT;
				tight_loop_contents();
			}
			// DHT pulls up, measure duration to know bit value.
			uint64_t time_start = time_us_64();
			while(gpio_get(pin)) {
				if (!timeout_cycles--) return DHT_DATA_TIMEOUT;
				tight_loop_contents();
			}
			uint64_t time_delta = time_us_64() - time_start;
			if (time_delta > 50) values[i] = values[i] | (1 << j);
		}
	}

	struct dht_data data;
	data.hmd_integral = values[0];
	data.hmd_decimal  = values[1];
	data.tmp_integral = values[2];
	data.tmp_decimal  = values[3];
	data.checksum     = values[4];
	return data;
}

bool dht_data_checksum_ok(struct dht_data data) {
	const uint8_t sum = data.hmd_integral + data.hmd_decimal + data.tmp_integral + data.tmp_decimal;
	return sum == data.checksum;
}

bool dht_data_timeout_ok(struct dht_data data) {
	// memcmp might not work due to padding
	return
		data.hmd_integral != DHT_DATA_TIMEOUT.hmd_integral ||
		data.hmd_decimal  != DHT_DATA_TIMEOUT.hmd_decimal  ||
		data.tmp_integral != DHT_DATA_TIMEOUT.tmp_integral ||
		data.tmp_decimal  != DHT_DATA_TIMEOUT.tmp_decimal  ||
		data.checksum     != DHT_DATA_TIMEOUT.checksum     ;;
}

enum dht_data_integrity dht_data_error(struct dht_data data) {
	if (!dht_data_timeout_ok (data)) return DHT_DATA_INTEGRITY_TIMEOUT ;
	if (!dht_data_checksum_ok(data)) return DHT_DATA_INTEGRITY_CHECKSUM;
	return DHT_DATA_INTEGRITY_OK;
}

static float float_integral_decimal(uint8_t integral, uint8_t decimal) {
	return ((float)integral) + ((float)decimal) / 10.f;
}

float dht_data_temperature(struct dht_data data) {
	return float_integral_decimal(data.tmp_integral, data.tmp_decimal);
}

float dht_data_humidity(struct dht_data data) {
	return float_integral_decimal(data.hmd_integral, data.hmd_decimal);
}

struct dht_info dht_data_info(struct dht_data data) {
	struct dht_info info;
	info.humidity    = dht_data_humidity   (data);
	info.temperature = dht_data_temperature(data);
	return info;
}
