# dht_c

A C library for the DHT11 temperature-humidity sensor.

Currently only implemented for the Raspberry Pi Pico SDK; support for additional boards is planned.

## Usage

See [`demo.c`](https://github.com/yehuthi/dht_c/blob/master/demo.c#L15-L31), which this section will break down.

- `dht_begin` sets the given pin to a ready state from which communication between the microcontroller and the sensor can begin. Make sure to not intefere the communication process (e.g. by setting its value or changing its function).
- Make sure the sensor has been powered on for at least a second before reading its data, to give the sensor time to stabilize, per the sensor's datasheet.
- `dht_read` requests and receives raw data from the sensor.
  - It takes a "timeout" value which is the amount of cycles it may spend in spin-loops.
    This is to mitigate communication process failures (e.g. disconnects) which can cause infinite loops. You can roughly set a realtime value by knowing your processor's frequency, or and when appropriate, you can simply not care and pass `-1u`.
  - The operation may also fail because the data received by the sensor, which includes a checksum, is invalid.
  - Check if either one of these errors occurred on the result of a `dht_read` with `dht_data_error`.
  - The raw data is quite usable by itself but you might want to use `dht_data_temperature` or `dht_data_humidity` to just obtain the information as floating-point numbers.
