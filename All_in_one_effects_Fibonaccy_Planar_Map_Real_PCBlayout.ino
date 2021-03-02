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

//21-02-2021 v1.2
//added cilindrical layout (resolution for cilindrical effects is 11x45)
//for code this effects use NUM_ROWS_CILINDR and NUM_COLS_CILINDR   
//added effects for this cilindrical map: 
//Fire2021_Cilindrical, CilindricalSwirl, RGB_Caleidoscope1, RGB_Caleidoscope2, 
//Fire_Tunnel, Cilindrical_Pattern, DiagonalPatternCilindr, FireButterfly  
//added function XY_fibon_cilindr(x,y) for calculate index in leds[] for cilindrical layout

//01-03-2021 v1.3 
//added fadein/out beetween effects
//added gamma correction table and gamma correction procedure. 
//it need for effect like RGB_Caleidoscope1 for contrast looking in real leds, 
//now gamma correction added for effects RGB_Caleidoscope1, RGB_Caleidoscope1 and Cilindrical_Pattern

#include <FastLED.h>

#define DATA_PIN    3                //set your datapin
#define LED_TYPE    WS2812B          //leds type
#define COLOR_ORDER GRB              //color order of leds

#define NUM_COLS_PLANAR 20            // resolution for planar lookup table
#define NUM_ROWS_PLANAR 20            // resolution for planar lookup table
#define NUM_COLS_SPIRALS 21           // resolution for spirals lookup table
#define NUM_ROWS_SPIRALS 13           // resolution for spirals lookup table
#define NUM_ROWS_CILINDR 11           // resolution for cinindrical lookup table
#define NUM_COLS_CILINDR 45           // resolution for cilindrical lookup table
#define NUM_LEDS_PLANAR  NUM_ROWS_PLANAR * NUM_COLS_PLANAR 
#define NUM_LEDS_SPIRALS NUM_ROWS_SPIRALS* NUM_COLS_SPIRALS  //not used yet. in future
#define NUM_LEDS_CILINDR NUM_ROWS_CILINDR* NUM_COLS_CILINDR //not used yet. in future

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

static const uint16_t FibonCilindrMap[] PROGMEM ={    //lookup table for cilindrical mapping on fibonacci layout
85, 256, 256, 86, 256, 110, 256, 111, 256, 134, 256, 135, 256, 158, 256, 256, 159, 182, 256, 256, 183, 256, 207, 256, 208, 256, 231, 256, 232, 256, 255, 256, 256, 12, 13, 256, 256, 36, 256, 38, 37, 61, 256, 256, 62,
256, 88, 87, 108, 109, 256, 112, 132, 133, 256, 136, 156, 157, 256, 161, 160, 181, 256, 185, 184, 205, 206, 256, 209, 229, 230, 256, 233, 256, 254, 256, 10, 11, 14, 256, 34, 35, 256, 39, 256, 60, 256, 64, 63, 84,
89, 106, 107, 256, 114, 113, 131, 256, 138, 137, 155, 256, 163, 162, 179, 180, 256, 186, 203, 204, 256, 211, 210, 228, 256, 235, 234, 252, 253, 256, 9, 16, 15, 256, 33, 256, 41, 40, 58, 59, 256, 65, 82, 83, 256,
105, 256, 116, 115, 129, 130, 256, 139, 256, 154, 256, 164, 256, 178, 256, 188, 187, 202, 256, 256, 212, 226, 227, 256, 236, 256, 251, 256, 7, 8, 17, 256, 256, 32, 256, 42, 256, 57, 256, 67, 66, 81, 256, 91, 90,
256, 117, 256, 128, 256, 256, 140, 152, 153, 256, 165, 256, 177, 256, 189, 256, 201, 256, 214, 213, 225, 256, 256, 237, 249, 250, 256, 6, 256, 18, 256, 30, 31, 256, 43, 256, 56, 256, 68, 256, 80, 256, 92, 256, 104,
118, 256, 127, 256, 256, 141, 151, 256, 256, 166, 256, 176, 256, 190, 256, 200, 256, 215, 256, 224, 256, 256, 238, 248, 256, 256, 5, 256, 19, 256, 29, 256, 256, 44, 256, 55, 256, 69, 256, 79, 256, 93, 256, 103, 256,
256, 126, 256, 256, 142, 150, 256, 256, 167, 256, 175, 256, 191, 256, 199, 256, 216, 256, 223, 256, 256, 239, 247, 256, 256, 4, 256, 20, 256, 28, 256, 256, 45, 256, 54, 256, 70, 256, 78, 256, 94, 256, 102, 256, 119,
125, 256, 256, 143, 149, 256, 256, 168, 256, 174, 256, 192, 256, 198, 256, 217, 256, 222, 256, 256, 240, 246, 256, 256, 3, 256, 21, 256, 27, 256, 256, 46, 256, 53, 256, 71, 256, 77, 256, 95, 256, 101, 256, 120, 256,
256, 256, 144, 148, 256, 256, 169, 256, 173, 256, 193, 256, 197, 256, 218, 256, 221, 256, 256, 241, 245, 256, 256, 2, 256, 22, 256, 26, 256, 256, 47, 256, 52, 256, 72, 256, 76, 256, 96, 256, 100, 256, 121, 256, 124,
256, 145, 147, 256, 256, 170, 256, 172, 256, 194, 256, 196, 256, 219, 256, 220, 256, 256, 242, 244, 256, 256, 1, 256, 23, 256, 25, 256, 256, 48, 256, 51, 256, 73, 256, 75, 256, 97, 256, 99, 256, 122, 256, 123, 256,
146, 256, 256, 256, 256, 256, 171, 256, 256, 256, 195, 256, 256, 256, 256, 256, 256, 243, 256, 256, 256, 0, 256, 24, 256, 256, 256, 256, 49, 256, 50, 256, 256, 256, 74, 256, 256, 256, 98, 256, 256, 256, 256, 256, 256,
};  

