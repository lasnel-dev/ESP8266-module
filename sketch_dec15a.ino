#include <SoftwareSerial.h>

bool DEBUG = true;

SoftwareSerial esp8266(10, 11);
int LED = 5;
int itsONled[] = {0, 0};

void sendData(String command, const int timeout, boolean DEBUG){
  esp8266.print(command);
  long int time = millis();
  while((time+timeout) > millis()){
    while(esp8266.available()){
      Serial.write(esp8266.read());
      }
    }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
  esp8266.begin(115200);
  sendData("AT+RST\r\n", 2000, DEBUG);
  sendData("AT+CWMODE=1\r\n", 1000, DEBUG);
  sendData("AT+CWJAP=\"SSID\",\"PASSWORD\"\r\n", 6000, DEBUG); //SSID - Назва мережі, PASSWORD - Пароль до мережі
  sendData("AT+CIFRS\r\n",2000, DEBUG);
  sendData("AT+CIPMUX=1\r\n", 1000, DEBUG);
  sendData("AT+CIPSERVER=1,80\r\n", 1000, DEBUG);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(esp8266.available()){
      if(esp8266.find("+IPD,")){
        int connectionId = esp8266.read()-48;

        String msg;
        esp8266.find("?");
        delay(100);
        msg = esp8266.readStringUntil(' ');
        String command1 = msg.substring(0);

        String webpage = "<html><head><title>ESP8266 WEB Switch</title>";
        webpage += "meta name=\"viewport\"content=\"widt=device-width, initial-scale=1.0\"><style>.button{background-color:orange;border: none;color: white;padding: 15px 32px;text-align: center;display: inline-block;font-size: 16px;}.cente {text-align:center;}</style>";
        webpage += "</head><body class\"centre\"><h1 class\"centre\">ESP8266 WEB SWITCH</h1>";
        
        if(command1 == "T"){
          if(itsONled[1]=1){
            digitalWrite(LED, LOW);
            itsONled[1]=0;
            webpage += "<p>LED status OFF</p>";
            }
            else{
              digitalWrite(LED, HIGH);
            itsONled[1]=1;
            webpage += "<p>LED status ON</p>";
              }
          
        }

        webpage +="<a class=\"button\"href=\"?T\">TAP<\a></body></html>";

      String cipSend = "AT+CIPSEND=";
      cipSend += connectionId;
      cipSend += ",";
      cipSend += webpage.length();
      cipSend += "\r\n";
      sendData(cipSend,200,DEBUG);
      sendData(webpage,200,DEBUG);

        String closeCommand = "AT+CIPCLOSE=";
        closeCommand+=connectionId;
        closeCommand+="\r\n";
        sendData(closeCommand, 300, DEBUG);
      }
   }
}
