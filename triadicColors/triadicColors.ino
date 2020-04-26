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

#define MAX_BRIGHT  256

#define SPEED       5000

void setup() {
  // put your setup code here, to run once:
  strip.begin();              // turn the strip on
  strip.show();               // show the strip
  strip.setBrightness(25);    // set or limit brightness

  randomSeed(analogRead(0));  // initilize a random number
  
  Serial.begin(9600);

  Serial.println("Hello Computer");  // prints to the serial to monitor to confirm setup is setting
  
}

unsigned long timeSinceRando = millis();  // millis is the amount of milliseconds since the chip started running the current program, resets about every 50 days. 
unsigned long altTime = millis();  // creats an unsigned long, a non negative number upto over 4 billion {0 to (2^32)-1}
unsigned long ledClock = altTime - timeSinceRando;  // ledClock is the differance in milliseconds of time and timeSinceRando

void loop() {
  // put your main code here, to run repeatedly:

  altTime = millis();  // time is assigned millis

  ledClock = altTime - timeSinceRando;  // ledClock is the differance in milliseconds of time and timeSinceRando

  int litPixels [NUM_LEDS];     // an array that holds the LEDs to be lit adjusted with NUM_LEDS


  if (ledClock > SPEED * 2) {   // if ledClock is greater than 5k re-random and resets the random clock

    randomPixels(litPixels);    

    while (dupePixels(litPixels) == true) {    // while dupePixels is true re-random
      randomPixels(litPixels);
    }
    
    timeSinceRando = altTime;    //  sets timeSinceRando to time
  }

  fade(litPixels, SECONDARY);

}

// gives litPixels a random of 0 to LED_COUNT with the index of i for NUM_LEDS
void randomPixels(int litPixels[]) {
  for (int i = 0; i < NUM_LEDS; i++ ){       
    int randoPixel = random(0, LED_COUNT);   
    litPixels[i] = randoPixel;               
  }
}

// a bool that checks litPixels for dupes and resets them
bool dupePixels(int litPixels[]) {
  for (int i = 0; i < NUM_LEDS; i++ ){       // a for loop that iterates from 0 to the number of LEDs to be lit
    int randoPixel = random(0, LED_COUNT);   // gets a random number between 0 and total LEDs
    for (int n = 0; n < NUM_LEDS; n++){      // inner for loop 0 throught number of LEDs
      if (litPixels[n] == litPixels[i]){     // if n and i are same
        if (n != i){                         // if n is not i
          return true;
        }
        randoPixel = random(0, LED_COUNT);
      }                                      
    }
  }
  return false;
}

// new fade func doesn't work since ledClock isn't declared in scope, doesn't compile.  
void fade(int litPixels[], int colorSet) {
  Serial.println("in fade");
  for (int i = 0; i < NUM_LEDS; i++) {       

    long red = allColorSets [colorSet][i % 3][0];         
    long green = allColorSets [colorSet][i % 3][1];
    long blue = allColorSets [colorSet][i  % 3][2];

    long redNow = red;
    long greenNow = green;
    long blueNow = blue;

    long brightness = ( ledClock * (MAX_BRIGHT - 1))  / SPEED;

    if (ledClock < SPEED) {
      int n = brightness % MAX_BRIGHT;
      redNow = (red * n) / MAX_BRIGHT;
      greenNow = (green * n) / MAX_BRIGHT;
      blueNow = (blue * n) / MAX_BRIGHT;
      strip.setPixelColor(litPixels[i], redNow, greenNow, blueNow);
      strip.show();
    }
    else{
      int n = (MAX_BRIGHT - 1) - (brightness - (MAX_BRIGHT - 1)) % MAX_BRIGHT;
      redNow = (red * n) / MAX_BRIGHT;
      greenNow = (green * n) / MAX_BRIGHT;
      blueNow = (blue * n) / MAX_BRIGHT;
      strip.setPixelColor(litPixels[i], redNow, greenNow, blueNow);
      strip.show();
    }
  }
}


void allLit(int R, int G, int B) {
   for (int i = 0; i <= LED_COUNT; i++){
    strip.setPixelColor(i, R, G, B);
  }
  strip.show();
}
