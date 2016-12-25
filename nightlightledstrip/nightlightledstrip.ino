#include "Adafruit_NeoPixel.h"
#ifdef __AVR__
  #include <avr/power.h>
#endif

const int stripPin = 4;
const int buttonPin = 2; 
const int photocellPin = 1;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, stripPin, NEO_GRB + NEO_KHZ800);
int buttonState = 0; 
int prevButtonState = 0;
int animation = 1;

int prevPhotocellReading = 0; 
int photocellReading = 1000; 

void setup() {
  // Initialize serial output
  Serial.begin(9600);
  
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);

  // Initiate Strip
  strip.begin();
  strip.setBrightness(64);
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  if (photocellReading > 500) {
    Serial.print("Analog reading = "); Serial.println(photocellReading);
    Serial.println("lalala");
    blank(20);
  }
  else if (animation == 0) {
    blank(20);
  }
  else if (animation == 1) {
    rainbowCycle(20);
  }
  else if (animation == 2) {
    candyChase(100);
  }
  else if (animation == 3) {
    snowFlakes(100, 1.0, 1.0, 1.0);
  }
  else if (animation == 4) {
    snowFlakes(100, 1.0, 0.0, 0.0);
  }
  else if (animation == 5) {
    snowFlakes(100, 0.0, 1.0, 0.0);
  }
  else if (animation == 6) {
    snowFlakes(100, 0.0, 0.0, 1.0);
  }
  else if (animation == 7) {
    snowFlakes(100, 0.3, 0.0, 1.0);
  }
}

bool checkButton() {
  if (checkPhotocell()) {
    return true;
  }
  
  prevButtonState = buttonState;
  buttonState = digitalRead(buttonPin);

  if (buttonState == prevButtonState) {
    return false;
  }

  if (buttonState == HIGH) {
    // turn LED on:
    Serial.print("Button pressed!");
    animation = (animation + 1) % 8;
    Serial.print("Animation is"); Serial.println(animation, DEC);
    return true;
  }
}

bool checkPhotocell() {
  prevPhotocellReading = photocellReading;
  photocellReading = analogRead(photocellPin);

  if (photocellReading >= 600) {
    return true;
  }
  return false;
}

void blank(uint8_t wait) {
  uint16_t i;
  for(i=0; i< strip.numPixels(); i++) {
    strip.setPixelColor(i, 0);
  }
  strip.show();
  if (checkButton()) {
    return;
  }
  delay(wait);
}

void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    if (checkButton()) {
      break;
    }
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}


void candyChase(uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i+q, 255,255,255);    //turn every pixel white
      }
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 255,0,0);    //turn every third pixel red
      }
      strip.show();

      if (checkButton()) {
        return;
      }

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }

      if (checkButton()) {
        return;
      }
    }
  }
}

void snowFlakes(uint8_t wait, float r, float g, float b) {
  // Setup the pixel array
  int pixel[60];
  for(int p=0; p<60; p++){
    pixel[p] = random(0,255); 
  }
  
  // Run some snowflake cycles
  for (int j=0; j<200; j++) {
  
    // Every five cycles, light a new pixel
    if((j%5)==0){
      Serial.print(int(255 * b), DEC);
      strip.setPixelColor(random(0,60), int(255 * r), int(255*g), int(255*b));
    }
    
    // Dim all pixels by 10
    for(int p=0; p<60; p++){
      strip.setPixelColor(p, int(pixel[p]* r), int(pixel[p] * g), int(pixel[p]* b) );
      pixel[p] =  pixel[p] - 10;
    }
    
    strip.show();
    
    if (checkButton()) {
      break;
    }
    delay(wait);
  }
}

