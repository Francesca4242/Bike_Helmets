#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LE_PIN    12

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 40

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LE_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

void setup() {
  strip.begin();
  Serial.begin(9600);
  
  if (MCUSR & 1) {
    // we have just been powered on
    MCUSR = 0x00;
  } else {
    // we have been reset with the button
    int val = EEPROM.read(0);
    val = val + 1;
    if (val > 6) val=0;
    EEPROM.update(0, val);  
  }
}

float oliversBatteryLevel() {
  ADMUX=0b01001110;
  delay(1);
  _SFR_BYTE(ADCSRA) |= _BV(ADSC);
  while(bit_is_set(ADCSRA, ADSC));
  uint8_t low = ADCL;
  uint8_t high = ADCH;

  int entire = (high << 8) | low;

  float voltage = 1.1f*1024.0f / entire;

  float batteryLevel = (voltage - 2.8) / (4.15-2.8);
  if (batteryLevel>1) batteryLevel=1;
  if (batteryLevel<0) {
    EEPROM.update(0,6);
  }
  

  Serial.println(entire);

  return batteryLevel;
}
void loop() {
  strip.setBrightness(oliversBatteryLevel()*255);
  if( EEPROM.read(0) == 0 ){
  
  
    // put your main code here, to run repeatedly:
    uint32_t red = strip.Color(255, 0, 0);
    uint32_t white = strip.Color(255, 255, 255);
    
    strip.fill(white, 8, 8);  
    strip.fill(red, 27, 14);
    strip.setPixelColor(0, 255, 64, 0);
    strip.setPixelColor(1, 255, 255, 0);
    strip.setPixelColor(2, 0, 255, 0);
    strip.setPixelColor(3, 0, 255, 64);
    strip.setPixelColor(4, 0, 255, 255);
    strip.setPixelColor(5, 0, 0, 255);
    strip.setPixelColor(6, 64, 0, 255);
    strip.setPixelColor(7, 255, 0, 255);
    strip.setPixelColor(26, 255, 64, 0);
    strip.setPixelColor(25, 255, 255, 0);
    strip.setPixelColor(24, 255, 255, 0);
    strip.setPixelColor(23, 0, 255, 0);
    strip.setPixelColor(22, 0, 255, 64);
    strip.setPixelColor(21, 0, 255, 255);
    strip.setPixelColor(20, 0, 255, 255);
    strip.setPixelColor(19, 0, 0, 255);
    strip.setPixelColor(18, 64, 0, 255);
    strip.setPixelColor(17, 255, 0, 255);
    strip.setPixelColor(16, 255, 0, 255);
  
   
    strip.show();
    //delay(1000);
  
  }



  if( EEPROM.read(0) == 1 ){
      strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
      uint32_t red = strip.Color(255, 0, 0);
      uint32_t white = strip.Color(255, 255, 255);
    
      strip.fill(white, 8, 8);  
      strip.fill(red, 27, 14);
      strip.setPixelColor(0, 255, 64, 0);
      strip.setPixelColor(1, 255, 255, 0);
      strip.setPixelColor(2, 0, 255, 0);
      strip.setPixelColor(3, 0, 255, 64);
      strip.setPixelColor(4, 0, 255, 255);
      strip.setPixelColor(5, 0, 0, 255);
      strip.setPixelColor(6, 64, 0, 255);
      strip.setPixelColor(7, 255, 0, 255);
      strip.setPixelColor(26, 255, 64, 0);
      strip.setPixelColor(25, 255, 255, 0);
      strip.setPixelColor(24, 255, 255, 0);
      strip.setPixelColor(23, 0, 255, 0);
      strip.setPixelColor(22, 0, 255, 64);
      strip.setPixelColor(21, 0, 255, 255);
      strip.setPixelColor(20, 0, 255, 255);
      strip.setPixelColor(19, 0, 0, 255);
      strip.setPixelColor(18, 64, 0, 255);
      strip.setPixelColor(17, 255, 0, 255);
      strip.setPixelColor(16, 255, 0, 255);
  
      strip.show();
      //delay(1000);
      }

    if( EEPROM.read(0) == 2 ){
      int wait=10;
      
      // Hue of first pixel runs 5 complete loops through the color wheel.
      // Color wheel has a range of 65536 but it's OK if we roll over, so
      // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
      // means we'll make 5*65536/256 = 1280 passes through this outer loop:
      for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
        for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
          // Offset pixel hue by an amount to make one full revolution of the
          // color wheel (range of 65536) along the length of the strip
          // (strip.numPixels() steps):
          int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
          // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
          // optionally add saturation and value (brightness) (each 0 to 255).
          // Here we're using just the single-argument hue variant. The result
          // is passed through strip.gamma32() to provide 'truer' colors
          // before assigning to each pixel:
          strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
        }
        strip.show(); // Update strip with new contents
        delay(wait);  // Pause for a moment
      }

    }

    

    if( EEPROM.read(0) == 3 ){
      strip.fill(strip.Color(128, 0, 200), 0, 40);
      strip.show();
    }

    if( EEPROM.read(0) == 4 ){
      strip.fill(strip.Color(0, 255, 255), 0, 40);
      strip.show();
    }

    if( EEPROM.read(0) == 5 ){
      float batLevel = oliversBatteryLevel();
      unsigned long start_time = millis();
      int shift=0;
      while (shift<40) {
        shift = (float)(millis()-start_time)/batLevel/100000*40*10;
        for (int i=0; i<40; i++) {
          strip.setPixelColor((i + shift)%40, strip.Color(2550/(i+10), 511/(i+2), 255/(2*i)));
        }
        strip.show();
      }
    }

    if( EEPROM.read(0) == 6 ){
      strip.fill(0, 0, 28);
      strip.show();
      delay(10000);
      pinMode(A3, OUTPUT);
      digitalWrite(A3
      , HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(10000); 

    }
}
