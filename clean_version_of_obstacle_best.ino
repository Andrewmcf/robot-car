#include <IRremote.h>
#include <Servo.h>  

#define f 16736925  // FORWARD
#define b 16754775  // BACK
#define l 16720605  // LEFT
#define r 16761405  // RIGHT
#define s 16712445  // STOP
#define KEY1 16738455 //Normal obstacle
#define KEY2 16750695 //faster and so on
#define KEY3 16756815 
#define KEY4 16724175 
#define KEY5 16718055
#define KEY6 16743045 
#define KEY7 16716015 
#define KEY8 16726215
#define KEY9 16734885
#define KEY0 16730805
#define KEY_STAR 16728765
#define KEY_HASH 16732845

#define RECV_PIN  12
#define ECHO_PIN  A4  
#define TRIG_PIN  A5 
#define ENA 5
#define ENB 6
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11
#define LED_Pin 13
#define LineTeacking_Pin_Right  10
#define LineTeacking_Pin_Middle 4
#define LineTeacking_Pin_Left   2
#define LineTeacking_Read_Right   !digitalRead(10)
#define LineTeacking_Read_Middle  !digitalRead(4)
#define LineTeacking_Read_Left    !digitalRead(2)
#define carSpeed 250 
#define gospeed  120
#define turnspeed 140
#define fasterspeed 175
#define fasterturn 160
#define slowspeed  130
#define quickspeed  200
#define quickturn   200
#define stopDistance 60
#define cleanDistance 35
#define cleanTurnDistance 15
#define turnDistance 30
#define sideDistance 25
#define breakPower 70
#define  floorcleanspeed  140
#define cleanturnspeed    190
#define specialDistance  20
#define lessturn   175
#define  specialdelay 4
#define specialstopdistance 45
Servo servo;
IRrecv irrecv(RECV_PIN);
decode_results results;
unsigned long IR_PreMillis;
unsigned long LT_PreMillis;
int rightDistance = 0, leftDistance = 0, middleDistance = 0;
int pos = 0;    // variable to store the servo position

enum FUNCTIONMODE{
  IDLE,
  
  ObstaclesAvoidance,
  
  FasterOA,
  MoreFast,
  BatteryUse,
  Battery,
  DifferentObstacle,
  BestClean,
  
  Bluetooth,
  IRremote
} func_mode = IDLE;

enum MOTIONMODE {
  STOP,
  FOREWARD,
  BACK,
  LEFT,
  RIGHT
} mov_mode = STOP;

void delays(unsigned long t) {
  for(unsigned long i = 0; i < t; i++) {
    getBTData();
    getIRData();
    delay(1);
  }
}

int getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  return (int)pulseIn(ECHO_PIN, HIGH) / 58;
}

void forward(bool debug = false){ 
  analogWrite(ENA,gospeed);
  analogWrite(ENB, gospeed);
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  if(debug) Serial.println("Go forward!");
}
 void cleanforward(bool debug = false){ 
  analogWrite(ENA,floorcleanspeed);
  analogWrite(ENB, floorcleanspeed + 7);
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  if(debug) Serial.println("Go forward!");
}                                                           
void morefast(bool debug = false){ 
  analogWrite(ENA,quickspeed);
  analogWrite(ENB, quickspeed);
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  if(debug) Serial.println("Go forward!");
}
void fastgo(bool debug = false){ 
  analogWrite(ENA,fasterspeed);
  analogWrite(ENB, fasterspeed);
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  if(debug) Serial.println("Go forward!");
}
void test1(bool debug = false){ 
  analogWrite(ENA,0);
  analogWrite(ENB, 80);
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  if(debug) Serial.println("Go forward!");
}
void test2(bool debug = false){ 
  analogWrite(ENA,80);
  analogWrite(ENB, 0);
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  if(debug) Serial.println("Go forward!");
}
void sforward(bool debug = false){ 
  analogWrite(ENA,slowspeed);
  analogWrite(ENB, slowspeed);
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  if(debug) Serial.println("Go forward!");
}
void foreward(bool debug = false){ 
  analogWrite(ENA,carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  if(debug) Serial.println("Go forward!");
}
void left(bool debug = false){
  analogWrite(ENA,carSpeed);
  analogWrite(ENB,carSpeed);
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH); 
  if(debug) Serial.println("Go left!");
}
void lessleft(bool debug = false){
  analogWrite(ENA,lessturn);
  analogWrite(ENB,lessturn);
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH); 
  if(debug) Serial.println("Go left!");
}
void cleanleft(bool debug = false){
  analogWrite(ENA,cleanturnspeed);
  analogWrite(ENB,cleanturnspeed);
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH); 
  if(debug) Serial.println("Go left!");
}
void morefastleft(bool debug = false){
  analogWrite(ENA,quickturn);
  analogWrite(ENB,quickturn);
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH); 
  if(debug) Serial.println("Go left!");
}
void sleft(bool debug = false){
  analogWrite(ENA,turnspeed);
  analogWrite(ENB,turnspeed);
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH); 
  if(debug) Serial.println("Go left!");
}
void fleft(bool debug = false){
  analogWrite(ENA,fasterturn);
  analogWrite(ENB,fasterturn);
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH); 
  if(debug) Serial.println("Go left!");
}
void right(bool debug = false){
  analogWrite(ENA,carSpeed);
  analogWrite(ENB,carSpeed);
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  if(debug) Serial.println("Go right!");
}
void lessright(bool debug = false){
  analogWrite(ENA,lessturn);
  analogWrite(ENB,lessturn);
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  if(debug) Serial.println("Go right!");
}
void cleanright(bool debug = false){
  analogWrite(ENA,cleanturnspeed);
  analogWrite(ENB,cleanturnspeed);
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  if(debug) Serial.println("Go right!");
}
void morefastright(bool debug = false){
  analogWrite(ENA,quickturn);
  analogWrite(ENB,quickturn);
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  if(debug) Serial.println("Go right!");
}
void fright(bool debug = false){
  analogWrite(ENA,fasterturn);
  analogWrite(ENB,fasterturn);
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  if(debug) Serial.println("Go right!");
}
void sright(bool debug = false){
  analogWrite(ENA,turnspeed);
  analogWrite(ENB,turnspeed);
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  if(debug) Serial.println("Go right!");
}
void back(bool debug = false){
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  if(debug) Serial.println("Go back!");
}
void cleanback(bool debug = false){
  analogWrite(ENA, floorcleanspeed);
  analogWrite(ENB, floorcleanspeed - 16);
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  if(debug) Serial.println("Go back!");
}
void sback(bool debug = false){
  analogWrite(ENA, slowspeed);
  analogWrite(ENB, slowspeed);
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  if(debug) Serial.println("Go back!");
}
void breaks(bool debug = false){
  analogWrite(ENA, breakPower);
  analogWrite(ENB, breakPower);
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  if(debug) Serial.println("Go back!");
}
void fbreaks(bool debug = false){ 
  analogWrite(ENA,breakPower);
  analogWrite(ENB, breakPower);
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  if(debug) Serial.println("Go forward!");
}
void Beep(bool debug = false){ 
  analogWrite(ENA,70);
  analogWrite(ENB, 70);
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  if(debug) Serial.println("Go forward!");
}
void stop(bool debug = false){
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
  if(debug) Serial.println("Stop!");

}

