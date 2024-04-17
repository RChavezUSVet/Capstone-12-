/* 
 * Project L13_07_Alarm
 * Author: Randall Chavez
 * Date: 03-26-2024
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include <Adafruit_MQTT.h>
#include "Adafruit_MQTT/Adafruit_MQTT_SPARK.h"
#include "Adafruit_MQTT/Adafruit_MQTT.h"
#include "credentials.h"
#include "neopixel.h"
#include <colors.h>
#include "IoTTimer.h"
 bool hallState;
 int lastInputValue;
 bool neoflash;
 
 
void pixelFill(int start,int end,int color);

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(SEMI_AUTOMATIC);

const int hallsensorpin = D3;
int inputValue;

// Run the application and system concurrently in separate threads

const int PIXELCOUNT = 12;
Adafruit_NeoPixel pixel ( PIXELCOUNT, SPI1, WS2812B); 
Adafruit_SSD1306 display(-1);

TCPClient TheClient;
Adafruit_MQTT_SPARK mqtt(&TheClient,AIO_SERVER,AIO_SERVERPORT,AIO_USERNAME,AIO_KEY); 

Adafruit_MQTT_Publish publishAlarm=Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/alarmState");

void MQTT_connect();
bool MQTT_ping();


// Show system, cloud connectivity, and application logs over USB
// View logs with CLI using 'particle serial monitor --follow'


// setup() runs once, when the device is first turned on
void setup() {

  Serial.begin (9600);
   pinMode(D3,INPUT);

   
   
   //toggle x (if x = 1 then set x = buttonState=! buttonState);
   
pixel.begin();
 pixel.setBrightness (30); 
 pixel.show();

 display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
display.setTextSize(1);
display.setTextColor(WHITE);
display.clearDisplay();
display.display();

 WiFi.on();
  WiFi.connect();
  while(WiFi.connecting()) {
    Serial.printf(".");
  }
  Serial.printf("\n\n");
 
  
   
  // Put initialization like pinMode and begin functions here
}


// loop() runs over and over again, as quickly as it can execute.
void loop() {
  display.setCursor(0,0); // top of loop
  MQTT_connect();
  MQTT_ping();
  

//Serial.printf("%d\n",inputValue);
 
 
    inputValue = digitalRead(D3);
    if(!inputValue){
      pixelFill(0,12,green);
      display.printf("Alarm Secured");
      display.display();
      display.clearDisplay();
      Serial.printf("green\n");
     
      
      
  }
  else{
    
    
        
     pixelFill(0,12,red);
     display.printf("Alarm Unsecured");
     display.display();
     display.clearDisplay();
      }
     
     if (inputValue != lastInputValue){ 
        publishAlarm.publish(inputValue);
        lastInputValue = inputValue;
     }
  
  }
  
  void MQTT_connect() {
  int8_t ret;
 
  // Return if already connected.
  if (mqtt.connected()) {
    return;
  }
 
  Serial.print("Connecting to MQTT... ");
 
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.printf("Error Code %s\n",mqtt.connectErrorString(ret));
       Serial.printf("Retrying MQTT connection in 5 seconds...\n");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds and try again
  }
  Serial.printf("MQTT Connected!\n");
  }

  // Example: Publish event to cloud every 10 seconds. Uncomment the next 3 lines to try it!
  // Log.info("Sending Hello World to the cloud!");
  // Particle.publish("Hello world!");
  // delay( 10 * 1000 ); // milliseconds and blocking - see docs for more info!

bool MQTT_ping() {
  static unsigned int last;
  bool pingStatus;

  if ((millis()-last)>120000) {
      Serial.printf("Pinging MQTT \n");
      pingStatus = mqtt.ping();
      if(!pingStatus) {
        Serial.printf("Disconnecting \n");
        mqtt.disconnect();
      }
      last = millis();
  }
  return pingStatus;
}


void pixelFill(int start, int end, int pixColor){ 
  int i;
   for(i=start; i<end; i ++){
    pixel.setPixelColor (i, pixColor);
    
   }
    pixel.show();
    }