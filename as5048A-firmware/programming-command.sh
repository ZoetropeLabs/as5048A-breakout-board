avrdude -C "/etc/avrdude/avrdude.conf" -pt44 -b19200 -P /dev/ttyACM0 -c arduino -U flash:w:".build/attiny44/firmware.hex"
