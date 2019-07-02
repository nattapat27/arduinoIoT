#define SW1 5
#define SW2 6
#define SW3 7
#define M1 A0
#define M2 A1
#define M3 A2
int valueM1;
int valueM2;
int valueM3;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(SW1, OUTPUT);
  pinMode(SW2, OUTPUT);
  pinMode(SW3, OUTPUT);
  pinMode(M1, INPUT);
  pinMode(M2, INPUT);
  pinMode(M3, INPUT);
  closeWater(SW1);
  closeWater(SW2);
  closeWater(SW3);
}

void loop() {
  // put your main code here, to run repeatedly:
  valueM1 = analogRead(M1);
  valueM2 = analogRead(M2);
  valueM3 = analogRead(M3);
  Serial.println("===================================");
  Serial.print("Value Moisture 1 : ");
  Serial.println(valueM1);
  Serial.print("Value Moisture 2 : ");
  Serial.println(valueM2);
  Serial.print("Value Moisture 3 : ");
  Serial.println(valueM3);
  if(valueM1 < 500){
    openWater(SW1);
    delay(2000);
    closeWater(SW1);
  }
  if(valueM2 < 500){
    openWater(SW2);
    delay(2000);
    closeWater(SW2);
  }
  if(valueM3 < 500){
    openWater(SW3);
    delay(2000);
    closeWater(SW3);
  }
  delay(1000);
}

void openWater(int sw){
  Serial.print(sw);
  Serial.println(" open");
  digitalWrite(sw, HIGH);
}

void closeWater(int sw){
  Serial.print(sw);
  Serial.println(" close");
  digitalWrite(sw, LOW);
}
