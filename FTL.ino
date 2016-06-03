#define l_engine_speed 10 //pin do kontroli predkosci lewego silnika
#define r_engine_speed 9 //pin do kontroli predkosci prawego silnika

#define l_engine_dir1 5 // pin do kontroli kierunku lewego silnika
#define l_engine_dir2 4 // pin do kontroli kierunku lewego silnika
#define r_engine_dir1 3 // pin do kontroli kierunku prawego silnika
#define r_engine_dir2 2 // pin do kontroli kierunku prawego silnika

#define changeColor 1 // zamienia wychwytywanie koloru czarnego z bialym

#define topSpeed 70 // max predkosc jazdy
#define maxTurn 70 // max spowolnienie jednego kola do skretu 

int sensorPorts[] = {8, 6, 7, 13, 12}; //porty sensorow
int sensorCount = 5; //liczba sensorow 

int kp = topSpeed/4; // wspolczynnik p
int kd = 40; // wspolczynnik d
int ki = 0; // wspolczynnik i
int kiMax = 100; //max wspolczynnika i

int kiJump = 1; //zwieksz wspolczynnik i o ten wspolczynnik
int kiSum = 0; // zmienna do liczenia sredniej z kilku wspoilczynnikow i
int kiDelay = 100; // opoznienie i
int kiLoop = 0; // zmienna do petli

int kdDelay = 350; // opoznienie d
int kdSum = 0; // zmienna do liczenia sredniej z kilku wspoilczynnikow d
int kdLoop = 0; // zmienna do petli

// kalibracja silnikow, jezeli mialyby nierowna moc
int kalibracjaL = topSpeed; 
int kalibracjaR = topSpeed;

// zwalnianie na zakretach o ulamek
int zwolnij_m = 1; // mnoznik
int zwolnij_d = 1; // dzielmik

//wartosci poczatkowe wspolczynnikow
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
 // wylicz p
  calcError();

 // wylicz d
  derivative = error - lastError;
  
// opoznienie i

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
  // wylicz skret
  int turn = proportional*kp + derivative*kd + integral*ki;

   if(turn >= maxTurn)
      turn = maxTurn;
   if(turn <= -maxTurn)
      turn = -maxTurn;
   
   int speedL=0;
   int speedR=0;

  //oblicz skret dla poszczegolnego kola
   if(turn>=0){
     speedL = topSpeed * kalibracjaL / topSpeed;
     speedR = ((topSpeed - turn)*kalibracjaR)/topSpeed;
   }
   else{
     speedL = ((topSpeed + turn)*kalibracjaL)/topSpeed;
     speedR = topSpeed * kalibracjaR / topSpeed;
   }

  // przekaz sterowanie na kola
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
   
// opoznienie d
   kdSum += error;
   if(kdLoop >= kdDelay){
      lastError = kdSum / kdDelay;
      kdLoop = 0;
      kdSum = 0;
   }
   kdLoop++;  
 
}

// funkcja do sterowania jazda
void ride(int dir, int ftlSpeedl, int ftlSpeedr){ //0-stop, 1-do przodu, 2-do tylu, 3-lewe kolo do przodu, prawe do tylu, 4-lewe kolo do tylu, prawe do przodu
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

//funkcja xor
int mxor(int a, int b){
  if( a==b ){
    return 0;
  }else{
    return 1;
  }
}

// wylicz odchylenie od prawidlowego kierunku jazdy i zapisz do p
void calcError(){
   int sum = 0;
   int pos = 0;
   
      if(mxor(changeColor, digitalRead(sensorPorts[0])) == 1){ // sprawdzenie sensora 1
        pos += -6;
        sum++;
      }
      if(mxor(changeColor, digitalRead(sensorPorts[1])) == 1){ // sprawdzenie sensora 2
        pos += -2;
        sum++;
      }
      if(mxor(changeColor, digitalRead(sensorPorts[2])) == 1){ // sprawdzenie sensora 3
        pos += 0;
        sum++;
      }
      if(mxor(changeColor, digitalRead(sensorPorts[3])) == 1){ // sprawdzenie sensora 4
        pos += 2;
        sum++;
      }
      if(mxor(changeColor, digitalRead(sensorPorts[4])) == 1){ // sprawdzenie sensora 5
        pos += 6;
        sum++;
      }
      
   if(sum == 0){ // jezli zaden sensor nie jest aktywny
      error = lastError;
   }else{
       error = pos/sum;
     }

   proportional = error;
}   
