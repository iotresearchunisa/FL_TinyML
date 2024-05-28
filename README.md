Raspberry connessione ssh: 
    hostname: pi3
    username: pi
    password: 123456789

Lo script MQTT su raspberry si avvia in automatico e salva i log in
 /home/pi/log.txt

Per avviare manualmente:
pkill mqtt/mqtt.py &&  /usr/bin/python /home/pi/mqtt/mqtt.py


LA ESP8266 ARDUINO E' WEMOS D1 R1:
Aumenta CPU FREQ
MMU 16+48

LA ESP32 ARDUINO E' WEMOS D1 R32
LA ESP32 E' ESP32 DEV MODULE

ARDUINO MKR1010:
Fa i capricci se i path dei file sono lunghi!

SD ESP32
GND	    GND
5V	    5V o VIN
MISO	19
MOSI	23
CLK/SCK	18
CS	    5

SD ESP8266 
GND	    GND
5V	    5V o VIN
MISO	MISO
MOSI	MOSI
CLK/SCK	SCK
CS      D8

SD ARDUINO MKR1010
GND	    GND
5V	    5V o VIN
MISO	MISO
MOSI	MOSI
CLK/SCK	SCK
CS      4 