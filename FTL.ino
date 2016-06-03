#define l_engine_speed 10 //analog pin controlling speed of left engine
#define r_engine_speed 9 //analog pin controlling speed of right engine

#define l_engine_dir1 5 // pin 1 controlling left engine
#define l_engine_dir2 4 // pin 2 controlling left engine
#define r_engine_dir1 3 // pin 1 controlling right engine
#define r_engine_dir2 2 // pin 2 controlling right engine

#define changeColor 1 // switch black and white

#define topSpeed 70
#define maxTurn 70

int sensorPorts[] = {8, 6, 7, 13, 12}; //ports number of sensors
int sensorCount = 5; 

int kp = topSpeed/4; // proportional ratio
int kd = 40; // derivative ratio
int ki = 0; // integral ratio
int kiMax = 100; //max integral

int kiJump = 1; //integral increment
int kiSum = 0; // avg integral
int kiDelay = 100; // integral delay
int kiLoop = 0; // loop

int kdDelay = 350; // derivative delay
int kdSum = 0; // avg derivative
int kdLoop = 0; // loop

// calibration of engines
int kalibracjaL = topSpeed; 
int kalibracjaR = topSpeed;

int zwolnij_m = 1;
int zwolnij_d = 1;

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

  kiSum += (error/2) * kiJump;
  if(kiLoop >= kiDelay){
    integral += kiSum / kiDelay;
    kiLoop = 0;
    kiSum = 0;
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
      if(turn == (-maxTurn)){
        ride(3,topSpeed+30,topSpeed);
      }else{
        if(turn == maxTurn){
          ride(4,topSpeed,topSpeed+30);
        }else{
          ride(1, speedL*zwolnij_m/zwolnij_d, speedR*zwolnij_m/zwolnij_d);
        }
      }
   }
   
// derivative delay
   kdSum += error;
   if(kdLoop >= kdDelay){
      lastError = kdSum / kdDelay;
      kdLoop = 0;
      kdSum = 0;
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
   int pos = 0;
   
      if(mxor(changeColor, digitalRead(sensorPorts[0])) == 1){ // which sensor is active
        pos += -6;
        sum++;
      }
      if(mxor(changeColor, digitalRead(sensorPorts[1])) == 1){ // which sensor is active
        pos += -2;
        sum++;
      }
      if(mxor(changeColor, digitalRead(sensorPorts[2])) == 1){ // which sensor is active
        pos += 0;
        sum++;
      }
      if(mxor(changeColor, digitalRead(sensorPorts[3])) == 1){ // which sensor is active
        pos += 2;
        sum++;
      }
      if(mxor(changeColor, digitalRead(sensorPorts[4])) == 1){ // which sensor is active
        pos += 6;
        sum++;
      }
      
   if(sum == 0){ // if no sensor is active
      error = lastError;
   }else{
       error = pos/sum;
     }

   proportional = error;
}   
