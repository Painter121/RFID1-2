
  //Wifi
const char* ssid = "@RMUTI-One-IoT"; //ssid ชื่อ 
const char* password = "IotRMUTI1"; //password รหัส 

//script ID ของ Spreadsheet
String GAS_ID = "AKfycbxSP4nyWTK8zSHbWncas7pyUskbRHR__LImFJyEukW5JvKF0RiaPcvDFotqAgF_bfZE"; 


#include <ESP8266WiFi.h> 
#include <WiFiClientSecure.h> 
#include <SPI.h> //
#include "MFRC522.h"
#include <LCD_I2C.h>
#include <SD.h>
File myFile;
//---------------------------------------------------------------------------------------
LCD_I2C lcd(0x27, 16, 2); 
#define ON_Board_LED 2 
#define RST_PIN  D3 //ขา RFID
#define SS_PIN  D4 // RFID
#define Buzzer D9 // buzzer
#define ResetButton D0 //ปุ่มกดรีเซท
#define SDcard D8 //cs
void sendData(String value, String value2, String value3/*, String value4*/);
//----------------------------------------Host & httpsPort
const char* host = "script.google.com";
const int httpsPort = 443;
//----------------------------------------
WiFiClientSecure client; //--> Create a WiFiClientSecure object.
void WiFistatus();
void RestartEsp();
MFRC522 mfrc522(SS_PIN, RST_PIN);
String rfid_in = "";
String lastRFID = "";
unsigned long lastTime = 0;
unsigned long timeLCD = 10000;
int Message = 0;
int WIFICURSOR = 0;
int SW = 0;

void setup() {
  Serial.begin(115200);
  pinMode(ResetButton,INPUT); // Button reset
  pinMode(Buzzer,OUTPUT); // buzzer 
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("");
    // LCD
    lcd.begin();                  // lcd.clear(); ล้างหน้าจอ
    lcd.backlight();              // เปิดไฟกำหนดความสว่างหน้าจอตั้งแต่0-255 มี noBacklight;

    //------------------------------------------ ตรวจสอบการใช้งาน SD Card
    /*lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("Connecting to");
    lcd.setCursor(3,1);
    lcd.print("SD card...");
    delay(3000);
    */
    
    if(!SD.begin(SDcard)) {
      lcd.clear();
      ResetEsp();
      lcd.setCursor(5,0);
      lcd.print("Enter");
      lcd.setCursor(2,1);
      lcd.print("Your SDcard");
      ResetEsp();
      while (true);
      delay(200);
    }
    lcd.clear();
    digitalWrite(Buzzer,1);
    delay(300);
    digitalWrite(Buzzer,0);
    delay(250);
    lcd.setCursor(4,0);
    lcd.print("Connect");
    lcd.setCursor(1,1);
    lcd.print(">> Success <<");
    ResetEsp();
    delay(3000);
   // เปิดไฟล์
   myFile = SD.open("Member.txt");


  //-----------------------------------------------รอการเชื่อมต่อกับ WIFI //WiFi.macAddress()
  //-----
    lcd.clear();
    lcd.setCursor(3,0);
    lcd.print("CONNECTING");
    lcd.setCursor(3,1);
    lcd.print(">> WIFI <<");
    delay(2000);
    ///
    delay(500);
    WiFi.begin(ssid, password); 
    lcd.clear();
    Serial.print("Connecting");
    lcd.setCursor(3,0);
    lcd.print("CONNECTING");
    while (WiFi.status() != WL_CONNECTED) {
      ResetEsp();
      Serial.print(".");
      lcd.setCursor(WIFICURSOR,1);
      lcd.print(".");
      WIFICURSOR = WIFICURSOR + 1;
        if(WIFICURSOR == 16){
          WIFICURSOR = 0;
          lcd.clear();
          lcd.setCursor(3,0);
          lcd.print("CONNECTING");
        }
      delay(300);
    }
   Serial.println(WiFi.macAddress());
   Serial.print("Successfully connected to : ");
   Serial.println(ssid);
   Serial.print("IP address: ");
   Serial.println(WiFi.localIP()); // คำสั่งแสดง ip ของ WIFI
   Serial.println();
  //แสดงออกทางจอ LCD 
   lcd.clear();
   digitalWrite(Buzzer,1);
   delay(300);
   digitalWrite(Buzzer,0);
   delay(250);
   lcd.setCursor(4,0);
   lcd.print("> SSID <");
   lcd.setCursor(0,1);
   lcd.print("->: ");
   lcd.print(ssid);
   delay(4000);
  //----------------------------------------
   client.setInsecure();
  //------------------------------------------
   
}

