#include <PString.h>
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
/*
 Basic MQTT example with Authentication
 
  - connects to an MQTT server, providing username
    and password
  - publishes "hello world" to the topic "outTopic"
  - subscribes to the topic "inTopic"
*/
boolean state = 0;
unsigned int count;
unsigned int temp;
//byte server[] = { 54, 75, 180, 40 };
byte server[] = { 198,41,30,241};
// Update these with values suitable for your network.
byte mac[]    = { 0x02, 0x5E, 0x77, 0xCE, 0x2A, 0xCC };
byte ip[]     = { 192, 168, 22, 182 };
byte gateway[]= { 192, 168, 22, 1};
byte subnet[] = { 255, 255, 255, 0};

void callback(char* topic, byte* payload, unsigned int length);
EthernetClient ethClient;
PubSubClient client(server, 1883/*12935*/, callback, ethClient);

void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
    byte* p = (byte*)malloc(length);
  // Copy the payload to the new buffer
  memcpy(p,payload,length);
  char* text = (char*)p;
  strlcpy(text,text,length+1);
  client.publish("outTopic", p, length);
  Serial.println((char*)text);
  Serial.println(text);
  if(!strcmp(text,"test"))
    Serial.println("test");
  else
    Serial.println("Not test");
  state = !state;
  digitalWrite(2,state);
  // Free the memory
  free(p);
}

void setup()
{ pinMode(2,OUTPUT);
  Serial.begin(9600);
  //Ethernet.begin(mac, ip, gateway, subnet);
  //Ethernet.begin(mac,ip);
  Ethernet.begin(mac);
  Serial.println("LAN CONNECTED");
  if(client.connect("arduinoClient")){
  //if (client.connect("mqtt_arduino","sqgjiqvd","LZ3JY63V9XlW")) 
    //client.publish("outTopic","hello world");
    client.subscribe("cmmc/chuan/in");
    Serial.println("Authen Passed");
  }
  else {
    Serial.println("AUTHEN FAILED");
  }
  count=0;
}

void loop()
{
  client.loop();
  count++;
  if(count>=20000)
  {
  if (client.connected()) {
    temp= ((5.0*analogRead(0))/1023.0)*100.0;
    client.publish("cmmc/chuan/out","A_on,L_on,M_on");
    Serial.println("Printed");
  }
  else
  {
    Serial.println("Connect have a problem");
    //client.connect("arduinoClient", "sqgjiqvd", "LZ3JY63V9XlW"); 
    client.connect("arduinoClient");
    client.subscribe("cmmc/chuan/in");
  }
  count=0;
  }
}

///////////////// P'MangKhon //////////////////
/*
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <dht.h>
#include <stdlib.h>
#include <string.h>
#include <avr/dtostrf.h>
#define DHT22_PIN 5

// Update these with values suitable for your network.
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
byte server[] = { 198, 41, 30, 241 };
byte ip[]     = { 192, 168, 22, 252 };
char buf_temp[20];
char buf_hud[20];
String Str;
char chararray[20];

dht DHT;


void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
  Serial.println((char*)payload);
}


EthernetClient ethClient;
PubSubClient client(server, 1883, callback, ethClient);

void setup()
{
  Serial.begin(9600); 
  Ethernet.begin(mac, ip);
  client.connect("arduinoClient");
}

void loop()
{
//  client.loop();
  int chk = DHT.read22(DHT22_PIN);
  Serial.print(DHT.humidity, 1);
  Serial.print(",\t"); 
  Serial.print(DHT.temperature, 1);
  Serial.print(",\t");
  Serial.println();
  dtostrf(DHT.temperature,3,2,buf_temp);
  dtostrf(DHT.humidity,3,2,buf_hud);
  Str += buf_temp;
  Str += ",";
  Str += buf_hud;
  Str.toCharArray(chararray,20);
  Serial.println(Str);
  client.publish("cmmc/Dragon",chararray);
  Str = "";
  delay(1000);
}
*/
