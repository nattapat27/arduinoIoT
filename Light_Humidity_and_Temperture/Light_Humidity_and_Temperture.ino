#include <dht.h>
#include <SoftwareSerial.h>

#define RX 10
#define TX 11
String AP = "LoTS_AP";
String PASS = "LoTS708AP";
String HOST = "192.168.50.77";
String PORT = "8080";
int countTrueCommand;
int countTimeCommand;
boolean found = false;
SoftwareSerial esp8266(RX, TX);

dht DHT1;
dht DHT2;
#define LIGHT_R A5
#define LIGHT_L A0
#define DHT_R 6
#define DHT_L 7

String tempLeftRequest, tempRightRequest;
String lightLeftRequest, lightRightRequest;
String humidityLeftRequest, humidityRightRequest;
void setup() {
  Serial.begin(9600);
  esp8266.begin(115200);
  esp8266.write("AT+UART_DEF=9600,8,1,0,0\r\n");
  esp8266.begin(9600);
  
  sendCommand("AT+RST", 10, "OK");
  sendCommand("AT+CWMODE=1", 10, "OK");
  sendCommand("AT+CWJAP=\"" + AP + "\",\"" + PASS + "\"", 15, "OK");

  pinMode(LIGHT_R, INPUT);
  pinMode(LIGHT_L, INPUT);
}

void loop() {
  int chk1 = DHT1.read11(DHT_R);
  int chk2 = DHT2.read11(DHT_L);
  int light_r = analogRead(LIGHT_R);
  int light_l = analogRead(LIGHT_L);
  float hum_r = DHT2.humidity;
  float hum_l = DHT1.humidity;
  float temp_l = DHT2.temperature;
  float temp_r = DHT1.temperature;
  Serial.print(light_l);
  Serial.print("\t\t");
  Serial.println(light_r);
  Serial.print(hum_l);
  Serial.print("\t\t");
  Serial.println(hum_r);
  Serial.print(temp_l);
  Serial.print("\t\t");
  Serial.println(temp_r);
  tempLeftRequest =  "GET /temperature/save/left/"+String(DHT2.temperature)+" HTTP/1.1\r\nHost: "+HOST+":"+PORT+"\r\nAccept: */*\r\n";
  tempRightRequest =  "GET /temperature/save/right/"+String(DHT1.temperature)+" HTTP/1.1\r\nHost: "+HOST+":"+PORT+"\r\nAccept: */*\r\n";
  sendValue(tempLeftRequest);
  sendValue(tempRightRequest);
  lightLeftRequest =  "GET /light/save/left/"+String(light_l)+" HTTP/1.1\r\nHost: "+HOST+":"+PORT+"\r\nAccept: */*\r\n";
  lightRightRequest =  "GET /light/save/right/"+String(light_r)+" HTTP/1.1\r\nHost: "+HOST+":"+PORT+"\r\nAccept: */*\r\n";
  sendValue(lightLeftRequest);
  sendValue(lightRightRequest);
  humidityLeftRequest =  "GET /humidity/save/left/"+String(DHT2.humidity)+" HTTP/1.1\r\nHost: "+HOST+":"+PORT+"\r\nAccept: */*\r\n";
  humidityRightRequest =  "GET /humidity/save/right/"+String(DHT1.humidity)+" HTTP/1.1\r\nHost: "+HOST+":"+PORT+"\r\nAccept: */*\r\n";
  sendValue(humidityLeftRequest);
  sendValue(humidityRightRequest);
  delay(900000);
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
