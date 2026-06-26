
#include <LiquidCrystal.h>

#define GREEN_PIN 6
#define YELLOW_PIN 5
#define RED_PIN 4
#define BUT_PIN 3
#define BUZZER 8

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

///////////////
///////////////
// BUZZER
const int buzzer = 8;
///////////////
///////////////


////////////////
////////////////
//time variables 
// for button
////////////////
////////////////
unsigned time_delay = 60;
byte button_state = LOW; 
byte last_button_state = LOW;
unsigned long last_time_button_changed = millis();
unsigned long debounce_delay = 50;

int min = 0;
int sec = 0;

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
int life_points = 3;
bool game_over = false;

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

void player_hit()
{
  if (life_points != 0)
  {
    if (life_points == 3)
    {
      digitalWrite(GREEN_PIN, LOW);
    }
    else if (life_points == 2)
    {
      digitalWrite(YELLOW_PIN, LOW);
    }
    else
    {
      digitalWrite(RED_PIN, LOW);
      tone(buzzer, 1000);
      delay(100);
      noTone(buzzer);

      // calculate total time
      unsigned long TOTAL_GAME_TIME = millis() / 1000;
      min = TOTAL_GAME_TIME / 60;
      sec = TOTAL_GAME_TIME % 60;

      show_game_over(min, sec);
      game_over = true;
    }

    life_points--;
  }
}

void show_game_over(int min, int sec)
{
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("YOU DIED");

  lcd.setCursor(3,1);
  lcd.print("TIME: ");
  lcd.print(min);
  lcd.print(":");
  lcd.print(sec);
}


////////////////
////////////////
//bullets variables
////////////////
////////////////
const int MAX_BULLETS = 20;
int bull_col[MAX_BULLETS];
int bull_row[MAX_BULLETS];
bool bull_active[MAX_BULLETS] = {false};


////////////////
////////////////
//time variables 
// for bullet
////////////////
////////////////
unsigned long last_speed_increase = 0;
const unsigned long speed_increase_interval = 10000;

//time variables
unsigned long last_spawn_time = 0;
unsigned long spawn_interval = 2000;

unsigned long last_move_time = 0;
unsigned long move_interval = 700;

const unsigned long MIN_INTERVAL = 250;

void increase_speed()
{
    spawn_interval = spawn_interval * 0.7;
    move_interval = move_interval * 0.7;

    if (spawn_interval < MIN_INTERVAL)
    {
      spawn_interval = MIN_INTERVAL;
    }
    if (move_interval < MIN_INTERVAL)
    {
      move_interval = MIN_INTERVAL;
    }

}


////////////////
////////////////
//bullets
////////////////
////////////////

void spawn_bullet()
{
  //check if enough space to draw new bullet
  for (int i = 0; i < MAX_BULLETS; i++)
  {
    if (bull_active[i] && bull_col[i] >= 14)
    {
      return;
    }
  }
  
  //drawing new bullet
  for (int i = 0; i < MAX_BULLETS; i++)
  {
    if (!bull_active[i]) //free space
    {
      bull_col[i] = 15;
      bull_row[i] = random(0,2);
      bull_active[i] = true;
      draw_bullet(i);
      break; // one bullet at once
    }
  }
}

void move_bullet()
{
  for(int i = 0; i < MAX_BULLETS; i++)
  {
    if(bull_active[i])
    {
      erase_bullet(i);
      bull_col[i]--;

      // determine if bullet hits player
      if ((bull_col[i] == col && bull_row[i] == row) || life_points == 0)
      {
        bull_active[i] = false;
        player_hit();
        if(game_over)
        {
          return;
        }
        continue;
      }

      // if not collide but hit bottom row
      if(bull_col[i] < 1)
      {
        bull_active[i] = false;
      }
      else
      {
        draw_bullet(i);
      }
    }
  }
}

void draw_bullet(int i)
{
  lcd.setCursor(bull_col[i], bull_row[i]);
  lcd.write(byte(0));
}

void erase_bullet(int i)
{
  lcd.setCursor(bull_col[i], bull_row[i]);
  lcd.print(" ");
}

////////////////
////////////////
//setup
////////////////
////////////////
void setup()
{
  Serial.begin(115200);
  randomSeed(analogRead(A0));

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

  //initalize buzzer
  pinMode(8, OUTPUT);
}


////////////////
////////////////
//main loop
////////////////
////////////////
void loop()
{
  if(game_over)
  {
    return;
  }


  // initialize current time
  unsigned long time_now = millis();

  // move button left or right when pressed
  if (time_now- last_time_button_changed > debounce_delay)
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

  //bullet spawning
  if(time_now - last_spawn_time >= spawn_interval)
  {
    last_spawn_time = time_now;
    spawn_bullet();
  }

  if(time_now - last_move_time >= move_interval)
  {
    last_move_time = time_now;
    move_bullet();
  }

  if(time_now - last_speed_increase >= speed_increase_interval)
  {
    last_speed_increase = time_now;
    increase_speed();
  }

}
