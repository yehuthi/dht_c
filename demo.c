#include <pico/stdlib.h>
#include <pico/binary_info.h>
#include <stdio.h>
#include "dht.h"

#ifndef DHT_SDA
#define DHT_SDA 15
#endif

bi_decl(bi_program_description("periodically prints DHT11 sensor data"));
bi_decl(bi_1pin_with_name(DHT_SDA, "DHT11 SDA (the second) pin"));

int main() {
	stdio_init_all();
	dht_begin(DHT_SDA);
	sleep_ms(1000); // wait for stability
	for(;;) {
		printf("reading pin %d..\n", DHT_SDA);
		const struct dht_data data = dht_read(DHT_SDA, -1u);
		const struct dht_info info = dht_data_info(data);
		const char *integrity_strings[] = { "ok", "timeout error", "checksum error"};
		printf(
			"%.1f\u2103, %.1f\u1D63\u2095 [raw %d,%d,%d,%d,%d %s]\n",
			//   ^celsius    ^r    ^h (relative humidity)
			info.temperature, info.humidity,
			data.hmd_integral, data.hmd_decimal,
			data.tmp_integral, data.tmp_decimal,
			data.checksum, integrity_strings[dht_data_error(data)]
		);
		sleep_ms(3000);
	}
}
