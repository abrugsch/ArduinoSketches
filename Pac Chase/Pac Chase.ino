// This is a demonstration on how to use an input device to trigger changes on your neo pixels.
// You should wire a momentary push button to connect from ground to a digital IO pin.  When you
// press the button it will change to a new pixel animation.  Note that you need to press the
// button once to start the first animation!

#include <Adafruit_NeoPixel.h>

#define BUTTON_PIN   2    // Digital IO pin connected to the button.  This will be
                          // driven with a pull-up resistor so the switch should
                          // pull the pin to ground momentarily.  On a high -> low
                          // transition the button press logic will execute.

#define PIXEL_PIN    6    // Digital IO pin connected to the NeoPixels.

#define PIXEL_COUNT 9
#define LOOP_MAX 2

// Parameter 1 = number of pixels in strip,  neopixel stick has 8
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream, correct for neopixel stick
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip), correct for neopixel stick
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

bool oldState = HIGH;
int showType = 0;
const uint32_t boardCol = 0x010008;
const uint32_t pacCol = 0x202000;
const uint32_t pacBigCol = 0xc0c000;
const uint32_t blinkyCol = 0x200000;
const uint32_t clydeCol = 0x201000;
const uint32_t inkyCol = 0x002020;
const uint32_t pinkyCol = 0x200d16;
const uint32_t scaredCol = 0x080060;

uint32_t ghostCol = blinkyCol;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  // Get current button state.
  bool newState = digitalRead(BUTTON_PIN);
  
  // Check if state changed from high to low (button press).
  if (newState == LOW && oldState == HIGH) {
    // Short delay to debounce button.
    delay(20);
    // Check if button is still low after debounce.
    newState = digitalRead(BUTTON_PIN);
    if (newState == LOW) {
      showType++;
      if (showType > 4)
        showType=0;
      startShow(showType);
    }
  }

  // Set the last button state to the old state.
  oldState = newState;
}

void startShow(int i) {
  switch(i){
    case 0: colorWipe(boardCol, 50);    // Black/off
            break;
    case 1:
       	ghostCol = clydeCol;
    	pacmanChase();
    		break;
    case 2:    	
    	ghostCol = inkyCol;
    	pacmanChase();
    		break;
    case 3:
    	ghostCol = blinkyCol;
    	pacmanChase();
    		break;    
    case 4:		
    	ghostCol = pinkyCol;
    	pacmanChase();
    		break;
    default:
    	ghostCol = blinkyCol;
    	pacmanChase();
    		break;
//    case 1: colorWipe(strip.Color(255, 0, 0), 50);  // Red
//            break;
//    case 2: colorWipe(strip.Color(0, 255, 0), 50);  // Green
//            break;
//    case 3: colorWipe(strip.Color(0, 0, 255), 50);  // Blue
//            break;
//    case 4: theaterChase(strip.Color(127, 127, 127), 50); // White
//            break;
//    case 5: theaterChase(strip.Color(127,   0,   0), 50); // Red
//            break;
//    case 6: theaterChase(strip.Color(  0,   0, 127), 50); // Blue
//            break;
//    case 7: rainbow(20);
//            break;
//    case 8: rainbowCycle(20);
//            break;
//    case 9: theaterChaseRainbow(50);
//            break;
  }
}
void pacmanChase()
{
	int pillPos = PIXEL_COUNT-2;
	uint32_t pillCol = 0x806f56;
	
	colorWipe(boardCol,30);
	//run pacman across the board
	
	//blinky chase
	for(int j=0;j<LOOP_MAX;j++)
	{
		runLine(true, 0);
	}
	//pill line
	strip.setPixelColor(pillPos, pillCol);
	runLine(true,0);
	//blinky run
	for(int j=0;j<LOOP_MAX;j++)
	{
		runLine(false, j);
	}	
}
//loopCnt allows the function to factor in a distance between pacman and the ghost
void runLine(bool bForwards, int loopCnt)
{

	
	uint8_t waittime = 500;
	
	if(bForwards)
	{
		for (int i=0; true; i++)
		{
			//pac pos
			if(i < PIXEL_COUNT)
      			strip.setPixelColor(i, pacCol);
      		//pac old pos
      		int oldPos = i-1;
      		if (oldPos < PIXEL_COUNT)
      			strip.setPixelColor(oldPos,boardCol);
      		//blinky pos
      		int blinkyPos = i-2;
      		if(blinkyPos < PIXEL_COUNT)
      			strip.setPixelColor(blinkyPos,ghostCol);
      		int oldBlinkyPos = i-3;
      		strip.setPixelColor(oldBlinkyPos,boardCol);
      		strip.show();
      		delay(waittime);
      		if(blinkyPos >= PIXEL_COUNT)
      			break;
		}	
	}
	else
	{
		//calculate the distance between Pac and Blinky
		int distance = LOOP_MAX - loopCnt;
		for (int i=PIXEL_COUNT-1; true; i--)
		{
			//fudge to allow Pac to catch the ghost
			if(distance == 1 && i == 3)
			{
				strip.setPixelColor(i+distance+1,boardCol);
				distance = 0;
			}
			//pac pos
			if(i >= 0)
      			strip.setPixelColor(i, scaredCol);
      		//blinky old pos
      		int oldPos = i+1;
      		if (oldPos >= 0)
      			strip.setPixelColor(oldPos,boardCol);
      		//pac pos
      		int pacPos = i+distance;
      		if(pacPos >= 0)
      			strip.setPixelColor(pacPos,pacBigCol);
      		int oldPacPos = i+distance+1;
      		strip.setPixelColor(oldPacPos,boardCol);
      		strip.show();
      		delay(waittime);
      		if(pacPos < 0)
      			break;
		}
	}
}
// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();
     
      delay(wait);
     
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
        }
        strip.show();
       
        delay(wait);
       
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, 0);        //turn every third pixel off
        }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}
