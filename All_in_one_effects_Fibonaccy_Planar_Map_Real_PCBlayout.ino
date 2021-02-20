//Fibonacci 256 demo effects compilation
//fastled fibonacci 256 leds  demo
//Yaroslaw Turbin 
//https://vk.com/ldirko
//https://www.reddit.com/user/ldirko/
//https://twitter.com/ldir_ko

//this sketch tested on real Splendia 256 from https://twitter.com/WokwiMakes with ESP Wemos D1 mini 

//how it look in emulator https://wokwi.com/arduino/projects/290606904304992776 

//15-02-2020 v1.0 initial. planarmap table and effects for this layout (resolution for planar effects is 20x20) 
//17-02-2021 v1.1          
//added spirals layout (resolution for spirals effects is 21x13), added effect swirl with new spirals layout. 
//NUM_COLS and NUM_ROWS change to NUM_COLS_PLANAR, NUM_ROWS_PLANAR, added NUM_COLS_SPIRALS and NUM_ROWS_SPIRALS for spirals layout
//added function XY_fibon_21spirals(x,y) for calculate index in leds[] for spiral layout


#include <FastLED.h>

#define DATA_PIN    3
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB

#define NUM_COLS_PLANAR 20            // resolution for planar lookup table
#define NUM_ROWS_PLANAR 20            // resolution for planar lookup table
#define NUM_COLS_SPIRALS 21           // resolution for spirals lookup table
#define NUM_ROWS_SPIRALS 13           // resolution for spirals lookup table
#define NUM_LEDS_PLANAR  NUM_ROWS_PLANAR * NUM_COLS_PLANAR 
#define NUM_LEDS_SPIRALS NUM_ROWS_SPIRALS* NUM_COLS_SPIRALS  

#define BRIGHTNESS          255  // for me good bright about 100-120
#define MAX_POWER_MILLIAMPS 800  //write here your power in milliamps. default i set 800 mA for safety

CRGB leds [257];
byte rain[NUM_LEDS_PLANAR];  //need for digital rain effect

static const uint16_t FibonPlanarTable[] PROGMEM ={    //lookup table for planar mapping on fibonacci layout
256,256,256,256,256,256,256,256,36,39,38,37,256,256,256,256,256,256,256,256,256,256,256,256,256,13,34,35,40,
256,58,59,60,61,256,256,256,256,256,256,256,256,256,256,14,33,256,41,56,57,68,67,66,65,64,63,256,256,256,256,
256,256,256,12,15,32,42,55,256,69,256,79,80,81,82,83,62,256,256,256,256,256,11,16,31,256,43,54,70,77,78,94,
93,92,91,90,84,85,256,256,256,255,10,17,30,44,53,71,76,256,95,256,101,102,103,104,89,88,256,256,256,254,9,18,
29,45,52,72,75,96,256,100,120,119,118,117,105,106,87,256,256,253,8,19,28,46,256,51,73,97,99,121,124,125,126,
256,116,256,107,86,232,252,7,20,256,27,47,256,50,74,122,123,145,144,256,127,256,115,256,108,233,251,6,256,21,
256,26,48,49,256,98,146,147,148,143,256,128,256,114,109,231,234,250,5,256,22,23,25,24,0,195,171,170,169,149,
142,256,129,113,110,230,235,256,249,4,3,2,1,244,243,256,194,172,256,168,150,141,130,112,256,256,229,236,256,
248,247,246,245,242,220,219,196,193,173,167,151,140,131,111,256,256,208,228,237,238,239,240,241,256,221,218,
197,192,174,166,152,139,132,256,256,256,256,209,227,226,256,256,256,222,256,217,198,191,175,165,153,138,133,
256,256,256,256,207,210,211,225,224,223,215,216,199,256,190,176,164,154,137,134,256,256,256,256,256,206,205,
212,213,214,201,200,256,189,177,163,256,155,136,256,256,256,256,256,256,256,256,204,203,202,256,187,188,178,
162,256,156,135,256,256,256,256,256,256,256,256,256,183,184,185,186,180,179,161,256,157,256,256,256,256,256,
256,256,256,256,256,256,256,256,182,181,159,160,256,158,256,256,256,256,256,256,256 
};  

