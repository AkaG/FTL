
int l_engine_speed = 5;
int r_engine_speed = 6;

int l_engine_dir1 = 8;//1 high, 2 low R
int l_engine_dir2 = 9;//1 high, 2 low R
int r_engine_dir1 = 10;
int r_engine_dir2 = 11;

int sensor1Port = 12;
int sensor2Port = 13;

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

// the loop function runs over and over again forever
void loop() {
/* digitalWrite(l_engine_dir1, HIGH);
 digitalWrite(l_engine_dir2, LOW);

 digitalWrite(r_engine_dir1, HIGH);
 digitalWrite(r_engine_dir2, LOW);

 analogWrite(l_engine_speed, 255);
 analogWrite(r_engine_speed, 255);
 delay(1000);

 digitalWrite(l_engine_dir1, LOW);
 digitalWrite(l_engine_dir2, LOW);

 digitalWrite(r_engine_dir1, LOW);
 digitalWrite(r_engine_dir2, LOW);
 delay(1000);*/
 int sensor1Val = digitalRead(sensor1Port);
 Serial.println(sensor1Val);
}
