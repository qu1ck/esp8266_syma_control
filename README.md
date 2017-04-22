# Syma helicopter IR protocol receiver

This is another simple project that I built to learn how to use more arduino
and esp8266 libraries. This time it's semi-useful: it displays the controls
of a Syma helicopter IR remote.

Had some fun decoding the protocol. Only after digging quite far did I find this
invaluable [article by Jim Hung](http://www.jimhung.co.uk/?p=901) which explains
how the protocol works in great detail.

I would like to think that my implementation of decoding is much simpler. Just
count the time between impulse rising edge (or falling since I'm using inverting
output of 38khz IR receiver IC) and push those bits into a uint32_t. After 32
impulses (not counting the preambula bit) you are done. Your yaw, pitch,
throttle and trim are encoded into the 4 bytes of uint32_t type in MSB to LSB
order.

The rest of the code is just display output including some nice visuals.

Dependencies:
* Brzo I2C: `platformio lib install 335`
* ESP8266_SSD1306 driver: `platformio lib install 562`

Note that as of this writing (11/19/2016) latest published version of SSD1306
driver lib has annoying bugs with drawing rectangles that are fixed in trunk [here](https://github.com/squix78/esp8266-oled-ssd1306/commit/3759e51365fadb78955811457295d8b82f90680a)
and [here](https://github.com/squix78/esp8266-oled-ssd1306/commit/abd5e8e57f1b57e43074e44e9ec34a7585d97f15).
You may want to patch those in.

To make it work connect SDA and SCL to D1 and D2 respectively, hook up a button
to pull D3 to the ground and upload the firmware.

On a press of a button the program will circulate between following modes:
* Draw random rectangles
* Draw random circles
* Draw random lines
* Draw a kawaii face (with a trick)

ESP8266 is fast enough to provide very decent fps. The screen driver lib is also
quite optimized so the first 3 modes will be very fast.

![Picture of a kawaii face](https://raw.githubusercontent.com/qu1ck/nodemcu_kawaii_oled_test/master/resources/demo1.jpg)
