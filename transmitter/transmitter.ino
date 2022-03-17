#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define led 2

RF24 radio(7, 8); // CE, CSN
const byte addresses[][6] = {"69420", "42069"};
unsigned long timer;
int angleValue;
int counter = 1;

void setup() {
  pinMode(A0, INPUT);
  pinMode(led, OUTPUT);
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(addresses[1]); // 00002
  radio.openReadingPipe(1, addresses[0]); // 00001
  radio.enableAckPayload();
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {
  int potValue = analogRead(A0);
  int angleValue = map(potValue, 0, 1023, 0, 180);
  radio.write(&angleValue, sizeof(angleValue));

 if(radio.isAckPayloadAvailable() ) {
  char text[32] = "";
  radio.read(&text, sizeof(text));
  Serial.print(text);
  Serial.println(counter);
  counter++;
  digitalWrite(led, HIGH);
  delay(1000);
  digitalWrite(led, LOW);
 }
}
