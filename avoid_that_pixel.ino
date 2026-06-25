
#include <LiquidCrystal.h>

#define GREEN_PIN 6
#define YELLOW_PIN 5
#define RED_PIN 4
#define BUT_PIN 3

////////////////
////////////////
//LDC Screen
////////////////
////////////////
LiquidCrystal lcd(7,9,10,11,12,13);
byte player[8] = {
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111  // whole block is lit up
};

int row = 0; 
int col = 0;

////////////////
////////////////
//time variables
////////////////
////////////////
unsigned time_delay = 60;
byte button_state = LOW; 
byte last_button_state = LOW;
unsigned long last_time_button_changed = millis();
unsigned long debounce_delay = 50;


////////////////
////////////////
// directional
// functions
////////////////
////////////////
int direction = 1;
void move_player()
{
 int next_row = row + direction;
  if(next_row >= 0 && next_row <=15)
  {
	erase_player();
    row = next_row;
    draw_player();
  }
}


////////////////
////////////////
// Character
////////////////
////////////////
void erase_player()
{
  lcd.setCursor(col,row);
  lcd.print(" "); 
}

void draw_player()
{
  lcd.setCursor(col,row); //bottom-left character cell
  lcd.write(byte(0)); //print customer character
}


////////////////
////////////////
//setup
////////////////
////////////////
void setup()
{
  Serial.begin(115200);
 
  //initalize LEDs
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  
  //initalize LDC
  lcd.begin(16,2);
  lcd.createChar(0,player);
  draw_player();
  
  //turn all LEDS on
  digitalWrite(GREEN_PIN, HIGH);
  digitalWrite(YELLOW_PIN, HIGH);
  digitalWrite(RED_PIN, HIGH);
  
  
  //initalize button
  pinMode(3, INPUT_PULLUP);
}


////////////////
////////////////
//main loop
////////////////
////////////////
void loop()
{
  // initialize current time
  unsigned long time_now = millis();
  
  // move button left or right when pressed
  if (time_now - last_time_button_changed > debounce_delay)
  {
    byte new_button_state = digitalRead(BUT_PIN);
    Serial.println(new_button_state);
    
  	if (new_button_state != button_state)
    {
      //update time
      last_time_button_changed = time_now;
      button_state = new_button_state;
      
      if(button_state == LOW)
       {
         direction = -direction;
         move_player();
       }
    
     }
  }
}