void loop(){

    unsigned long currentTime = millis();
    WiFistatus();
    ResetEsp();
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()){
      String rfid_in = rfid_read();
      Serial.println(">>>> " + rfid_in);
      if (rfid_in != lastRFID){ // ป้องกันการสแกนซ้ำ
      Admin(rfid_in);
      WhoseCard(rfid_in);
      lastRFID = rfid_in; 
      }
    }
    
    if (currentTime - lastTime >= timeLCD) { 
    lcd.clear();
    if (Message == 0) {
      lcd.setCursor(0,0);
      lcd.print(">>> CHECK-IN <<<");
      Message = 1;
    } else {
      for(int ll=0;ll<=16;ll++){
        lcd.setCursor(2, 0);
        lcd.print("-> Scan Card");
        lcd.setCursor(ll,1);
        lcd.print(".");
        delay(200);
        WiFistatus();
        ResetEsp();
        if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()){
          ll = 16;
          String rfid_in = rfid_read();
          Serial.println(">>>> " + rfid_in);
        if (rfid_in != lastRFID){
          Admin(rfid_in);
          WhoseCard(rfid_in); 
          lastRFID = rfid_in; 
        }
        }
      }
      Message = 0;
    }

    lastTime = currentTime;
  }
}


void output(String StudentID,String Firstname,String Lastname){ // ฟังชั่นการแสดงค่าและส่งข้อมูล
    lcd.clear();
    digitalWrite(Buzzer,1);
    lcd.setCursor(2,0);
    lcd.print(Firstname);
    lcd.setCursor(2,1);
    lcd.print(StudentID);
     Serial.println(StudentID);
     Serial.println(Firstname);
     Serial.println(Lastname);
    delay(200);
    digitalWrite(Buzzer,0);
    sendData(StudentID,Firstname,Lastname); //sendData(ข้อมูลที่จะส่ง1,ข้อมูลที่จะส่ง2,ข้อมูลที่จะส่ง3,ข้อมูลที่จะส่ง4)
    lcd.clear();
}

void WhoseCard (String x){ //เซ็ทว่าเป็นการ์ดของใคร
  String firstname="",lastname="",studentID="",RFID="";
  bool cardFound = false;
  // ค้นหาชื่อจากไฟล์
  myFile = SD.open("Member.txt", FILE_READ);
    if (myFile) {
      while (myFile.available()) { // line.indexOf หาตำแหน่งของข้อความที่ต้องการค้นหา
        String line = myFile.readStringUntil('\n'); // substring(From,To) form: ตำแหน่งเริ่มต้น To: คือตำแหน่งสิ้นสุด
          if (line.indexOf(x) != -1) { // 
            int pos1 = line.indexOf(',');
            RFID = line.substring(0, pos1);
            int pos2 = line.indexOf(',', pos1 + 1);
            firstname = line.substring(pos1 + 1, pos2);
            int pos3 = line.indexOf(',', pos2 + 1);
            lastname = line.substring(pos2 + 1, pos3);
            studentID = line.substring(pos3 + 1);
            studentID.trim();
            lastname.trim();
            firstname.trim();
            Serial.println("Firstname: " + firstname);
            Serial.println("Lastname: " + lastname);
            Serial.println("Student ID: " + studentID);
            output(studentID,firstname,lastname);
            cardFound = true;
          }
      }
      myFile.close();
  }else {
      int i = 1;
      do{
        digitalWrite(Buzzer,1);
        delay(500);
        digitalWrite(Buzzer,0);
        delay(500);
        i++;
      }while(i<=3);
      Serial.println("Error opening Member.txt");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Connect SD Card ");
      lcd.setCursor(2,1);
      lcd.print(">> Failed! <<");
      lastRFID = "";
      delay(4000);
      lcd.clear();
      lcd.setCursor(2,0);
      lcd.print(">> Restart <<");
      delay(3000);
      ESP.restart();
    } 
    delay(2000);
    if (!cardFound) { // ถ้าไม่พบการ์ด
      int i = 1;
      do{
        digitalWrite(Buzzer,1);
        delay(100);
        digitalWrite(Buzzer,0);
        delay(100);
        i++;
      }while(i<=4);
      Serial.println("No DATA");
      lcd.clear();
      lcd.setCursor(4,0);
      lcd.print("NO DATA!");
      lcd.setCursor(2,1);
      lcd.print("Who are you ?");
      delay(3000);
  }
}


