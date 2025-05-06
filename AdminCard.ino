// C3 01 A2 0F //Tag Admin
const char* AdminTag = "C3 01 A2 0F";

void Admin(String x) {
  String lastIDTag;
  ResetEsp();
  if (x == AdminTag) {
    digitalWrite(Buzzer,1);
    delay(500);
    digitalWrite(Buzzer,0);
    delay(500);
    digitalWrite(Buzzer,1);
    delay(500);
    digitalWrite(Buzzer,0);
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("ADMIN MODE");
    ResetEsp();
    delay(4000);
    while (true) {
      
      File dataFile = SD.open("Member.txt", FILE_WRITE); // เปิดไฟล์สำหรับเขียน (ถ้าไฟล์ไม่มี จะสร้างไฟล์ใหม่)
      if (!dataFile) { // ตรวจสอบว่าสามารถเปิดไฟล์เพื่อเขียนได้หรือไม่
        Serial.println("Error opening Member.txt");
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("Error opening");
        lcd.setCursor(2, 1);
        lcd.print("Member.txt");
        ResetEsp();
        return;
      }
      
      if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
        String IDTag = rfid_read();
        Serial.println(">>>> " + IDTag);
        if(!SD.begin(SDcard)) {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Connect SD Card ");
        lcd.setCursor(2,1);
        lcd.print(">> Failed! <<");
        delay(4000);
        lcd.clear();
        lcd.setCursor(2,0);
        lcd.print(">> Restart <<");
        delay(3000);
        ESP.restart();
        }
        if (IDTag == AdminTag) {
          digitalWrite(Buzzer,1);
          delay(500);
          digitalWrite(Buzzer,0);
          delay(500);
          digitalWrite(Buzzer,1);
          delay(500);
          digitalWrite(Buzzer,0);
          lcd.clear();
          lcd.setCursor(3,0);
          lcd.print("ADMIN TAG");
          lcd.setCursor(3,1);
          lcd.print("DETECTED!");
          delay(2000);
          ResetEsp();
          lcd.clear();
          lcd.setCursor(3,0);
          lcd.print("-> RESET");
          for(int ii = 0;ii<=16;ii++){
          lcd.setCursor(ii,1);
          lcd.print("-");
          delay(100);
          }
          myFile.close();
          ESP.restart();
        }
        
        // ดู tag มีอยู่ในไฟล์ Member.txtไหม
        bool found = false;
        File existingTags = SD.open("Member.txt");
        while (existingTags.available()) {
          String line = existingTags.readStringUntil('\n');
          line.trim();
          if (line.indexOf(IDTag) != -1) {
            found = true;
            ResetEsp();
            break;
          }
        }
        existingTags.close();
        
        if (found) {
          lcd.clear();
          lcd.setCursor(1,0);
          for(int kk = 0;kk<=3;kk++){
            digitalWrite(Buzzer,1);
            delay(100);
            digitalWrite(Buzzer,0);
            delay(100);
          }
          lcd.print(">> TAG USED <<");
          lcd.setCursor(2, 1);
          lcd.print(":" + IDTag);
          delay(2000);
          lcd.clear();
          lcd.setCursor(3, 0);
          lcd.print("ADMIN MODE");
          ResetEsp();
        } else {
          
          // บันทึก tag ในไฟล์ Member.txt
          dataFile.print(IDTag);
          dataFile.print(",");
          dataFile.print("firstname");
          dataFile.print(",");
          dataFile.print("lastname");
          dataFile.print(",");
          dataFile.print("studentID");
          dataFile.println("");
          dataFile.close();
  
          lcd.clear();
          digitalWrite(Buzzer,1);
          delay(100);
          digitalWrite(Buzzer,0);
          lcd.setCursor(1, 0);
          lcd.print(":NEW TAG ADDED");
          lcd.setCursor(2, 1);
          lcd.print(":" + IDTag);
          delay(2000);
          ResetEsp();
          lcd.clear();
          lcd.setCursor(3, 0);
          lcd.print("ADMIN MODE");
        }
      }
      ResetEsp();
    }
  }
}

