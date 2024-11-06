// LDR
const int ldr = A0;

// ULTRASONIC
const int trigPin = 6;
const int echoPin = 7;

// IR Sensors 
const int IR1  = A1 ;
const int IR2  = A2 ;
const int IR3  = A3 ;
const int IR4  = A4 ;
const int IR5  = A5 ;

// Motor 
const int enA = 10; 
const int enB = 11; 
const int in1 = 8;  //left backward 
const int in2 = 9 ;  //left forward
const int in3 = 12 ;  //right forward
const int in4 = 13;  //right backward

//ตัวแปรที่ที่ต้องใช้ในฟังก์ชันต่างๆ
unsigned long white_detect_startTime ;
unsigned long  stop_startTime ;
unsigned long  light_detect_startTime;
unsigned long  intersection_detected_startTime;
unsigned long  all_blackline_detected_mode_startTime;
bool intersection_detected = false;
long duration;
int distance;
long temp_light = 0;
int LDRvalue = 0 ;
int value_light_detect = 600;

//ตัวแปรควบคุมความเร็วของ มอเตอร์
int motorSpeed = 63;   
int default_motorSpeed = 63;



void setup() {
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(9600);
}

void loop() {
  distance = measureDistance();
  LDRvalue = analogRead(ldr);

  if (distance < 15) {
    stopMotors();
  } else {
    int sensor1 = analogRead(IR1) , sensor2 = analogRead(IR2) ,  sensor3 = analogRead(IR3) ,  sensor4 = analogRead(IR4) , sensor5 = analogRead(IR5);

    if(sensor1 < 600 && sensor2 < 650 && sensor3 < 800 && sensor4 < 600 && sensor5 < 650){
        stop_startTime = millis();
         while(millis() - stop_startTime < 8000){
            stopMotors();
            LDRvalue = analogRead(ldr);
            sensor1 = analogRead(IR1); sensor2 = analogRead(IR2); sensor3 = analogRead(IR3); sensor4 = analogRead(IR4);sensor5 = analogRead(IR5);
            if(LDRvalue < value_light_detect){
                 light_detect_startTime = millis();
                if(temp_light == 0){
                   temp_light = 1 ;
                   delay(2000);
                  while(millis() - light_detect_startTime < 3000){
                     LDRvalue = analogRead(ldr);
                     if(LDRvalue < value_light_detect){
                        temp_light = 2;
                     }
                 }
               }
            }
         }
         Serial.print(" temp_light : "); Serial.println(temp_light);
        if(temp_light == 0 ){
            moveBackward(motorSpeed);
            delay(500);
        }
         else if(temp_light == 1){
             turnLeftSuperSharp(motorSpeed);
             delay(1000);
             motorSpeed = motorSpeed - 5;
             intersection_detected_startTime = millis();
             intersection_detected = true ;
         }else if(temp_light == 2){
            turnRightSuperSharp(motorSpeed);
            delay(1000);
            motorSpeed = motorSpeed - 5;
            intersection_detected_startTime = millis();
            intersection_detected = true ;
       }
         temp_light = 0 ;
    }else if (sensor3 < 800 && sensor2 < 650 && sensor1 < 600) {
      sensor5 = analogRead(IR5); sensor4 = analogRead(IR4);
      while(sensor5 > 800 || sensor4 > 750){
        sensor5 = analogRead(IR5); sensor4 = analogRead(IR4);
        rotateRight(motorSpeed);
          if(sensor5 < 800 || sensor4 < 750) {
          return;
      }
    }
    }else if( sensor3 < 800 && sensor4 < 750 && sensor5 < 650){
         sensor1 = analogRead(IR5); sensor2 = analogRead(IR2);
      while(sensor1 > 600 || sensor2 > 650){
        sensor1 = analogRead(IR5); sensor2 = analogRead(IR2);
        rotateLeft(motorSpeed);
            if(sensor1 < 600 || sensor2 < 650) {
          return;
        }
      }
    }else if (sensor3 < 800 && sensor2 < 650) {
      turnLeftSuperSharp(motorSpeed);
    }else if( sensor3 < 800 && sensor4 < 750){
      turnRightSuperSharp(motorSpeed); 
    }else if( sensor3 < 800 && sensor5 < 800 && sensor2 < 650){
       moveForward(motorSpeed);
      all_blackline_detected_mode_startTime = millis();
      while(millis() - all_blackline_detected_mode_startTime < 1000){
         sensor1 = analogRead(IR1); sensor2 = analogRead(IR2); sensor3 = analogRead(IR3); sensor4 = analogRead(IR4);sensor5 = analogRead(IR5);
        if(sensor1 < 600 && sensor2 < 650 && sensor3 < 800 && sensor4 < 600 && sensor5 < 650){
          return;
      }
    }
      sensor1 = analogRead(IR1);
      while(sensor1 >= 800){
          sensor1 = analogRead(IR1);
          rotateRight(motorSpeed);
      }
      return;
    }else if( sensor3 < 800 && sensor1 < 800 && sensor4 < 750){
      moveForward(motorSpeed);
      all_blackline_detected_mode_startTime = millis();
      while(millis() - all_blackline_detected_mode_startTime < 1000){
              sensor1 = analogRead(IR1); sensor2 = analogRead(IR2); sensor3 = analogRead(IR3); sensor4 = analogRead(IR4);sensor5 = analogRead(IR5);
        if(sensor1 < 600 && sensor2 < 650 && sensor3 < 800 && sensor4 < 600 && sensor5 < 650){
          return;
      }}
      sensor5 = analogRead(IR5);
      while(sensor5 >= 800){
          sensor5 = analogRead(IR5);
          rotateLeft(motorSpeed);
      }
    }else if( sensor3 < 800 && sensor1 < 600){
      moveForward(motorSpeed);
      all_blackline_detected_mode_startTime = millis();
      while(millis() - all_blackline_detected_mode_startTime < 1000){
                sensor1 = analogRead(IR1); sensor2 = analogRead(IR2); sensor3 = analogRead(IR3); sensor4 = analogRead(IR4);sensor5 = analogRead(IR5);
        if(sensor1 < 600 && sensor2 < 650 && sensor3 < 800 && sensor4 < 600 && sensor5 < 650){
          return;
      }
    }
      sensor1 = analogRead(IR1);
      while(sensor1 >= 800){
          sensor1 = analogRead(IR1);
          rotateRight(motorSpeed);
      }
      return;
    }else if( sensor3 < 800 && sensor5 < 800){
      moveForward(motorSpeed);
      all_blackline_detected_mode_startTime = millis();
      while(millis() - all_blackline_detected_mode_startTime < 1000){
                sensor1 = analogRead(IR1); sensor2 = analogRead(IR2); sensor3 = analogRead(IR3); sensor4 = analogRead(IR4);sensor5 = analogRead(IR5);
        if(sensor1 < 600 && sensor2 < 650 && sensor3 < 800 && sensor4 < 600 && sensor5 < 650){
          return;
      }
    }
      sensor5 = analogRead(IR5);
      while(sensor5 >= 800){
          sensor5 = analogRead(IR5);
          rotateLeft(motorSpeed);
      }
      return;
    }else if (sensor3 < 800) {
      moveForward(motorSpeed);
    }
    else if (sensor2 < 650) {
      turnLeft(motorSpeed);
    } else if (sensor4 < 750) {
      turnRight(motorSpeed);
    } else if (sensor1 < 600) {
      turnLeftSharp(motorSpeed);
    } else if (sensor5 < 800) {
      turnRightSharp(motorSpeed); 
    }else if (sensor1 >= 600 && sensor2 >= 650 && sensor3 >= 800 && sensor4 >= 600 && sensor5 >= 650){
      white_detect_startTime = millis();
      while(millis() - white_detect_startTime <= 600){
         sensor1 = analogRead(IR1); sensor2 = analogRead(IR2); sensor3 = analogRead(IR3); sensor4 = analogRead(IR4);sensor5 = analogRead(IR5);
          Serial.println("White Detect mode (forward)");
          moveForward(motorSpeed);
          if(sensor1 < 600 || sensor2 < 650 || sensor3 < 800 || sensor4 < 600 || sensor5 < 650){
            return;
          }
        while(millis() - white_detect_startTime > 600){
         sensor1 = analogRead(IR1); sensor2 = analogRead(IR2); sensor3 = analogRead(IR3); sensor4 = analogRead(IR4);sensor5 = analogRead(IR5);
          Serial.println("White Detect mode (backward)");
          moveBackward(motorSpeed);
           if(sensor1 < 600 || sensor2 < 650 || sensor3 < 800 || sensor4 < 600 || sensor5 < 650){
            return;
            }
          }
        }
      }
    if(intersection_detected == true && millis() - intersection_detected_startTime > 15000){
      motorSpeed = default_motorSpeed;
      intersection_detected = false ;
    }
  }
  delay(50);
}

