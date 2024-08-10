#pragma once
#include <stdint.h>
#include <stdbool.h>

/// DHT raw data.
struct dht_data {
	/// Humidity integral.
	uint8_t hmd_integral;
	/// Humidity decimal.
	uint8_t hmd_decimal;
	/// Temperature integral.
	uint8_t tmp_integral;
	/// Temperature decimal.
	uint8_t tmp_decimal;
	/// Checksum.
	uint8_t checksum;
};

/// Timeout value.
#define DHT_DATA_TIMEOUT ((struct dht_data){0xFF,0xFF,0xFF,0xFF,252})

/// Checks if the DHT data satisfies the checksum.
bool dht_data_checksum_ok(struct dht_data data);

/// Checks if the DHT data does not represent a timeout error.
bool dht_data_timeout_ok(struct dht_data data);

enum dht_data_integrity {
	DHT_DATA_INTEGRITY_OK,
	DHT_DATA_INTEGRITY_TIMEOUT,
	DHT_DATA_INTEGRITY_CHECKSUM,
	DHT_DATA_INTEGRITY_COUNT,
};
enum dht_data_integrity dht_data_error(struct dht_data data);

void dht_begin(unsigned int pin);
struct dht_data dht_read(unsigned int pin, unsigned int timeout_cycles);

struct dht_info {
	float humidity;
	float temperature;
};

float dht_data_temperature(struct dht_data data);
float dht_data_humidity(struct dht_data data);
struct dht_info dht_data_info(struct dht_data data);
