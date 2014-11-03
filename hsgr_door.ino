// Hackerspace.gr Door Code

#include <OneWire.h>
#include "keys.h"

OneWire ds(4);  // OneWire read pin

int permited = 0;
byte id[8] = { 0, 0, 0, 0, 0, 0, 0, 0};


void setup(void) {
  Serial.begin(9600);
  pinMode(2, OUTPUT);          // Relay
  pinMode(7, OUTPUT);          // Reader's LED green
  pinMode(8, OUTPUT);          // Reader's LED red
  pinMode(9, INPUT_PULLUP);    // Wall Button
  Serial.println("Serial start");
}

void loop(void) {
  
  if( digitalRead(9) == LOW ){
    Serial.println("Button Pressed");
    open(BD_size);
  }

  if ( !ds.search(id)) {
    ds.reset_search();
    delay(100);
    return;
  }

  //Show the read ID 
  Serial.print("ID - ");
  for( int i = 0; i < 8; i++) {
    Serial.print(id[i], DEC);
    Serial.print(" ");
  }
  Serial.println(" ");

  //Go through keys permitted for an ID match
  for(int k = 0; k < BD_size; k++) {
    permited=0;

    //Verify if there is a match
    for(int b = 0; b < 8; b++) {
      if(keys[k][b]==id[b]){
         permited++;
      }
    }
    if(permited==8){
      open(k);            // Open the door
      permited=0;
      for(int b = 0; b < 8; b++) {
      id[b]=B0000000;}
      break; 
    }
    else if(k==BD_size-1) // No match found
      closed();
  }

}

//*****   Functions    ******

void open(int k) {       
  digitalWrite(2, HIGH);
  delay(100);
  digitalWrite(2, LOW);
  Serial.print("Door open, key: ");
  Serial.println(k+1, DEC);
  Serial.println("");

  for(int i = 0; i < 1; i++) {
    digitalWrite(7, HIGH);
    delay(500);
    digitalWrite(7, LOW);
    delay(500); 
  }
}

void closed() {
  Serial.println("Wrong code!");
  Serial.println("");
  for(int i = 0; i < 10; i++) {
    digitalWrite(8, HIGH);
    delay(150);
    digitalWrite(8, LOW);
    delay(150); 
  }
}
