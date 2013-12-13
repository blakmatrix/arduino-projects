const int ledPinR = 0;      // led connected to digital pin PB0
const int ledPinG = 1;      // led connected to digital pin PB1
const int ledPinB = 2;      // led connected to digital pin PB2
const int knockSensor = 4; // the piezo is connected to PB4


const int threshold = 3;           // Minimum signal from the piezo to register as a knock
const int rejectValue = 25;        // If an individual knock is off by this percentage of a knock we don't unlock..
const int averageRejectValue = 15; // If the average timing of the knocks is off by this percent we don't unlock.
const int knockFadeTime = 150;     // milliseconds we allow a knock to fade before we listen for another one. (Debounce timer.)
const int lockTurnTime = 650;      // milliseconds that we run the motor to get it to go a half turn.

const int maximumKnocks = 20;       // Maximum number of knocks to listen for.
const int knockComplete = 1200;     // Longest time to wait for a knock before we assume that it's finished.

// Variables.
int secretCode[maximumKnocks] = {25, 25, 25, 50, 50, 50, 25, 25, 25, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  // Initial setup: "Shave and a Hair Cut, two bits."
int knockReadings[maximumKnocks];   // When someone knocks this array fills with delays between knocks.
int knockSensorValue = 0;           // Last reading of the knock sensor.


void setup() {
 pinMode(ledPinR, OUTPUT); // declare the ledPin as as OUTPUT
 pinMode(ledPinG, OUTPUT); // declare the ledPin as as OUTPUT
 pinMode(ledPinB, OUTPUT); // declare the ledPin as as OUTPUT
 //Serial.begin(9600);       // use the serial port
 //Serial.println("Program start.");             // but feel free to comment them out after it's working right.
 setColor(0, 255, 0);  // green
 delay(300);
 setColor(0, 0, 0);  // off
 delay(300);
 setColor(0, 255, 0);  // green
 delay(300);
 setColor(0, 0, 0);  // off
}

void loop() {
  // read the sensor and store it in the variable sensorReading:
  knockSensorValue = analogRead(knockSensor);    
  
  // if the sensor reading is greater than the threshold:
  if (knockSensorValue >= threshold) {
    //ledState = !ledState;   
    // update the LED pin itself:        
    //digitalWrite(ledPin, ledState);

    listenToKnocks();
  }
    
  delay(100);  // delay to avoid overloading the serial port buffer
}

void setColor(int red, int green, int blue)
{
  analogWrite(ledPinR, red);
  analogWrite(ledPinG, green);
  analogWrite(ledPinB, blue);  
}


void listenToKnocks(){
  int i = 0;
  // First lets reset the listening array.
  for (i=0;i<maximumKnocks;i++){
    knockReadings[i]=0;
  }
  int currentKnockNumber=0;  // Incrementer for the array.
  int startTime=millis(); // Reference for when this knock started.
  int now=millis();

  
  setColor(0, 0, 0); 
  delay(knockFadeTime);                                 
  setColor(0, 255, 0);  // green

  do {
    //listen for the next knock or wait for it to timeout. 
    knockSensorValue = analogRead(knockSensor);
    if (knockSensorValue >=threshold){             
      now=millis();
      knockReadings[currentKnockNumber] = now-startTime;
      currentKnockNumber ++;                          
      startTime=now;          

      setColor(0, 0, 0); 

      delay(knockFadeTime);                              // again, a little delay to let the knock decay.
      setColor(0, 255, 0);  // green

    }

    now=millis();
    
    //did we timeout or run out of knocks?
  } while ((now-startTime < knockComplete) && (currentKnockNumber < maximumKnocks));
  

  if (validateKnock() == true){ //special knock
    specialLights(); //special LED pattern
  } else {
    int finalKnockCount = 0;
    for(i=0;i<maximumKnocks;i++){
      if(knockReadings[i]==0) break;
      ++finalKnockCount;
    }
    if(finalKnockCount==1)  setColor(255, 0, 0);      // red
    if(finalKnockCount==2)  setColor(0, 255, 0);      // green
    if(finalKnockCount==3)  setColor(0, 0, 255);      // blue
    if(finalKnockCount==4)  setColor(255, 255, 0);    // yellow
    if(finalKnockCount==5)  setColor(80, 0, 80);      // purple
    if(finalKnockCount==6)  setColor(0, 255, 255);    // aqua
    if(finalKnockCount==7)  setColor(255, 255, 255);  // white
    if(finalKnockCount==8)  setColor(255, 0, 255);    // Fuchsia
    if(finalKnockCount==9)  setColor(192,192,192);    // silver
    if(finalKnockCount==10) setColor(128,0,0);        // maroon
    if(finalKnockCount==11) setColor(128,128,0);      // olive
    if(finalKnockCount==12) setColor(0,128,128);      // teal
    if(finalKnockCount==13) setColor(220,20,60);      // crimson
    if(finalKnockCount==14) setColor(255,165,0);      // orange
    if(finalKnockCount==15) setColor(127,255,0);      // chartreuse
    if(finalKnockCount==16) setColor(32,178,170);     // light sea green
    if(finalKnockCount==17) setColor(60,179,113);     // medium sea green
    if(finalKnockCount==18) setColor(30,144,255);     // dodger blue
    if(finalKnockCount==19) setColor(255,228,196);    // bisque
    if(finalKnockCount==20) setColor(65,105,225);     // royal blue

    delay(4000);
  }

}

void specialLights(){
  unsigned int rgbColour[3];
 
  // Start off with red.
  rgbColour[0] = 255;
  rgbColour[1] = 0;
  rgbColour[2] = 0;  
 
  // Choose the colours to increment and decrement.
  for (int decColour = 0; decColour < 3; decColour += 1) {
    int incColour = decColour == 2 ? 0 : decColour + 1;
 
    // cross-fade the two colours.
    for(int i = 0; i < 255; i += 1) {
      rgbColour[decColour] -= 1;
      rgbColour[incColour] += 1;
      
      setColor(rgbColour[0], rgbColour[1], rgbColour[2]);
      delay(5);
    }
  }

}

// Sees if our knock matches the secret.
// returns true if it's a good knock, false if it's not.
// todo: break it into smaller functions for readability.
boolean validateKnock(){
  int i=0;
 
  // simplest check first: Did we get the right number of knocks?
  int currentKnockCount = 0;
  int secretKnockCount = 0;
  int maxKnockInterval = 0;  // used to normalize the times.
  
  for (i=0;i<maximumKnocks;i++){
    if (knockReadings[i] > 0){
      currentKnockCount++;
    }
    if (secretCode[i] > 0){ //todo: precalculate this.
      secretKnockCount++;
    }
    
    if (knockReadings[i] > maxKnockInterval){   // collect normalization data
      maxKnockInterval = knockReadings[i];
    }
  }
  
  
  if (currentKnockCount != secretKnockCount){
    return false; 
  }
  
  /*  Now we compare the relative intervals of the knocks, not the absolute time between them. */
  int totaltimeDifferences=0;
  int timeDiff=0;
  for (i=0;i<maximumKnocks;i++){ // Normalize the times
    knockReadings[i]= map(knockReadings[i],0, maxKnockInterval, 0, 100);      
    timeDiff = abs(knockReadings[i]-secretCode[i]);
    if (timeDiff > rejectValue){ // Individual value too far out of whack
      return false;
    }
    totaltimeDifferences += timeDiff;
  }
  // It can fail if the whole thing is too inaccurate.
  if (totaltimeDifferences/secretKnockCount>averageRejectValue){
    return false; 
  }
  
  return true;
  
}