static const uint16_t Fibon21Spirals[] PROGMEM ={    //lookup table for spirals mapping on fibonacci layout
255, 12, 13, 36, 37, 61, 62, 85, 86, 110, 111, 134, 135, 158, 159, 182, 183, 207, 208, 231, 232,
254, 11, 14, 35, 38, 60, 63, 84, 87, 109, 112, 133, 136, 157, 160, 181, 184, 206, 209, 230, 233,
253, 10, 15, 34, 39, 59, 64, 83, 88, 108, 113, 132, 137, 156, 161, 180, 185, 205, 210, 229, 234,
252, 9, 16, 33, 40, 58, 65, 82, 89, 107, 114, 131, 138, 155, 162, 179, 186, 204, 211, 228, 235,
251, 8, 17, 32, 41, 57, 66, 81, 90, 106, 115, 130, 139, 154, 163, 178, 187, 203, 212, 227, 236,
250, 7, 18, 31, 42, 56, 67, 80, 91, 105, 116, 129, 140, 153, 164, 177, 188, 202, 213, 226, 237,
249, 6, 19, 30, 43, 55, 68, 79, 92, 104, 117, 128, 141, 152, 165, 176, 189, 201, 214, 225, 238,
248, 5, 20, 29, 44, 54, 69, 78, 93, 103, 118, 127, 142, 151, 166, 175, 190, 200, 215, 224, 239,
247, 4, 21, 28, 45, 53, 70, 77, 94, 102, 119, 126, 143, 150, 167, 174, 191, 199, 216, 223, 240,
246, 3, 22, 27, 46, 52, 71, 76, 95, 101, 120, 125, 144, 149, 168, 173, 192, 198, 217, 222, 241,
245, 2, 23, 26, 47, 51, 72, 75, 96, 100, 121, 124, 145, 148, 169, 172, 193, 197, 218, 221, 242,
244, 1, 24, 25, 48, 50, 73, 74, 97, 99, 122, 123, 256, 147, 170, 256, 194, 196, 219, 220, 243,
0, 256, 256, 256, 49, 256, 256, 256, 98, 256, 256, 146, 256, 256, 171, 256, 195, 256, 256, 256, 256
};  


uint8_t gCurrentPatternNumber =0; // Index number of which pattern is current
uint8_t InitNeeded = 1;           //global variable for effects initial needed

void setup() {
  delay(1000); // 1 second delay for boot recovery, and a moment of silence
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, 256)
  .setCorrection( TypicalLEDStrip );
  FastLED.setMaxPowerInVoltsAndMilliamps( 5, MAX_POWER_MILLIAMPS);   
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns =     // this is list of patterns
{    
Spirals_Swirl,
fire2021,RGBTunnel,metaballs,
DigitalRain,SinPattern,F_lying,
DiagonalPattern
}; 

void loop() {
EVERY_N_SECONDS( 30 ) // speed of change patterns periodically
{
gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE(gPatterns);
InitNeeded=1; //flag if init something need
} 

gPatterns[gCurrentPatternNumber]();

FastLED.show();  
} // main cycle

//_____________________________________ effects

//Spirals_Swirl_____________________________________

void Spirals_Swirl() {

uint16_t  a = millis()/6;

for (int j = 0; j < NUM_ROWS_SPIRALS; j++) {
   for (int i = 0; i < NUM_COLS_SPIRALS; i++) {
      uint16_t index = XY_fibon_21spirals(i,j);
      leds[index].setHue(i*24+(sin8(j*16+a))>>1);  
}}
}

//fire2021_____________________________________

void fire2021 () {
int  a = millis();
int  a1 = a/2;
uint16_t index=0;
for (byte j = 0; j < NUM_ROWS_PLANAR; j++) { 
for (byte i = 0; i < NUM_COLS_PLANAR; i++) {
uint16_t ledsindex = pgm_read_word (FibonPlanarTable+index++);
if (ledsindex!=256) leds[ledsindex] = HeatColor(qsub8 (inoise8 (i * 90, j * 90+a, a1), 
abs8(j - (NUM_ROWS_PLANAR-1)) * 255 / (NUM_ROWS_PLANAR+4)));
}}
} 

