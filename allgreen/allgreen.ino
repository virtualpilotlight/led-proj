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

  allRed();
  
  int randoGreen = random(25, 256);
  int randoBlue = random(25, 256);
  
  allGreenOn(100, randoGreen, randoBlue);

  allGreenOff(100, randoGreen, randoBlue);
  
}

void allOff(){
   for (int i = 0; i <= LED_COUNT; i++){
    strip.setPixelColor(i, 0, 0, 0);
  }
}

void allRed(){
  for (int i = 0; i <= LED_COUNT; i++){
    strip.setPixelColor(i, 255, 0, 0);
  }
  strip.show();
}

void allGreenOn(int speed, int randoGreen, int randoBlue){

  int maxBrightness = 25;

  for (int n = 0; n <= LED_COUNT; n++){
     
     for (int i = 0; i <= maxBrightness; i++){
      int dimGreen = (randoGreen * i) /  maxBrightness;
      int dimBlue = (randoBlue * i) / maxBrightness; 
      strip.setPixelColor(n, 0, dimGreen, dimBlue);
      strip.show();
      delay(speed);
     }
  }
}

void allGreenOff(int speed, int randoGreen, int randoBlue){
  
  int maxBrightness = 25;

  for (int n = 0; n <= LED_COUNT; n++){

     for (int i = maxBrightness; i >= 0; i--){
      int briteGreen = (randoGreen * i) / maxBrightness;
      int briteBlue = (randoBlue * i) / maxBrightness;
      strip.setPixelColor(n, 0, briteGreen, briteBlue);
      strip.show();
      delay(speed);
     }
  }
}
