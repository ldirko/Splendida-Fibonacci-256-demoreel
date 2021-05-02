Skip to content
Search or jump to…

Pull requests
Issues
Marketplace
Explore
 
@ldirko 
ldirko
/
Splendida-Fibonacci-256-demoreel
2
41
Code
Issues
2
Pull requests
1
Actions
Projects
Wiki
Security
Insights
Settings
Splendida-Fibonacci-256-demoreel/All_in_one_effects_Fibonaccy_Planar_Map_Real_PCBlayout.ino
@ldirko
ldirko Update All_in_one_effects_Fibonaccy_Planar_Map_Real_PCBlayout.ino
Latest commit 9524f50 28 days ago
 History
 1 contributor
503 lines (408 sloc)  20.7 KB
  
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

const uint32_t sprite4 [] PROGMEM = {   //20x60
0x000100, 0x000100, 0x000000, 0x000406, 0x000910, 0x002a36, 0x006c84, 0x0394a9, 0x1da9b8, 0x38c6d4, 0x1eaec6, 0x198eb1, 0x255d8c, 0x10214f, 0x04001f, 0x07000d, 0x010005, 0x000000, 0x000100, 0x000100, 0x000000, 0x000000, 0x010000, 0x000406, 0x000d15, 0x003542, 0x00738b, 0x0597ae, 0x24b0bf, 0x38c4d1, 0x1caac2, 0x1a88ab, 0x265485, 0x0f1644, 0x02001b, 0x05000c, 0x010004, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000508, 0x00101a, 0x004150, 0x007c92, 0x099bb2, 0x28b6c4, 0x38c1d1, 0x19a3bd, 0x1a82a7, 0x264a7e, 0x0b0c3c, 0x030015, 0x030007, 0x010002, 0x000000, 0x000000, 0x000000, 
0x000100, 0x000102, 0x000304, 0x00090d, 0x002934, 0x147182, 0x1fa1b7, 0x2abfd5, 0x4ee1f1, 0x36c3d5, 0x24a4c1, 0x1c77a3, 0x1b3679, 0x21155f, 0x230648, 0x130026, 0x080013, 0x010007, 0x000002, 0x000100, 0x000000, 0x000002, 0x000305, 0x000911, 0x003543, 0x157a8c, 0x20a7bd, 0x2fc6db, 0x4fe2f2, 0x31bccf, 0x239fbb, 0x1b6d9c, 0x192c71, 0x24125c, 0x1f0340, 0x110023, 0x07000f, 0x010005, 0x000002, 0x000000, 0x000000, 0x000102, 0x000307, 0x000c15, 0x004350, 0x188395, 0x22aac0, 0x37cee3, 0x4bdef0, 0x2fb6cc, 0x2597b9, 0x196394, 0x1b256d, 0x270f5b, 0x1e013b, 0x0f001d, 0x07000d, 0x010004, 0x000002, 0x000000, 
0x000100, 0x000304, 0x00070c, 0x002a35, 0x0b7b91, 0x1fadc5, 0x33ccde, 0x53f2ff, 0x32d7ed, 0x24b1cd, 0x2381a7, 0x143777, 0x452a93, 0x6522a3, 0x5c0f93, 0x41006a, 0x170031, 0x070014, 0x010007, 0x000002, 0x010000, 0x000305, 0x000913, 0x003646, 0x0f869e, 0x20b2cb, 0x38d3e7, 0x53f2ff, 0x2dcfe6, 0x22acc9, 0x2376a0, 0x132e73, 0x522e9c, 0x631da1, 0x5a0d8f, 0x3b0064, 0x130029, 0x050011, 0x010005, 0x000002, 0x010101, 0x000408, 0x000c15, 0x004453, 0x158fa6, 0x22b8d0, 0x3fdaee, 0x50efff, 0x2ac8e1, 0x22a2c5, 0x216798, 0x1a2873, 0x5a2fa2, 0x60199d, 0x550b88, 0x35005b, 0x110025, 0x05000d, 0x010005, 0x000002, 
0x000204, 0x00080c, 0x002632, 0x177688, 0x1eaec8, 0x2bd3ed, 0x52f9ff, 0x3adbef, 0x21b9d0, 0x148aa6, 0x003a5c, 0x0f1051, 0x4d138b, 0x8f32cb, 0x982bd4, 0x7a15af, 0x4b0872, 0x1b0035, 0x090016, 0x02000a, 0x000106, 0x00080f, 0x003241, 0x1b8198, 0x22b6d0, 0x2fd9f3, 0x52f9ff, 0x34d3e8, 0x1eb3cb, 0x11809e, 0x002a52, 0x181157, 0x541695, 0x9737d6, 0x9025cd, 0x7414a9, 0x430266, 0x16002d, 0x070013, 0x000007, 0x000306, 0x000a12, 0x004050, 0x1d8ba2, 0x23bbd3, 0x38e2fb, 0x50f7ff, 0x30cce2, 0x1fadc5, 0x0d7495, 0x001f4c, 0x25105d, 0x5f199d, 0x9c3adb, 0x8b20c6, 0x6f129f, 0x39005a, 0x130027, 0x080013, 0x010007, 
0x00070c, 0x001e28, 0x07657e, 0x20a3bf, 0x33cee4, 0x51faff, 0x34dff9, 0x21bbd3, 0x1a899d, 0x003e4e, 0x000919, 0x0b002c, 0x3f066f, 0x7618ae, 0xba3fef, 0xb635ea, 0x8a17bc, 0x560180, 0x1c0035, 0x020018, 0x000411, 0x002634, 0x0b718a, 0x22acc8, 0x37d6ec, 0x53fcff, 0x2dd7f0, 0x1fb5cd, 0x167f92, 0x003041, 0x00061b, 0x120035, 0x49087c, 0x7f1bb9, 0xc245f7, 0xad2edf, 0x8414b4, 0x4c0074, 0x17002e, 0x010015, 0x000513, 0x00323e, 0x0e7c95, 0x23b3ce, 0x3edef4, 0x50f9ff, 0x28d0ea, 0x1eaec8, 0x137487, 0x002335, 0x00031c, 0x1b003a, 0x550b88, 0x8b20c4, 0xc64afa, 0xa327d5, 0x7b12ac, 0x420069, 0x140028, 0x070011, 
0x000e26, 0x0d435d, 0x1283a3, 0x25b9d7, 0x52f1ff, 0x3dddf3, 0x21bbd3, 0x128fa5, 0x00404e, 0x000b14, 0x000209, 0x0a0017, 0x200041, 0x570983, 0x921dc3, 0xcf47ff, 0xbe32f3, 0x8b16bc, 0x4f0574, 0x120037, 0x020e34, 0x0f4d66, 0x158cac, 0x2cc3e2, 0x54f2ff, 0x36d5ea, 0x20b6ce, 0x0d849a, 0x00323e, 0x000911, 0x00010b, 0x0b001a, 0x2a004d, 0x620c91, 0x9b21ca, 0xd64bff, 0xb32ae9, 0x8212b2, 0x440067, 0x0d0031, 0x04183b, 0x0f576f, 0x1594b4, 0x31cfec, 0x53f3ff, 0x31cde3, 0x1fafc9, 0x0b7990, 0x002531, 0x00060e, 0x00000b, 0x0f001d, 0x340058, 0x6d109c, 0xa327d3, 0xd94eff, 0xa924df, 0x7b10ac, 0x39005b, 0x180031, 
0x0a1f56, 0x1d507f, 0x1f8baf, 0x46d3f1, 0x31c8e7, 0x23b0cc, 0x1c899e, 0x00424f, 0x000d13, 0x000307, 0x000205, 0x030009, 0x09001a, 0x260041, 0x670a96, 0x971ccc, 0xd247ff, 0xbb33ed, 0x8b16bd, 0x4a0385, 0x1c1c66, 0x1b5884, 0x2597b9, 0x49d8f6, 0x2ec5e4, 0x21abc7, 0x198093, 0x00333f, 0x000911, 0x000305, 0x000105, 0x04000b, 0x0c001d, 0x2f004d, 0x720fa0, 0xa020d5, 0xd84dff, 0xb12be0, 0x8313b3, 0x40007b, 0x1c256a, 0x1e638c, 0x2ea5c7, 0x48d9f6, 0x28bfde, 0x21a7c2, 0x157486, 0x002632, 0x00090d, 0x000205, 0x000105, 0x06000c, 0x0e0021, 0x370058, 0x7b12ac, 0xa924df, 0xda4fff, 0xa524d6, 0x7a11ab, 0x4d007e, 
0x311c83, 0x2a368c, 0x4a96ca, 0x2d9dc2, 0x178fb1, 0x0d7791, 0x003c4c, 0x000c14, 0x000406, 0x000000, 0x000002, 0x020106, 0x010007, 0x0e001d, 0x2f0052, 0x690b97, 0x991dc9, 0xd247ff, 0xbc30f1, 0x8218c2, 0x391289, 0x2f4694, 0x4c9ed0, 0x2799bd, 0x168dad, 0x0a7087, 0x00313e, 0x000b10, 0x000406, 0x000002, 0x000000, 0x020005, 0x030009, 0x100022, 0x38005e, 0x720ea2, 0xa121d0, 0xd84dff, 0xb029e5, 0x7816b9, 0x301282, 0x36569f, 0x4ba1d0, 0x2196b7, 0x148aa8, 0x066980, 0x002531, 0x00070d, 0x000305, 0x000000, 0x000000, 0x020005, 0x03000b, 0x120026, 0x42006b, 0x7b10ac, 0xab27db, 0xd94eff, 0xa624dc, 0x8407bb, 
0x811ec6, 0x4c0f9a, 0x334398, 0x205a8a, 0x11526e, 0x00303d, 0x000a10, 0x000305, 0x000102, 0x000000, 0x000000, 0x000002, 0x010103, 0x04000b, 0x0e001d, 0x2b0049, 0x690a98, 0x981dce, 0xd447ff, 0xb833ee, 0x8019c2, 0x461298, 0x304999, 0x1b5a85, 0x0f4f68, 0x002835, 0x00080c, 0x000304, 0x000102, 0x000000, 0x000000, 0x000002, 0x010004, 0x06000e, 0x100022, 0x320053, 0x740ea2, 0xa121d6, 0xd84dff, 0xa92ce0, 0x7617b9, 0x401694, 0x2d4d96, 0x195a80, 0x0d4a5f, 0x001f29, 0x00070b, 0x000204, 0x000002, 0x000000, 0x000000, 0x000002, 0x000005, 0x060010, 0x14002a, 0x3d0060, 0x7f11b0, 0xac25e1, 0xd94eff, 0xaa1ed9, 
0xc537fd, 0x8719c6, 0x411390, 0x1f1a6c, 0x091640, 0x000515, 0x000107, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x010206, 0x00000c, 0x050024, 0x290062, 0x610fa2, 0x9620ce, 0xd149ff, 0xb82fef, 0x7e17c0, 0x371188, 0x1b1d68, 0x041235, 0x00050f, 0x000105, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000002, 0x000106, 0x00000c, 0x080029, 0x32006d, 0x6913ac, 0x9c24d4, 0xd64fff, 0xab29e5, 0x7516b8, 0x2f1280, 0x182061, 0x000e2b, 0x00040c, 0x000104, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000002, 0x000008, 0x000012, 0x0b002f, 0x3d0079, 0x7515b7, 0xa828dd, 0xdd4eff, 
0xd23eff, 0xc039f5, 0x8719c6, 0x51058c, 0x15003c, 0x010019, 0x00000b, 0x000002, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000205, 0x00040c, 0x00132d, 0x132962, 0x251b7c, 0x5d11a5, 0x9720d2, 0xd34bff, 0xb631ea, 0x7e17c0, 0x470283, 0x0f0034, 0x000013, 0x000007, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000306, 0x00060f, 0x011935, 0x162a67, 0x291880, 0x6914af, 0xa124da, 0xd94eff, 0xaa2adf, 0x7715b6, 0x3c0074, 0x0d002e, 0x00000e, 0x000005, 0x000002, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000306, 0x000714, 0x041c3e, 0x1b286c, 0x2f1583, 0x7415b7, 0xaa29e5, 
0x9f15cd, 0xce43ff, 0xc337fa, 0x8d16c0, 0x540779, 0x1d0037, 0x0a0017, 0x030009, 0x010004, 0x000000, 0x000000, 0x000102, 0x000204, 0x00080d, 0x00212f, 0x0d5166, 0x156483, 0x2b5d9a, 0x4531a0, 0x5e12a6, 0x9721cf, 0xd74cff, 0xb62ded, 0x8714b9, 0x47026c, 0x170030, 0x070013, 0x010007, 0x000002, 0x000000, 0x000000, 0x000102, 0x000406, 0x000910, 0x002a38, 0x0f566c, 0x196587, 0x315ca0, 0x43249b, 0x6b14b0, 0xa124d6, 0xda4dff, 0xad26e2, 0x7f11ae, 0x3e005f, 0x120028, 0x070012, 0x010005, 0x000002, 0x000000, 0x000000, 0x000102, 0x000609, 0x000a12, 0x003543, 0x115b74, 0x1a648b, 0x385da4, 0x431997, 0x6c1bb5, 
0x6d01a5, 0x921cc8, 0xcc44ff, 0xc136f3, 0x8e17bf, 0x580183, 0x1e0038, 0x080016, 0x040008, 0x000000, 0x000000, 0x000304, 0x00070c, 0x002a36, 0x0a7087, 0x1590af, 0x239fbb, 0x4db0d9, 0x3265a4, 0x241c7f, 0x6311a4, 0x9b1ed0, 0xd54aff, 0xb42ee5, 0x8714b7, 0x4d0076, 0x170030, 0x050011, 0x020006, 0x000000, 0x000000, 0x000305, 0x000a0f, 0x003544, 0x0d778f, 0x1895b3, 0x2aa3c2, 0x4ca9d5, 0x2e569b, 0x2a157e, 0x6e13b0, 0xa621da, 0xd94eff, 0xac29db, 0x7d12ae, 0x43006a, 0x140028, 0x05000d, 0x010004, 0x000000, 0x000000, 0x000407, 0x000d16, 0x004352, 0x0f7d98, 0x1796b6, 0x30a9c8, 0x48a0ce, 0x2a4891, 0x1d1d75, 
0x3d006a, 0x620a92, 0x961cc7, 0xcf44ff, 0xc034f5, 0x8c17be, 0x4f0775, 0x180032, 0x070012, 0x000005, 0x000205, 0x000a0e, 0x002a35, 0x17788b, 0x21abc7, 0x29c4e2, 0x4ce2fd, 0x2ca9c7, 0x176a8a, 0x142e67, 0x2d0068, 0x700e9f, 0x9e1fce, 0xd54aff, 0xb42bea, 0x8414b6, 0x440369, 0x13002b, 0x060010, 0x000004, 0x000207, 0x000b12, 0x003844, 0x1a8398, 0x23b0cc, 0x2ec9e8, 0x4ce0fe, 0x259ebd, 0x166083, 0x152362, 0x35006f, 0x7910a8, 0xa724d6, 0xd94eff, 0xaa25e0, 0x7c13ae, 0x3a005b, 0x100024, 0x07000d, 0x000104, 0x000408, 0x000d15, 0x004553, 0x1e8ca3, 0x24b3d1, 0x35d0ef, 0x49daf7, 0x1e91b0, 0x16567c, 0x022751, 
0x100025, 0x260044, 0x660995, 0x971ccc, 0xcf47ff, 0xbc38ee, 0x7f15b3, 0x47006f, 0x140028, 0x00000e, 0x000712, 0x002a35, 0x0d7d93, 0x1fb1ca, 0x34d0e6, 0x55f5ff, 0x32d0ed, 0x1899b6, 0x0f5a71, 0x001b36, 0x050026, 0x2f004e, 0x710da1, 0x9f1fd4, 0xd84dff, 0xaf2ee1, 0x7814aa, 0x3d0065, 0x100022, 0x00000c, 0x000a15, 0x003545, 0x1088a0, 0x21b7d0, 0x39d8ee, 0x55f5ff, 0x2ac5e3, 0x148fae, 0x0d4f65, 0x011230, 0x09002a, 0x390059, 0x7b10ab, 0xa826e0, 0xd84fff, 0xa228d5, 0x6d109d, 0x340058, 0x0c001d, 0x00020d, 0x000c15, 0x004452, 0x1490a8, 0x22bcd6, 0x3fdff5, 0x52f2ff, 0x24bbd8, 0x1187a3, 0x0b455b, 0x001123, 
0x010009, 0x0c001c, 0x2d0050, 0x660a93, 0x941dc5, 0xc242f7, 0xa12cdb, 0x64109a, 0x27084e, 0x000423, 0x00253a, 0x15788b, 0x1eb0c9, 0x29d3ec, 0x51faff, 0x3dddf3, 0x23b4cf, 0x0e7c95, 0x00303d, 0x000710, 0x00000c, 0x0f0021, 0x36005c, 0x700ea1, 0x9c20cc, 0xc647f9, 0x9323cd, 0x5b0e92, 0x1d0543, 0x000520, 0x003347, 0x198498, 0x20b6cf, 0x30daf4, 0x52fbff, 0x36d5ea, 0x21adc8, 0x087187, 0x002331, 0x00040f, 0x00000e, 0x100024, 0x3f0067, 0x7811aa, 0xa127d4, 0xc449fa, 0x871ec1, 0x510b87, 0x16023d, 0x00091e, 0x004255, 0x1b8ea1, 0x21bbd5, 0x37e2fc, 0x4ffaff, 0x32cfe2, 0x20a6bf, 0x05657d, 0x001c27, 0x00060a, 
0x000004, 0x04000b, 0x0c001d, 0x280046, 0x5d0b8b, 0x7f1ab8, 0x9c35da, 0x7124b2, 0x2b176e, 0x00275a, 0x10789d, 0x1eb0c9, 0x30cfe4, 0x53fcff, 0x33e1f9, 0x21bbd3, 0x1c8b9f, 0x003d4c, 0x00090f, 0x000205, 0x000004, 0x04000c, 0x100022, 0x30004e, 0x660e96, 0x841bbd, 0x9d39da, 0x611ea2, 0x201865, 0x003460, 0x1183a5, 0x20b6cf, 0x36d7eb, 0x55fbff, 0x2fd9f3, 0x20b6cf, 0x1a8096, 0x00303d, 0x00070c, 0x000105, 0x000005, 0x060010, 0x120026, 0x370058, 0x6d109d, 0x8a1fc5, 0x9b3ad9, 0x58199b, 0x141660, 0x034369, 0x148dac, 0x21bbd3, 0x3be0f4, 0x51faff, 0x29d3ec, 0x1fafc7, 0x157486, 0x002430, 0x00060b, 0x000104, 
0x000000, 0x010103, 0x010007, 0x0b001c, 0x250046, 0x480578, 0x5a1498, 0x5730a7, 0x1d3386, 0x2174aa, 0x22aed2, 0x28d0ea, 0x4ff8ff, 0x3ce1f5, 0x21bdd4, 0x118fa5, 0x004150, 0x000c14, 0x000406, 0x000000, 0x000100, 0x010005, 0x030009, 0x0c001d, 0x2c0050, 0x4e0980, 0x5a1999, 0x5133a3, 0x163a84, 0x2281af, 0x21b4d6, 0x2dd9f1, 0x54faff, 0x38d9ed, 0x21b7d0, 0x0f869c, 0x003341, 0x000b10, 0x000305, 0x000000, 0x000000, 0x020005, 0x03000b, 0x0f0022, 0x340058, 0x530b88, 0x5a1b9d, 0x4b33a1, 0x144387, 0x228cb6, 0x23bad9, 0x35e1f9, 0x50f9ff, 0x30d1e5, 0x1eb0c9, 0x0b7b91, 0x002732, 0x00080c, 0x000204, 0x000000, 
0x000000, 0x000000, 0x000002, 0x04000a, 0x0a0017, 0x10002f, 0x1c0f57, 0x132b75, 0x1677ae, 0x22b0d6, 0x2ecee6, 0x4ef9ff, 0x34e2fb, 0x21bdd6, 0x1b8ea1, 0x004351, 0x000b14, 0x000406, 0x000102, 0x000000, 0x000000, 0x000002, 0x010004, 0x04000b, 0x0c001a, 0x140037, 0x1b135c, 0x13357c, 0x1983b5, 0x23b8d9, 0x34d6ed, 0x50fbff, 0x30dbf5, 0x22b8d1, 0x188397, 0x003543, 0x000911, 0x000406, 0x000102, 0x000000, 0x000000, 0x000000, 0x010004, 0x04000c, 0x0c001d, 0x16033b, 0x17175f, 0x144185, 0x1d8dbd, 0x24bddd, 0x38ddf3, 0x4ffaff, 0x29d3ec, 0x1db2ca, 0x14778a, 0x002833, 0x00090d, 0x000305, 0x000002, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000002, 0x010004, 0x000012, 0x091642, 0x306a9c, 0x28add6, 0x26d1ef, 0x4ef7ff, 0x38e3f5, 0x14c3de, 0x0198b3, 0x004b5d, 0x001019, 0x00050a, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000002, 0x030007, 0x000015, 0x0e234e, 0x3377a8, 0x26b4da, 0x29d8f5, 0x4ffaff, 0x31dced, 0x11bfda, 0x008faa, 0x003c4d, 0x000c14, 0x000407, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000002, 0x030009, 0x000119, 0x14335f, 0x3482b2, 0x27b9e0, 0x30e2fe, 0x4ef9ff, 0x2bd5e6, 0x0eb9d5, 0x0085a0, 0x002d3b, 0x000910, 0x000305, 0x000000, 0x000000, 0x000000
};

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
SpriteScroll,  
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
FadeOut (150);        // fade out current effect
gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE(gPatterns); //next effect
InitNeeded=1; //flag if init something need
FadeIn (150);        // fade in current effect
} 

