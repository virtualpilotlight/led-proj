#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h>
#endif

#define LED_PIN     6   // pin data is running on

#define LED_COUNT   8   // number of total LEDs 

#define NUM_LEDS    8   // how many to light up

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

//there are 4 color sets of 3 colors each, each color has an RGB value. 
int allColorSets [5][3][3] = { 
  { { 255, 0, 0}, { 255, 250, 0}, { 0, 0, 255} }, // Red, Yellow, Blue
  { { 255, 69, 0}, { 173, 255, 47}, { 138, 43, 226} }, // Red-orange, yellow-green, blue violet
  { { 255, 100, 0}, { 0, 128, 0}, { 148, 0, 211} }, // Orange, Green, Violet
  { { 255, 215, 0}, { 32, 178, 170}, { 199, 21, 133} }, // yellow-orange, blue-green, Red-violet
  { { 0, 0, 0}, { 0, 0, 0}, { 0, 0, 0} }  // no color 
};

// these are const to specify the  indix of the color groups in allColorSets
#define PRIMARY     0
#define WILD        1
#define SECONDARY   2
#define PASTEL      3
#define DARK        4

#define MAX_BRIGHT  256

#define SPEED       1000  // number of milliseconds for fade up and down, smaller is faster

//the class Colors is set up to hold an RGB value
class Colors {
  int red;
  int green;
  int blue; 

  public:
  Colors(int r, int g, int b){
    red = r;
    green = g;
    blue = b; 
  }
  void getColor(int outputColor[]){
    outputColor[0] = red;
    outputColor[1] = green;
    outputColor[2] = blue;
  }
  void setColor (int pixel){
    strip.setPixelColor(pixel, red, green, blue);
    printColor();
  }
  void printColor(){
    Serial.print("red = ");
    Serial.print(red);
    Serial.print(" green = ");
    Serial.print(green);
    Serial.print(" blue = ");
    Serial.print(blue);
    Serial.println(" ");
  }
  Colors aveColor(Colors colorOne, Colors colorTwo, long weight){
    long redNow = ((colorOne.red * weight) / 255) + ((colorTwo.red * (255 - weight)) / 255) ;
    long greenNow = ((colorOne.green * weight) / 255) + ((colorTwo.green * (255 - weight)) / 255) ;
    long blueNow = ((colorOne.blue * weight) / 255) + ((colorTwo.blue * (255 - weight)) / 255) ;
    Colors colorNow(redNow, greenNow, blueNow);
    return colorNow;
  }
};

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

  int litPixels[]  = { 0, 1, 2, 3, 4, 5, 6, 7};             // [NUM_LEDS];     // an array that holds the LEDs to be lit adjusted with NUM_LEDS

 
  if (ledClock > SPEED) {   // if ledClock is greater than speed re-random and resets the random clock
    
    timeSinceRando = altTime;    //  sets timeSinceRando to altTime
  }

  Colors wildberry(25, 30, 200);
  Colors cherryblossom(200, 0, 25);

  for (int i = 0; i < NUM_LEDS; i++ ){
    fadeSingle(litPixels[i], wildberry, cherryblossom, (i * 100) );
  }

  //lightAll(wildberry);
}

void lightAll(Colors ledColor) {
   Serial.print("light all");
   for (int i = 0; i <= LED_COUNT; i++){
    ledColor.setColor(i);
  }
  strip.show();
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

void colorFade(int litPixels[], int firstColor, int secondColor){
  Serial.println("in color fade");
  for (int i = 0; i < NUM_LEDS; i++) {       

    long red1 = allColorSets [firstColor][i % 3][0];         
    long green1 = allColorSets [firstColor][i % 3][1];
    long blue1 = allColorSets [firstColor][i  % 3][2];

    long red2 = allColorSets [secondColor][i % 3][0];         
    long green2 = allColorSets [secondColor][i % 3][1];
    long blue2 = allColorSets [secondColor][i  % 3][2];

    long weight = ( ledClock * (MAX_BRIGHT - 1))  / SPEED;

    long redNow = ((red1 * weight) / 255) + ((red2 * (255 - weight)) / 255) ;
    long greenNow = ((green1 * weight) / 255) + ((green2 * (255 - weight)) / 255) ;
    long blueNow = ((blue1 * weight) / 255) + ((blue2 * (255 - weight)) / 255) ;

    strip.setPixelColor(litPixels[i], redNow, greenNow, blueNow);
    strip.show();
  }
  
}

void fadeSingle (int pixel, Colors colorOne, Colors colorTwo, int offset) {
  long clockWeight = (ledClock * 255) / SPEED;
  long weight = ( (clockWeight + offset) * 255 )  / SPEED;
  Serial.print("weight = ");
  Serial.println(weight);
  Colors newColor(0,0,0);
  newColor = newColor.aveColor(colorOne, colorTwo, weight);
  newColor.setColor(pixel); 
  strip.show();
}

void singleFade(int pixel, int setOne, int colorOne, int setTwo, int colorTwo, int offset) {
  
  long redOne = allColorSets [setOne][colorOne][0];         
  long greenOne = allColorSets [setOne][colorOne][1];
  long blueOne = allColorSets [setOne][colorOne][2];

  long redTwo = allColorSets [setTwo][colorTwo][0];         
  long greenTwo = allColorSets [setTwo][colorTwo][1];
  long blueTwo = allColorSets [setTwo][colorTwo][2];

  long weight = ( (ledClock + offset) * (MAX_BRIGHT - 1) )  / SPEED;

  long redNow = ((redOne * weight) / 255) + ((redTwo * (255 - weight)) / 255) ;
  long greenNow = ((greenOne * weight) / 255) + ((greenTwo * (255 - weight)) / 255) ;
  long blueNow = ((blueOne * weight) / 255) + ((blueTwo * (255 - weight)) / 255) ;

  strip.setPixelColor(pixel, redNow, greenNow, blueNow);
  strip.show();
}
