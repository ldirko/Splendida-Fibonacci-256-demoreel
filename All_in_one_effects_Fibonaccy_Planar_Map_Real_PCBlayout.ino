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

//02-05-2021
//added SpriteScroll effect. This can be used for hand made CRGB sprites with cilindrical or spirals layouts 

//03-06-2021 
//remap planar table. Now planar effects is not rotated.

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
256, 256, 256, 256, 256, 256, 256, 256, 183, 205, 206, 207, 256, 256, 256, 256, 256, 256, 256, 256, 
256, 256, 256, 256, 256, 182, 185, 184, 204, 256, 211, 210, 209, 208, 256, 256, 256, 256, 256, 256, 
256, 256, 256, 256, 181, 186, 256, 203, 213, 212, 225, 226, 227, 228, 229, 230, 256, 256, 256, 256, 
256, 256, 256, 159, 180, 187, 202, 214, 256, 224, 256, 238, 237, 236, 235, 234, 231, 256, 256, 256, 
256, 256, 160, 179, 188, 256, 201, 215, 223, 240, 239, 247, 248, 249, 250, 251, 233, 232, 256, 256, 
256, 158, 161, 178, 189, 200, 216, 222, 241, 256, 246, 256, 3, 4, 5, 6, 252, 253, 256, 256, 
256, 157, 162, 177, 190, 199, 217, 221, 242, 245, 256, 2, 22, 21, 20, 19, 7, 8, 254, 256, 
256, 156, 163, 176, 191, 198, 256, 218, 220, 244, 1, 23, 26, 27, 28, 256, 18, 256, 9, 255, 
135, 155, 164, 175, 256, 192, 197, 256, 219, 243, 24, 25, 48, 47, 256, 29, 256, 17, 256, 10, 
136, 154, 165, 256, 174, 256, 193, 196, 195, 256, 0, 49, 50, 51, 46, 256, 30, 256, 16, 11, 
134, 137, 153, 166, 256, 173, 172, 194, 171, 256, 98, 74, 73, 72, 52, 45, 256, 31, 15, 12, 
133, 138, 256, 152, 167, 168, 169, 170, 147, 146, 256, 97, 75, 256, 71, 53, 44, 32, 14, 256, 
256, 132, 139, 256, 151, 150, 149, 148, 145, 123, 122, 99, 96, 76, 70, 54, 43, 33, 13, 256, 
256, 111, 131, 140, 141, 142, 143, 144, 256, 124, 121, 100, 95, 77, 69, 55, 42, 34, 256, 256, 
256, 256, 112, 130, 129, 256, 256, 256, 125, 256, 120, 101, 94, 78, 68, 56, 41, 35, 256, 256, 
256, 256, 110, 113, 114, 128, 127, 126, 118, 119, 102, 256, 93, 79, 67, 57, 40, 36, 256, 256, 
256, 256, 256, 109, 108, 115, 116, 117, 104, 103, 256, 92, 80, 66, 256, 58, 39, 256, 256, 256, 
256, 256, 256, 256, 256, 107, 106, 105, 256, 90, 91, 81, 65, 256, 59, 38, 256, 256, 256, 256, 
256, 256, 256, 256, 256, 86, 87, 88, 89, 83, 82, 64, 256, 60, 37, 256, 256, 256, 256, 256, 
256, 256, 256, 256, 256, 256, 256, 85, 84, 62, 63, 256, 61, 256, 256, 256, 256, 256, 256, 256
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

