#define SerialDebug false
#define SleepTest false //  false/true
int Out1 = 14; //output pin    
const char* ssid     = "";
const char* password = "";
const char mqtt_clientid[] = "ESP_MoistureNode1"; // MQTT ClientID 
const char mqtt_sub_topic[] = "Command_WaterSpliterContorller_Out1";
int mqtt_host[4] = {192,168,100,5}; //Server IP
char mqtt_port[6] = "1883";
char mqtt_user[32] = "";
char mqtt_pass[32] = "";
const long interval = 1000;
const int sleepNotWateringTestTimeS = 2; //min
const int sleepWateringTestTimeS = 1; //min
const int sleepNotWateringTimeS = 10; //min
const int sleepWateringTimeS = 5; //min
const int conectionTimeout = 5; // time before next try to conect in seconds
const int retryNumber = 5; //tries befor sleep
const int conFailSleep = 5; // time time for sleep if conection is failed in minutes
