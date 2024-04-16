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

#include "neopixel.h"
#include <colors.h>
#include "IoTTimer.h"
 bool hallState;
 
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
 
  
   
  // Put initialization like pinMode and begin functions here
}


// loop() runs over and over again, as quickly as it can execute.
void loop() {
  display.setCursor(0,0); // top of loop
  

//Serial.printf("%d\n",inputValue);
 
 
    inputValue = digitalRead(D3);
    if(!inputValue){
      pixelFill(0,12,green);
      display.printf("Alarm Secured");
      display.display();
      display.clearDisplay();
      Serial.printf("green\n");
      SystemSleepConfiguration config;
      config.mode(SystemSleepMode::STOP).gpio(D3, RISING);
      System.sleep(config);
      
      
  }
  else{
    
    
        
     pixelFill(0,12,red);
     display.printf("Alarm Unsecured");
     display.display();
     display.clearDisplay();
      }
     
      
  
  
  }
  
  

  // Example: Publish event to cloud every 10 seconds. Uncomment the next 3 lines to try it!
  // Log.info("Sending Hello World to the cloud!");
  // Particle.publish("Hello world!");
  // delay( 10 * 1000 ); // milliseconds and blocking - see docs for more info!



void pixelFill(int start, int end, int pixColor){ 
  int i;
   for(i=start; i<end; i ++){
    pixel.setPixelColor (i, pixColor);
    
   }
    pixel.show();
    }