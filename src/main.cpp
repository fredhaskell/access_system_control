#include "Door.h"

Door door;

void setup() {
  Serial.begin(9600);
  door.Init();
}

void loop() 
{
  // check card
  door.CheckCard();

  // open button
  door.CheckButton();
  // keepOpen button

  // close lock
  door.closeLock();
}