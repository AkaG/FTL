
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

// the loop function runs over and over again forever
void loop() {
 ride(1,200,200);
 delay(1000);
 ride(2,200,200);
 delay(1000);
 ride(3,200,200);
 delay(1000);
 ride(4,200,200);
 delay(1000);
 ride(0,200,200);
 delay(1000);
 int sensor1Val = digitalRead(sensor1Port);
 int sensor2Val = digitalRead(sensor2Port);
 Serial.println(sensor1Val);
}