static const uint8_t exp_gamma[256] PROGMEM = {
0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,
1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
1,   2,   2,   2,   2,   2,   2,   2,   2,   2,   3,   3,   3,   3,   3,
4,   4,   4,   4,   4,   5,   5,   5,   5,   5,   6,   6,   6,   7,   7,
7,   7,   8,   8,   8,   9,   9,   9,   10,  10,  10,  11,  11,  12,  12,
12,  13,  13,  14,  14,  14,  15,  15,  16,  16,  17,  17,  18,  18,  19,
19,  20,  20,  21,  21,  22,  23,  23,  24,  24,  25,  26,  26,  27,  28,
28,  29,  30,  30,  31,  32,  32,  33,  34,  35,  35,  36,  37,  38,  39,
39,  40,  41,  42,  43,  44,  44,  45,  46,  47,  48,  49,  50,  51,  52,
53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,
68,  70,  71,  72,  73,  74,  75,  77,  78,  79,  80,  82,  83,  84,  85,
87,  89,  91,  92,  93,  95,  96,  98,  99,  100, 101, 102, 105, 106, 108,
109, 111, 112, 114, 115, 117, 118, 120, 121, 123, 125, 126, 128, 130, 131,
133, 135, 136, 138, 140, 142, 143, 145, 147, 149, 151, 152, 154, 156, 158,
160, 162, 164, 165, 167, 169, 171, 173, 175, 177, 179, 181, 183, 185, 187,
190, 192, 194, 196, 198, 200, 202, 204, 207, 209, 211, 213, 216, 218, 220,
222, 225, 227, 229, 232, 234, 236, 239, 241, 244, 246, 249, 251, 253, 254,
255};

uint8_t gCurrentPatternNumber =0; // Index number of which pattern is current
uint8_t InitNeeded = 1;           //global variable for effects initial needed
byte BRIGHTNESS = 110;      // for me good bright about 100-120, don't turn leds in full brightness long time! it may overheat


void setup() {
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
Fire2021_Cilindrical, CilindricalSwirl, RGB_Caleidoscope2, RGB_Caleidoscope1,
Fire_Tunnel, Cilindrical_Pattern, DiagonalPatternCilindr, FireButterfly,     
Spirals_Swirl,
fire2021, RGBTunnel, metaballs,
DigitalRain, SinPattern, F_lying,
DiagonalPattern
}; 

void loop() {
EVERY_N_SECONDS( 30 ) // speed of change patterns periodically
{
FadeOut (50);        // fade out current effect
gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE(gPatterns); //next effect
InitNeeded=1; //flag if init something need
FadeIn (50);        // fade in current effect
} 

gPatterns[gCurrentPatternNumber]();

FastLED.show();  
} // main cycle

//_____________________________________ effects for cilindrical layout

//CilindricalSwirl_____________________________________

void CilindricalSwirl() {
uint16_t  a = millis()/4;

for (int j = 0; j < NUM_ROWS_CILINDR; j++) {
   for (int i = 0; i < NUM_COLS_CILINDR; i++) {
      uint16_t index = XY_fibon_cilindr(i,j);
if (index!=256) leds[index].setHue(i*24+(sin8(j*16+a))>>1); 
}}
}

//FireButterfly_____________________________________

void FireButterfly() {
uint16_t  a = millis()/2;

for (int j = 0; j < NUM_ROWS_CILINDR; j++) {
   for (int i = 0; i < NUM_COLS_CILINDR; i++) {
      uint16_t index = XY_fibon_cilindr(i,j);
if (index!=256) leds[index]=HeatColor(qsub8 (inoise8 (i * 80+a , j * 5+ a , a /3), abs8(j - (NUM_ROWS_CILINDR-1)) * 255 / (NUM_ROWS_CILINDR+3)));
}}
}

//DiagonalPatternCilindr_____________________________________

void DiagonalPatternCilindr() {
uint16_t  a = millis()/4;

for (int j = 0; j < NUM_ROWS_CILINDR; j++) {
   for (int i = 0; i < NUM_COLS_CILINDR; i++) {
      uint16_t index = XY_fibon_cilindr(i,j);
if (index!=256) leds[index].setHue (i*16+j*16+a);
}}
}

//Fire2021_Cilindrical_____________________________________