void getBTData() {
  if(Serial.available()) {
    switch(Serial.read()) {
      case 'f': func_mode = Bluetooth; mov_mode = FOREWARD;  break;
      case 'b': func_mode = Bluetooth; mov_mode = BACK;     break;
      case 'l': func_mode = Bluetooth; mov_mode = LEFT;     break;
      case 'r': func_mode = Bluetooth; mov_mode = RIGHT;    break;
      case 's': func_mode = Bluetooth; mov_mode = STOP;     break;
      case '2': func_mode = ObstaclesAvoidance;             break;
      
      default:  break;
    }  
  }
}

void getIRData() {
  if (irrecv.decode(&results)){ 
    IR_PreMillis = millis();
    switch(results.value){
      case f:   func_mode = IRremote; mov_mode = FOREWARD;  break;
      case b:   func_mode = IRremote; mov_mode = BACK;     break;
      case l:   func_mode = IRremote; mov_mode = LEFT;     break;
      case r:   func_mode = IRremote; mov_mode = RIGHT;    break;
      case s:   func_mode = IRremote; mov_mode = STOP;     break;
      case KEY1:  func_mode = ObstaclesAvoidance;          break;
      case KEY2:  func_mode = FasterOA;                   break;
      case KEY3: func_mode = MoreFast;                    break;
      case KEY4:  func_mode = BatteryUse;                 break;
     case KEY5:  func_mode = Battery;                   break;
     case KEY6:  func_mode = DifferentObstacle;         break;
     case KEY7:  func_mode = BestClean;                 break;
          default: break;
    }
    irrecv.resume();
  }
}

void bluetooth_mode() {
  if(func_mode == Bluetooth){
    switch(mov_mode){
      case FOREWARD: foreward();  break;
      case BACK:    back();     break;
      case LEFT:    left();     break;
      case RIGHT:   right();    break;
      case STOP:    stop();     break;
      default: break;
    }
  }
}

void irremote_mode() {
  if(func_mode == IRremote){
    switch(mov_mode){
      case FOREWARD: foreward();  break;
      case BACK:    back();     break;
      case LEFT:    left();     break;
      case RIGHT:   right();    break;
      case STOP:    stop();     break;
      default: break;
    }
    if(millis() - IR_PreMillis > 500){
      mov_mode = STOP;
      IR_PreMillis = millis();
    }
  }   
}


