#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <stdlib.h>
#include <string.h>

// constants won't change. They're used here to 
// set pin numbers:
#define D0 16     // USER LED Wake and 
#define D1 5     //MotorA
#define D2 4      //MotorA
#define D3 0      //MotorB
#define D4 2      //MotorB
#define D5 14     //Standby
#define D6 12     //PWMA

#define D8 15     //MotorC
#define D9 3      //MotorC

#define ledPin D0
#define MotorA1  D1        // the number of the LED pin
#define MotorA2  D2        // the number of the LED pin

#define MotorB1  D3        // the number of the LED pin
#define MotorB2  D4        // the number of the LED pin

#define MotorC1  D8        // the number of the LED pin
#define MotorC2  D9        // the number of the LED pin

#define STANDBY  D5

#define PWMPORTA D6

#define ALIAS   "PartiiRobot_Cleint1"

int duty_cycle = 300; //0..1023 (0% ถึง 100%)
int updown = 0;

const char* ssid     = ""; // wifi name
const char* password = ""; // wifi password

WiFiClient WIFI_CLIENT;
WiFiUDP Udp;
unsigned int localUdpPort = 4210;
char incomingPacket[512];

void setup() {
    Serial.begin(115200);
    Serial.println("Starting...");

    Serial.println("Start setup pin");
    delay(250);
    
    pinMode(ledPin,OUTPUT);
    pinMode(MotorA1,OUTPUT);
    pinMode(MotorA2,OUTPUT);
    
    pinMode(MotorB1,OUTPUT);
    pinMode(MotorB2,OUTPUT);
    
    pinMode(MotorC1,OUTPUT);
    pinMode(MotorC2,OUTPUT);
    
    pinMode(STANDBY,OUTPUT);
    
    pinMode(PWMPORTA,OUTPUT);

    Serial.println("Pinmode finished...");
    delay(250);

    digitalWrite(MotorA1, LOW);
    digitalWrite(MotorA2, LOW);

    digitalWrite(MotorB1, LOW);
    digitalWrite(MotorB2, LOW);

    digitalWrite(MotorC1, LOW);
    digitalWrite(MotorC2, LOW);
    
    Serial.println("Finished setup pin...");
    delay(250);

     WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) 
    {
       delay(250);
       Serial.print(".");
    }

    Serial.println("WiFi connected");  
    delay(250);
    Serial.println("IP address: ");
    delay(250);
    Serial.println(WiFi.localIP());
    delay(250);

    Udp.begin(localUdpPort);
    Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
    delay(250);

    digitalWrite(STANDBY,HIGH); //Motorn ON
    digitalWrite(ledPin,LOW); // Turn On LED
    
}

void loop() {
  
   // update the duty cycle of the PWM signal
   analogWrite( PWMPORTA, duty_cycle );
   delayMicroseconds(1000);
  
   handleUDPServer();
   delay(1);
}

void handleUDPServer() {

  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    // receive incoming UDP packets
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 512);
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }
    Serial.printf("UDP packet contents: %s\n", incomingPacket);
    Udp.endPacket();

    //String stateStr = String(incomingPacket).substring(0, len);

    char * pch ="";
    char * cmd ="";
    pch = strtok (incomingPacket,":");
    while (pch != NULL)
    {
      printf ("client name :%s\n",pch);
      if(strcmp(pch,ALIAS) == 0){
        pch = strtok (NULL, ":");
        cmd = pch;
      }
      break;
    }

    Serial.printf("State: %s\n", cmd);
    
    if(strcmp(cmd,"Go") == 0 ) 
      {
          digitalWrite(MotorA1, LOW);
          digitalWrite(MotorA2, HIGH);

          digitalWrite(MotorB1, LOW);
          digitalWrite(MotorB2, HIGH);

          Serial.println("Make...Go");
    
      } 
      else if (strcmp(cmd,"TurnLeft") == 0) 
      {
          digitalWrite(MotorA1, HIGH);
          digitalWrite(MotorA2, LOW);
      
          digitalWrite(MotorB1, LOW);
          digitalWrite(MotorB2, HIGH);
          
          Serial.println("Make...TurnLeft");
    }
    else if (strcmp(cmd,"TurnRight") == 0 ) 
    {
          digitalWrite(MotorA1, LOW);
          digitalWrite(MotorA2, HIGH);
      
          digitalWrite(MotorB1, HIGH);
          digitalWrite(MotorB2, LOW);
          
          Serial.println("Make...TurnRight");
    }
    else if (strcmp(cmd,"Upturn") == 0) 
    {
          digitalWrite(MotorA1, HIGH);
          digitalWrite(MotorA2, LOW);
      
          digitalWrite(MotorB1, HIGH);
          digitalWrite(MotorB2, LOW);
          
          Serial.println("Make...Upturn");
    }
    else if (strcmp(cmd,"Stop") == 0 ) 
    {
          digitalWrite(MotorA1, LOW);
          digitalWrite(MotorA2, LOW);
      
          digitalWrite(MotorB1, LOW);
          digitalWrite(MotorB2, LOW);
          
          Serial.println("Make...Stop");
    }
    else if (strcmp(cmd,"Hold") == 0 ) 
    {
          digitalWrite(MotorC1, LOW);
          digitalWrite(MotorC2, LOW);
          
          Serial.println("Make...Hold");
    }
    else if (strcmp(cmd,"Catch") == 0) 
    {
    
          digitalWrite(MotorC1, LOW);
          digitalWrite(MotorC2, HIGH);
          delay(2000);
          digitalWrite(MotorC1, LOW);
          digitalWrite(MotorC2, LOW);
          
          Serial.println("Make...Catch");
    }
    else if (strcmp(cmd,"Put") == 0 ) 
    {
          digitalWrite(MotorC1, HIGH);
          digitalWrite(MotorC2, LOW);
          delay(2000);
          digitalWrite(MotorC1, LOW);
          digitalWrite(MotorC2, LOW);
          
          Serial.println("Make...Put");
    }else if( strcmp(cmd,"IncreaseSpeed") == 0 ) 
    {
          duty_cycle = duty_cycle+100;
          Serial.print(duty_cycle);
          Serial.println();
    }else if( strcmp(cmd,"DecreaseSpeed") == 0 ) 
    {
        duty_cycle = duty_cycle-100;
        Serial.print(duty_cycle);
        Serial.println();
    }
    
  }
}
