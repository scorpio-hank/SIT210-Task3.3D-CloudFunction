/* 
  TASK 3.3D - Buddy System
  The music component of this code is by Robson Couto, 2019 - https://github.com/robsoncouto/arduino-songs  
  My component of the code is below the music - i.e. the PIR and LEDs, in addition to the communication with events
*/

// Music code
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_E6  1319
#define REST 0

int tempo = 200;
int buzzer = D6;
int melody[] = {
  NOTE_E5,8, NOTE_E5,8, REST,8, NOTE_E5,8, REST,8, NOTE_C5,8, NOTE_E5,8, //1
  NOTE_G5,4, REST,4, NOTE_G4,8, REST,4, 
  NOTE_C5,-4, NOTE_G4,8, REST,4, NOTE_E4,-4, // 3
  NOTE_A4,4, NOTE_B4,4, NOTE_AS4,8, NOTE_A4,4,
  NOTE_G4,-8, NOTE_E5,-8, NOTE_G5,-8, NOTE_A5,4, NOTE_F5,8, NOTE_G5,8,
  REST,8, NOTE_E5,4,NOTE_C5,8, NOTE_D5,8, NOTE_B4,-4,
  NOTE_C5,-4, NOTE_G4,8, REST,4, NOTE_E4,-4, // repeats from 3
  NOTE_A4,4, NOTE_B4,4, NOTE_AS4,8, NOTE_A4,4,
  NOTE_G4,-8, NOTE_E5,-8, NOTE_G5,-8, NOTE_A5,4, NOTE_F5,8, NOTE_G5,8,
  REST,8, NOTE_E5,4,NOTE_C5,8, NOTE_D5,8, NOTE_B4,-4,
  REST,4, NOTE_G5,8, NOTE_FS5,8, NOTE_F5,8, NOTE_DS5,4, NOTE_E5,8,//7
  REST,8, NOTE_GS4,8, NOTE_A4,8, NOTE_C4,8, REST,8, NOTE_A4,8, NOTE_C5,8, NOTE_D5,8,
  REST,4, NOTE_DS5,4, REST,8, NOTE_D5,-4,
  NOTE_C5,2, REST,2,
};

int notes = sizeof(melody) / sizeof(melody[0]) / 2;
int wholenote = (60000 * 4) / tempo;
int divider = 0, noteDuration = 0;

// This is my code for the additional sensor and LEDs
int inputPin = D4;              // PIR input pin
int led1 = A5;                  // blue LED
int led2 = D2;                  // red LED
int pirState = LOW;             // Initial motion sensor state
int val = 0;                    // variable for reading the pin status


void setup()
{
  pinMode( led2, OUTPUT );
  pinMode( led1, OUTPUT );
  pinMode( buzzer, OUTPUT );
  pinMode( inputPin, INPUT );
  Particle.subscribe("wave", myWaveHandler);
  Particle.subscribe("pat", myMusicHandler);
  delay(15000); // this gives the motion sensor time to calibrate
}

void loop()
{
    readSensor();
    reportData();
}

void readSensor() {
  val = digitalRead(inputPin);
}


void reportData() {
  if (val == HIGH) 
  { 
    if (pirState == LOW) 
    {
      Particle.publish("wave", "MovementDetected", PRIVATE);
      pirState = HIGH;
    }
  } else 
  {
    if (pirState == HIGH) 
    {
      pirState = LOW;
    }
  }
}


// This blinks the LED three times when a wave is detected
void myWaveHandler(const char *event, const char *data)
{
  for (int i=0; i<3; i++)
  {
    digitalWrite( led2, HIGH );
    digitalWrite( led1, HIGH );
    delay(500);
    digitalWrite( led2, LOW );  
    digitalWrite( led1, LOW );
    delay(500);
  }
}

// This is the music code that deals with the pat event
void myMusicHandler(const char *event, const char *data)
{
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {
    // I have added this code in to flash the LEDs in time with the music
    if( (thisNote / 2) % 2 == 0)
    {
        digitalWrite( led2, HIGH );
        digitalWrite( led1, LOW );
    } else
    {
        digitalWrite( led2, LOW );
        digitalWrite( led1, HIGH );
    }
    
    divider = melody[thisNote + 1];
    if (divider > 0) {
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; 
    }
    tone(buzzer, melody[thisNote], noteDuration*0.9);
    delay(noteDuration);
    noTone(buzzer);
  }
}