void Fire2021_Cilindrical() {
uint16_t  a = millis()/2;

for (int j = 0; j < NUM_ROWS_CILINDR; j++) {
   for (int i = 0; i < NUM_COLS_CILINDR; i++) {
      uint16_t index = XY_fibon_cilindr(i,j);
if (index!=256) leds[index]= HeatColor(qsub8 (inoise8 (i * 80 , j * 40+ a , a /3), abs8(j - (NUM_ROWS_CILINDR-1)) * 255 / (NUM_ROWS_CILINDR+4)));
}}
}

//Cilindrical_Pattern_____________________________________

void Cilindrical_Pattern() {
uint16_t  a = millis()/10;

for (int j = 0; j < NUM_ROWS_CILINDR; j++) {
   for (int i = 0; i < NUM_COLS_CILINDR; i++) {
      uint16_t index = XY_fibon_cilindr(i,j);
if (index!=256) leds[index].setHue ((sin8((i<<4)+a)>>1)+(sin8((j<<4)+a))>>1);
}}
GammaCorrection();
}

//Fire_Tunnel_____________________________________

void Fire_Tunnel() {
uint16_t  a = millis()/2;

for (int j = 0; j < NUM_ROWS_CILINDR; j++) {
   for (int i = 0; i < NUM_COLS_CILINDR; i++) {
      uint16_t index = XY_fibon_cilindr(i,j);
if (index!=256) leds[index]= HeatColor(inoise8(i*50+a,j*40+a));
}}
}

//RGB_Caleidoscope1_____________________________________

void RGB_Caleidoscope1() {
uint16_t  a = millis()/8;

for (int j = 0; j < NUM_ROWS_CILINDR; j++) {
   for (int i = 0; i < NUM_COLS_CILINDR; i++) {
      uint16_t index = XY_fibon_cilindr(i,j);
if (index!=256) 
leds[index].setRGB( (sin8(i*16+a)+cos8(j*16+a/2))/2, sin8(j*16+a/2+sin8(leds[index].r+a)/16), cos8(i*16+j*16-a/2+leds[index].g));
}}
GammaCorrection();
}

//RGB_Caleidoscope2_____________________________________

void RGB_Caleidoscope2() {
uint16_t  a = millis()/8;

for (int j = 0; j < NUM_ROWS_CILINDR; j++) {
   for (int i = 0; i < NUM_COLS_CILINDR; i++) {
      uint16_t index = XY_fibon_cilindr(i,j);
if (index!=256)
leds[index].setRGB((sin8(i*32+a)+cos8(j*32+a))>>1,(sin8(i*32-a)+cos8(j*32+a>>1))>>1,sin8(j*16+a));
}}
GammaCorrection();
}

//_____________________________________ effects for spirals layout

//Spirals_Swirl_____________________________________

void Spirals_Swirl() {

uint16_t  a = millis()/6;

for (int j = 0; j < NUM_ROWS_SPIRALS; j++) {
   for (int i = 0; i < NUM_COLS_SPIRALS; i++) {
      uint16_t index = XY_fibon_21spirals(i,j);
      leds[index].setHue(i*24+(sin8(j*16+a))>>1);  
}}
}

//_____________________________________ effects for planar layout

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

//system procedures_____________________________________

uint16_t XY_fibon(byte x, byte y) {             // calculate index in leds from XY coordinates for planar mapping
uint16_t ledsindex = pgm_read_word (FibonPlanarTable+y*NUM_COLS_PLANAR+x);
return (ledsindex);
}

uint16_t XY_fibon_21spirals(byte x, byte y) {   // calculate index in leds from XY coordinates for spirals mapping
uint16_t ledsindex = pgm_read_word (Fibon21Spirals+y*NUM_COLS_SPIRALS+x);
return (ledsindex);
}

uint16_t XY_fibon_cilindr(byte x, byte y) {     // calculate index in leds from XY coordinates for cilindrical mapping
uint16_t ledsindex = pgm_read_word (FibonCilindrMap+y*NUM_COLS_CILINDR+x);
return (ledsindex);
}

void GammaCorrection(){   //gamma correction function 
byte r,g,b;
for (uint16_t i=0; i<256; i++){
r=leds[i].r;
g=leds[i].g;
b=leds[i].b;
leds[i].r = pgm_read_byte(exp_gamma + r);
leds[i].g = pgm_read_byte(exp_gamma + g);
leds[i].b = pgm_read_byte(exp_gamma + b);
}
}

void FadeOut (byte steps){
for (byte i=0; i<=steps; i++) {
gPatterns[gCurrentPatternNumber]();
byte fadeOut = lerp8by8 (BRIGHTNESS, 0, 255*i/steps);
FastLED.setBrightness(fadeOut);
FastLED.show();  
}
}

void FadeIn (byte steps){
byte fadeOut;
for (byte i=steps+1; i--; i>=0) {
gPatterns[gCurrentPatternNumber]();
fadeOut = lerp8by8 (BRIGHTNESS, 0, 255*i/steps);
FastLED.setBrightness(fadeOut);
FastLED.show();  
}
}