gPatterns[gCurrentPatternNumber]();

FastLED.show();  
} // main cycle

//_____________________________________ effects for cilindrical layout

//Sprite scroll_____________________________________
void SpriteScroll() {  
static float a =0;
static uint16_t yspeed = 255; 
static uint16_t xspeed = 255;

byte sprOffs = sin8(a);
sprOffs = map8(sprOffs,0,(60-9));

for (byte j = 0; j < NUM_ROWS_CILINDR; j++) {
  byte yoffs = (j+yspeed+5)%20;

   for (byte i = 0; i < NUM_COLS_CILINDR; i++) {
    byte xoffs = (i+xspeed)%9;
    uint16_t index = XY_fibon_cilindr(i,j);
    if (index!=256) leds[index] = pgm_read_dword (sprite4+yoffs*60+xoffs+sprOffs);
}}    
// yspeed++;  
xspeed--;
delay(60);
// GammaCorrection();
a+=0.5;
}

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
FastLED.delay(20);  
}
}

void FadeIn (byte steps){
for (byte i=steps+1; i--; i>=0) {
gPatterns[gCurrentPatternNumber]();
byte fadeOut = lerp8by8 (BRIGHTNESS, 0, 255*i/steps);
FastLED.setBrightness(fadeOut);
FastLED.delay(20);  
}
}
© 2021 GitHub, Inc.
Terms
Privacy
Security
Status
Docs
Contact GitHub
Pricing
API
Training
Blog
About
Loading complete
