serial_to_ethernet
==================

Serial to ethernet bridge for Atmel microcontrollors with Arduino and uIP ethernet library.
It listens serial input (RX) end when input received, it sends to server (in this repo which is adjust as "IPAddress(192,168,1,100),6000") then it receives server response and sends to serial output (TX).


Be careful CS pin moved to D8 pin of Arduino in uIP library source file.