//RGBTunnel_____________________________________

void RGBTunnel() {
  int t = millis()>>3;
  int index=0;
  for (byte y = 0; y < NUM_ROWS_PLANAR; y++) {
  for (byte x = 0; x < NUM_COLS_PLANAR; x++) {
  int ledindex = pgm_read_word (FibonPlanarTable+index++);  
  if (ledindex!=256) leds[ledindex] = CHSV(code(x,y,t), 255, 255);
}}
}
byte code(int x, int y, int t) { //put your formula here 
  uint16_t outputcode = sqrt16((x-=sin8(x+t/3)>>4)*x+(y-=cos8(y+t>>1)>>4)*y)*20-t*2;  // moving tunnel variation        
  return outputcode;
}

//metaballs_____________________________________

void metaballs() {

  uint8_t bx1 = beatsin8(15, 0, NUM_COLS_PLANAR - 1, 0, 0);
  uint8_t by1 = beatsin8(18, 0, NUM_ROWS_PLANAR - 1, 0, 0);
  uint8_t bx2 = beatsin8(28, 0, NUM_COLS_PLANAR - 1, 0, 32);
  uint8_t by2 = beatsin8(23, 0, NUM_ROWS_PLANAR - 1, 0, 32);
  uint8_t bx3 = beatsin8(30, 0, NUM_COLS_PLANAR - 1, 0, 64);
  uint8_t by3 = beatsin8(24, 0, NUM_ROWS_PLANAR - 1, 0, 64);
  uint8_t bx4 = beatsin8(17, 0, NUM_COLS_PLANAR - 1, 0, 128);
  uint8_t by4 = beatsin8(25, 0, NUM_ROWS_PLANAR - 1, 0, 128);
  uint8_t bx5 = beatsin8(19, 0, NUM_COLS_PLANAR - 1, 0, 170);
  uint8_t by5 = beatsin8(21, 0, NUM_ROWS_PLANAR - 1, 0, 170);

  for (int i = 0; i < NUM_COLS_PLANAR; i++)    {
    for (int j = 0; j < NUM_ROWS_PLANAR; j++) {
      byte  sum =  dist(i, j, bx1, by1);
      sum = qadd8(sum, dist(i, j, bx2, by2));
      sum = qadd8(sum, dist(i, j, bx3, by3));
      sum = qadd8(sum, dist(i, j, bx4, by4));
      sum = qadd8(sum, dist(i, j, bx5, by5));
      int index= XY_fibon(i, j);
     if(index!=256) leds[XY_fibon(i, j)] =  ColorFromPalette(HeatColors_p , qsub8(sum, 100), BRIGHTNESS);
    }
  }
}

byte dist (uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)  {
  byte dist;
  int a = y2 - y1;
  int b = x2 - x1;
  a *= a;
  b *= b;
  a+=b;
  dist = a? 200 / sqrt16(a):200;  
  return dist;
}

//DigitalRain_____________________________________

void DigitalRain() {
if (InitNeeded) {raininit(); InitNeeded=0;}
EVERY_N_MILLISECONDS(80) {updaterain();}
EVERY_N_MILLISECONDS(15) {changepattern();}
} 

void changepattern () {
  int rand1 = random16 (NUM_LEDS_PLANAR);
  int rand2 = random16 (NUM_LEDS_PLANAR);
  if (rain[rand1] && !rain[rand2]) {rain[rand1] = 0; rain[rand2] = 1;}
} //changepattern

void raininit() {     //init array of dots. run once
  for (int i = 0; i < NUM_LEDS_PLANAR; i++) rain[i] = !random8(15)? 1:0; 
} //raininit


void updaterain() {
static int speed = 1;

for (byte j = 0; j < NUM_ROWS_PLANAR; j++) {
   int yindex=(j + speed) % NUM_ROWS_PLANAR*NUM_COLS_PLANAR;
      for (byte i = 0; i < NUM_COLS_PLANAR; i++) {
         byte layer = rain[yindex+i];   
         if (layer) leds[XY_fibon((NUM_COLS_PLANAR - 1) - i, (NUM_ROWS_PLANAR - 1) - j)].setHue (100);
}}

fadeToBlackBy(leds, 256, 70);
speed ++;
}//updaterain

