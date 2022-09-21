// A basic everyday NeoPixel strip test program.

// NEOPIXEL BEST PRACTICES for most reliable operation:
// - Add 1000 uF CAPACITOR between NeoPixel strip's + and - connections.
// - MINIMIZE WIRING LENGTH between microcontroller board and first pixel.
// - NeoPixel strip's DATA-IN should pass through a 300-500 OHM RESISTOR.
// - AVOID connecting NeoPixels on a LIVE CIRCUIT. If you must, ALWAYS
//   connect GROUND (-) first, then +, then data.
// - When using a 3.3V microcontroller with a 5V-powered NeoPixel strip,
//   a LOGIC-LEVEL CONVERTER on the data line is STRONGLY RECOMMENDED.
// (Skipping these may work OK on your workbench but can fail in the field)

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN 5

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 20
#define LOT_LEDS 7
#define LOOP_MILLIS 50 // 20hz

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);


void setup()
{
  Serial.begin(115200);
  pinMode(A4, OUTPUT);
  digitalWrite(A4, LOW);
  pinMode(A2, INPUT_PULLUP);

  strip.begin();            // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();             // Turn OFF all pixels ASAP
  strip.setBrightness(255); // Set BRIGHTNESS to max
}

// loop() function -- runs repeatedly as long as board is on ---------------
enum
{
  basic_flicker,
  steady_t,
  steady_breathe_t,
  rainbow_party_t,
  mode_invalid
} mode;
int *modee = (int *)&mode;

long time = INT32_MAX;
uint32_t count;

void loop()
{
  if (!digitalRead(A2))
  {
    *modee = (*modee + 1) % mode_invalid;
    delay(500); // have 100 mills after mode switch to pick up
  }
  if ((millis() - time) > LOOP_MILLIS)
  {
    time = millis();
    count = time / LOOP_MILLIS;

    switch (mode)
    {
    case basic_flicker:
      flicker_set();
      break;
    case steady_t:
      steady();
      break;
    case steady_breathe_t:
      steady_breathe();
      break;
    case rainbow_party_t:
      rainbow_party();
      break;
    };

    if (!(count % 20))
      Serial.println(*modee);
  }
}

int f = 1;
int f_count = 0;
void flicker_set()
{
  for (int i = 0; i < LOT_LEDS; i++)
  {
    uint32_t color = strip.ColorHSV(10923, 150, 255);
    float r = (float)rand() / (float)RAND_MAX;
    if (r < 0.01)
    {
      color = strip.ColorHSV(10923, 150, 60);
    }

    if (f_count >= 40)
    {
      if (r > 0.99)
      {
        f = i;
        f_count = 0;
      }
    }
    else
    {

      if (f == i)
      {
        Serial.print(i);
        Serial.print(" ");
        Serial.print(f);
        Serial.print(" ");
        Serial.print(f_count);
        Serial.println("running");
        color = 0;
        f_count++;
      }
    }
    strip.setPixelColor(i, color);
    strip.show();
  }
}

void steady()
{
  for (int i = 0; i < LOT_LEDS; i++)
  {
    strip.setPixelColor(i, strip.ColorHSV(10923, 150, 255));
    strip.show();
  }
}
void steady_breathe()
{
  for (int i = 0; i < LOT_LEDS; i++)
  {
    strip.setPixelColor(i, strip.ColorHSV(10923, 150, 255.0 * 0.5 * (sin(count * 3. / 100.) + 1.)));
    strip.show();
  }
}

void rainbow_party()
{
  strip.rainbow((600 * count % 65536));
  strip.show();
}