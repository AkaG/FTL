#define l_engine_speed 5
#define r_engine_speed 6

#define l_engine_dir1 8
#define l_engine_dir2 9
#define r_engine_dir1 10
#define r_engine_dir2 11

#define changeColor 1

#define topSpeed 150
#define maxTurn 100

int sensorPorts[] = {12, 13, 14, 15, 16};
int sensorCount = 5;

int kp = 1;
int kd = 1;
int ki = 1;

int proportional = 0;
int derivative = 0;
int integral = 0;

int dir = 0;
int lastDir = 0;

void ride(int dir, int ftlSpeedl, int ftlSpeedr);
void readAndCalc();

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

// the loop function runs over and over again forever
void loop() {
  
  readAndCalc();
  
  derivative = dir - lastDir;
  integral += proportional;

  if(integral > topSpeed)
      integral = topSpeed;
  if(integral < -topSpeed)
      integral = -topSpeed;
  
  int turn = proportional*kp + derivative*kd + integral*ki;

   if(turn >= maxTurn)
      turn = maxTurn;
   if(turn <= -maxTurn)
      turn = -maxTurn;
   
   int speedL=0;
   int speedR=0;
  
   if(turn>=0){
     speedL=topSpeed;
     speedR=topSpeed - turn;
   }
   else{
     speedL=topSpeed + turn;
     speedR=topSpeed;
   }

   ride(1, speedL, speedR);

   lastDir = dir;
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

void readAndCalc(){
   int sum = 0;
   int posLeft = 10;
   int posRight = 10;
   
   for(int i = 0; i < sensorCount/2; i++){    //czytaj sensory po lewej
      sum += changeColor ^ digitalRead(sensorPorts[i]); // liczba aktywnych sensorow
      if((changeColor ^ digitalRead(sensorPorts[i])) == 1){ // ustal ktory jest aktywny
        posLeft = i - sensorCount/2;
      }
   }
   
   for(int i = sensorCount - 1; i >= sensorCount/2; i--){   //czytaj sensory po prawej
      sum += changeColor ^ digitalRead(sensorPorts[i]);
      if((changeColor ^ digitalRead(sensorPorts[i])) == 1){
        posRight = i - sensorCount/2 - 1;
      }
   }

/*   if(sum >= 3){
      sum = 2;
   }
*/
   if(sum == 0){ // jesli zaden czyjnik nie jest aktywny
      if(lastDir < 0){
        dir = -sensorCount;
      }else{
        dir = sensorCount;
      }
   }else{
       if((posLeft != 10) && (posRight != 10)){ // jesli sa aktywne po lewej i prawej
         dir = 0;
       }
       else{
          if(posRight != 10){
            dir = posRight*2 + sum;
          }
          else{
            if(posLeft != 10){
              dir = posLeft*2 - sum;
            }
          }
       }
   }

   proportional = dir;
}   
   


