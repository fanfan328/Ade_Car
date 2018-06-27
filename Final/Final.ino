#define IRkiri_depan A5
#define IRkanan_depan A4
#define echoPinKiri 6
#define echoPinKanan 9
#define trigPinKiri 7
#define trigPinKanan 8
#define buzzer A0
#define button_jalan 12
#define button_counter 4
#define CLK 2
#define DIO 3

#define COLUMN 3
#define ROW 3
#include <Servo.h>
#include <TM1637Display.h>

Servo kiri, kanan;
/*----------------------------------------------------------*/
                     /*Deklarasi Variabel/
/*----------------------------------------------------------*/

int buttonPushCounter = 0;
int buttonState = 0;
int lastButtonState = 0;
int lastButtonJalan = 0;
int buttonJalan = 0;
int NoMeja;
int jarak;
long distance_kiri, distance_kanan;

TM1637Display display(CLK, DIO);
/*----------------------------------------------------------*/


void setup() {
  Serial.begin(9600);
  pinMode(IRkiri_depan, INPUT);
  pinMode(IRkanan_depan, INPUT);
  pinMode(button_counter, INPUT);
  pinMode(button_jalan, INPUT);
  pinMode(trigPinKiri, OUTPUT);
  pinMode(trigPinKanan, OUTPUT);
  pinMode(echoPinKiri, INPUT);
  pinMode(echoPinKanan, INPUT);
  display.setBrightness(0x0a);

}

/*----------------------------------------------------------*/
                     /*Aktivasi 7 Segment*/
/*----------------------------------------------------------*/
int counter_7segment() {
  buttonPushCounter = 0;
  buttonState = 0;
  lastButtonState = 0;
  lastButtonJalan = 0;
  buttonJalan = 0;
  while (buttonPushCounter < 10 && buttonJalan == LOW) {
    buttonState = digitalRead(button_counter);
    buttonJalan = digitalRead(button_jalan);
    // compare the buttonState to its previous state
    if (buttonState != lastButtonState) {
      if (buttonState == HIGH) {
        //Display angka counter
        buttonPushCounter++;
        display.showNumberDec(buttonPushCounter);
        // if the current state is HIGH then the button went from off to on:
//        Serial.print("Counter : ");
//        Serial.println(buttonPushCounter);
      }
      delay(50);
    }
    // save the current state as the last state, for next time through the loop
    lastButtonState = buttonState;
    //Tau Kapan jalan
    if (buttonJalan != lastButtonJalan) {
      if (buttonJalan == HIGH) {
//        Serial.println("Yuk Jalan");
        return buttonPushCounter;
      }
     }
      delay(50);
      if (buttonPushCounter == 9)buttonPushCounter = 0;
    }
    // save the current state as the last state, for next time through the loop
    lastButtonJalan = buttonJalan;
    
}

/*---------------Batas Kegiatan 7 segment---------------------*/



/*----------------------------------------------------------*/
                          /*Hitung Jarak*/
/*----------------------------------------------------------*/
bool cekjarak() {
//  distance_kiri, distance_kanan;
//  digitalWrite(trigPinKiri, LOW);
//  delayMicroseconds(2);digitalWrite(trigPinKiri, HIGH);
//  delayMicroseconds(10);digitalWrite(trigPinKiri, LOW);
//  distance_kiri = (pulseIn(echoPinKiri, HIGH, 23200) / 29) / 2;
////Serial.print("Jarak Kiri : ");Serial.println(distance_kiri);
//  
//  digitalWrite(trigPinKanan, LOW);
//  delayMicroseconds(2);digitalWrite(trigPinKanan, HIGH);
//  delayMicroseconds(10);digitalWrite(trigPinKanan, LOW);
//  distance_kanan = (pulseIn(echoPinKanan, HIGH, 23200) / 29) / 2;
////Serial.print("Jarak kanan : ");Serial.println(distance_kanan);
//  
//  Serial.print("Status : ");
//  while ((distance_kiri < 50 && distance_kiri>0) || (distance_kanan<50 && distance_kanan>0)) {
//    delayMicroseconds(2);digitalWrite(trigPinKiri, HIGH);
//    delayMicroseconds(10);digitalWrite(trigPinKiri, LOW);
//    distance_kiri = (pulseIn(echoPinKiri, HIGH, 23200) / 29) / 2;
// //Serial.print("Jarak Kiri : ");Serial.println(distance_kiri);
// 
//    delayMicroseconds(2);digitalWrite(trigPinKanan, HIGH);
//    delayMicroseconds(10);digitalWrite(trigPinKanan, LOW);
//    distance_kanan = (pulseIn(echoPinKanan, HIGH, 23200) / 29) / 2;
//  //Serial.print("Jarak kanan : ");Serial.println(distance_kanan);
//    analogWrite(buzzer, HIGH);
//  //Serial.println("USC Kiri atau Kanan tidak aman ! STOP !"); delay(1000);
//    return false;
//  }
//  analogWrite(buzzer, LOW);
//Serial.println("Aman dong");
return true;
}
/*---------------------Akhir hitung jarak---------------------------*/


/*----------------------------------------------------------*/
                          /*Jalan/Tidak*/
/*----------------------------------------------------------*/
void jalan(int jarak){
  if(jarak<50){
//    Serial.println("Stop");
    berhenti();
  }
  else if(jarak>50){
//    Serial.println("Jalan");
    aktif_servo();
    lurus();
  }
}
/*---------------------Akhir Kegiatan penentuan---------------------------*/