void obstacles_avoidance_mode() {
  if(func_mode == ObstaclesAvoidance){
     servo.write(90);  //setservo position according to scaled value
    delay(250);
    middleDistance = getDistance();
    if (middleDistance <= stopDistance) {
      breaks();
     delay(100);
     stop(); 
     delay(200);     
     Beep();
     delay(300);
     back();
     delay(200);
     fbreaks();
     delay(250);
      stop();
      servo.write(15);
       delay(250);
      
      rightDistance = getDistance();

      servo.write(200);
      delay(250);
      leftDistance = getDistance();

      delay(250);
      servo.write(90);
      
      if(rightDistance > leftDistance) {
        Beep();
        delay(600);
        stop();
        delay(200);
        Beep();
        delay(200);
        sforward();
        delay(400);
        breaks();
        delay(200);
        sright();
        delay(100);
      }
        else if(rightDistance < leftDistance) {
        Beep();
        delay(600);
        stop();
        delay(200);
        Beep();
        delay(200);
        sforward();
        delay(400);
        breaks();
        delay(200);
        sleft();
        delay(100);
      }
      else if((rightDistance <= 50) || (leftDistance <= 50)) {
        back();
        delay(200);
        servo.write(0);
        delay(200);
        rightDistance = getDistance();
        delay(100);
        servo.write(180);
        delay(400);
        leftDistance = getDistance();
      if (rightDistance > leftDistance)  {
        sright();
      }
   else if (rightDistance < leftDistance) {
    sleft();                 }
   
      }
        
      
      else {
       forward();
      }
    }
    else {
        forward();
    }

servo.write(10);  //setservo position according to scaled value
    delay(200);
    rightDistance = getDistance();

    if(rightDistance <= sideDistance) {
      left();
      delay(170);
      forward();
    }
      
 

    servo.write(180);
      delay(300);
      leftDistance = getDistance();
    
    if(leftDistance <= sideDistance) {
      right();
      delay(170);
      forward();
      }


      servo.write(90);  //setservo position according to scaled value
    delay(325);
    middleDistance = getDistance();
    if (middleDistance <= cleanDistance) {
      breaks();
     delay(100);
     stop(); 
     delay(200);     
     Beep();
     delay(300);
     back();
     delay(150);
     fbreaks();
     delay(250);
      stop();
      servo.write(15);
       delay(250);
      
      rightDistance = getDistance();

      servo.write(200);
      delay(250);
      leftDistance = getDistance();

      delay(250);
      servo.write(90);
      
      if(rightDistance > leftDistance) {
        Beep();
        delay(600);
        stop();
        delay(200);
        Beep();
        delay(200);
        sforward();
        delay(400);
        breaks();
        delay(200);
        cleanright();
        delay(20);
        
      }
        else if(rightDistance < leftDistance) {
        Beep();
        delay(600);
        stop();
        delay(200);
        Beep();
        delay(200);
        sforward();
        delay(400);
        breaks();
        delay(200);
        cleanleft();
        delay(20);
        
      }
      else if((rightDistance <= 50) || (leftDistance <= 50)) {
        back();
        delay(100);
        servo.write(0);
        delay(100);
        rightDistance = getDistance();
        delay(100);
        servo.write(180);
        delay(300);
        leftDistance = getDistance();
      if (rightDistance > leftDistance)  {
        morefastright();
      }
   else if (rightDistance < leftDistance) {
    morefastleft();                 }
   
      }
        
      
      else {
       forward();
      }
    }
    else {
      forward();
    }

      
      servo.write(50);
      delay(300);
      rightDistance = getDistance();
      if (rightDistance <=turnDistance){
        left();
        delay(100);
        forward();
      }

      servo.write(130);
      delay(300);
      leftDistance = getDistance();
      if(leftDistance <= turnDistance){
        right();
        delay(100);
        forward();
      }
      
          
           
           
           
           
       
       
       // if(rightDistance > leftDistance) {
       // slowBack();
       // delay(600);
       // right();
     // }
      //else if(rightDistance < leftDistance) {
        //slowBack();
       // delay(400);
  //      right();
    //  }
      //else if((rightDistance <= 40) || (leftDistance <= 40)) {
   //     back();
  //      delay(500);
    //  }
   //   else {
    //    forward();
   //   }
   // }
    else {
        forward();
    }
   }
}









