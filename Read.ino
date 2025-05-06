#include <SPI.h>
#include <MFRC522.h>
#include <SD.h>

#define RST_PIN  D3 // ขา RFID
#define SS_PIN  D4  // ขา RFID

MFRC522 mfrc522(SS_PIN, RST_PIN);
String lastRFID = "";

File myFile;

void setup() {
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.begin(115200);
  Serial.println("Initializing...");

  // เริ่มต้นการใช้งาน SD Card
  Serial.print("Initializing SD card...");
  if (!SD.begin(D0)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  // เปิดการอ่านไฟล์
  myFile = SD.open("Member.txt");
}

void loop() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    String rfid_in = rfid_read();
    Serial.println(">>>> " + rfid_in);

    // ค้นหาชื่อจากไฟล์
    if (myFile) {
      while (myFile.available()) {
        String line = myFile.readStringUntil('\n');
          if (line.indexOf(rfid_in) != -1) {
            int pos1 = line.indexOf(',');
            String firstname = line.substring(0, pos1);
            int pos2 = line.indexOf(',', pos1 + 1);
            String lastname = line.substring(pos1 + 1, pos2);
            String studentID = line.substring(pos2 + 1);
            Serial.println("Firstname: " + firstname);
            Serial.println("Lastname: " + lastname);
            Serial.println("Student ID: " + studentID);
      break;
    }
  }
  myFile.close();
  myFile = SD.open("Member.txt", FILE_READ);
} else {
  Serial.println("Error opening Member.txt");
}
  delay(2000);


  }

}

String rfid_read() {
  String content = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  return content.substring(1);
}
