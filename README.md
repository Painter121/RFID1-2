# RFID Reader — Smart Access Control

มินิโปรเจกต์เครื่องอ่านและบันทึกข้อมูลบัตร RFID ด้วยบอร์ด NodeMCU ESP8266 ร่วมกับโมดูล RC522, จอแสดงผล LCD I2C, และการออกแบบโครงสร้างกล่องสำหรับติดตั้งอุปกรณ์

## การออกแบบชิ้นงาน 3D & แบบกล่องติดตั้ง

ออกแบบกล่องบรรจุอุปกรณ์พร้อมกำหนดตำแหน่งยึดบอร์ด หน้าจอ LCD I2C และโมดูล RFID RC522 ด้วยโปรแกรม SolidWorks

![แบบออกแบบเครื่องอ่าน RFID: Isometric, Top View, Right View และ Front View](docs/images/rfid-reader-mechanical-drawing.png)

- [เปิดแบบพิมพ์เขียวฉบับ PDF](Drawing-RFID%20Reader.pdf)
- [ดาวน์โหลดไฟล์ SolidWorks (.SLDDRW)](Drawing.SLDDRW)

## การต่อวงจรและการเชื่อมต่อฮาร์ดแวร์

อุปกรณ์หลักในวงจรประกอบด้วย:
- **MCU:** NodeMCU ESP8266 (Wi-Fi Enabled)
- **RFID:** RC522 Reader Module (SPI Interface)
- **Display:** LCD 16x2 with I2C Backpack
- **Storage:** MicroSD Card Module สำหรับบันทึก Log การแตะบัตรแบบออฟไลน์
- **Indicator:** Buzzer และไฟ LED แสดงสถานะการแตะบัตรผ่าน/ไม่ผ่าน

*(ดูผังการเดินสายวงจรฉบับเต็มได้ในไฟล์ [`wiring diagram.pptx`](wiring%20diagram.pptx))*

## โครงสร้างโค้ดในโปรเจกต์

| ไฟล์โค้ด (.ino) | หน้าที่การทำงาน |
|---|---|
| [`MainProject.ino`](MainProject.ino) | โปรแกรมหลัก จัดการการอ่านบัตร RFID, แสดงผลบน LCD, บันทึกลง SD Card และส่งข้อมูลผ่าน Wi-Fi |
| [`AdminCard.ino`](AdminCard.ino) | ระบบจัดการสิทธิ์และลงทะเบียนบัตร Master / Admin Card |
| [`Read.ino`](Read.ino) | โมดูลตรวจจับและอ่านค่า UID จากบัตร RFID |