String rfid_read() {
  String content = "";
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  return content.substring(1);
}

void sendData(String value, String value2, String value3) { //กำหนดตัวแปรที่จะส่งข้อมูล
  Serial.println("==========");
  Serial.print("connecting to ");
  Serial.println(host);
  //----------------------------------------//เชื่อมต่อกับ Host
  //----------------------------------------Process Data
  String string_StudentID = value; 
  String string_Firstname = value2;
  String string_Lastname = value3;
  String url = "/macros/s/" + GAS_ID + "/exec?StudentID=" + string_StudentID + "&Firstname=" + string_Firstname + "&Lastname=" + string_Lastname /*+ "&RFID=" + string_RFID*/; //  4 variables 

  Serial.print("requesting URL: ");
  Serial.println(url);

  // กรณีล้มเหลว
  if (!client.connect(host, httpsPort)) {
    int i = 1;
    do{
      digitalWrite(Buzzer,1);
      delay(100);
      digitalWrite(Buzzer,0);
      delay(100);
      i++;
    }while(i<=4);
    Serial.println("connection failed");
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("!!! Failed !!!");
    delay(2000);
    lcd.clear();
    return;
  }
   // ส่งข้อมูล GET ไปยัง URL ของ Google Apps Script
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
         "Host: " + host + "\r\n" +
         "User-Agent: BuildFailureDetectorESP8266\r\n" +
         "Connection: close\r\n\r\n");

  Serial.println("request sent");
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("Successfully");
  delay(2000);

  
/* 
  //---------------------------------------
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  String line = client.readStringUntil('\n');
  if (line.startsWith("{\"state\":\"success\"")) {
    Serial.println("esp8266 CI successfull!");
  } /*else {
    Serial.println("esp8266 CI has failed");
  }
  Serial.print("reply was : ");
  Serial.println(line);
  Serial.println("closing connection");
  Serial.println("==========");
  Serial.println();
  //----------------------------------------
*/  
} 

void WiFistatus(){
  if (WiFi.status() != WL_CONNECTED){
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("Internet");
    for(int jj=0;jj<=3;jj++){
       digitalWrite(Buzzer,1);
       delay(250);
       digitalWrite(Buzzer,0);
       delay(250);
    }
    lcd.setCursor(2,1);
    lcd.print("Disconnected");
    delay(3000);
    ESP.restart();
    return;
  }
}

void ResetEsp(){
  SW = digitalRead(ResetButton);
      if(SW == 1){
        lcd.clear();
        lcd.setCursor(3,0);
        lcd.print("-> RESET");
        for(int ii = 0;ii<=16;ii++){
          lcd.setCursor(ii,1);
          lcd.print("-");
          delay(100);
        }
          digitalWrite(Buzzer,1);
          delay(1500);
          digitalWrite(Buzzer,0);
          myFile.close();
          ESP.restart();
        }
}



