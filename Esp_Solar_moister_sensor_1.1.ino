#include <ESP8266WiFi.h>
#include "definitions.h"
#include <PubSubClient.h>
  

unsigned long previousMillis = 0;
int tryCount= 0;
int moisture = 0;
char message_buff[100];

IPAddress server(mqtt_host[0],mqtt_host[1],mqtt_host[2],mqtt_host[3]);
WiFiClient wifiClient;
PubSubClient client(wifiClient);
int watering = 0;
int count = 0;
    





void doubleToString(double invalue, char* invVarName) { 
  
    int lengthBuffMqtt;
    if ( invalue > -10 && invalue < 0) {
      lengthBuffMqtt = 4;
    } else if (invalue >= 0 && invalue < 10) {
      lengthBuffMqtt = 3;
    } else if (invalue >= 10 && invalue < 100) {
      lengthBuffMqtt = 4;
    } else {
      lengthBuffMqtt = 5;
    }
    char outValue[lengthBuffMqtt];
    dtostrf(invalue, lengthBuffMqtt, 1, outValue);
    client.publish(invVarName, outValue);
 }

void reconnect() {
  // Loop until we're reconnected
while (!client.connected()) {
  #if SerialDebug
Serial.print("Attempting MQTT connection...");
#endif
    
    // Attempt to connect
    if (client.connect(mqtt_clientid)) {
      tryCount++;
client.subscribe(mqtt_sub_topic);
      #if SerialDebug
    Serial.println("connected");
      #endif   
    } else {
      #if SerialDebug
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
#endif
      if (tryCount > retryNumber){
        ESP.deepSleep(conFailSleep * 60000000); // Test
        }else{
           // Wait 5 seconds before retrying
      delay(conectionTimeout*1000);
          }
     
    }
  }




    
  }

  void callback(char* topic, byte* payload, unsigned int length) {
  
  
  
int i = 0;
for (i = 0; i < length; i++) {
    message_buff[i] = payload[i];
  }
  message_buff[i] = '\0';
  String msgString = String(message_buff);

#if SerialDebug
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  Serial.print(message_buff);
  Serial.println();
  #endif

  

  
  if (msgString == "ON" ) {
    watering = 1;
  }else if(msgString == "OFF"){
    watering = 0;
    }

}

void setup() {
  #if SleepTest
pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  #endif
#if SerialDebug
Serial.begin(115200);
  Serial.setTimeout(2000);
#endif
  pinMode(A0, INPUT);
  pinMode(Out1, OUTPUT);
  digitalWrite(Out1, LOW);
#if SleepTest
pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output

  #endif
  #if SerialDebug
  Serial.print("Connecting to ");
  Serial.println(ssid);
  #endif
  client.setServer(server, 1883);
  client.setCallback(callback);

  
  //WiFi.mode(WIFI_STA);
  WiFi.softAPdisconnect(true);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    #if SerialDebug
    Serial.print(".");
    #endif
  }
#if SerialDebug
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
#endif
  #if SleepTest
 digitalWrite(LED_BUILTIN, HIGH); 
  #endif
}

void loop() {
  unsigned long currentMillis = millis();
if (!client.connected()) {
    reconnect();
    previousMillis = currentMillis;
  }
  client.loop();
  
if(count == 0){
  #if SerialDebug
  Serial.println("count = 0");
#endif
digitalWrite(Out1, HIGH );
count++;
}else if(count == 2){
  #if SerialDebug
  Serial.println("count = 2");
#endif
int adcValue = analogRead(A0);

moisture = map(adcValue, 700,1024,100,0);
digitalWrite(Out1, LOW );
doubleToString(moisture, "Moisture_Node_1");
#if SerialDebug
      Serial.print("Mapped input value = ");
     
      Serial.println(moisture);
      Serial.print("Analog input value = ");
     
      Serial.println(adcValue);
#endif
 count++;
  }else if(count == 4){
#if SerialDebug
  Serial.println("count = 4");
#endif
    if(watering == 0){
 

#if SleepTest
#if SerialDebug
  Serial.println("I'm awake, but I'm going into deep sleep mode for 1 minute");
#endif
 ESP.deepSleep(sleepNotWateringTestTimeS * 60000000); // Test   
#else
#if SerialDebug
  Serial.println("I'm awake, but I'm going into deep sleep mode for 33 minutes");
#endif
   ESP.deepSleep(sleepNotWateringTimeS * 60000000); 
#endif  
    }else{
     

#if SleepTest
 #if SerialDebug
  Serial.println("I'm awake, but I'm going into deep sleep mode for 30 seconds");
  
#endif
 ESP.deepSleep(sleepWateringTestTimeS * 60000000); // Test 
#else
Serial.println("I'm awake, but I'm going into deep sleep mode for 5 minutes");
  ESP.deepSleep(sleepWateringTimeS * 60000000); 
#endif
      }
   }


  if(currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;     
count++;
}
 
}
