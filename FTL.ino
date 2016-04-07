
int l_engine_speed = 5;
int r_engine_speed = 6;

int l_engine_dir1 = 8;
int l_engine_dir2 = 9;
int r_engine_dir1 = 10;
int r_engine_dir2 = 11;

int sensor1Port = 12;
int sensor2Port = 13;

int isBlack = 1;

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
  
  pinMode(l_engine_speed, OUTPUT);
  pinMode(r_engine_speed, OUTPUT);
  pinMode(l_engine_dir1, OUTPUT);
  pinMode(l_engine_dir2, OUTPUT);
  pinMode(r_engine_dir1, OUTPUT);
  pinMode(r_engine_dir2, OUTPUT);
  pinMode(sensor1Port, INPUT);
  pinMode(sensor2Port, INPUT);
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

int prev1, prev2 = 0;

// the loop function runs over and over again forever
void loop() {
 int sensor1Val = digitalRead(sensor1Port);
 int sensor2Val = digitalRead(sensor2Port);
 if(sensor1Val == 1 and sensor2Val == 1){
    ride(1,70,70);
 }else{
  if(sensor1Val == 1 and sensor2Val == 0){
    if(prev1 == 0 and prev2 == 0){
      ride(1,70,70);
      delay(200);
    }
    ride(1,40,70);
  }
  if(sensor1Val == 0 and sensor2Val == 1){
    if(prev1 == 0 and prev2 == 0){
      ride(1,70,70);
      delay(200);
    }
    ride(1,70,40);
  }
  if(sensor1Val == 0 and sensor2Val == 0){
    ride(0,100,100);
  }
 }
 prev1 = sensor1Val;
 prev2 = sensor2Val;
 delay(100);
}
