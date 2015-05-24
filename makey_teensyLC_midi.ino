//Rebuild for teensyLC

int bounceThreshold = 0; // a press must last this many loop cycles (about 20ms each) before triggering. 
// higher values make it less sensitive, fewer false triggers, higher latency

#define NUM_INPUTS 11

int channel = 2;

// edit these arrays and choose one to change the MIDI notes sent; work towards multiple keys, aeolian etc 
int pitches[NUM_INPUTS] = {
  60, 61, 62, 63, 
  64, 65, 66, 67, 68, 69, 70      // right side of female header, up,down,left,right,left-click,right-click
};

int pitches2[NUM_INPUTS] = {
  60, 62, 64, 65,
  67, 69, 71, 72, 74, 76, 77      // right side of female header, up,down,left,right,left-click,right-click
};

int pitches3[NUM_INPUTS] = {
  79, 81, 83, 84,
  86, 88, 89, 91, 93, 95, 96      // right side of female header, up,down,left,right,left-click,right-click
};  

int pitches4[NUM_INPUTS] = {
  79, 81, 83, 84,
  86, 88, 89, 91, 93, 95, 96      // right side of female header, up,down,left,right,left-click,right-click
};
// teensy makey pin numbers
int pinNumbers[NUM_INPUTS] = {
  0,1,3,4,    // left side starting at USB connector
  15,16,17,18,19,22,23 // all the touchRead inputs
};

// cap sense thresholds for each pin
// this is proportional to the capacitance on the pin that will count as a press
// it is units of a very small unit of time, in iterations through an unrolled loop
// higher values make it less sensitive (i.e. require larger capacitance)
//int capThresholds[NUM_INPUTS] = {
//  1, 1, 1, 1, 1, 2,
//  2, 2, 1, 1, 1, 1,
//  2, 1, 1, 1, 1, 1,
//};

int bounceCounter[NUM_INPUTS];    
boolean pressed[NUM_INPUTS];
int baseline[NUM_INPUTS];
int sensitivity=50;

void setup(){
  for (int i=0; i<NUM_INPUTS; i++) {
    baseline[i]=touchRead(pinNumbers[i])+sensitivity;
    usbMIDI.sendNoteOff(pitches3[i],127,channel);
    bounceCounter[i] = 0;
    pressed[i] = false;
  }
}

void loop() { 
  //Serial.print("\n");
  for (int i=0; i<NUM_INPUTS; i++) {                      // for each pin
    int c = touchRead(pinNumbers[i]);             // check capacitance
   //Serial.print(c);
   //Serial.print("\t");

    if (c>baseline[i]){                                                       // if we detect a touch on the pin
      if (!pressed[i]) {                                          // and if we're not already pressed
        bounceCounter[i]++;                                           // increment the bounce counter
        if(bounceCounter[i] > bounceThreshold){                       // if we're over the bounce threshold
          usbMIDI.sendNoteOn(pitches3[i],127,channel);                      // send a MIDI note on
          pressed[i] = true;                                              // remember it was pressed
          bounceCounter[i]=0;                                             // reset the bounce counter
        }
      }
    } 
    else {                                                  // if we don't a detect touch on the pin
      if (pressed[i]) {                                           // if this key was pressed before
        usbMIDI.sendNoteOff(pitches3[i],127,channel);                    // send a MIDI note off
        pressed[i] = false;                                          // remember we are not pressed
        bounceCounter[i] = 0;                                        // reset the bounce counter
      }        
    }
  }
  //Serial.println(" ");
}  


