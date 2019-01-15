#include <LiquidCrystal.h>
#include <Servo.h>
#include <IRremote.h>
IRrecv irrecv(8);
decode_results signals;
Servo myservo;
const int rs = 12, en = 11, d4 = 6, d5 = 5, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);  
// Define pins for ultrasonic and buzzer
int const trigPin = 10;
int const echoPin = 9;
int const buzzPin = 2;
int const led = 4;
int total_counter=0;
int pos = 0;
//stall 1
int stall_1_counter=0;
int ledPin = 8;               
int stall_1_input_pin = 7;   
int pirState = LOW;  
//lcd 
        
void setup()
{
  irrecv.enableIRIn();
  lcd.begin(16, 2);
  lcd.print("Welcome to Visitor Counter!");
  pinMode(led, OUTPUT);
  pinMode(trigPin, OUTPUT); // trig pin will have pulses output
  pinMode(echoPin, INPUT); // echo pin should be input to get pulse width
  pinMode(buzzPin, OUTPUT); // buzz pin is output to control buzzering
  Serial.begin(9600);
  myservo.attach(13);
}
void loop()
{
  // Duration will be the input pulse width and distance will be the distance to the obstacle in centimeters
  int duration, distance;
  // Output pulse with 1ms width on trigPin
  digitalWrite(trigPin, HIGH);
  delay(1);
  digitalWrite(trigPin, LOW);
  // Measure the pulse input in echo pin
  duration = pulseIn(echoPin, HIGH);
  // Distance is half the duration devided by 29.1 (from datasheet)
  distance = (duration/2) / 29.1;
  //digitalWrite(led, HIGH);
  // if distance less than 0.5 meter and more than 0 (0 or less means over range)
  if (distance <= 20 && distance >= 0) {
    // Buzz
      digitalWrite(led, HIGH);
      total_counter++;
      counterDisplay();
      servo();
  } else {
      // Don't buzz
      digitalWrite(led, LOW);
  }

    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    // Calculating the distance
    distance= duration*0.034/2;
  // Prints the distance on the Serial Monitor

//remote_control() function call
remote_control();


    
//stall 1 
int val = 0;
val = digitalRead(stall_1_input_pin);  // read input value
  if (val == HIGH) {            // check if the input is HIGH
    digitalWrite(ledPin, HIGH);  // turn LED ON
    if (pirState == LOW) {
      // we have just turned on
      if(stall_1_counter <= total_counter){
        stall_1_counter++;
      }      
      counterStall_1();
      // We only want to print on the output change, not state
      pirState = HIGH;
    }
  } else {
    digitalWrite(ledPin, LOW); // turn LED OFF
    if (pirState == HIGH){
      // we have just turned of
//      stall_1_exit();
      // We only want to print on the output change, not state
      pirState = LOW;
    }
  }
  
delay(60);
}

int counterDisplay(){
  lcd.clear();
  lcd.print("Total :");
  lcd.print(total_counter);
   delay(500);
}

int counterStall_1(){
  lcd.clear();
  lcd.print("Stall (1) :");
  lcd.print(stall_1_counter);
   delay(500);
}

void stall_1_exit(){
  lcd.print("Someone Exit From Stall (1):");
   delay(500);
   lcd.clear();
}
//gateopen
void servo(){
   for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(10);                       // waits 15ms for the servo to reach the position
  }
  delay(1000);
  for (pos = 90; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(30);                       // waits 15ms for the servo to reach the position
  }
}

void remote_control(){
  if (irrecv.decode(&signals)) {
//        if(signals.value==16738455){
//          counterDisplay();
//        }else if(signals.value==16724175){
//          counterStall_1();
//        }else if(signals.value==REMOTE_KEY_1){
//          servo();
//        }
      switch(signals.value)  // formatted to be less 'long'
        {
          case 0xFF6897: 
            servo();   
            total_counter++;  
            break;
          case 0xFF30CF: counterStall_1(); break;
          case 0xFF52AD: counterDisplay();     break;
        }
        irrecv.resume(); // get the next signal
    }
}
