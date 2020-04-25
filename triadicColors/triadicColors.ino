#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h>
#endif

#define LED_PIN     6   // pin data is running on

#define LED_COUNT   8   // number of total LEDs 

#define NUM_LEDS    6   // how many to light up

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

//there are 4 color sets of 3 colors each, each color has an RGB value. 
int allColorSets [4][3][3] = { 
  { { 255, 0, 0}, { 255, 250, 0}, { 0, 0, 255} }, // Red, Yellow, Blue
  { { 255, 69, 0}, { 173, 255, 47}, { 138, 43, 226} }, // Red-orange, yellow-green, blue violet
  { { 255, 100, 0}, { 0, 128, 0}, { 148, 0, 211} }, // Orange, Green, Violet
  { { 255, 215, 0}, { 32, 178, 170}, { 199, 21, 133} }, // yellow-orange, blue-green, Red-violet
};

#define PRIMARY     0
#define WARM        1
#define SECONDARY   2
#define COOL        3

unsigned long time;

void setup() {
  // put your setup code here, to run once:
  strip.begin();              // turn the strip on
  strip.show();               // show the strip
  strip.setBrightness(25);    // set or limit brightness

  randomSeed(analogRead(0));  // initilize a random number
  
  Serial.begin(9600);


  Serial.println("Hello Computer");
  
}

unsigned long timeSinceRando = millis();

void loop() {
  // put your main code here, to run repeatedly:

  time = millis();

  unsigned long ledClock = time - timeSinceRando;

  int litPixels [NUM_LEDS];     // an array that holds the LEDs to be lit in this case 3 

  if (ledClock > 5000) {

    randomPixels(litPixels);

    while (dupePixels(litPixels) == true) {
      randomPixels(litPixels);
    }
    timeSinceRando = time;
  }  

  if (ledClock < 2500) {
    fadeUp(litPixels, SECONDARY, ( ledClock * 255)  / 2500);
  }
  else {
    fadeDown(litPixels, SECONDARY, (( ledClock * 255)  / 2500) - 255);
  }


}


void randomPixels(int litPixels[]) {
  for (int i = 0; i < NUM_LEDS; i++ ){       
    int randoPixel = random(0, LED_COUNT);   
    litPixels[i] = randoPixel;               
  }
}

bool dupePixels(int litPixels[]) {
  for (int i = 0; i < NUM_LEDS; i++ ){       // a for loop that iterates from 0 to the number of LEDs we want lit
    int randoPixel = random(0, LED_COUNT);   // gets a random number between 0 and total LEDs
    for (int n = 0; n < NUM_LEDS; n++){      // inner for loop 0 throught number of LEDs
      if (litPixels[n] == litPixels[i]){
        if (n != i){
          return true;
        }
        randoPixel = random(0, LED_COUNT);    
      }                                      
    }
  }
  return false;
}

void fadeUp(int litPixels[], int colorSet, int fadeAmount) {
  Serial.println("in fade up");
  Serial.println(fadeAmount);
  for (int i = 0; i < NUM_LEDS; i++) {       

    long red = allColorSets [colorSet][i % 3][0];         
    long green = allColorSets [colorSet][i % 3][1];
    long blue = allColorSets [colorSet][i  % 3][2];

    long redNow = red;
    long greenNow = green;
    long blueNow = blue;
    
    int n = fadeAmount % 256;
      redNow = (red * n) / 256;
      greenNow = (green * n) / 256;
      blueNow = (blue * n) / 256;
      strip.setPixelColor(litPixels[i], redNow, greenNow, blueNow);
      strip.show();
  }
}


void fadeDown(int litPixels[], int colorSet, int fadeAmount) {
  Serial.println("in fade down");
  Serial.println(fadeAmount);
  
  for (int i = 0; i < NUM_LEDS; i++) {       

    long red = allColorSets [colorSet][i % 3][0];         
    long green = allColorSets [colorSet][i % 3][1];
    long blue = allColorSets [colorSet][i  % 3][2];

    long redNow = red;
    long greenNow = green;
    long blueNow = blue;

    
    int n = 255 - (fadeAmount % 256);
      redNow = (red * n) / 256;
      greenNow = (green * n) / 256;
      blueNow = (blue * n) / 256;
      strip.setPixelColor(litPixels[i], redNow, greenNow, blueNow);
      strip.show();
  
  }
 
}

void setColors(int litPixels[], int colorSet) {
  Serial.println("in set colors");
  for (int i = 0; i < NUM_LEDS; i++) {       
    Serial.print("in for loop: ");
    Serial.print(litPixels[i]);
    Serial.println("");
    long red = allColorSets [colorSet][i % 3][0];         
    long green = allColorSets [colorSet][i % 3][1];
    long blue = allColorSets [colorSet][i  % 3][2];
    //strip.setPixelColor(litPixels[i], red, green, blue);

    long redNow = red;
    long greenNow = green;
    long blueNow = blue;
    
    for (long n = 0; n <= 255; n++) {
      redNow = (red * n) / 255;
      greenNow = (green * n) / 255;
      blueNow = (blue * n) / 255;
      strip.setPixelColor(litPixels[i], redNow, greenNow, blueNow);
      strip.show();
      delay(10);
    }
 
    delay(50);

    for (long n = 255; n > 0; n--) {
      redNow = (red * n) / 255;
      greenNow = (green * n) / 255;
      blueNow = (blue * n) / 255;
      strip.setPixelColor(litPixels[i], redNow, greenNow, blueNow);
      strip.show();
      delay(10);
    }
  } 
}



void allLit(int R, int G, int B) {
   for (int i = 0; i <= LED_COUNT; i++){
    strip.setPixelColor(i, R, G, B);
  }
  strip.show();
}
