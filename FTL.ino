#define l_engine_speed 9 //analog pin controlling speed of left engine
#define r_engine_speed 10 //analog pin controlling speed of right engine

#define l_engine_dir1 3 // pin 1 controlling left engine
#define l_engine_dir2 4 // pin 2 controlling left engine
#define r_engine_dir1 5 // pin 1 controlling right engine
#define r_engine_dir2 6 // pin 2 controlling right engine

#define changeColor 1 // switch black and white

#define topSpeed 150 
#define maxTurn 150

int sensorPorts[] = {8, 6, 7, 13, 12}; //ports number of sensors
int sensorCount = 5; 

int kp = maxTurn/2; // proportional ratio
int kd = kp/2; // derivative ratio
int ki = 1; // integral ratio
int kiMax = (kp*2)/3; //max integral

int kiJump = 5; //integral increment
int kiDelay = 50; // integral delay
int kiLoop = 0; // loop

int kdDelay = 50; // derivative delay
int kdLoop = 0; // loop

// calibration of engines
int kalibracjaL = topSpeed; 
int kalibracjaR = topSpeed;

//start values
int proportional = 0; 
int derivative = 0;
int integral = 0;

int error = 0;
int lastError = 0;

void ride(int dir, int ftlSpeedl, int ftlSpeedr);
void calcError();
int mxor(int a, int b);

void setup() {
  Serial.begin(9600);
  
  pinMode(l_engine_speed, OUTPUT);
  pinMode(r_engine_speed, OUTPUT);
  pinMode(l_engine_dir1, OUTPUT);
  pinMode(l_engine_dir2, OUTPUT);
  pinMode(r_engine_dir1, OUTPUT);
  pinMode(r_engine_dir2, OUTPUT);

  for(int i = 0; i < sensorCount; i++){
    pinMode(sensorPorts[i], INPUT);
  }

  ride(1, topSpeed * kalibracjaL / topSpeed, topSpeed * kalibracjaR / topSpeed);
}

void loop() {
 // calculate int, der
  calcError();
  
  derivative = error - lastError;
  
// integral delay
  if(kiLoop >= kiDelay){
    integral += error * kiJump;
    kiLoop = 0;
  }
  kiLoop++;

  if(integral >= kiMax){
    integral = kiMax;
  }
  if(integral <= -kiMax){
    integral = -kiMax;
  }
  // calculate turn
  int turn = proportional*kp + derivative*kd + integral*ki;

   if(turn >= maxTurn)
      turn = maxTurn;
   if(turn <= -maxTurn)
      turn = -maxTurn;
   
   int speedL=0;
   int speedR=0;

  //calculate turn to wheels
   if(turn>=0){
     speedL = topSpeed * kalibracjaL / topSpeed;
     speedR = ((topSpeed - turn)*kalibracjaR)/topSpeed;
   }
   else{
     speedL = ((topSpeed + turn)*kalibracjaL)/topSpeed;
     speedR = topSpeed * kalibracjaR / topSpeed;
   }

  // move turn to wheels
   if(proportional == 0){
      ride(1, speedL, speedR);
   }else{
      ride(1, speedL/2, speedR/2);
   }
   
// derivative delay
   if(kdLoop >= kdDelay){
      lastError = error;
      kdLoop = 0;
   }
   kdLoop++;   
}

void ride(int dir, int ftlSpeedl, int ftlSpeedr){ //0-stop, 1-forward, 2-backward, 3-FB, 4-BF
  if(dir == 0){
     digitalWrite(l_engine_dir1, LOW);
     digitalWrite(l_engine_dir2, LOW);

     digitalWrite(r_engine_dir1, LOW);
     digitalWrite(r_engine_dir2, LOW);
  }
  if(dir == 1){
    digitalWrite(l_engine_dir1, LOW);
    digitalWrite(l_engine_dir2, HIGH);

    digitalWrite(r_engine_dir1, LOW);
    digitalWrite(r_engine_dir2, HIGH);

    analogWrite(l_engine_speed, ftlSpeedl);
    analogWrite(r_engine_speed, ftlSpeedr);
  }
   if(dir == 2){
    digitalWrite(l_engine_dir1, HIGH);
    digitalWrite(l_engine_dir2, LOW);

    digitalWrite(r_engine_dir1, HIGH);
    digitalWrite(r_engine_dir2, LOW);

    
    analogWrite(l_engine_speed, ftlSpeedl);
    analogWrite(r_engine_speed, ftlSpeedr);
  }
   if(dir == 3){
    digitalWrite(l_engine_dir1, HIGH);
    digitalWrite(l_engine_dir2, LOW);

    digitalWrite(r_engine_dir1, LOW);
    digitalWrite(r_engine_dir2, HIGH);

    
    analogWrite(l_engine_speed, ftlSpeedl);
    analogWrite(r_engine_speed, ftlSpeedr);
  }
   if(dir == 4){
    digitalWrite(l_engine_dir1, LOW);
    digitalWrite(l_engine_dir2, HIGH);

    digitalWrite(r_engine_dir1, HIGH);
    digitalWrite(r_engine_dir2, LOW);

    
    analogWrite(l_engine_speed, ftlSpeedl);
    analogWrite(r_engine_speed, ftlSpeedr);
  }
}

int mxor(int a, int b){
  if( a==b ){
    return 0;
  }else{
    return 1;
  }
}

void calcError(){
   int sum = 0;
   int posLeft = 10;
   int posRight = 10;

   // left side
   for(int i = sensorCount/2 - 1; i >= 0; i--){    //read left sensors
      sum += mxor(changeColor, digitalRead(sensorPorts[i])); // number of active left side sensors
      if(mxor(changeColor, digitalRead(sensorPorts[i])) == 1){ // which sensor is active
        posLeft = i - sensorCount/2;
      }
   }

   //right side
   for(int i = sensorCount/2 + (sensorCount % 2); i < sensorCount; i++){   //read right sensors
      sum += mxor(changeColor, digitalRead(sensorPorts[i])); // number of active right side sensors
      if(mxor(changeColor, digitalRead(sensorPorts[i])) == 1){ // which sensor is active
        posRight = i - sensorCount/2;
      }
   }


   if(sum == 0){ // if no sensor are active
      if(lastError < 0){
        error = -sensorCount;
      }else{
        error = sensorCount;
      }
   }else{
       if((posLeft != 10) && (posRight != 10)){ // if sensors are active on both sides
         error = 0;
       }
       else{
          if(posRight != 10){ //if sensors are active on right side
            error = posRight;
          }
          else{
            if(posLeft != 10){ //if sensors are active on left side
              error = posLeft;
            }
          }
       }
   }

    if(sensorCount % 2 == 1){ //if center sensor is active go straight on
     if(mxor(changeColor, digitalRead(sensorPorts[sensorCount/2])) == 1){
      error = 0;
     }
   }
   
   proportional = error;
}   
   


