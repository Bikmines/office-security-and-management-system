#include <SD.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Keypad.h>
#include <Servo.h> 

#define SS_PIN 10
#define RST_PIN 9
 
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key; 

// Init array that will store new NUID 
byte nuidPICC[3];
byte card1[5] = {0xC4, 0x77, 0x08, 0x60};
byte card2[5] = {0x69, 0xAF, 0x8E, 0x45};


int CS_pin = 2;

char* password = "15"; 
char* password1 = "12";// change the password here, just pick any 3 numbers
int position = 0;
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
{'1','2','3','A'},
{'4','5','6','B'},
{'7','8','9','C'},
{'*','0','#','D'}
};

byte rowPins[ROWS] = { 5, 4, 3 };
byte colPins[COLS] = { 8, 7, 6 };
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

File file;
Servo myservo; 

void setup() { 
  Serial.begin(9600);
  myservo.attach(3);
//disable sd card
        pinMode(CS_pin, OUTPUT);
        digitalWrite(CS_pin, HIGH);

       
        // Disable RFID
        pinMode(SS_PIN, OUTPUT);
   // digitalWrite(SS_PIN, HIGH);
  
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 

  Serial.println("Initializing Card");

        if (!SD.begin(CS_pin)) {
           Serial.println("Card Failed");
           return; 
        }

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

   // Enable RFID
      // digitalWrite(SS_PIN, LOW); 

  Serial.println(F("This code scan the MIFARE Classsic NUID."));
  Serial.print(F("Using the following key:"));
  printHex(key.keyByte, MFRC522::MF_KEY_SIZE);
  LockedPosition(true);
  
}

void loop() { 
  char key = keypad.getKey();
if (key == '4')
{
position = 0;
LockedPosition(true);
}
if (key == password[position])
{
position ++;
}

if (key == password1[position])
{
position ++;
}

if (position == 2)
{
  Serial.println("You are");
LockedPosition(false);
}

 boolean card_1 = true;
    boolean card_2 = true;


  // Look for new cards
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;

 if (rfid.uid.uidByte[0] != nuidPICC[0] || 
    rfid.uid.uidByte[1] != nuidPICC[1] || 
    rfid.uid.uidByte[2] != nuidPICC[2] || 
    rfid.uid.uidByte[3] != nuidPICC[3] ) {
    Serial.println(F("A new card has been detected."));

    // Store NUID into nuidPICC array

    
    for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];

      if (nuidPICC[i] != card1[i]) card_1 = false;
      if (nuidPICC[i] != card2[i]) card_2 = false;
    }
   
    Serial.println(F("The NUID tag is:"));
    Serial.print(F("In hex: "));
    printHex(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();
    Serial.print(F("In dec: "));
    printDec(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();
  }
  else Serial.println(F("Card read previously."));

 if(card_1) {
    
    Serial.println("Hello! sir, you may enter ");
     // Disable RFID
        digitalWrite(SS_PIN, HIGH);
        
        //Enable SD Card
        digitalWrite(CS_pin, LOW);
        
        //pinMode(CS_pin,OUTPUT);
        file = SD.open("Sir.txt", FILE_WRITE);
         
      if (file) {
  
    Serial.print("Writing to sir.txt...");
    file.println("Sir attendence:");
   
    // close the file:
    
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening sir.txt");
  }

         
         file.close();
         // Disable SD Card
        digitalWrite(CS_pin, HIGH);
        
        //Enable RFID
        digitalWrite(SS_PIN, LOW);
        
  
    }
  
  else if(card_2) {
     
    Serial.println("Hello! madam,you may enter ");
     // Disable RFID
        digitalWrite(SS_PIN, HIGH);
        
        //Enable SD Card
        digitalWrite(CS_pin, LOW);

        //pinMode(CS_pin,OUTPUT);
        
        file = SD.open("Madam.txt", FILE_WRITE);
         
         if (file) {
 
    Serial.print("Writing to madam.txt...");
    file.println("madam attendence:");
   
   
    // close the file:

    Serial.println("done.");
  }
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening madam.txt");
  }

          file.close();
        
         // Disable SD Card
        digitalWrite(CS_pin, HIGH);
        
        //Enable RFID
        digitalWrite(SS_PIN, LOW);
        
   
   }
  
  else {Serial.println("you're not allowed");
  
  
  }




  Serial.println();
  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();


  
}


void LockedPosition(int locked)
{
if (locked)
{
Serial.println("Not allowed");
Serial.println();
}
else
{
Serial.println("Allowed");
Serial.println();
 myservo.write(90);
  delay(5000);
  myservo.write(11);
}
}

/**
 * Helper routine to dump a byte array as hex values to Serial. 
 */
void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

/**
 * Helper routine to dump a byte array as dec values to Serial.
 */
void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
}
