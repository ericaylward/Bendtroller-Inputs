#include <Keyboard.h>

// constants - pin mappings and bend vars
const int bPin = 2;
const int aPin = 3;
const int rightPin = 4;
const int leftPin = 5;
const int downPin = 7;
const int upPin = 6;
const int settingPin = 8;
const int bend1 = 0; //left
const int bend2 = 1; // right
const int threshold = 80;
int bend1Init = analogRead(bend1);
int bend2Init = analogRead(bend2);

//flags for pressed/triggered states
int settingPressed = 0;
int upPressed = 0;
int downPressed = 0;
int leftPressed = 0;
int rightPressed = 0;
int aPressed = 0;
int bPressed = 0;
int bend1Neutral = 0;
int bend2Neutral = 0;


// variables will change:
int settingState = 0;
int upState = 0;
int downState = 0;
int leftState = 0;
int rightState = 0;
int aState = 0;
int bState = 0;
int bend1Analog = 0;
int bend2Analog = 0;
int bend1State = 0;
int bend2State = 0;
int controllerBendState = 0;

void setup() {
  // initialize the LED pin as an output:
  Serial.begin(9600);

  // initialize the pushbutton pin as an input:
  pinMode(settingPin, INPUT);
  pinMode(upPin, INPUT);
  pinMode(downPin, INPUT);
  pinMode(leftPin, INPUT);
  pinMode(rightPin, INPUT);
  pinMode(aPin, INPUT);
  pinMode(bPin, INPUT);
  //keyboard input
  Keyboard.begin();
  int bend1Init = analogRead(bend1);
  int bend2Init = analogRead(bend2);
  
}
void loop() {

  // set inital bend (needs to be in loop)
  if(!bend1Init && !bend2Init)
  {
    delay(100);
    bend1Init = analogRead(bend1);
    bend2Init = analogRead(bend2);
  }
  // read the state of the pushbutton value:
  settingState = digitalRead(settingPin);
  upState = digitalRead(upPin);
  downState = digitalRead(downPin);
  leftState = digitalRead(leftPin);
  rightState = digitalRead(rightPin);
  aState = digitalRead(aPin);
  bState = digitalRead(bPin);
  bend1Analog = analogRead(bend1); //left
  bend2Analog = analogRead(bend2); //right

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (settingState == HIGH) {
    if (!settingPressed)
    {
      settingPressed = 1;
      Serial.println("Recalibrated");
      bend1Init = analogRead(bend1);
      bend2Init = analogRead(bend2);
    }
  }
  else if(settingState == LOW)
  {
    settingPressed = 0;
  }
  
  if (upState == HIGH) {
    if(!upPressed)
    {
      upPressed = 1;
      Serial.println("Up");
      Keyboard.press(KEY_UP_ARROW);
    }
  }
  else if(upState == LOW)
  {
    Keyboard.release(KEY_UP_ARROW);
    upPressed = 0;
  }
  
  if (downState == HIGH) {
    if(!downPressed)
    {
      downPressed = 1;
      Keyboard.press(KEY_DOWN_ARROW);
      Serial.println("Down");
    }
  }
  else if (downState == LOW)
  {
    Keyboard.release(KEY_DOWN_ARROW);
    downPressed = 0;
  }
  
  if (leftState == HIGH) {
    if(!leftPressed)
    {
      leftPressed = 1;
      Keyboard.press(KEY_LEFT_ARROW);
      Serial.println("Left");
    }
  }
  else if(leftState == LOW)
  {
    Keyboard.release(KEY_LEFT_ARROW);
    leftPressed = 0;
  }
  
  if (rightState == HIGH) {
    if(!rightPressed)
    {
      rightPressed = 1;
      Keyboard.press(KEY_RIGHT_ARROW);
      Serial.println("Right");
    }
  }
  else if(rightState == LOW)
  {
    Keyboard.release(KEY_RIGHT_ARROW);
    rightPressed = 0;
  }

  //uncomment for use of A and B
  /*
  if (aState == HIGH) {
    if(!aPressed)
    {
      aPressed = 1;
      Keyboard.press('D');
      Serial.println("A");
    }
  }
  else if (aState == LOW)
  {
    Keyboard.release('D');
    aPressed = 0;
  }
  
  if (bState == HIGH) {
    if (!bPressed)
    {
      bPressed = 1;
      Keyboard.press('F');
      Serial.println("B");
    }
  }
  else if(bState == LOW)
  {
    Keyboard.release('F');
    bPressed = 0;
  }
  */

  //setting flags for bend / twist
  //bend 1 flags
  if((bend1Analog <= (bend1Init + threshold)) && (bend1Analog >= (bend1Init - threshold)))
  {
    if(!bend1Neutral)
    {
      bend1Neutral = 1;
      bend1State = 0;
      //Keyboard.release('F');
      //Keyboard.release('D');
    }
  }
  else if(!((bend1Analog <= (bend1Init + threshold)) && (bend1Analog >= (bend1Init - threshold))))
  {
    if(bend1Neutral == 1)
    {
      bend1Neutral = 0;
    }
    
    if(bend1Analog <= (bend1Init + threshold))
    {
      bend1Neutral = 0;
      bend1State = -1;
    }
    else if(bend1Analog >= (bend1Init - threshold))
    {
      bend1Neutral = 0;
      bend1State = 1;
    }
  }

  //bend 2 flags
  if((bend2Analog <= (bend2Init + threshold)) && (bend2Analog >= (bend2Init - threshold)))
  {
    if(!bend2Neutral)
    {
      bend2Neutral = 1;
      bend2State = 0;
      //Keyboard.release('F');
      //Keyboard.release('D');
    }
  }
  else if(!((bend2Analog <= (bend2Init + threshold)) && (bend2Analog >= (bend2Init - threshold))))
  {
    if(bend2Neutral == 1)
    {
      bend2Neutral = 0;
    }

    if(bend2Analog <= (bend2Init + threshold))
    {
      bend2Neutral = 0;
      bend2State = -1;
    }
    else if(bend2Analog >= (bend2Init - threshold))
    {
      bend2Neutral = 0;
      bend2State = 1;
    }
  }

  //triggers based on flags
  //bends
  if(bend1State == 1 && bend2State == 1)
  {
    if(controllerBendState != 1)
    {
      controllerBendState = 1;
      Keyboard.press('D');
      Serial.println('D');
    }
  }
  if(bend1State == 0 && bend2State == 0)
  {
    if(controllerBendState!=0)
    {
      controllerBendState = 0;
      Keyboard.release('D');
      Keyboard.release('F');
      Serial.println("release");
    }
  }
  if(bend1State == -1 && bend2State == -1)
  {
    if(controllerBendState!=-1)
    {
      controllerBendState = -1;
      Keyboard.press('F');
      Serial.println('F');
    }
  }

  //twists
  if((bend1State == -1 && bend2State == 1) || (bend1State == 0 && bend2State == 1))
  {
    if(controllerBendState!=1)
    {
      controllerBendState = 1;
      Keyboard.press('D');
      Serial.println('D');
    }
  }
  if((bend1State == 1 && bend2State == -1) || (bend1State == 1 && bend2State == 0))
  {
    if(controllerBendState!=-1)
    {
      controllerBendState = -1;
      Keyboard.press('F');
      Serial.println('F');
    }
  }
  //Serial.println(bend1State);
  
  //Serial.println("LEFT: "+String(bend1State)+", Right: " + String(bend2State));
  //Serial.println("LEFT: "+String(bend1Analog)+", Right: " + String(bend2Analog));

}
