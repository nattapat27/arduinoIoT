#include <SoftwareSerial.h>
#define RX 10
#define TX 11
#define SW1 5
#define SW2 6
#define SW3 7
#define M1 A0
#define M2 A1
#define M3 A2
int valueM1;
int valueM2;
int valueM3;
SoftwareSerial esp8266(RX, TX);
String AP = "LoTS_AP";
String PASS = "LoTS708AP";
String HOST = "192.168.50.77";
String PORT = "8080";
int countTrueCommand;
int countTimeCommand;
boolean found = false;

String wateringRequest;
String moistureRequest;
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
  
  esp8266.begin(115200);
  esp8266.write("AT+UART_DEF=9600,8,1,0,0\r\n");
  esp8266.begin(9600);
  
  sendCommand("AT+RST", 10, "OK");
  sendCommand("AT+CWMODE=1", 10, "OK");
  sendCommand("AT+CWJAP=\"" + AP + "\",\"" + PASS + "\"", 15, "OK");
}

void loop() {
  // put your main code here, to run repeatedly:
  valueM1 = analogRead(M1);
  valueM2 = analogRead(M2);
  valueM3 = analogRead(M3);
  Serial.println("===================================");
  Serial.print("Value Moisture 1 : ");
  Serial.println(valueM1);
  sendMoisture(1, valueM1);
  delay(1000);
  Serial.print("Value Moisture 2 : ");
  Serial.println(valueM2);
  sendMoisture(2, valueM2);
  delay(1000);
  Serial.print("Value Moisture 3 : ");
  Serial.println(valueM3);
  sendMoisture(3, valueM3);
  delay(1000);
  if(valueM1 > 250){
    openWater(SW1);
    delay(2000);
    closeWater(SW1);
  }
  if(valueM2 > 250){
    openWater(SW2);
    delay(2000);
    closeWater(SW2);
  }
  if(valueM3 > 250){
    openWater(SW3);
    delay(2000);
    closeWater(SW3);
  }
  delay(1800000);
}

void openWater(int sw){
  Serial.print(sw);
  Serial.println(" open");
  wateringRequest = "GET /watering/save/"+String(sw-4)+" HTTP/1.1\r\nHost: "+HOST+":"+PORT+"\r\nAccept: */*\r\n";
  sendValue(wateringRequest);
  digitalWrite(sw, HIGH);
}

void closeWater(int sw){
  Serial.print(sw);
  Serial.println(" close");
  digitalWrite(sw, LOW);
}

void sendMoisture(int id, int m){
  moistureRequest = "GET /moisture/save/"+String(id)+"/"+String(m)+" HTTP/1.1\r\nHost: "+HOST+":"+PORT+"\r\nAccept: */*\r\n";
  sendValue(moistureRequest);
}
void sendValue(String request){
  sendCommand("AT+CIPSTART=\"TCP\",\"" + HOST + "\"," + PORT , 16, "OK");
  sendCommand("AT+CIPSEND=" + String(request.length()+2), 40, ">");
  sendCommand(request, 40, "OK");
  sendCommand("AT+CIPCLOSE", 5, "OK");
}
void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while (countTimeCommand < (maxTime * 1))
  {
    esp8266.println(command);//at+cipsend
    if (esp8266.find(readReplay)) //ok
    {
      found = true;
      break;
    }
    Serial.print(".");
    countTimeCommand++;

  }

  if (found)
  {
    Serial.println("OYI");
    countTrueCommand++;
    countTimeCommand = 0;
  } else
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }

  found = false;
}