void faster_oa_mode() {
  if(func_mode == FasterOA){
 servo.write(90);  //setservo position according to scaled value
    delay(250);
    middleDistance = getDistance();
    if (middleDistance <= stopDistance) {
      breaks();
     delay(100);
     stop(); 
     delay(200);     
     Beep();
     delay(300);
     back();
     delay(200);
     fbreaks();
     delay(250);
      stop();
      servo.write(15);
       delay(250);
      
      rightDistance = getDistance();

      servo.write(200);
      delay(250);
      leftDistance = getDistance();

      delay(250);
      servo.write(90);
      
      if(rightDistance > leftDistance) {
        Beep();
        delay(600);
        stop();
        delay(200);
        Beep();
        delay(200);
        sforward();
        delay(400);
        breaks();
        delay(200);
        fright();
        delay(100);
      }
        else if(rightDistance < leftDistance) {
        Beep();
        delay(600);
        stop();
        delay(200);
        Beep();
        delay(200);
        sforward();
        delay(400);
        breaks();
        delay(200);
        fleft();
        delay(100);
      }
      else if((rightDistance <= 50) || (leftDistance <= 50)) {
        back();
        delay(200);
        servo.write(0);
        delay(200);
        rightDistance = getDistance();
        delay(100);
        servo.write(180);
        delay(400);
        leftDistance = getDistance();
      if (rightDistance > leftDistance)  {
        sright();
      }
   else if (rightDistance < leftDistance) {
    sleft();                 }
   
      }
        
      
      else {
       fastgo();
      }
    }
    else {
        fastgo();
    }

servo.write(10);  //setservo position according to scaled value
    delay(200);
    rightDistance = getDistance();

    if(rightDistance <= sideDistance) {
      left();
      delay(170);
      fastgo();
    }
      
 

    servo.write(180);
      delay(300);
      leftDistance = getDistance();
    
    if(leftDistance <= sideDistance) {
      right();
      delay(170);
      fastgo();
      }


      servo.write(90);  //setservo position according to scaled value
    delay(325);
    middleDistance = getDistance();
    if (middleDistance <= cleanDistance) {
      breaks();
     delay(100);
     stop(); 
     delay(200);     
     Beep();
     delay(300);
     back();
     delay(150);
     fbreaks();
     delay(250);
      stop();
      servo.write(15);
       delay(250);
      
      rightDistance = getDistance();

      servo.write(200);
      delay(250);
      leftDistance = getDistance();

      delay(250);
      servo.write(90);
      
      if(rightDistance > leftDistance) {
        Beep();
        delay(600);
        stop();
        delay(200);
        Beep();
        delay(200);
        sforward();
        delay(400);
        breaks();
        delay(200);
        cleanright();
        delay(20);
        
      }
        else if(rightDistance < leftDistance) {
        Beep();
        delay(600);
        stop();
        delay(200);
        Beep();
        delay(200);
        sforward();
        delay(400);
        breaks();
        delay(200);
        cleanleft();
        delay(20);
        
      }
      else if((rightDistance <= 50) || (leftDistance <= 50)) {
        back();
        delay(100);
        servo.write(0);
        delay(100);
        rightDistance = getDistance();
        delay(100);
        servo.write(180);
        delay(300);
        leftDistance = getDistance();
      if (rightDistance > leftDistance)  {
        morefastright();
      }
   else if (rightDistance < leftDistance) {
    morefastleft();                 }
   
      }
        
      
      else {
       fastgo();
      }
    }
    else {
      fastgo();
    }

      servo.write(50);
      delay(300);
      rightDistance = getDistance();
      if (rightDistance <=turnDistance){
        left();
        delay(100);
        fastgo();
      }

      servo.write(130);
      delay(300);
      leftDistance = getDistance();
      if(leftDistance <= turnDistance){
        right();
        delay(100);
       fastgo();
      }
    else {
        fastgo();
    }
   }
}

    
 void more_fast_mode() {
  if(func_mode == MoreFast){ 
servo.write(90);  //setservo position according to scaled value
    delay(250);
    middleDistance = getDistance();
    if (middleDistance <= stopDistance) {
      breaks();
     delay(100);
     stop(); 
     delay(200);     
     Beep();
     delay(300);
     back();
     delay(200);
     fbreaks();
     delay(250);
      stop();
      servo.write(15);
       delay(250);
      
      rightDistance = getDistance();

      servo.write(200);
      delay(250);
      leftDistance = getDistance();

      delay(250);
      servo.write(90);
      
      if(rightDistance > leftDistance) {
        Beep();
        delay(600);
        stop();
        delay(200);
        Beep();
        delay(200);
        sforward();
        delay(400);
        breaks();
        delay(200);
        morefastright();
        delay(100);
      }
        else if(rightDistance < leftDistance) {
        Beep();
        delay(600);
        stop();
        delay(200);
        Beep();
        delay(200);
        sforward();
        delay(400);
        breaks();
        delay(200);
        morefastleft();
        delay(100);
      }
      else if((rightDistance <= 50) || (leftDistance <= 50)) {
        back();
        delay(200);
        servo.write(0);
        delay(200);
        rightDistance = getDistance();
        delay(100);
        servo.write(180);
        delay(400);
        leftDistance = getDistance();
      if (rightDistance > leftDistance)  {
        morefastright();
      }
   else if (rightDistance < leftDistance) {
    morefastleft();                 }
   
      }
        
      
      else {
       morefast();
      }
    }
    else {
       morefast();
    }

servo.write(10);  //setservo position according to scaled value
    delay(200);
    rightDistance = getDistance();

    if(rightDistance <= sideDistance) {
      left();
      delay(170);
      forward();
    }
      
 

    servo.write(180);
      delay(300);
      leftDistance = getDistance();
    
    if(leftDistance <= sideDistance) {
      right();
      delay(170);
      forward();
      }

      servo.write(90);  //setservo position according to scaled value
    delay(325);
    middleDistance = getDistance();
    if (middleDistance <= cleanDistance) {
      breaks();
     delay(100);
     stop(); 
     delay(200);     
     Beep();
     delay(300);
     back();
     delay(150);
     fbreaks();
     delay(250);
      stop();
      servo.write(15);
       delay(250);
      
      rightDistance = getDistance();

      servo.write(200);
      delay(250);
      leftDistance = getDistance();

      delay(250);
      servo.write(90);
      
      if(rightDistance > leftDistance) {
        Beep();
        delay(600);
        stop();
        delay(200);
        Beep();
        delay(200);
        sforward();
        delay(400);
        breaks();
        delay(200);
        cleanright();
        delay(20);
        
      }
        else if(rightDistance < leftDistance) {
        Beep();
        delay(600);
        stop();
        delay(200);
        Beep();
        delay(200);
        sforward();
        delay(400);
        breaks();
        delay(200);
        cleanleft();
        delay(20);
        
      }
      else if((rightDistance <= 50) || (leftDistance <= 50)) {
        back();
        delay(100);
        servo.write(0);
        delay(100);
        rightDistance = getDistance();
        delay(100);
        servo.write(180);
        delay(300);
        leftDistance = getDistance();
      if (rightDistance > leftDistance)  {
        morefastright();
      }
   else if (rightDistance < leftDistance) {
    morefastleft();                 }
   
      }
        
      
      else {
       morefast();
      }
    }
    else {
      morefast();
    }


      servo.write(50);
      delay(300);
      rightDistance = getDistance();
      if (rightDistance <=turnDistance){
        left();
        delay(100);
        forward();
      }

      servo.write(130);
      delay(300);
      leftDistance = getDistance();
      if(leftDistance <= turnDistance){
        right();
        delay(100);
        forward();
      }
    else {
        morefast();
    }


    
  }
 }

 void battery_use_mode() {
  if(func_mode == BatteryUse){
    servo.write(90);  //setservo position according to scaled value
    delay(250);
    middleDistance = getDistance();
    if (middleDistance <= cleanDistance) {
      breaks();
     delay(100);
     stop(); 
     delay(200);     
     Beep();
     delay(300);
     back();
     delay(150);
     fbreaks();
     delay(250);
      stop();
      servo.write(15);
       delay(250);
      
      rightDistance = getDistance();

      servo.write(200);
      delay(250);
      leftDistance = getDistance();

      delay(250);
      servo.write(90);
      
      if(rightDistance > leftDistance) {
        Beep();
        delay(600);
        stop();
        delay(200);
        Beep();
        delay(200);
        sforward();
        delay(400);
        breaks();
        delay(200);
        cleanright();
        delay(20);
        
      }
        else if(rightDistance < leftDistance) {
        Beep();
        delay(600);
        stop();
        delay(200);
        Beep();
        delay(200);
        sforward();
        delay(400);
        breaks();
        delay(200);
        cleanleft();
        delay(20);
        
      }
      else if((rightDistance <= 50) || (leftDistance <= 50)) {
        back();
        delay(100);
        servo.write(0);
        delay(100);
        rightDistance = getDistance();
        delay(100);
        servo.write(180);
        delay(300);
        leftDistance = getDistance();
      if (rightDistance > leftDistance)  {
        morefastright();
      }
   else if (rightDistance < leftDistance) {
    morefastleft();                 }
   
      }
        
      
      else {
       cleanforward();
      }
    }
    else {
      cleanforward();
    }

servo.write(10);  //setservo position according to scaled value
    delay(200);
    rightDistance = getDistance();

    if(rightDistance <= cleanTurnDistance) {
      left();
      delay(170);
      cleanforward();
    }
      
 

    servo.write(180);
      delay(300);
      leftDistance = getDistance();
    
    if(leftDistance <= cleanTurnDistance) {
      right();
      delay(170);
      cleanforward();
      }



      servo.write(90);  //setservo position according to scaled value
    delay(325);
    middleDistance = getDistance();
    if (middleDistance <= cleanDistance) {
      breaks();
     delay(100);
     stop(); 
     delay(200);     
     Beep();
     delay(300);
     back();
     delay(150);
     fbreaks();
     delay(250);
      stop();
      servo.write(15);
       delay(250);
      
      rightDistance = getDistance();

      servo.write(200);
      delay(250);
      leftDistance = getDistance();

      delay(250);
      servo.write(90);
      
      if(rightDistance > leftDistance) {
        Beep();
        delay(600);
        stop();
        delay(200);
        Beep();
        delay(200);
        sforward();
        delay(400);
        breaks();
        delay(200);
        cleanright();
        delay(20);
        
      }
        else if(rightDistance < leftDistance) {
        Beep();
        delay(600);
        stop();
        delay(200);
        Beep();
        delay(200);
        sforward();
        delay(400);
        breaks();
        delay(200);
        cleanleft();
        delay(20);
        
      }
      else if((rightDistance <= 50) || (leftDistance <= 50)) {
        back();
        delay(100);
        servo.write(0);
        delay(100);
        rightDistance = getDistance();
        delay(100);
        servo.write(180);
        delay(300);
        leftDistance = getDistance();
      if (rightDistance > leftDistance)  {
        morefastright();
      }
   else if (rightDistance < leftDistance) {
    morefastleft();                 }
   
      }
        
      
      else {
       cleanforward();
      }
    }
    else {
      cleanforward();
    }




      servo.write(50);
      delay(300);
      rightDistance = getDistance();
      if (rightDistance <= cleanTurnDistance){
        left();
        delay(100);
        cleanforward();
      }

      servo.write(130);
      delay(300);
      leftDistance = getDistance();
      if(leftDistance <= cleanTurnDistance){
        right();
        delay(100);
        cleanforward();
      }
    else {
        cleanforward();
    }

  }
 }





 void battery_mode() {
  if(func_mode == Battery){
    right();
  }
 }





 void different_obstacle_mode() {
  if(func_mode == DifferentObstacle){
   { servo.write(90);  //setservo position according to scaled value
    delay(250);
    middleDistance = getDistance();
    if (middleDistance <= specialstopdistance) {
      breaks();
     delay(75);
     stop(); 
     delay(100);     
     back();
     delay(200);
     fbreaks();
     delay(100);
      stop();
      servo.write(15);
       delay(250);
      
      rightDistance = getDistance();

      servo.write(200);
      delay(250);
      leftDistance = getDistance();

      delay(250);
      servo.write(90);
      
      if(rightDistance > leftDistance) {
        Beep();
        delay(600);
        stop();
        delay(200);
        Beep();
        delay(200);
        sforward();
        delay(400);
        breaks();
        delay(200);
        right();
        delay(100);
      }
        else if(rightDistance < leftDistance) {
        Beep();
        delay(600);
        stop();
        delay(200);
        Beep();
        delay(200);
        sforward();
        delay(400);
        breaks();
        delay(200);
        left();
        delay(100);
      }
      else if((rightDistance <= 50) || (leftDistance <= 50)) {
        back();
        delay(200);
        servo.write(0);
        delay(200);
        rightDistance = getDistance();
        delay(100);
        servo.write(180);
        delay(400);
        leftDistance = getDistance();
      if (rightDistance > leftDistance)  {
        sright();
      }
   else if (rightDistance < leftDistance) {
    sleft();                 }
   
      }
        
      
      else {
       forward();
      }
    }
    else {
        forward();
    }
  }

    {
    {servo.write(10);
    delay(100);
rightDistance = getDistance();
for(pos = 0; pos < 10; pos +=1)  // goes from 0 degrees to 180 degrees
   {                                // in steps of 1 degree
    servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(specialdelay);                       // waits 15ms for the servo to reach the position
   }  //setservo position according to scaled value
    
    

    if(rightDistance <= specialDistance) {
      lessleft();
      delay(170);
      forward();
    }
    else {
        forward();
    }
    }
    {
rightDistance = getDistance();
for(pos = 10; pos < 20; pos +=1)  // goes from 0 degrees to 180 degrees
   {                                // in steps of 1 degree
    servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(specialdelay);                       // waits 15ms for the servo to reach the position
   }  //setservo position according to scaled value
    
    

    if(rightDistance <= specialDistance) {
      lessleft();
      delay(170);
      forward();
    }
    else {
        forward();
    }
    }
    {
rightDistance = getDistance();
for(pos = 20; pos < 30; pos +=1)  // goes from 0 degrees to 180 degrees
   {                                // in steps of 1 degree
    servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(specialdelay);                       // waits 15ms for the servo to reach the position
   }  //setservo position according to scaled value
    
    

    if(rightDistance <= specialDistance) {
      lessleft();
      delay(170);
      forward();
    }
    else {
        forward();
    }
    }
    {
rightDistance = getDistance();
for(pos = 30; pos < 40; pos +=1)  // goes from 0 degrees to 180 degrees
   {                                // in steps of 1 degree
    servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(specialdelay);                       // waits 15ms for the servo to reach the position
   }  //setservo position according to scaled value
    
    

    if(rightDistance <= specialDistance) {
      lessleft();
      delay(170);
      forward();
    }
    else {
        forward();
    }
    }
     {
rightDistance = getDistance();
for(pos = 40; pos < 50; pos +=1)  // goes from 0 degrees to 180 degrees
   {                                // in steps of 1 degree
    servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(specialdelay);                       // waits 15ms for the servo to reach the position
   }  //setservo position according to scaled value
    
    

    if(rightDistance <= specialDistance) {
      lessleft();
      delay(180);
      forward();
    }
    else {
        forward();
    }
    }
   
 { servo.write(180);



 { servo.write(90);  //setservo position according to scaled value
    delay(250);
    middleDistance = getDistance();
    if (middleDistance <= specialstopdistance) {
      breaks();
     delay(75);
     stop(); 
     delay(100);     
     back();
     delay(200);
     fbreaks();
     delay(100);
      stop();
      servo.write(15);
       delay(250);
      
      rightDistance = getDistance();

      servo.write(200);
      delay(250);
      leftDistance = getDistance();

      delay(250);
      servo.write(90);
      
      if(rightDistance > leftDistance) {
        Beep();
        delay(600);
        stop();
        delay(200);
        Beep();
        delay(200);
        sforward();
        delay(400);
        breaks();
        delay(200);
        right();
        delay(100);
      }
        else if(rightDistance < leftDistance) {
        Beep();
        delay(600);
        stop();
        delay(200);
        Beep();
        delay(200);
        sforward();
        delay(400);
        breaks();
        delay(200);
        left();
        delay(100);
      }
      else if((rightDistance <= 50) || (leftDistance <= 50)) {
        back();
        delay(200);
        servo.write(0);
        delay(200);
        rightDistance = getDistance();
        delay(100);
        servo.write(180);
        delay(400);
        leftDistance = getDistance();
      if (rightDistance > leftDistance)  {
        sright();
      }
   else if (rightDistance < leftDistance) {
    sleft();                 }
   
      }
        
      
      else {
       forward();
      }
    }
    else {
        forward();
    }
  }



  
 
leftDistance = getDistance();
    for(pos = 180; pos > 170; pos -=1)  // goes from 0 degrees to 180 degrees
   {                                // in steps of 1 degree
    servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(specialdelay);                       // waits 15ms for the servo to reach the position
   }
      
    
    if(leftDistance <= specialDistance) {
      lessright();
      delay(170);
      forward();
      }
 
 
    else {
        forward();
    }
 }
  {
leftDistance = getDistance();
    for(pos = 170; pos > 160; pos -=1)  // goes from 0 degrees to 180 degrees
   {                                // in steps of 1 degree
    servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(specialdelay);                       // waits 15ms for the servo to reach the position
   }
      
    
    if(leftDistance <= specialDistance) {
      lessright();
      delay(170);
      forward();
      }
 
 
    else {
        forward();
    }
 }
  {
leftDistance = getDistance();
    for(pos = 160; pos > 150; pos -=1)  // goes from 0 degrees to 180 degrees
   {                                // in steps of 1 degree
    servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(specialdelay);                       // waits 15ms for the servo to reach the position
   }
      
    
    if(leftDistance <= specialDistance) {
      lessright();
      delay(170);
      forward();
      }
 
 
    else {
        forward();
    }
 }
  {
leftDistance = getDistance();
    for(pos = 150; pos > 140; pos -=1)  // goes from 0 degrees to 180 degrees
   {                                // in steps of 1 degree
    servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(specialdelay);                       // waits 15ms for the servo to reach the position
   }
      
    
    if(leftDistance <= specialDistance) {
      lessright();
      delay(170);
      forward();
      }
 
 
    else {
        forward();
    }
 }
 {
leftDistance = getDistance();
    for(pos = 140; pos > 130; pos -=1)  // goes from 0 degrees to 180 degrees
   {                                // in steps of 1 degree
    servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(specialdelay);                       // waits 15ms for the servo to reach the position
   }
      
    
    if(leftDistance <= specialDistance) {
     lessright();
      delay(170);
      forward();
      }
 
 
    else {
        forward();
    }
 }
 {
leftDistance = getDistance();
    for(pos = 130; pos > 120; pos -=1)  // goes from 0 degrees to 180 degrees
   {                                // in steps of 1 degree
    servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(specialdelay);                       // waits 15ms for the servo to reach the position
   }
      
    
    if(leftDistance <= specialDistance) {
     lessright();
      delay(180);
      forward();
      }
 
 
    else {
        forward();
    }
 }


    }
  }}


