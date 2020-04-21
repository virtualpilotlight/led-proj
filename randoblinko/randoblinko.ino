#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h>
#endif

#define LED_PIN   6

#define LED_COUNT   8

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  // put your setup code here, to run once:
  strip.begin();
  strip.show();
  strip.setBrightness(25);
  
  randomSeed(analogRead(0));
}

void loop() {
  // put your main code here, to run repeatedly:
  randomColor(50);


}

  void randomColor(int speed){
    // have the first pixel go from 0 to 25 brightness of a 
    // random color, do 2 quick blinks before moving to the next
    // pixel. Once this gets to the last pixel, slowley 0 to 25 
    // brightness of all the pixels with their prevously assigned 
    // random colors. each color will need 3 numbers from 0 to 255
    // inclsive.

     int randoPixel = random(0, 8);
     int randoRed = random(0, 256);
     int randoGreen = random(0, 256);
     int randoBlue = random(0, 256);
     
     for (int i = 0; i <= LED_COUNT; i++){
      strip.setPixelColor(i, 0, 0, 0); 
     }
     
     int maxBrightness = 25;

     for (int i = 0; i <= maxBrightness; i++){
      strip.setBrightness(i);
      strip.setPixelColor(randoPixel, randoRed, randoGreen, randoBlue);
      strip.show();
      delay(speed);
     }

     for (int i = maxBrightness; i >= 0; i--){
      strip.setBrightness(i);
      strip.setPixelColor(randoPixel, randoRed, randoGreen, randoBlue);
      strip.show();
      delay(speed);
     }

    delay(10);
  }
