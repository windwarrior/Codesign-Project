

// Graphing sketch


// This program takes ASCII-encoded strings
// from the serial port at 9600 baud and graphs them. It expects values in the
// range 0 to 1023, followed by a newline, or newline and carriage return

// Created 20 Apr 2005
// Updated 18 Jan 2008
// by Tom Igoe
// This example code is in the public domain.

import processing.serial.*;

Serial myPort;        // The serial port
int xPos = 1;         // horizontal position of the graph
int[] prev;  // een array om het vorige gemeten punt in op te slaan, per sensor heeft de array een element
boolean notDef = true;
//Een array van 8 kleuren
int[][] colors = {
  {
    255, 0, 0
  }
  , {
    0, 255, 0
  }
  , {
    0, 0, 255
  }
  , {
    255, 255, 0
  }
  , {
    255, 0, 255
  }
  , {
    0, 255, 255
  }
  , {
    255, 255, 255
  }
};
void setup () {
  // set the window size:
  size(400, 300);        

  // List all the available serial ports
  println(Serial.list());
  // I know that the first port in the serial list on my mac
  // is always my  Arduino, so I open Serial.list()[0].
  // Open whatever port is the one you're using.
  myPort = new Serial(this, Serial.list()[0], 115200);
  // don't generate a serialEvent() unless you get a newline character:
  myPort.bufferUntil('\n');
  // set inital background:
  background(0);
}
void draw () {
  // everything happens in the serialEvent()
}
void initSerial(int count){
  //Init array lengte op de hoeveelheid
  //sensorvalues die aangesloten is
  prev = new int[count];
  //zet de 'vorige' value array op height/2
  for(int i = 0; i < prev.length; i++){
    prev[i] = height/2;  
  }  
}

void serialEvent (Serial myPort) {
  // get the ASCII string:
  String inString = myPort.readStringUntil('\n');
  // splits de lijn op per spatie
  String[] splits = inString.split(" ");
  int i = 0;
  //Als dit de eerste keer is dat de serialEvent methode
  //aangeroepen wordt, doe dan wat initialisatie
  if(notDef){
    initSerial(splits.length);
    notDef = false;
  }  
  
  //voor alle stukjes string in de splits-array
  for (String split : splits) {
    // trim off any whitespace:
    inString = trim(split);
    // convert to an int and map to the screen height:
    float inByte = float(inString); 
    inByte = map(inByte + 512, 0, 1023, 0, height);

    // draw the line:
    // col is the colorvalue in the color array
    int col = i % colors.length;
    // zet de kleur van de lijn die we gaan tekenen
    stroke(colors[col][0], colors[col][1], colors[col][2]);
    // teken een lijn van het huidige punt naar het opgeslagen punt
    line(xPos-1, prev[col], xPos, height - inByte);
    // sla het getekende punt op in de previous array, om zo lijnen
    // te kunnen trekken ipv puntjes te zetten
    prev[col] = height - (int) inByte;
    i++;
  }

  // at the edge of the screen, go back to the beginning:
  if (xPos >= width) {
    xPos = 0;
    
    background(0);
  } 
  else {
    // increment the horizontal position:
    xPos++;
  }
}


