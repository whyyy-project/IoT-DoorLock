#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN D2
#define RST_PIN D1

MFRC522 mfrc522(SS_PIN, RST_PIN); // Membuat objek MFRC522

String allowedIDs[] = {"f35c5e", "129d51c"}; // Array untuk menyimpan data ID yang diperbolehkan
const int numIDs = sizeof(allowedIDs) / sizeof(allowedIDs[0]); // Jumlah ID yang diperbolehkan

const int ledPin1 = D3; // Pin LED 1
const int ledPin2 = D4; // Pin LED 2
const int buzzer  = D0; 
const int relay   = D8; 

void setup() {
  Serial.begin(9600); // Inisialisasi komunikasi serial
  SPI.begin(); // Inisialisasi SPI bus
  mfrc522.PCD_Init(); // Inisialisasi MFRC522
  Serial.println("Scan RFID untuk mendapatkan ID...");

  pinMode(ledPin1, OUTPUT); // Atur pin LED 1 sebagai OUTPUT
  pinMode(ledPin2, OUTPUT); // Atur pin LED 2 sebagai OUTPUT
  pinMode(buzzer, OUTPUT); // Atur pin Buzzer sebagai OUTPUT
  pinMode(relay, OUTPUT); // Atur pin relay sebagai OUTPUT

}

void loop() {
  // Periksa apakah ada kartu RFID yang didekatkan
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // Baca ID kartu
    String uid = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      uid += String(mfrc522.uid.uidByte[i], HEX);
    }
    Serial.println("RFID terdeteksi! ID Kartu: " + uid);
    mfrc522.PICC_HaltA(); // Hentikan komunikasi kartu RFID

    // Periksa ID kartu dengan data ID yang diperbolehkan
    bool idMatched = false;
    for (int i = 0; i < numIDs; i++) {
      if (uid == allowedIDs[i]) {
        Serial.println("ID memiliki akses");
        idMatched = true;
        break;
      }
    }

    // Nyalakan LED sesuai dengan kecocokan ID
    if (idMatched) {
      digitalWrite(ledPin1, HIGH);
      digitalWrite(buzzer, HIGH);
      digitalWrite(relay, HIGH);
      delay(100);
      digitalWrite(buzzer, LOW);
      delay(100);
      digitalWrite(buzzer, HIGH);
      delay(100);
      digitalWrite(buzzer, LOW);
      delay(1000);
      digitalWrite(relay, LOW);
      digitalWrite(ledPin1, LOW);
    } else {
      digitalWrite(ledPin2, HIGH);
      digitalWrite(buzzer, HIGH);
      Serial.println("ID DITOLAK");
      delay(1000);
      digitalWrite(buzzer, LOW);
      digitalWrite(ledPin2, LOW);
    }
  }
}