const uint32_t sprite4 [] PROGMEM = {   //19x45

0x010101, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x010000, 0x010000, 0x000000, 0x000000, 0x010101, 0x010101, 0x000002, 0x000002, 0x000000, 0x000000, 0x010000, 0x010000, 0x000000, 0x010101, 0x000000, 0x0b0001, 0x210003, 0x420013, 0x5e0523, 0x500019, 0x270006, 0x110000, 0x060000, 0x010000, 0x010100, 0x000000, 0x000100, 0x000100, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000002, 0x000002, 0x000000, 
0x000000, 0x000000, 0x000000, 0x000000, 0x010101, 0x000000, 0x000000, 0x010101, 0x000000, 0x000000, 0x000000, 0x000002, 0x000206, 0x00040a, 0x00040c, 0x00040c, 0x000309, 0x000104, 0x000002, 0x000000, 0x000000, 0x000000, 0x010000, 0x110001, 0x3e0317, 0x7b2240, 0x982a51, 0x882247, 0x4d0922, 0x210008, 0x0b0001, 0x010000, 0x000000, 0x010100, 0x000100, 0x000100, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000002, 0x000002, 0x000000, 
0x010101, 0x000000, 0x010101, 0x000000, 0x010101, 0x000000, 0x000000, 0x000000, 0x000000, 0x000100, 0x000002, 0x000206, 0x000610, 0x000d1c, 0x00223a, 0x00223a, 0x000e1d, 0x00080f, 0x000306, 0x000002, 0x010103, 0x020001, 0x040000, 0x1d0003, 0x721534, 0xa11d4e, 0xa9114c, 0xa61550, 0x8c1c4c, 0x47001c, 0x170005, 0x030000, 0x020001, 0x000000, 0x010101, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x070001, 0x050001, 0x030000, 0x010000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000100, 0x000102, 0x000104, 0x000610, 0x001c2f, 0x296b8b, 0x46a2cb, 0x48a1c9, 0x347490, 0x001e2e, 0x00060d, 0x000002, 0x010103, 0x030000, 0x080000, 0x2c0002, 0x982e45, 0xa71440, 0xb4094d, 0xb10954, 0xa6165b, 0x7a1245, 0x280011, 0x0c0005, 0x040002, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x010000, 0x010000, 0x030000, 
0x170003, 0x110001, 0x070000, 0x030000, 0x010000, 0x000000, 0x000000, 0x000000, 0x000100, 0x000205, 0x00060c, 0x001223, 0x196f92, 0x2fa5d7, 0x1ba3e1, 0x1ea0dc, 0x3ea3cd, 0x0b4d65, 0x000b15, 0x000203, 0x000100, 0x040000, 0x110100, 0x663829, 0xaa4943, 0xa1182c, 0xb00c49, 0xb5075c, 0xae0c62, 0xa01d69, 0x510534, 0x1e0012, 0x090005, 0x010000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x010000, 0x030002, 0x070003, 0x0c0004, 
0x540620, 0x3b0010, 0x200003, 0x0f0000, 0x060000, 0x000000, 0x000000, 0x000100, 0x000301, 0x000807, 0x001619, 0x196676, 0x31b0d3, 0x10aae0, 0x06a5e9, 0x10a1e0, 0x34a5cd, 0x156073, 0x072525, 0x000400, 0x000100, 0x080000, 0x190200, 0xad7c51, 0xca7348, 0xac3223, 0xa9103e, 0xb0095b, 0xb0076c, 0xa40f73, 0x811b67, 0x3a002f, 0x12000f, 0x030004, 0x010002, 0x000000, 0x000000, 0x000000, 0x000002, 0x000004, 0x010005, 0x05000a, 0x0f000d, 0x1f0012, 0x370017, 
0xc11f5b, 0xb52b5c, 0x6a102a, 0x280001, 0x0f0000, 0x010000, 0x000000, 0x000301, 0x000900, 0x001f16, 0x147470, 0x32b9bf, 0x13b5ce, 0x07b2dd, 0x0aaae4, 0x1aa4d5, 0x2c90a7, 0x1c5c5c, 0x3b5a48, 0x0e1a04, 0x010200, 0x0c0000, 0x533300, 0xdba451, 0xe79a3e, 0xd46b31, 0xa92233, 0xa70d56, 0xa90774, 0xa20882, 0x961687, 0x6b0f66, 0x230025, 0x0a000d, 0x030004, 0x000000, 0x000000, 0x010103, 0x000004, 0x010009, 0x040013, 0x11001d, 0x2f002f, 0x680c4d, 0xaa1f62, 
0xe5175f, 0xdb2862, 0xc03e60, 0x5a0816, 0x210000, 0x080000, 0x000200, 0x000c00, 0x001b05, 0x1b775e, 0x37c7b0, 0x16c4ba, 0x07bdc8, 0x09b7d0, 0x1aaed2, 0x249ab4, 0x196966, 0x326449, 0x607b4e, 0x565f32, 0x121000, 0x160300, 0x885e0a, 0xe8ac3c, 0xf4aa2d, 0xfa9f44, 0xc9574c, 0x9c154d, 0xa30b7a, 0xa20695, 0x9c0d9d, 0x8c1a94, 0x480854, 0x14001e, 0x06000b, 0x000002, 0x000004, 0x000007, 0x00000e, 0x03001d, 0x100036, 0x330a56, 0x651574, 0x9d1f83, 0xcd1a78, 
0xec1c5a, 0xe6275a, 0xd83b5a, 0xae3b4a, 0x480a0f, 0x120000, 0x000500, 0x001c02, 0x077244, 0x39d09b, 0x1bd2a4, 0x09ccac, 0x0ac5ba, 0x1bbfc0, 0x36adb3, 0x1f726a, 0x356945, 0x587839, 0x778937, 0x959a48, 0x4d440b, 0x311300, 0xac7612, 0xe49f20, 0xeda213, 0xfba733, 0xffa478, 0x851734, 0x9b117e, 0x9d08a2, 0x9b05ae, 0x9111ac, 0x761d93, 0x26003d, 0x0d0019, 0x010007, 0x000009, 0x000013, 0x000025, 0x150a4f, 0x482895, 0x6529af, 0x7a19a7, 0xa4189f, 0xd21885, 
0xec2755, 0xec2f57, 0xe23a51, 0xd65059, 0x95403d, 0x2a0900, 0x001500, 0x1b6f3e, 0x38d087, 0x1bdc8d, 0x0cd996, 0x0bd19e, 0x20caaf, 0x31b5a7, 0x004e44, 0x35674e, 0x648246, 0x798930, 0x909a27, 0xacab37, 0xccb75a, 0x623900, 0xd38f20, 0xe29010, 0xe89909, 0xeea22a, 0xe8a066, 0x550110, 0x8a177c, 0x950ea9, 0x9605bc, 0x8f08bd, 0x841ab2, 0x56127f, 0x140030, 0x000015, 0x000619, 0x010e30, 0x2e3472, 0x584eaf, 0x5d3bc6, 0x6527ca, 0x7513be, 0x9613af, 0xc5219a, 
0xe33451, 0xe53c51, 0xe3434b, 0xda514b, 0xd16b55, 0x744a24, 0x29551a, 0x4ec575, 0x2ae07c, 0x0ee57e, 0x0be189, 0x1dd693, 0x3abb95, 0x025340, 0x001509, 0x172c0d, 0x828f47, 0x98a031, 0xaaad1e, 0xc2b71f, 0xf0ce48, 0xd0961b, 0xd27f09, 0xde8108, 0xe28d0c, 0xe7a036, 0x834d1e, 0x300005, 0x5c065d, 0x8717a7, 0x8f08c4, 0x8e07ce, 0x820fc4, 0x6c1fab, 0x210d55, 0x000f35, 0x1b3f61, 0x416798, 0x5067b7, 0x4c4fc0, 0x553dcf, 0x5d2cd3, 0x6a1dcb, 0x7513b4, 0x9623a4, 
0xb42c3a, 0xdb5259, 0xdc4f46, 0xdb5941, 0xd36740, 0xb58348, 0x6d9242, 0x4bbd5b, 0x24d968, 0x17e578, 0x24e185, 0x42d996, 0x015835, 0x001605, 0x000800, 0x090d00, 0x656825, 0xb5b547, 0xc3be22, 0xe0cb22, 0xffd743, 0xb97000, 0xcf7000, 0xd97506, 0xdd8615, 0xd6933a, 0x4e2700, 0x1c0004, 0x31003a, 0x6b1a90, 0x8313bf, 0x7e0ec8, 0x6e19bd, 0x522aa4, 0x344480, 0x326786, 0x4587a9, 0x3c7aad, 0x3d66b8, 0x4454c3, 0x4f45cc, 0x5837cc, 0x6430c3, 0x6220a6, 0x500b76, 
0x5b090b, 0xae514a, 0xd3634b, 0xd8643b, 0xd36f33, 0xbf8033, 0x9a993c, 0x61a43b, 0x30bd4d, 0x33d570, 0x49cd82, 0x096637, 0x001801, 0x000901, 0x000200, 0x020300, 0x181700, 0xaca84d, 0xe0d73c, 0xf8dc32, 0xf6b831, 0xb25b00, 0xcf6500, 0xd46909, 0xd77f1e, 0xa4641c, 0x2a0d00, 0x0c0001, 0x14001c, 0x2e004b, 0x7224ab, 0x6421af, 0x403498, 0x4a72ad, 0x449dab, 0x2f9d9e, 0x278e9f, 0x2b80a7, 0x3570b4, 0x405fba, 0x5054c0, 0x584bb7, 0x422b86, 0x1d014b, 0x170030, 
0x230000, 0x531b0e, 0xbb6a4c, 0xd17542, 0xd1792f, 0xc7862a, 0xb29a30, 0x8eab3d, 0x42a13d, 0x46b664, 0x12622d, 0x001b00, 0x000900, 0x000100, 0x000000, 0x010100, 0x060400, 0x3f3700, 0xf5e764, 0xffde4b, 0xc17505, 0xb85400, 0xca5800, 0xcd600e, 0xcb752a, 0x783f08, 0x150000, 0x040000, 0x06000b, 0x0f0025, 0x3e136d, 0x46338d, 0x5585b3, 0x3da4ab, 0x21ac99, 0x16a893, 0x1d999b, 0x2989a2, 0x3a7aaa, 0x486bab, 0x404c96, 0x181759, 0x03002e, 0x03001d, 0x010010, 
0x0e0000, 0x180000, 0x56260f, 0xcc8e5d, 0xcb8935, 0xc68d22, 0xbb9d25, 0xb4b648, 0x5c8536, 0x194e18, 0x001700, 0x000900, 0x000200, 0x000000, 0x000000, 0x000000, 0x040100, 0x0e0600, 0x99852f, 0xf6c660, 0xad5201, 0xba4801, 0xc64d04, 0xc55815, 0xb36330, 0x471600, 0x0f0000, 0x030000, 0x030004, 0x040011, 0x0d092c, 0x2b4866, 0x3fa29f, 0x1dad92, 0x10b58d, 0x11b18d, 0x21a194, 0x39979f, 0x427a93, 0x1d3758, 0x030e2e, 0x000018, 0x010013, 0x00000b, 0x000005, 
0x040000, 0x090000, 0x180000, 0x68451d, 0xc99d52, 0xc49a38, 0xc0a53a, 0xc3ba5d, 0x3d4810, 0x000d00, 0x000600, 0x000300, 0x000000, 0x000000, 0x000002, 0x000000, 0x010000, 0x090200, 0x392500, 0xb28244, 0xa54711, 0xb13e07, 0xba4309, 0xb8531f, 0x955331, 0x260100, 0x0c0000, 0x010000, 0x010002, 0x000306, 0x00070d, 0x0a3835, 0x3ba68c, 0x23af88, 0x1cb388, 0x27b38e, 0x3ca795, 0x206867, 0x002630, 0x000715, 0x00020a, 0x000005, 0x000004, 0x000002, 0x010101, 
0x020001, 0x040000, 0x090000, 0x170600, 0x6a5425, 0xae985c, 0xafa05d, 0x645d26, 0x0b0e00, 0x000300, 0x010200, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x040000, 0x100100, 0x50270b, 0x954520, 0xa33c13, 0xac3f14, 0xae542f, 0x4f1f09, 0x130000, 0x070000, 0x000000, 0x000000, 0x000002, 0x000303, 0x00120a, 0x256a55, 0x429f82, 0x3ea383, 0x389077, 0x00372b, 0x001110, 0x00050a, 0x000107, 0x000004, 0x000002, 0x000002, 0x010101, 0x010101, 
0x000000, 0x010000, 0x030000, 0x070000, 0x0b0300, 0x231a00, 0x211b00, 0x080400, 0x030100, 0x010000, 0x010000, 0x010000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x030000, 0x090000, 0x1c0000, 0x581b06, 0x88381d, 0x8f3a1d, 0x722e17, 0x1d0000, 0x080000, 0x030000, 0x000000, 0x000000, 0x000000, 0x000200, 0x000700, 0x001203, 0x053a26, 0x063f2c, 0x001e0e, 0x000a04, 0x000504, 0x000203, 0x000002, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
0x000000, 0x000000, 0x010000, 0x030000, 0x040100, 0x040200, 0x030100, 0x020200, 0x010000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x010000, 0x040000, 0x0f0000, 0x1d0000, 0x330700, 0x390700, 0x260000, 0x0e0000, 0x030000, 0x010000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000200, 0x000600, 0x000b00, 0x000a01, 0x000800, 0x000401, 0x000100, 0x000100, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000

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
  FadeIn (150); 
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
static uint16_t yspeed = 255; 
static uint16_t xspeed = 255;

for (byte j = 0; j < NUM_ROWS_CILINDR; j++) {
  byte yoffs = (j+yspeed)%19;

   for (byte i = 0; i < NUM_COLS_CILINDR; i++) {
    byte xoffs = (i+xspeed)%45;
    uint16_t index = XY_fibon_cilindr(i,j);
    if (index!=256) leds[index] = pgm_read_dword (sprite4+yoffs*45+xoffs);
}}    
yspeed++;  
xspeed++;
delay(60);
GammaCorrection();
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
