#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#define BUTTON_A_PIN 2  
#define BUTTON_B_PIN 3  
#define LED1_PIN 8 
#define LED2_PIN 9  

/*
BUTTON A = PIN 4
BUTTON B = PIN 5
SDA      = PIN 6
SCL      = PIN 7
*/

const char* woorden[] = {"Stoom", "Steen", "Blauw", "Bloem", "Grond", "Grijs", "Klopt", "Klein", "Plank", "Plons"};
int aantalWoorden = sizeof(woorden) / sizeof(woorden[0]);

void setup() 
{
  Serial.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("Hello World!"));
  display.display();

  // Stel LED pinnen in als output
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);

  pinMode(BUTTON_A_PIN, INPUT_PULLUP);
  pinMode(BUTTON_B_PIN, INPUT_PULLUP);
  
  delay(2000);  
  display.clearDisplay();

  randomSeed(analogRead(0));  // Gebruik een analoge pin voor willekeurige getallen
}


void loop()
{

  int buttonAState = digitalRead(BUTTON_A_PIN);
  int buttonBState = digitalRead(BUTTON_B_PIN);

    int randomIndex = random(aantalWoorden);  
    const char* Paswoord = woorden[randomIndex];

    GetLeds(Paswoord);

    display.clearDisplay();
    display.setCursor(0, 0);
    display.println(Paswoord); 
    display.display();
   
  

  delay(10000); 
}




void GetLeds(const char* Paswoord)
{
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);

  if (strncmp(Paswoord, "St", 2) == 0)          //Check of het woord begint met St
  {
    digitalWrite(LED1_PIN, HIGH);  
  } 
  else if (strncmp(Paswoord, "Bl", 2) == 0)     //Check of het woord begint met Bl
  {
    digitalWrite(LED1_PIN, HIGH);  
    digitalWrite(LED2_PIN, HIGH);  
  }
    else if (strncmp(Paswoord, "Kl", 2) == 0)    //Check of het woord begint met Kl
  {
 
  }
    else if (strncmp(Paswoord, "Gr", 2) == 0)    //Check of het woord begint met Gr
  {
 
  }
    else if (strncmp(Paswoord, "Pl", 2) == 0)    //Check of het woord begint met Pl
  {
  
  }
}
