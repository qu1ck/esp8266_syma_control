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

The rest of the code is just display output including some nice visuals of the
controls.

Dependencies:
* Brzo I2C: `platformio lib install 335`
* ESP8266_SSD1306 driver: `platformio lib install 562`

To make it work connect SDA and SCL to D1 and D2 respectively, hook up IR
receiver output to D3 and that's it.

![Example](https://raw.githubusercontent.com/qu1ck/esp8266_syma_control/master/resources/receiver_demo.jpg)
