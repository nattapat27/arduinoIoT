#include <dht.h>
dht DHT1;
dht DHT2;
#define LIGHT_R A5
#define LIGHT_L A0
#define DHT_R 6
#define DHT_L 7
void setup() {
  Serial.begin(9600);
  pinMode(LIGHT_R, INPUT);
  pinMode(LIGHT_L, INPUT);
}

void loop() {
  int chk1 = DHT1.read11(DHT_R);
  int chk2 = DHT2.read11(DHT_L);
  int light_r = analogRead(LIGHT_R);
  int light_l = analogRead(LIGHT_L);
  Serial.println("========================================");
  Serial.println("\t\tL\t\tR");
  Serial.print("light \t\t");
  Serial.print(light_l);
  Serial.print("\t\t");
  Serial.println(light_r);
  Serial.print("humidity \t");
  Serial.print(DHT2.humidity);
  Serial.print("\t\t");
  Serial.println(DHT1.humidity);
  Serial.print("temperature \t");
  Serial.print(DHT2.temperature);
  Serial.print("\t\t");
  Serial.println(DHT1.temperature);
  delay(2000);
}
