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
#define LED3_PIN 10 
#define LED4_PIN 11 
#define LED5_PIN 12

/*
BUTTON A = PIN 4
BUTTON B = PIN 5
SDA      = PIN 6
SCL      = PIN 7

Led 1    = PIN 11
Led 2    = PIN 12
Led 3    = PIN 14
Led 4    = PIN 15
Led 5    = PIN 16
*/

char woord[6] = "abcde";
const char* Paswoord;
int Letter = 0;
bool Status = false;

void setup() 
{
  Serial.begin(115200);

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
  pinMode(LED3_PIN, OUTPUT);
  pinMode(LED4_PIN, OUTPUT);
  pinMode(LED5_PIN, OUTPUT);

  pinMode(BUTTON_A_PIN, INPUT_PULLUP);
  pinMode(BUTTON_B_PIN, INPUT_PULLUP);


  delay(2000);  
  display.clearDisplay();

}


void loop()
{
  randomSeed(millis() + analogRead(A0));
  Paswoord = GeneratePassword();

do 
{
  
  int buttonAState = digitalRead(BUTTON_A_PIN);
  int buttonBState = digitalRead(BUTTON_B_PIN);
 
    TurnOnLeds(GetLeds(Paswoord, Letter));
    
    if(buttonAState == LOW)
    { 
     ScrollLettersUP(Letter);
     delay(400);
    }
    
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println(Paswoord);
    display.setCursor(0, 20);
    display.println(woord);
    display.display();
    

    if(buttonBState == LOW)
    {
     Letter = CompareLetter(Letter, Paswoord, woord, Status);
     delay(400);
    }
  
}
while(Status == false);
  


}

//Genereer een random paswoord.
const char* GeneratePassword()
{
    static char password[6];  
    const char alphabet[] = "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < 5; i++)
    {
        int randomIndex = random(26); 
        password[i] = alphabet[randomIndex];
    }

    password[5] = '\0';  

    return password;  
}

//Scroll door de letters.
void ScrollLettersUP(int Letter)
{

 woord[Letter] = woord[Letter] + 1;
 if(woord[Letter] == '{')
 {
  woord[Letter] = 'a';
 }
 Serial.print(woord);
 Serial.print("\n");
}

//Check of de letter hetzelfde is als de letter van het paswoord.
int CompareLetter(int Letter, const char* Paswoord, char woord[],bool Status)
{
  //Checken of de letter juist is.
  if(Paswoord[Letter] == woord[Letter])
  {
     display.setCursor(0, 45);
     display.println("Correct Letter!");
     display.display();

   Letter++;
  }
  else 
  {
     display.setCursor(0, 45);
     display.println("Wrong Letter!");
     display.display();

     //hier zou een fout moeten geven 
     //eventueel een uitgang die een signaal stuurt hier.
     
     Paswoord = GeneratePassword();
     Letter = 0;
  }
  
  //Mag niet boven de array gaan.
  if(Letter > 4)
  {
    Letter = 0;
  }

  //Checken of het hele woord juist is.
  if(strcmp(Paswoord, woord) == 0)
  {
    Letter = 0;
    display.clearDisplay();
    display.setCursor(0, 25);
    display.println("PASSWORD CORRECT!");
    display.display();
    delay(5000);
    //Hier is de module complete.
    //Eventueel een uitgang die een signaal stuurt hier.

    //opnieuw een woord genereren.
    Paswoord = GeneratePassword();
    Letter = 0;
  }

 return Letter;
}

//Krijg een getal dat wijst naar een letter.
int GetLeds(const char* Paswoord, int digitnumber)
{
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
    
    const char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
    
    for (int i = 0; i < 26; i++) 
    {
        if (Paswoord[digitnumber] == alphabet[i])
        {
            return i + 1;   //(1 voor 'a', 2 voor 'b', etc.)
        }
    }
    
    return 0;  
}

//Zet de correcte leds aan.
void TurnOnLeds(int GetledsID)
{
    const int ledPatterns[26][5] = {
    //abcdefghijklmnopqrstuvwxyz
      {1, 1, 1, 0, 0},  // 'a' 
      {1, 1, 0, 0, 1},  // 'b' 
      {1, 1, 0, 1, 0},  // 'c' 
      {1, 0, 0, 0, 1},  // 'd' 
      {0, 1, 1, 1, 0},  // 'e' 
      {1, 1, 1, 1, 1},  // 'f' 
      {1, 1, 0, 1, 1},  // 'g' 
      {1, 0, 1, 1, 0},  // 'h' 
      {0, 0, 0, 1, 0},  // 'i' 
      {1, 0, 1, 0, 1},  // 'j' 
      {1, 1, 0, 0, 0},  // 'k' 
      {0, 0, 0, 0, 1},  // 'l' 
      {0, 1, 1, 0, 1},  // 'm' 
      {0, 1, 0, 0, 1},  // 'n' 
      {1, 0, 1, 1, 1},  // 'o' 
      {1, 0, 0, 1, 0},  // 'p' 
      {0, 0, 1, 1, 0},  // 'q'  
      {0, 0, 0, 1, 0},  // 'r' 
      {0, 1, 0, 1, 1},  // 's' 
      {1, 1, 0, 1, 1},  // 't' 
      {1, 1, 1, 0, 1},  // 'u' 
      {1, 0, 1, 1, 0},  // 'v' 
      {0, 1, 1, 1, 1},  // 'w' 
      {1, 0, 1, 0, 0},  // 'x' 
      {1, 1, 0, 0, 1},  // 'y' 
      {0, 1, 1, 0, 0}   // 'z'
    };

   
    if (GetledsID < 1 || GetledsID > 26)
    {
        return; 
    }
    
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
    digitalWrite(LED3_PIN, LOW);
    digitalWrite(LED4_PIN, LOW);
    digitalWrite(LED5_PIN, LOW);
    
    int index = GetledsID - 1;  // Omdat GetledsID van 1 tot 26 is, maar array-index van 0 tot 25
    if (ledPatterns[index][0]) digitalWrite(LED1_PIN, HIGH);
    if (ledPatterns[index][1]) digitalWrite(LED2_PIN, HIGH);
    if (ledPatterns[index][2]) digitalWrite(LED3_PIN, HIGH);
    if (ledPatterns[index][3]) digitalWrite(LED4_PIN, HIGH);
    if (ledPatterns[index][4]) digitalWrite(LED5_PIN, HIGH);
}

