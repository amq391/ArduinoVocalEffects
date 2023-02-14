/* Vocal Sound Effects Device Code
 * University of Illinois at Chicago
 * Aaron Quino and Ayomide Taiwo
 * The Whole Grain Breadboards
 * 
 * Heading from original project:
 * Final effects box code
 * by Amanda Ghassaei
 * https://www.instructables.com/id/Arduino-Vocal-Effects-Box/
 * Aug 2012
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
*/

//audio storage
byte buffer1[500];
byte buffer2[500];

//buffer recording variables
boolean toggle = 0;
boolean rec = 1;

//pot checking storage
int scale = 20;       // not used
int scalePot;         // not used
int newScale = scale; // not used
byte multiplier = 0;
int grain = 500;      // Changed by serial connection signal
int grainPot;
int newGrain = grain;
int wetDryPot;
int wet = 16;         // Changed by serial connection signal
int dry = 16-wet;
int newWetDry = wet;

//data retrieval variables
unsigned int i = 0;       //index variable
int iscale = 0;           //index variable
int iscalerev = grain-1;

//clipping indicator variables
boolean clipping;
int clippingCounter = 5000;

//reverse switch variables
boolean forward = 1;      // Changed by serial connection signal
boolean newForward = forward;

int controlMode = 0;

byte blist[8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40, 0x80};
void setup(){
  //Serial.begin(9600);
  for (int i = 2; i < 14; i++) {
    pinMode(i, OUTPUT);
  }
  //DDRD=0xFE;//set digital pins 0-7 as outputs
  //DDRB=0xFD;//set digital pins 10-13 as outputs, 9 as input
  DDRC=0x00;//set all analog pins as inputs
  
  cli();//diable interrupts
  
  //set up continuous sampling of analog pin 0
  
  //clear ADCSRA and ADCSRB registers
  ADCSRA = 0;
  ADCSRB = 0;
  
  ADMUX = 0;//Clear ADMUX register
  ADMUX |= (1 << REFS0); //set reference voltage
  ADMUX |= (1 << ADLAR); //left align the ADC value- so I can read highest 8 bits from ADCH register only
  //since I'm reading A0, I don't need to specifiy which analog pin I want to read from (0 is default)
  
  ADCSRA |= (1 << ADPS2) | (1 << ADPS0); //set ADC clock with 32 prescaler- 16mHz/32=500kHz
  ADCSRA |= (1 << ADATE); //enabble auto trigger
  ADCSRA |= (1 << ADIE); //enable interrupts when measurement complete
  ADCSRA |= (1 << ADEN); //enable ADC
  ADCSRA |= (1 << ADSC); //start ADC measurements
  
  sei();//enable interrupts
}



ISR(ADC_vect) {//when new ADC value ready
  
  if (rec){
    if (toggle){
      buffer1[i] = ADCH;//store incoming
      if (ADCH == 0 || ADCH == 1023){//if clipping
        digitalWrite(13, HIGH);
        //PORTB |= 32;//set pin 13 high
        clippingCounter = 5000;//reset clipping counter
        clipping = 1;//currently clipping
      }
      if (forward){//forward
        byte temp = (wet*int(buffer2[iscale])+dry*int(ADCH))>>4;
        for (int i = 0; i < 8; i++) {
          if (int(temp & blist[i]) != 0) {
            digitalWrite(i+2,HIGH);
          } else {
            digitalWrite(i+2,LOW);
          }
        }
        
        //PORTD = (wet*int(buffer2[iscale])+dry*int(ADCH))>>4;//send out DAC
      }
      else{//reverse
        //Serial.println("here");
        //PORTD = (wet*int(buffer2[iscalerev])+dry*int(ADCH))>>4;//send out DAC
      }
    }
    else{
      buffer2[i] = ADCH;//store incoming
      if (ADCH == 0 || ADCH == 1023){//if clipping
        digitalWrite(13, HIGH);
        //PORTB |= 32;//set pin 13 high
        clippingCounter = 5000;//reset clipping counter
        clipping = 1;//currently clipping
      }
      if (forward){
        byte temp = (wet*int(buffer1[iscale])+dry*int(ADCH))>>4;
        for (int i = 0; i < 8; i++) {
          if (int(temp & blist[i]) != 0) {
            digitalWrite(i+2,HIGH);
          } else {
            digitalWrite(i+2,LOW);
          }
        }
        //PORTD = (wet*int(buffer1[iscale])+dry*int(ADCH))>>4;//send out DAC
      }
      else{
        byte temp = (wet*int(buffer1[iscalerev])+dry*int(ADCH))>>4;
        for (int i = 0; i < 8; i++) {
          if (int(temp & blist[i]) != 0) {
            digitalWrite(i+2,HIGH);
          } else {
            digitalWrite(i+2,LOW);
          }
        }
        //PORTD = (wet*int(buffer1[iscalerev])+dry*int(ADCH))>>4;//send out DAC
      }
    }
  }
  else{
    if (toggle){
      if (forward){
        byte temp = (wet*int(buffer2[iscale])+dry*int(ADCH))>>4;
        for (int i = 0; i < 8; i++) {
          if (int(temp & blist[i]) != 0) {
            digitalWrite(i+2,HIGH);
          } else {
            digitalWrite(i+2,LOW);
          }
        }
        //PORTD = (wet*int(buffer2[iscale])+dry*int(ADCH))>>4;//send out DAC
      }
      else{
        byte temp = (wet*int(buffer2[iscalerev])+dry*int(ADCH))>>4;
        for (int i = 0; i < 8; i++) {
          if (int(temp & blist[i]) != 0) {
            digitalWrite(i+2,HIGH);
          } else {
            digitalWrite(i+2,LOW);
          }
        }
        //PORTD = (wet*int(buffer2[iscalerev])+dry*int(ADCH))>>4;//send out DAC
      }
    }
    else{
      if (forward){
        byte temp = (wet*int(buffer1[iscale])+dry*int(ADCH))>>4;
        for (int i = 0; i < 8; i++) {
          if (int(temp & blist[i]) != 0) {
            digitalWrite(i+2,HIGH);
          } else {
            digitalWrite(i+2,LOW);
          }
        }
        //PORTD = (wet*int(buffer1[iscale])+dry*int(ADCH))>>4;//send out DAC
      }
      else{
        byte temp = (wet*int(buffer1[iscalerev])+dry*int(ADCH))>>4;
        for (int i = 0; i < 8; i++) {
          if (int(temp & blist[i]) != 0) {
            digitalWrite(i+2,HIGH);
          } else {
            digitalWrite(i+2,LOW);
          }
        }
        //PORTD = (wet*int(buffer1[iscalerev])+dry*int(ADCH))>>4;//send out DAC
      }
    }
  }
  i++;//increment i
  iscale = i*10/scale-grain*multiplier;
  iscalerev = grain-iscale-1;
  if (i==grain){
    rec = 0;//stop recording
  }
  if (i>=(grain*scale/10)){
    if (scale<10){
      if (i==grain){
        i = 0;
        iscale = 0;
        iscalerev = grain-1;
        forward = newForward;//update direction
        scale = newScale;//update scale
        grain = newGrain;//update grain
        dry = newWetDry;//update wet dry
        wet = 16-dry;
        toggle ^= 1;//try removing this
        rec = 1;
        multiplier = 0;
      }
      else if (iscale>=grain){
        iscale = 0;
        iscalerev = grain-1;
        multiplier++;
      }
    }
    else{
      i = 0;
      iscale = 0;
      iscalerev = grain-1;
      forward = newForward;//update direction
      scale = newScale;//update scale
      grain = newGrain;//update grain
      dry = newWetDry;//update wet dry
      wet = 16-dry;
      toggle ^= 1;//try removing this
      rec = 1;
      multiplier = 0;
    }
  }
  if (clipping){
    clippingCounter--;//decrement clipping counter
  }
}

