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

  //the public part of the class can get called outside of the class
  public:
  Colors(int r, int g, int b){
    red = r;
    green = g;
    blue = b; 
  }
  
  //when you need a color as an array
  void getColor(int outputColor[]){
    outputColor[0] = red;
    outputColor[1] = green;
    outputColor[2] = blue;
  }
  
  //sets individual pixel to a color and prints the RGB values to the Serial monitor, remove that? 
  void setColor (int pixel){
    strip.setPixelColor(pixel, red, green, blue);
    printColor();
  }
  
  //use for debugging line A prints readable, line B prints the varible
  void printColor(){
    Serial.print("red = ");
    Serial.print(red);
    Serial.print(" green = ");
    Serial.print(green);
    Serial.print(" blue = ");
    Serial.print(blue);
    Serial.println(" ");
  }
  
  //weighted average of two colors
  Colors aveColor(Colors colorOne, Colors colorTwo, long weight){
    long redNow = ((colorOne.red * weight) / 255) + ((colorTwo.red * (255 - weight)) / 255) ;
    long greenNow = ((colorOne.green * weight) / 255) + ((colorTwo.green * (255 - weight)) / 255) ;
    long blueNow = ((colorOne.blue * weight) / 255) + ((colorTwo.blue * (255 - weight)) / 255) ;
    Colors colorNow(redNow, greenNow, blueNow);
    return colorNow;
  }
};

class ColorSets {
  Colors colorArray[3];  //would like to be able to have less or more colors in sets

  // not compiling 
  public:
  ColorSets (Colors colorList[]){
    
  }
};

int allColorSets [5][3][3] = { 
  { { 255, 0, 0}, { 255, 250, 0}, { 0, 0, 255} }, // Red, Yellow, Blue
  { { 255, 69, 0}, { 173, 255, 47}, { 138, 43, 226} }, // Red-orange, yellow-green, blue violet
  { { 255, 100, 0}, { 0, 128, 0}, { 148, 0, 211} }, // Orange, Green, Violet
  { { 255, 215, 0}, { 32, 178, 170}, { 199, 21, 133} }, // yellow-orange, blue-green, Red-violet
  { { 0, 0, 0}, { 0, 0, 0}, { 0, 0, 0} }  // no color 
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
//consider some additional class for time
unsigned long timeSinceRando = millis();  // millis is the amount of milliseconds since the chip started running the current program, resets about every 50 days. 
unsigned long altTime = millis();  // creats an unsigned long, a non negative number upto over 4 billion {0 to (2^32)-1}
unsigned long ledClock = altTime - timeSinceRando;  // ledClock is the differance in milliseconds of time and timeSinceRando


//FIXME strip.show(); needs to be in loop only once, and removed from the individual functions 
void loop() {
  // put your main code here, to run repeatedly:

  altTime = millis();  // time is assigned millis

  ledClock = altTime - timeSinceRando;  // ledClock is the differance in milliseconds of time and timeSinceRando

  int litPixels[]  = { 0, 1, 2, 3, 4, 5, 6, 7};               // an array that holds the LEDs to be lit adjusted with NUM_LEDS

 
  if (ledClock > SPEED) {   // if ledClock is greater than speed re-random and resets the random clock
    
    timeSinceRando = altTime;    //  sets timeSinceRando to altTime
  }

  // instances of the class Colors 
  Colors wildberry(25, 30, 200);
  Colors cherryblossom(200, 0, 25);

  //for NUM_LEDS fade from wildberry to cherryblossom using the array litPixels with indisis i and offset of i*100
  for (int i = 0; i < NUM_LEDS; i++ ){
    fadeSingle(litPixels[i], wildberry, cherryblossom, (i * 100) );    //fadeSingle has a strip.show rn, rm?
  }

}

//function that lights all the LEDs, LED_COUNT, to an instance of a Colors, includes strip show
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

//previous version of fade probably needs to get yeeted. grabs colors from the array of colors 
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


void fadeSingle (int pixel, Colors colorOne, Colors colorTwo, int offset) {
  long clockWeight = (ledClock * 255) / SPEED;
  long weight = ( (clockWeight + offset) * 255 )  / SPEED;
  Colors newColor(0,0,0);
  newColor = newColor.aveColor(colorOne, colorTwo, weight);
  newColor.setColor(pixel); 
  strip.show();
}
