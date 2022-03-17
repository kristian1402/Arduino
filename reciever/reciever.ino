#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define led 2

RF24 radio(7, 8); // CE, CSN
const byte addresses[][6] = {"69420", "42069"};
int val = 0;
int randVal;
const char text[] = "Correct";
int counter = 0;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(1, addresses[1]); // 00002
  radio.openWritingPipe(addresses[0]); // 00001
  radio.setPALevel(RF24_PA_MIN);
  radio.enableAckPayload();
  radio.startListening();
  randVal = random(1,179);
}

void loop() {
  while (!radio.available());
  radio.read(&val, sizeof(val));
  Serial.print("Current val: ");
  Serial.print(val);
  Serial.print(" | ");
  Serial.print(randVal);
  Serial.print(" | ");
  Serial.println(counter);
  if(randVal == val){
    counter++;
    radio.writeAckPayload(1, text, sizeof(text));
    delay(200);
    randVal = random(1,179);
  }
}