void best_clean_mode() {
  if(func_mode == BestClean){
{ servo.write(90);  //setservo position according to scaled value
    delay(250);
    middleDistance = getDistance();
    if (middleDistance <= cleanDistance) {
      breaks();
     delay(75);
     stop(); 
     delay(100);     
     back();
     delay(200);
     fbreaks();
     delay(100);
      stop();
      servo.write(15);
       delay(250);
      
      rightDistance = getDistance();

      servo.write(200);
      delay(250);
      leftDistance = getDistance();

      delay(250);
      servo.write(90);
      
      if(rightDistance > leftDistance) {
        Beep();
        delay(600);
        stop();
        delay(200);
        Beep();
        delay(200);
        sforward();
        delay(400);
        breaks();
        delay(200);
        cleanright();
        delay(700);
      }
        else if(rightDistance < leftDistance) {
        Beep();
        delay(600);
        stop();
        delay(200);
        Beep();
        delay(200);
        sforward();
        delay(400);
        breaks();
        delay(200);
        cleanleft();
        delay(700);
      }
      else if((rightDistance <= 50) || (leftDistance <= 50)) {
        back();
        delay(200);
        servo.write(0);
        delay(200);
        rightDistance = getDistance();
        delay(100);
        servo.write(180);
        delay(400);
        leftDistance = getDistance();
      if (rightDistance > leftDistance)  {
        cleanright();
      }
   else if (rightDistance < leftDistance) {
    cleanleft();                 }
   
      }
        
      
      else {
       cleanforward();
      }
    }
    else {
        cleanforward();
    }
  }

    {
    {servo.write(10);
    delay(100);
rightDistance = getDistance();
for(pos = 0; pos < 10; pos +=1)  // goes from 0 degrees to 180 degrees
   {                                // in steps of 1 degree
    servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(specialdelay);                       // waits 15ms for the servo to reach the position
   }  //setservo position according to scaled value
    
    

    if(rightDistance <= cleanTurnDistance) {
      cleanleft();
      delay(170);
      cleanforward();
    }
    else {
        cleanforward();
    }
    }
    {
rightDistance = getDistance();
for(pos = 10; pos < 20; pos +=1)  // goes from 0 degrees to 180 degrees
   {                                // in steps of 1 degree
    servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(specialdelay);                       // waits 15ms for the servo to reach the position
   }  //setservo position according to scaled value
    
    

    if(rightDistance <= cleanTurnDistance) {
     cleanleft();
      delay(170);
      cleanforward();
    }
    else {
       cleanforward();
    }
    }
    {
rightDistance = getDistance();
for(pos = 20; pos < 30; pos +=1)  // goes from 0 degrees to 180 degrees
   {                                // in steps of 1 degree
    servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(specialdelay);                       // waits 15ms for the servo to reach the position
   }  //setservo position according to scaled value
    
    

    if(rightDistance <= cleanTurnDistance) {
     cleanleft();
      delay(170);
      cleanforward();
    }
    else {
        cleanforward();
    }
    }
    {
rightDistance = getDistance();
for(pos = 30; pos < 40; pos +=1)  // goes from 0 degrees to 180 degrees
   {                                // in steps of 1 degree
    servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(specialdelay);                       // waits 15ms for the servo to reach the position
   }  //setservo position according to scaled value
    
    

    if(rightDistance <= cleanTurnDistance) {
   cleanleft();
      delay(170);
      cleanforward();
    }
    else {
        cleanforward();
    }
    }
     {
rightDistance = getDistance();
for(pos = 40; pos < 50; pos +=1)  // goes from 0 degrees to 180 degrees
   {                                // in steps of 1 degree
    servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(specialdelay);                       // waits 15ms for the servo to reach the position
   }  //setservo position according to scaled value
    
    

    if(rightDistance <= cleanTurnDistance) {
      cleanleft();
      delay(180);
      cleanforward();
    }
    else {
        cleanforward();
    }
    }
   
 { servo.write(180);



 { servo.write(90);  //setservo position according to scaled value
    delay(250);
    middleDistance = getDistance();
    if (middleDistance <= cleanDistance) {
      breaks();
     delay(75);
     stop(); 
     delay(100);     
     back();
     delay(200);
     fbreaks();
     delay(100);
      stop();
      servo.write(15);
       delay(250);
      
      rightDistance = getDistance();

      servo.write(200);
      delay(250);
      leftDistance = getDistance();

      delay(250);
      servo.write(90);
      
      if(rightDistance > leftDistance) {
        Beep();
        delay(600);
        stop();
        delay(200);
        Beep();
        delay(200);
        sforward();
        delay(400);
        breaks();
        delay(200);
        cleanright();
        delay(700);
      }
        else if(rightDistance < leftDistance) {
        Beep();
        delay(600);
        stop();
        delay(200);
        Beep();
        delay(200);
        sforward();
        delay(400);
        breaks();
        delay(200);
        cleanleft();
        delay(700);
      }
      else if((rightDistance <= 50) || (leftDistance <= 50)) {
        back();
        delay(200);
        servo.write(0);
        delay(200);
        rightDistance = getDistance();
        delay(100);
        servo.write(180);
        delay(400);
        leftDistance = getDistance();
      if (rightDistance > leftDistance)  {
        cleanright();
      }
   else if (rightDistance < leftDistance) {
    cleanleft();                 }
   
      }
        
      
      else {
       cleanforward();
      }
    }
    else {
        cleanforward();
    }
  }



  
 
leftDistance = getDistance();
    for(pos = 180; pos > 170; pos -=1)  // goes from 0 degrees to 180 degrees
   {                                // in steps of 1 degree
    servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(specialdelay);                       // waits 15ms for the servo to reach the position
   }
      
    
    if(leftDistance <= cleanTurnDistance) {
    cleanright();
      delay(170);
      cleanforward();
      }
 
 
    else {
        cleanforward();
    }
 }
  {
leftDistance = getDistance();
    for(pos = 170; pos > 160; pos -=1)  // goes from 0 degrees to 180 degrees
   {                                // in steps of 1 degree
    servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(specialdelay);                       // waits 15ms for the servo to reach the position
   }
      
    
    if(leftDistance <= cleanTurnDistance) {
      cleanright();
      delay(170);
      cleanforward();
      }
 
 
    else {
        cleanforward();
    }
 }
  {
leftDistance = getDistance();
    for(pos = 160; pos > 150; pos -=1)  // goes from 0 degrees to 180 degrees
   {                                // in steps of 1 degree
    servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(specialdelay);                       // waits 15ms for the servo to reach the position
   }
      
    
    if(leftDistance <= cleanTurnDistance) {
   cleanright();
      delay(170);
      cleanforward();
      }
 
 
    else {
        cleanforward();
    }
 }
  {
leftDistance = getDistance();
    for(pos = 150; pos > 140; pos -=1)  // goes from 0 degrees to 180 degrees
   {                                // in steps of 1 degree
    servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(specialdelay);                       // waits 15ms for the servo to reach the position
   }
      
    
    if(leftDistance <= cleanTurnDistance) {
      cleanright();
      delay(170);
      cleanforward();
      }
 
 
    else {
        cleanforward();
    }
 }
 {
leftDistance = getDistance();
    for(pos = 140; pos > 130; pos -=1)  // goes from 0 degrees to 180 degrees
   {                                // in steps of 1 degree
    servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(specialdelay);                       // waits 15ms for the servo to reach the position
   }
      
    
    if(leftDistance <= cleanTurnDistance) {
     cleanright();
      delay(170);
      cleanforward();
      }
 
 
    else {
        cleanforward();
    }
 }
 {
leftDistance = getDistance();
    for(pos = 130; pos > 120; pos -=1)  // goes from 0 degrees to 180 degrees
   {                                // in steps of 1 degree
    servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(specialdelay);                       // waits 15ms for the servo to reach the position
   }
      
    
    if(leftDistance <= cleanTurnDistance) {
     cleanright();
      delay(180);
      cleanforward();
      }
 
 
    else {
       cleanforward();
    }
 }


    }
  }}

  
  

