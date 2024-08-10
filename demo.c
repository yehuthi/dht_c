#include <pico/stdlib.h>
#include <pico/cyw43_arch.h>
#include "./src/dht.h"

int main(int argc, char **argv) {
	stdio_init_all();
	cyw43_arch_init();
#define DHT 15
	dht_begin(DHT);
	sleep_ms(1000); // wait for stability
	for(;;) {
		printf("reading..\n");
		const struct dht_data data = dht_read(DHT, -1u);
		const struct dht_info info = dht_data_info(data);
#undef DHT
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
