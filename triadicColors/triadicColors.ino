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

  Colors(){}
  
  //when you need a color as an array
  void getColorArray(int outputColor[]){
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
  
  //weighted average of two Colors
  Colors aveColor(Colors colorOne, Colors colorTwo, long weight){
    long redNow = ((colorOne.red * weight) / 255) + ((colorTwo.red * (255 - weight)) / 255) ;
    long greenNow = ((colorOne.green * weight) / 255) + ((colorTwo.green * (255 - weight)) / 255) ;
    long blueNow = ((colorOne.blue * weight) / 255) + ((colorTwo.blue * (255 - weight)) / 255) ;
    Colors colorNow(redNow, greenNow, blueNow);
    return colorNow;
  }

  //FIX ME range for brightness? - uses aveColor to lower the brightness
  Colors dimColor(Colors inputColor, int brightness) {
      int n = brightness % MAX_BRIGHT;
      Colors newColor(n, n, n);
      return aveColor(newColor, inputColor, MAX_BRIGHT);
    }
};


// FIX ME ColorSets is a class to deal with sets of no more than 8 instances of Colors. not sure if the class is having issues or if its just the newFade function
class ColorSets {
  int numColors;  //an int to indicate number of colors, is this needed?
  Colors colorArray[8];  //let's keep it to 8 or less Colors 

  // compiling 
  public:

  //ColorSets takes a Colors array of nC number of colors Colors instances   
  ColorSets (Colors colorList[], int nC){
    numColors = nC;  //is this line necessary? why not just use nC in the for loop?
    for (int i = 0; i < numColors; i++){
      colorArray[i] = colorList[i];
    }
  }
  
  ColorSets(){}

// FIX ME - is this correct? since the class is ColorSets or is this a Colors called getColor? - getColor gets a Colors from colorArray at the index of index and returns the Colors returnColor
  Colors getColor (int index){
    Colors returnColor = colorArray[index];
    return returnColor;
  }

};

//this uses instances of the classes Colors and ColorSets. By creating an instance of Colors that stores the RGV value of a color, and placing those in a Colors array and placing that array in a ColorSet with the int for total colors in the array
Colors red (255, 0, 0);
Colors yellow (255, 250, 0);
Colors blue (0, 0, 255);

Colors primary[]{
  red, yellow, blue
};

ColorSets primarySet (primary, 3);

Colors orange (255, 100, 0);
Colors green (0, 128, 0);
Colors violet (148, 0, 211);

Colors secondary[]{
  orange, green, violet
};

ColorSets secondarySets (secondary, 3);


//FIX ME duped for constructing ColorSets with the already created colors  
int allColorSetsDemo [5][3][3] = { 
  { { 255, 0, 0}, { 255, 250, 0}, { 0, 0, 255} }, // Red, Yellow, Blue
  { { 255, 69, 0}, { 173, 255, 47}, { 138, 43, 226} }, // Red-orange, yellow-green, blue violet
  { { 255, 100, 0}, { 0, 128, 0}, { 148, 0, 211} }, // Orange, Green, Violet
  { { 255, 215, 0}, { 32, 178, 170}, { 199, 21, 133} }, // yellow-orange, blue-green, Red-violet
  { { 0, 0, 0}, { 0, 0, 0}, { 0, 0, 0} }  // no color 
};

//additional custom colors
  Colors wildberry(25, 30, 200);
  Colors cherryblossom(200, 0, 25);

  Colors biTillIDi[]{
    wildberry, cherryblossom
  };

  ColorSets biTillIDiSet (biTillIDi, 2);

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

  timingFunc();
  
  int litPixels[]  = { 0, 1, 2, 3, 4, 5, 6, 7};               // an array that holds the LEDs to be lit adjusted with NUM_LEDS
  
  for (int i = 0; i < NUM_LEDS; i++){
    fadeSingle(litPixels[i], wildberry, cherryblossom, i * 100 );
  }    
}

void timingFunc(){
  altTime = millis();  // time is assigned millis

  ledClock = altTime - timeSinceRando;  // ledClock is the differance in milliseconds of time and timeSinceRando
 
  if (ledClock > SPEED) {   // if ledClock is greater than speed re-random and resets the random clock
    timeSinceRando = altTime;    //  sets timeSinceRando to altTime
  }
}


/*FIXME not working? all white  previous version of fade probably needs to get yeeted. 
 * newFade takes the array litPixels as an int and an instance of ColorSets called fadeSet
 * for the number of LEDs 
 * 
 */
void newFade(int litPixels[], ColorSets fadeSet) {
  Serial.println("in new fade");
  for (int i = 0; i < NUM_LEDS; i++) {       

    Colors thisColor = fadeSet.getColor(2);  //

    thisColor.printColor();

    long brightness = ( ledClock * (MAX_BRIGHT - 1))  / SPEED;

    if (ledClock < SPEED) {
      int n = brightness % MAX_BRIGHT;
      thisColor = thisColor.dimColor(thisColor, n);
      //thisColor.printColor();
      thisColor.setColor(litPixels[i]);
    }
    else{
      int n = (MAX_BRIGHT - 1) - (brightness - (MAX_BRIGHT - 1)) % MAX_BRIGHT;
      thisColor = thisColor.dimColor(thisColor, n);
      thisColor.setColor(litPixels[i]);
    }
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