void setup() { 
  
  Serial.begin(9600);
  servo.attach(3,500,2400);// 500: 0 degree  2400: 180 degree
  
   servo.write(13);
   delay(400);
    
  for(pos = 13; pos < 200; pos +=1)  // goes from 0 degrees to 180 degrees
   {                                // in steps of 1 degree
    servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(6);                       // waits 15ms for the servo to reach the position
   }
   delay(200);
  for(pos = 180; pos>=13; pos-=1)     // goes from 180 degrees to 0 degrees
     {                            
    servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(2);                       // waits 15ms for the servo to reach the position
     }


  
  test1(); 
  delay(300);
  stop();
  delay(150);
  test2();
  delay(300);
 
  
  stop();
  delay(100);
  foreward();
  delay(75);
  breaks();
  delay(50);
  back();
  delay(75);
  fbreaks();
  delay(75);
  stop();
  left();
  delay(75);
  stop();
  delay(75);
  right();
  delay(75);
  stop();
  delay(100);
  {
    
  for(pos = 17; pos < 90; pos +=1)  // goes from 0 degrees to 90 degrees
  {                                 // in steps of 1 degree
    servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(20);                       // waits 15ms for the servo to reach the position
  }

  }

servo.write(90);
stop();


  irrecv.enableIRIn();
  pinMode(ECHO_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(LineTeacking_Pin_Right, INPUT);
  pinMode(LineTeacking_Pin_Middle, INPUT);
  pinMode(LineTeacking_Pin_Left, INPUT);
}

void loop() {
  getBTData();
  getIRData();
  bluetooth_mode();
  irremote_mode();
  obstacles_avoidance_mode();  
 faster_oa_mode();
 more_fast_mode();
 battery_use_mode();
 battery_mode();
 different_obstacle_mode();
 best_clean_mode();
 }
