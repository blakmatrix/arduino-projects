
int ledPins[] = {2,3,4,5,6,7,8,9}; 

void setup()
{
  
  for(int i = 0; i < 8; i++){  
      pinMode(ledPins[i],OUTPUT); 
  }                                   
   Serial.begin(9600);
   Serial.println("BOOOTING UP...");
   Serial.println("Welcome to the binary addition program! Lets get started...");
   
}
 
 

void loop() 
{
  binaryCount();
  blinkMe();
}
 

void binaryCount(){
  Serial.println("");
  Serial.print("Count");
  Serial.print('\t');
  Serial.println("BIN");
  Serial.println("-----   ---------");
  
  for(int i =0;i<256;i++){
    
    for(int j = 0; j <8; j++){
      if ( ((i>>j)%2) == 0 ){
        digitalWrite(ledPins[7-j], LOW);
      }else{
        digitalWrite(ledPins[7-j], HIGH);
      }
    }
    Serial.print(i);
    Serial.print('\t');
    Serial.println(i, BIN);

    delay(200);
  }                                                                    
}
 

void blinkMe(){
  Serial.print("WOW... That was fun! Lets do it again!");
  int delayTime = 100; 
  for(int j = 0; j<5; j++){
    Serial.print(".");
    for(int i = 0; i <= 7; i++){
      int offLED = i - 1;
      if(i == 0) {
        offLED = 7;
      }
      digitalWrite(ledPins[i], HIGH);
      digitalWrite(ledPins[offLED], LOW);
      delay(delayTime);
    }
  }
  Serial.println("");
  
}
 
