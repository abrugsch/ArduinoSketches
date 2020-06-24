#include <FastLED.h>

FASTLED_USING_NAMESPACE

//Simulates the light pattern of the ST:TNG warp core along the length of a string of lights
//NUM_LEDS can be either even or odd, though odd is preferred. 
//Adjust BRIGHTNESS, FRAME_DELAY and FADE_FACTOR for higher density strings like 144/m
// 
#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    3
//#define CLK_PIN   4
#define LED_TYPE    NEOPIXEL
#define COLOR_ORDER GRB
#define NUM_LEDS    25
CRGB leds[NUM_LEDS];

#define BRIGHTNESS       75
//adjust these to the size of the string
//the "speed" of the core. smaller = faster
#define FRAME_DELAY      100
//fade amount. smaller = quicker fade
#define FADE_FACTOR      150

int gLaserPos;
uint8_t gRemainder;

void setup() {
  delay(3000); // 3 second delay for recovery
  gRemainder = NUM_LEDS%2;
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);


}

void fadeall() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(FADE_FACTOR); } }

void loop() { 
  static uint8_t hue = 0;
  //do we have a even or odd number
  uint8_t halfway = NUM_LEDS /2;
  // First slide the led in one direction
  int8_t halfway_offset = 0;
  if (gRemainder == 0)
  {
    halfway_offset = 1;
  }
  
  for(int i = 0; i < halfway-halfway_offset; i++) 
  {
    //do this every loop so that fadeall doesn't fade the centre
      
    leds[halfway] = CHSV(0,50,255);
    if(gRemainder == 0)
    {
      leds[halfway-1] = CHSV(0,50,255);
    }
    
    // Set the i'th led to pale teal
    leds[i] = CHSV(175, 75, 255);
    leds[NUM_LEDS - 1 - i] = CHSV(175, 75, 255);
    // Show the leds
    FastLED.show(); 
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(FRAME_DELAY);
  
  
  }
  leds[halfway] = CHSV(0,200,255);
  if(gRemainder == 0)
  {
    leds[halfway-1] = CHSV(0,200,255);
  }
  FastLED.show();
  delay(FRAME_DELAY);
  fadeall();
  FastLED.show();
  delay(FRAME_DELAY);
  fadeall();
  FastLED.show();
}
