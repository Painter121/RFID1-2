# RFID1-2 — RFID Reader

มินิโปรเจกต์วิชาแนะนำวิศวกรรม ปี 1 เทอม 2
ออกแบบเครื่องอ่านบัตร RFID โดยใช้ NodeMCU ESP8266 ร่วมกับ RC522 และจอ LCD I2C พร้อมแบบกล่องสำหรับติดตั้งอุปกรณ์

## การออกแบบชิ้นงาน

![แบบออกแบบเครื่องอ่าน RFID: Isometric, Top View, Right View และ Front View](docs/images/rfid-reader-mechanical-drawing.png)

แบบเขียนแสดงรูปทรงกล่อง ขนาดชิ้นงาน จุดยึด และตำแหน่งติดตั้ง LCD I2C, RFID RC522 และ NodeMCU ภายในกล่อง ภาพนี้แปลงจากไฟล์ PDF แบบเดิมของโปรเจกต์

[ดูภาพขนาดเต็ม](docs/images/rfid-reader-mechanical-drawing.png) · [เปิดแบบ PDF](Drawing-RFID%20Reader.pdf) · [ดาวน์โหลดแบบ SolidWorks](Drawing.SLDDRW)

## การต่อวงจร

แผนภาพการต่ออุปกรณ์อยู่ใน [wiring diagram.pptx](wiring%20diagram.pptx) เปิดด้วย PowerPoint เพื่อดูภาพและเส้นการเชื่อมต่อในแบบต้นฉบับ

อุปกรณ์ที่ปรากฏในแผนภาพ: NodeMCU ESP8266, RFID RC522, LCD I2C, SD card module, buzzer และปุ่มกด

## โค้ดในโปรเจกต์

| ไฟล์ | ส่วนของงาน |
|---|---|
| [MainProject.ino](MainProject.ino) | โปรแกรมหลัก เชื่อมต่อ RFID, LCD, SD card และ Wi-Fi |
| [AdminCard.ino](AdminCard.ino) | โค้ดส่วน AdminCard |
| [Read.ino](Read.ino) | โค้ดส่วนอ่านบัตร |
| [Sc code.txt](Sc%20code.txt) | โค้ดสคริปต์ที่เก็บร่วมกับโปรเจกต์ |

ไฟล์ `.txt` อื่นเก็บโค้ดทดลองและเวอร์ชันระหว่างพัฒนา ส่วนไฟล์ออกแบบต้นฉบับยังอยู่ใน repository ครบ