/*----------------------------------------------------------*/
                       /*Fungsi Bergerak*/
/*----------------------------------------------------------*/
void berhenti() {
//  Serial.println("STOP");
  kiri.detach();
  kanan.detach();
}
void belok_kiri() {
//    Serial.println("Maju kiri");
    while(digitalRead(IRkiri_depan) && !digitalRead(IRkanan_depan)){
      kanan.write(0);
      kiri.write(0);
    }
    return;
}
void belok_kanan() {
//    Serial.println("Maju kanan");
    while(!digitalRead(IRkiri_depan) && digitalRead(IRkanan_depan)){
      kanan.write(180);
      kiri.write(180);
    }
    return; 
}

void kanan_setpoint(){
//  Serial.println("SetPoint Belok");
  int counter=0;
  if(digitalRead(IRkiri_depan) && !digitalRead(IRkanan_depan))counter=1;
  while(true){
    kanan.write(180);
    kiri.write(180);
    if(!digitalRead(IRkiri_depan) && digitalRead(IRkanan_depan))break;
    //if(counter==2)break;
  }
  return;
 }

void lurus(){
//  Serial.println("Lurus");
  while (!digitalRead(IRkiri_depan) && !digitalRead(IRkanan_depan)){
    kanan.write(0);
    kiri.write(180);
    if(!digitalRead(IRkiri_depan) && digitalRead(IRkanan_depan))break;
    if(digitalRead(IRkiri_depan) && !digitalRead(IRkanan_depan))break;
  }
    if(!digitalRead(IRkiri_depan) && digitalRead(IRkanan_depan))belok_kanan();
    else if(digitalRead(IRkiri_depan) && !digitalRead(IRkanan_depan))belok_kiri();
return;
}

void lurus_node(){
    kanan.write(0);
    kiri.write(180);
    delay(300);
return;
}

void aktif_servo(){
  kiri.attach(10);
  kanan.attach(11);
//  Serial.println("Servo Aktif");
}
/*---------------------------Akhir Fungsi bergerak------------------------------------*/



void loop() {
   int per=0, pos=0, i=0, pulang=0;
   bool flag;
   char rutepergi[7] = "", rutepulang[7]="";
/*=============================================================*/
/*----------------------Menentukan Nomor Meja-----------------*/
/*=============================================================*/


   NoMeja=counter_7segment();
//   Serial.print("Nomor Meja tujuan anda :");
//   Serial.println(NoMeja);

/*=============================================================*/
/*------------------------PERGI-------------------------------*/
/*=============================================================*/
   //Menentukan rute pergi
   per=NoMeja;
   for(i=0; i<(per-1)/3; i++)rutepergi[pos++]='L';
      rutepergi[pos++]='B';
   for(i=0; i<(per-1)%3; i++)rutepergi[pos++]='M';
      rutepergi[pos++]='S';
//   Serial.print("Rute Pergi : ");
//   Serial.println(rutepergi);

      
  int x = 0;
// Serial.println(pos);   
  while(x < pos){
    //Serial.println("mulai masukpertama");
    while(cekjarak()){
      bool flag = cekjarak();
      //Serial.println(flag);
      //Serial.println("mulai masuk");
      aktif_servo();
      lurus();
      if (digitalRead(IRkiri_depan) && digitalRead(IRkanan_depan)){ 
//        Serial.println("SetPoint");
        char rute = rutepergi[x];
//        Serial.println(rute);
//        Serial.println(x);
        if(rutepergi[x]=='L')lurus_node();
        else if(rutepergi[x]=='M')lurus_node();
        else if(rutepergi[x]=='B')kanan_setpoint();
        else if(rutepergi[x]=='S')berhenti();
        x++;
        delay (2000);
        break;
   //     Serial.println("udah sampai ujung");
      }
    }
    berhenti();
  }
  berhenti();


/*=============================================================*/
/*------------------------PULANG-------------------------------*/
/*=============================================================*/


//  //Menentukan rute pulang
  pos=0;
  x=0;
  pulang = 10-per;
  for(i=0; i<(pulang-1)%3; i++)rutepulang[pos++]='M';
  rutepulang[pos++]='B';
  for(i=0; i<2-(pulang-1)/3; i++)rutepulang[pos++]='L';
  rutepulang[pos++]='S';
//  Serial.print("Rute Pulang : ");
//  Serial.println(rutepulang);
//  Serial.println(pos);
  buttonJalan = digitalRead(button_jalan);
  while(!buttonJalan){
//    Serial.println("Baca Button Pulang");
    buttonJalan=digitalRead(button_jalan);
  }
  
  if(buttonJalan){
      Serial.println("Yuk Pulang");
      while(x<pos){
          while(cekjarak()){
            aktif_servo();
            lurus();
              if (digitalRead(IRkiri_depan) && digitalRead(IRkanan_depan)){ //
//                  Serial.println("SetPoint Pulang");
                  char rute = rutepulang[x];
                  
                  if(rutepulang[x]=='L')lurus_node();
                  else if(rutepulang[x]=='M')lurus_node();
                  else if(rutepulang[x]=='B')kanan_setpoint();
                  else if(rutepulang[x]=='S')berhenti();
//                  Serial.println(rute);
//                  Serial.println(x);
                  x++;
                  delay (2000);
                  break;
         //     Serial.println("udah sampai ujung");
              }
         }
         berhenti();
     }
     berhenti();
  }
}
