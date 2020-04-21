#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h>
#endif

#define LED_PIN     6   // pin data is running on

#define LED_COUNT   8   // number of total LEDs 

#define NUM_LEDS    3   // how many to light up

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

void setup() {
  // put your setup code here, to run once:
  strip.begin();              // turn the strip on
  strip.show();               // show the strip
  strip.setBrightness(25);    // set or limit brightness

  randomSeed(analogRead(0));  // initilize a random number
  
  Serial.begin(9600);


  Serial.println("Hello Computer");
  
}

void loop() {
  // put your main code here, to run repeatedly:

  int litPixels [NUM_LEDS];     // an array that holds the LEDs to be lit in this case 3 

  randomPixels(litPixels);

  while (dupePixels(litPixels) == true) {
    randomPixels(litPixels);
  }
  
  allLit(0, 0, 0);                                   

  setColors(litPixels, SECONDARY);
  
  delay(500);
}

/* int maxBrightness = 25;

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



void fadeOn(int lit Pixels[], int red, int green, int blue) {
  for (int r = 0; r < red; r++) {
     strip.setPixelColor(litPixels[i], r, 0, 0);
  }
  for (int g = 0; g < green; g++) {
     strip.setPixelColor(litPixels[i], red, g, 0);
  }
  for (int b = 0; b < blue; b++) {
     strip.setPixelColor(litPixels[i], red, green, b);
  }
}
*/ 

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

void setColors(int litPixels[], int colorSet) {
  Serial.println("in set colors");
  for (int i = 0; i < NUM_LEDS; i++) {       
    Serial.print("in for loop: ");
    Serial.print(litPixels[i]);
    Serial.println("");
    int red = allColorSets [colorSet][i][0];         
    int green = allColorSets [colorSet][i][1];
    int blue = allColorSets [colorSet][i][2];
    //strip.setPixelColor(litPixels[i], red, green, blue);
    
    for (int n = 255; n > 0; n--) {
      red = (red * n) / 255;
      green = (green * n) / 255;
      blue = (blue * n) / 255;
      strip.setPixelColor(litPixels[i], red, green, blue);
      strip.show();
      delay(50);
    }

/*
   for (int r = 0; r < red; r++) {
      strip.setPixelColor(litPixels[i], r, 0, 0);
      strip.show();
      delay(5);
    }
    Serial.print("red on. "); 
    
    for (int g = 0; g < green; g++) {
      strip.setPixelColor(litPixels[i], red, g, 0);
      strip.show();
      delay(5);
    }
    Serial.print("green on. ");
    
    for (int b = 0; b < blue; b++) {
      strip.setPixelColor(litPixels[i], red, green, b);
      strip.show();
      delay(5);
    }
    Serial.print("blue on. ");
  */ 
  
  }
 
}

void redFadeOn(){
  
}

void allLit(int R, int G, int B) {
   for (int i = 0; i <= LED_COUNT; i++){
    strip.setPixelColor(i, R, G, B);
  }
  strip.show();
}