void moveForward(int speed) {
  analogWrite(enA, speed);
  analogWrite(enB, speed);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void moveBackward(int speed) {
  analogWrite(enA, speed);
  analogWrite(enB, speed);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void stopMotors() {
  analogWrite(enA, 0);
  analogWrite(enB, 0);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void turnLeft(int speed) {
  analogWrite(enA, speed );  
  analogWrite(enB, speed/2);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void turnRight(int speed) {
  analogWrite(enA, speed/2 );  
  analogWrite(enB,  speed);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void turnLeftSharp(int speed) {
  analogWrite(enA, speed ); 
  analogWrite(enB,  speed/3);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void turnRightSharp(int speed) {
  analogWrite(enA,  speed/3);
  analogWrite(enB, speed);  
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void turnRightSuperSharp(int speed) {
  analogWrite(enA, speed/3 );  
  analogWrite(enB,  speed+10);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void turnLeftSuperSharp(int speed) {
  analogWrite(enA, speed+10 ); 
  analogWrite(enB,  speed/3);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void rotateLeft(int speed) {
  analogWrite(enA, speed);  
  analogWrite(enB,  speed);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void rotateRight(int speed) {
  analogWrite(enA, speed);  
  analogWrite(enB,  speed);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

int measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  
  int distance = duration * 0.034 / 2;
  return distance;
}