//SinPattern_____________________________________

void SinPattern() {
int  a = millis();
int index=0;

for (byte j = 0; j < NUM_ROWS_PLANAR; j++) { 
for (byte i = 0; i < NUM_COLS_PLANAR; i++) {

int ledsindex = pgm_read_word (FibonPlanarTable+index++);
 if (ledsindex!=256) leds[ledsindex].setHue(sin8((i<<4)+a/6)/2+sin8((j<<4)+a/6)/2);
}}
}

//F_lying_____________________________________

void F_lying() {
static byte hue =0;
EVERY_N_MILLISECONDS(30) { hue++; }  //30 - speed of hue change

byte x1 = beatsin8 (18, 0, (NUM_COLS_PLANAR-1));
byte x2 = beatsin8 (23, 0, (NUM_COLS_PLANAR-1)); 
byte x3 = beatsin8 (27, 0, (NUM_COLS_PLANAR-1)); 
byte x4 = beatsin8 (31, 0, (NUM_COLS_PLANAR-1)); 
byte x5 = beatsin8 (33, 0, (NUM_COLS_PLANAR-1)); 

byte y1 = beatsin8 (20, 0, (NUM_ROWS_PLANAR-1)); 
byte y2 = beatsin8 (26, 0, (NUM_ROWS_PLANAR-1));
byte y3 = beatsin8 (15, 0, (NUM_ROWS_PLANAR-1));
byte y4 = beatsin8 (27, 0, (NUM_ROWS_PLANAR-1));
byte y5 = beatsin8 (30, 0, (NUM_ROWS_PLANAR-1));

CRGB color = CHSV (hue,255,BRIGHTNESS);

fadeToBlackBy (leds,256, 40 );

mydrawLine(x1, y1,  x2, y2, color,1);
mydrawLine(x2, y2,  x3, y3, color,1);
mydrawLine(x2, y2,  x4, y4, color,1);
mydrawLine(x3, y3,  x4, y4, color,1);
mydrawLine(x3, y3,  x1, y1, color,1);
mydrawLine(x4, y4,  x1, y1, color,1);
} 

void mydrawLine (byte x, byte y, byte x1, byte y1, CRGB color, bool dot){   // my ugly line draw function )))

byte xsteps = abs8(x-x1)+1;  
byte ysteps = abs8(y-y1)+1;
byte steps =  xsteps >= ysteps? xsteps:ysteps;

for (byte i = 1; i <= steps; i++) {
byte dx = lerp8by8 (x, x1, i*255/steps);
byte dy = lerp8by8 (y, y1, i*255/steps);
leds[XY_fibon(dx, dy)] += color;    // change to += for brightness look
}

if (dot) {     //add white point at the ends of line 
    leds[XY_fibon(x,y)] += CRGB ::White;
    leds[XY_fibon(x1,y1)] += CRGB ::White;
         }
}

//DiagonalPattern_____________________________________

void DiagonalPattern() {
 uint16_t ms = millis();
 int index=0;

for (byte j = 0; j < NUM_ROWS_PLANAR; j++) { 
   for (byte i = 0; i < NUM_COLS_PLANAR; i++) {
     int ledsindex = pgm_read_word (FibonPlanarTable+index++);
if (ledsindex!=256) leds[ledsindex].setHue(i*8+j*8+ms/16); // with palette
}}
}

//sistem procedures_____________________________________

uint16_t XY_fibon(byte x, byte y) {             // calculate index in leds from XY coordinates for planar mapping
uint16_t ledsindex = pgm_read_word (FibonPlanarTable+y*NUM_COLS_PLANAR+x);
return (ledsindex);
}

uint16_t XY_fibon_21spirals(byte x, byte y) {   // calculate index in leds from XY coordinates for spirals mapping
uint16_t ledsindex = pgm_read_word (Fibon21Spirals+y*NUM_COLS_SPIRALS+x);
return (ledsindex);
}
