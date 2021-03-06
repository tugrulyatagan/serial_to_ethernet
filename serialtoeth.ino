#include <Dns.h>

// cs pini D8 14e kütüphane icinde alindi
// Enc28J60Network_H icinde
// #define ENC28J60_CONTROL_CS     8

#include <UIPEthernet.h>
#include <avr/wdt.h>

EthernetClient client;
signed long next;
String serialInput = "";
char serialCharacter;
IPAddress remote_addr;

void setup() {
  Serial.begin(9600);
  serialInput.reserve(50);

  wdt_enable(WDTO_8S);

  uint8_t mac[6] = { 
    0x00,0x01,0x02,0x03,0x04,0x05     };
  Ethernet.begin(mac);

//  Serial.print("\nlocalIP: ");
//  Serial.println(Ethernet.localIP());
//  Serial.print("subnetMask: ");
//  Serial.println(Ethernet.subnetMask());
//  Serial.print("gatewayIP: ");
//  Serial.println(Ethernet.gatewayIP());
//  Serial.print("dnsServerIP: ");
//  Serial.println(Ethernet.dnsServerIP());
//  Serial.println();


  DNSClient dns;
  dns.begin(Ethernet.dnsServerIP());
  dns.getHostByName("ituieeelab.org", remote_addr);

//  Serial.println("remote ip:");
//  Serial.print(remote_addr);

  next = 0;
}

void loop() {
  //Serial.print(">>> ");
  serialInput = "";
  while(!Serial.available()){
    wdt_reset();
  }
  while(Serial.available()) {
    delay(2);
    serialCharacter = Serial.read();
    serialInput.concat(serialCharacter);
    if (serialCharacter == '}') {
      break;
    } 
  }

  next = millis() + 6000; // timeout

  //if (client.connect("ituieeelab.org",6000)){
  if (client.connect(remote_addr,6000)){
    //Client connected
    //Serial.println(serialInput);
    client.println(serialInput);

    while(client.available()==0){
      if (next - millis() < 0)
        goto close;
    }
    int size;
    //Serial.print("<<< ");
    while((size = client.available()) > 0) {
      uint8_t* msg = (uint8_t*)malloc(size);
      size = client.read(msg,size);
      Serial.write(msg,size);
      free(msg);
    }
    Serial.println();
close:
    //disconnect client
    client.stop();
  }
  else
    Serial.println("Client connect failed\n");
}