void checkScale(){
  digitalWrite(10, LOW);
  //PORTB &= 251;//set pin 10 low
  scalePot = 0;
  while(digitalRead(10)){
    scalePot++;
  }
  newScale = scalePot+2;
}

void checkGrainSize(){
  digitalWrite(11, LOW);
  //PORTB &= 247;//set pin 11 low
  grainPot = 0;
  while(digitalRead(11)){
    grainPot++;
  }
  if (grainPot < 1){
    grainPot = 1;
  }
  else if (grainPot > 20){
    grainPot = 20;
  }
  newGrain = grainPot*25;
}

void checkWetDry(){
  digitalWrite(12, LOW);
  //PORTB &= 239;//set pin 12 low
  wetDryPot = 0;
  while(digitalRead(12)){
    wetDryPot += 10;//from 0 to ~185
  }
  if (wetDryPot < 10){
    wetDryPot = 0;
  }
  else if (wetDryPot > 160){
    wetDryPot = 160;
  }
  newWetDry = wetDryPot/10;//scale down to 16
}

/*void checkRev(){//check reverse switch
  if (digitalRead(9)){
    newForward = 1;
  }
  else{
    newForward = 0;
  }
}*/

void loop(){
  if (clipping){//if currently clipping
    if (clippingCounter<=0){//if enough time has passed since clipping
      clipping = 0;//not currently clipping
      digitalWrite(13, LOW);
      //PORTB &= 223;//turn off clipping led indicator (pin 13)
    }
  }
  for (int i = 10; i < 13; i++) {
    pinMode(i, OUTPUT);
  }
  //DDRB=0xFD;//set pins 10-12 as outputs
  for (int i = 10; i < 13; i++) {
    digitalWrite(i, HIGH);
  }
  //PORTB |= 28;//set pins 10-12 high
  delay(1);//wait for capacitor to discharge
  //checkRev();//check reverse switch
  for (int i = 10; i < 13; i++) {
    pinMode(i, INPUT);
  }
  //DDRB= 0xE1;//set pins 10-12 as inputs

  
  //checkScale();

  if (controlMode == 0) {
    checkGrainSize();
    checkWetDry();
  } else {
    while (Serial.available() > 0) {
      char val = Serial.read();
      switch (val) {
          case 'a':
            // increase grain by 5
            if (grain <= 495) {
              grain += 5;
            }
            break;
          case 'b':
            // decrease grain by 5
            if (grain >= 10) {
              grain -= 5;
            }
            break;
          case 'c':
            // increase wet by 1
            if (wet <= 15) {
              wet += 1;
            }
            break;
          case 'd':
            // decrease wet by 1
            if (wet >= 1) {
              wet -= 1;
            }
            break;
          case 'e':
            // switch between forward and reverse
            if (forward == 0) {
              forward = 1;
            } else {
              forward = 0;
            }
            break;
          case 'f':
            // switch between button and pot control
            if (controlMode == 0) {
              grain = 355;
              wet = 10;
              controlMode = 1;
            } else {
              controlMode = 0;
            }
            break;
          default:
            break;
      }
    }
  }

  
}
