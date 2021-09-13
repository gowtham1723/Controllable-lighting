#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>


const char* ssid = "Ragu pc";
const char* password = "87654321";
const char* mqtt_server = "m16.cloudmqtt.com";
const int mqttPort =    17235;
const char* mqttUser = "pjekrmyg";
const char* mqttPassword = "0qFPGUJ_eoJC";

WiFiClient espClient;
PubSubClient client(espClient);

String wifistring,topicmsg,intensitystring;
int intensityvalue,rvalue,gvalue,bvalue;


int const rOutPin = 12;
int const gOutPin = 14;
int const bOutPin = 4;

void RGB_light(int r,int g,int b)
{
  analogWrite(rOutPin,r);
  analogWrite(gOutPin,g);
  analogWrite(bOutPin,b);
  
}
void wifi_change_intensity(int intensityvalue,int rvalue,int gvalue,int bvalue)
{

  rvalue=rvalue*intensityvalue/100;
  gvalue=gvalue*intensityvalue/100;
  bvalue=bvalue*intensityvalue/100;
  RGB_light(rvalue,gvalue,bvalue);
}

void wifi_colour_change(String colour)
{
  if(colour=="red")
  {
    RGB_light(1023,0,0);
  }
  if(colour=="blue")
  {
    RGB_light(0,0,1023);
  }
  if(colour=="green")
  {
    RGB_light(0,1023,0);
  }
  if(colour=="pink")
  {
    RGB_light(512,20,294);
  }
  if(colour=="purple")
  {
    RGB_light(512,0,512);
  }
  if(colour=="orange")
  {
    RGB_light(512,127,0);
  }
  if(colour=="white")
  {
    RGB_light(512,512,512);
  }
  if(colour=="off")
  {
    RGB_light(0,0,0);
  }
}

void wifi_colour_change_and_intensity(String colour,int n)
{
  if(colour=="red")
  {
    wifi_change_intensity(n,1023,0,0);
  }
  if(colour=="blue")
  {
    wifi_change_intensity(n,0,0,1023);
  }
  if(colour=="green")
  {
    wifi_change_intensity(n,0,1023,0);
  }
  if(colour=="pink")
  {
    wifi_change_intensity(n,512,20,294);
  }
  if(colour=="purple")
  {
    wifi_change_intensity(n,512,0,512);
  }
  if(colour=="orange")
  {
    wifi_change_intensity(n,512,127,0);
  }
  if(colour=="white")
  {
    wifi_change_intensity(n,512,512,512);
  }
  if(colour=="off")
  {
    wifi_change_intensity(n,0,0,0);
  }
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network




  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);

  }

  randomSeed(micros());


}

void callback(char* topic, byte* payload, unsigned int length) {
 

  topicmsg=topic;
  if(topicmsg.equals("change colour"))
  {
  wifistring="";
  intensitystring="";
  for (int i = 0; i < length; i++) {
    wifistring=wifistring+(char)payload[i];
  }
  if(wifistring.charAt(2)==('%'))
  {
    for (int i = 0; i < 2; i++) {
    intensitystring=intensitystring+(char)payload[i];}
    intensityvalue=intensitystring.toInt();
    wifistring="";
    for (int i = 3; i < length; i++) {
    wifistring=wifistring+(char)payload[i];}
    wifi_colour_change_and_intensity(wifistring,intensityvalue);
  }
  else
  {
    wifi_colour_change(wifistring);
  }

  
  }
  
  else if(topicmsg.equals("colour mixing"))
  {
  wifistring="";
  for (int i = 0; i < 4; i++) {
    wifistring=wifistring+(char)payload[i];
    rvalue=wifistring.toInt();
  }
  wifistring="";
  for (int i = 5; i < 9; i++) {
    wifistring=wifistring+(char)payload[i];
    gvalue=wifistring.toInt();
  }
  wifistring="";
  for (int i = 10; i < 13; i++) {
    wifistring=wifistring+(char)payload[i];
    bvalue=wifistring.toInt();
  }

  RGB_light(rvalue,gvalue,bvalue);}
  
}


void setup() {

  Serial.begin(115200);
   
  setup_wifi();
  client.setServer(mqtt_server, mqttPort);
  client.setCallback(callback);

  while (!client.connected()) {
    RGB_light(0,0,0);

 
    if (client.connect("ESP32Client", mqttUser, mqttPassword )) {

 
    } else {

      delay(2000);
 
    }
  }
  client.subscribe("change colour");
  client.subscribe("colour mixing");
  pinMode(rOutPin, OUTPUT);
  pinMode(gOutPin, OUTPUT);
  pinMode(bOutPin, OUTPUT);
  

}

void loop() {
      
     client.loop();

}

