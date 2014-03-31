/* Robot navigation algorithm. The arduino microcontroller
   will call the setup() function, then loop continuously
   through the loop() function.

   Sebastian Dunn 2012 */

#include <Servo.h>                           // Include servo library

Servo leftWheel;                             // Declare left and right wheel servos
Servo rightWheel;
long freq = 38000;

void setup()                          
{
  leftWheel.attach(12);                      // Attach left signal to pin 12
  rightWheel.attach(13);                     // Attach right signal to pin 13
  
  pinMode(9, OUTPUT); pinMode(10, INPUT);     //left IR LED at pin 9, receiver at pin 10
  pinMode(2, OUTPUT); pinMode(3, INPUT);     //right IR LED at pin 2, receiver at pin 3
  pinMode(5, OUTPUT); pinMode(6, INPUT);      //front IR LED at pin 5, receiver at pin 6
  
  Serial.begin(9600);
}  

void loop()                                  
{     
  int front = detect(5, 6); //Dectect the input from the front eye
  int right = detect(2, 3);
  int left = detect(9, 10);
  
  Serial.print(left);    //prints the state of each eye to the serial monitor. Used for debugging purposes
  Serial.print(" ");
  Serial.print(front);
  Serial.print(" ");
  Serial.print(right);
  Serial.println(" ");
  // delay(500);
  
  if (front == 1){    // 1 means NOTHING IN THE WAY
    if (right == 0 and left == 0){ //walls on both sides
     move(200, 200);     //move forward
     delay(250);
    }
    else if (right == 0 && left == 1) { //wall on right side
      turnLeft(100);  //turn left slightly
      move(200, 200); //then move forward
      delay(100);     //for a short time
    }
    else if (left == 0 && right == 1){  //wall on left side
      turnRight(100);  //turn right slightly
      move(200, 200);  //then move forward
      delay(100);      //for a short time
    }
    
  }
  else {  //if there is something directly ahead
    if (right == 0 && left == 0) {  //walls on both sides, turn around
      turnAround();
    }
    else if (right == 0){  //wall on right side only
      while (front == 0){  //turn left until nothing is detected in front
      turnLeft(100);
      front = detect(5, 6);
      }
    }
    else if (left == 0){   //wall on the left side
      while (front == 0){  //turn right until nothing is detected in front
      turnRight(100); 
      front = detect(5, 6);
      }
    }
    
}
}

int detect(int ledPin, int receiverPin)
{
  tone(ledPin, freq, 8);              // pulse IR eye at 38 kHz for at least 1 ms
  delay(1);                                  // Wait 1 ms
  int ir = digitalRead(receiverPin);       // IR receiver -> ir variable
  delay(1);                                  // Down time before recheck
  return ir;                                 // Return 1 if not detecting, 0 detecting
} 

//helper function to make robot movements more intuitive
//two positive numbers for forward, two negative for back
void move(int left, int right){                               
  leftWheel.writeMicroseconds(1500 + left); 
  rightWheel.writeMicroseconds(1500 - right); 
}

void turnLeft(int del){
//  Serial.println("Inside turnLeft");
  move(-200, 200);
  delay(del);
}

void turnRight(int del){
//  Serial.println("Inside turnRight");
  move(200, -200);
  delay(del);
}

void turnAround(){
 turnRight(1400);
}


