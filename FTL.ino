#define l_engine_speed 5
#define r_engine_speed 6

#define l_engine_dir1 8
#define l_engine_dir2 9
#define r_engine_dir1 10
#define r_engine_dir2 11

#define changeColor 1

#define topSpeed 130
#define maxTurn 100

int sensorPorts[] = {4, 2, 3, 13, 7};
int sensorCount = 5;

int kp = 10;
int kd = 0;
int ki = 0;

int proportional = 0;
int derivative = 0;
int integral = 0;

int error = 0;
int lastError = 0;

void ride(int dir, int ftlSpeedl, int ftlSpeedr);
void calcError();
int mxor(int a, int b);

// the setup function runs once when you press reset or power the board
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

  ride(1, 100, 100);
}

void loop() {
 
  calcError();
  
  derivative = error - lastError;
  integral += error;

  int turn = proportional*kp + derivative*kd + integral*ki;

   if(turn >= maxTurn)
      turn = maxTurn;
   if(turn <= -maxTurn)
      turn = -maxTurn;
   
   int speedL=0;
   int speedR=0;
  
   if(turn>=0){
     speedL = topSpeed;
     speedR = topSpeed - turn;
   }
   else{
     speedL = topSpeed + turn;
     speedR = topSpeed;
   }

   ride(1, speedL, speedR);

   lastError = error;
   
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
   
   for(int i = sensorCount/2 - 1; i >= 0; i--){    //czytaj sensory po lewej
      sum += mxor(changeColor, digitalRead(sensorPorts[i])); // liczba aktywnych sensorow
      if(mxor(changeColor, digitalRead(sensorPorts[i])) == 1){ // ustal ktory jest aktywny
        posLeft = i - sensorCount/2;
      }
   }
   
   for(int i = sensorCount/2 + (sensorCount % 2); i < sensorCount; i++){   //czytaj sensory po prawej
      sum += mxor(changeColor, digitalRead(sensorPorts[i]));
      if(mxor(changeColor, digitalRead(sensorPorts[i])) == 1){
        posRight = i - sensorCount/2;
      }
   }


   if(sum == 0){ // jesli zaden czyjnik nie jest aktywny
      if(lastError < 0){
        error = -sensorCount;
      }else{
        error = sensorCount;
      }
   }else{
       if((posLeft != 10) && (posRight != 10)){ // jesli sa aktywne po lewej i prawej
         error = 0;
       }
       else{
          if(posRight != 10){
            error = posRight;
          }
          else{
            if(posLeft != 10){
              error = posLeft;
            }
          }
       }
   }

    if(sensorCount % 2 == 1){
     if(mxor(changeColor, digitalRead(sensorPorts[sensorCount/2])) == 1){
      error = 0;
     }
   }
   
   proportional = error;
}   
   


