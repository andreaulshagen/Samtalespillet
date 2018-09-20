/**

 * Spillets kode. Spillets logikk er skrevet av gruppen.

 RFID-lesing er fra eksternt bibliotek. Noen led-metoder er hentet fra FastLed.



 Krever Arduino, led-strip og rfid-leser.

 */



#include <SPI.h>

#include <MFRC522.h>

#include <FastLED.h>



// LED

#define NUM_LEDS 30

#define DATA_PIN 3



CRGB leds[NUM_LEDS];



// RFID

#define RST_PIN         9

#define SS_PIN          10



MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.



MFRC522::MIFARE_Key key;





int currCorrectAlternative = -1;

int score = 0;

boolean waitingForAnswer = false;

boolean gameHasStarted = false;



uint8_t hue = 0;





void setup() {

    Serial.begin(9600); // Initialize serial communications with the PC

    while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)

    SPI.begin();        // Init SPI bus

    mfrc522.PCD_Init(); // Init MFRC522 card



    // Prepare the key (used both as key A and as key B)

    // using FFFFFFFFFFFFh which is the default at chip delivery from the factory

    for (byte i = 0; i < 6; i++) {

        key.keyByte[i] = 0xFF;

    }



    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);





}



/**

 * Main loop.

 */

void loop() {



    if ( ! gameHasStarted) {

      // Spillet har ikke startet, lyser dermed ventelys i alle dioder

      fill_solid(leds, NUM_LEDS, CHSV(120,120,50));

      FastLED.show();



    }

    // RFID-kode vi ikke har skrevet selv, brukt fra RDID-library for MFRC522. Leter og scanner kort.

    // Look for new cards

    if ( ! mfrc522.PICC_IsNewCardPresent())

        return;



    // Select one of the cards

    if ( ! mfrc522.PICC_ReadCardSerial())

        return;



    byte sector         = 1;

    byte blockAddr      = 4;



    byte trailerBlock   = 7;

    MFRC522::StatusCode status;

    byte buffer[18];

    byte size = sizeof(buffer);



    // Authenticate using key A

    Serial.println(F("Authenticating using key A..."));

    status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));

    if (status != MFRC522::STATUS_OK) {

        Serial.print(F("PCD_Authenticate() failed: "));

        Serial.println(mfrc522.GetStatusCodeName(status));

        return;

    }





    // Read data from the block

    Serial.print(F("Reading data from block ")); Serial.print(blockAddr);

    Serial.println(F(" ..."));

    status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(blockAddr, buffer, &size);

    if (status != MFRC522::STATUS_OK) {

        Serial.print(F("MIFARE_Read() failed: "));

        Serial.println(mfrc522.GetStatusCodeName(status));

    }



    updateGame(buffer, 2); Serial.println();







    // Halt PICC

    mfrc522.PICC_HaltA();

    // Stop encryption on PCD

    mfrc522.PCD_StopCrypto1();

}





/**

 * Oppdaterer spillet når et kort scannes

 */





void updateGame(byte *buffer, byte bufferSize) {

  if (! gameHasStarted ) {

    // Fjern ventelyset gradvis.

    colorWipe(CRGB::Black, 100, 0);

  }

  gameHasStarted = true;

  // buffer[0] = 0 indikerer at det er et spørsmålskort,. buffer[1] er kortets alternativ.

  if ( waitingForAnswer && buffer[0] == 0) {

    int answer = buffer[1];



    if (answer == currCorrectAlternative) {



      createSuspense();

      correctAnswerBlink();

      score += 3;

      fillScore();

      if (score > 28) {

        // Spillet er ferdig, og paret har vunnet

        theaterChase(CRGB( 100, 100, 100), 30, 75);



        // Spillet restartes

        score = 0;

        gameHasStarted = false;

      }





    } else {

      createSuspense();

      wrongAnswerBlink();

      fillScore();



    }



    waitingForAnswer = false;



  }

  if (! waitingForAnswer) {

    if (buffer[0] > 0) {

      currCorrectAlternative = buffer[0];

      Serial.println("Scanned question card, correct alternative is: ");

      Serial.print(currCorrectAlternative);

      scannedQuestion();

      waitingForAnswer = true;

    }



  }



}



void createSuspense() {

  cylon(CRGB( 100, 100, 100), 3, 80);

}



void cylon(CRGB c, int width, int speed){

  // Noe modifisert fra eksempelkode på nettet..

  for(int i = NUM_LEDS-width; i >= 0; i--) {

    for(int j=0; j<width; j++){

      leds[i+j] = c;

    }

    FastLED.show();

    for(int j=0; j<width; j++){

      leds[i+j] = CRGB::Black;

    }



    delay(speed);

  }

}



void scannedQuestion() {

  for (int i = 0; i < 1; i++) {

    fill_solid(leds, NUM_LEDS, CHSV(hue, 120, 200));

    FastLED.show();

    delay(600);

    fill_solid(leds, NUM_LEDS, CHSV(0, 0, 0));



    FastLED.show();

    delay(150);

    fillScore();

  }





}



void wrongAnswerBlink() {

  for (int i = 0; i < 2; i++) {

    fill_solid(leds, NUM_LEDS, CRGB(255, 10, 20));

    FastLED.show();

    delay(200);

    fill_solid(leds, NUM_LEDS, CHSV(0, 0, 0));



    FastLED.show();

    delay(150);

  }



}



void correctAnswerBlink() {

  for (int i = 0; i < 2; i++) {

    fill_solid(leds, NUM_LEDS, CRGB(85, 255, 40));

    FastLED.show();

    delay(200);

    fill_solid(leds, NUM_LEDS, CHSV(0, 0, 0));



    FastLED.show();

    delay(150);

  }

}



void fillScore() {

  fill_solid(leds, score, CHSV(120, 120, 120));

  FastLED.show();

}



void theaterChase(CRGB c, int cycles, int speed){

  // Noe modifisert fra eksempelkode fra FastLED

  for (int j=0; j<cycles; j++) {

    for (int q=0; q < 3; q++) {

      for (int i=0; i < NUM_LEDS; i=i+3) {

        int pos = i+q;

        leds[pos] = c;    //turn every third pixel on

      }

      FastLED.show();



      delay(speed);



      for (int i=0; i < NUM_LEDS; i=i+3) {

        leds[i+q] = CRGB::Black;        //turn every third pixel off

      }



      delay(speed);



      for (int i=0; i < NUM_LEDS; i=i+3) {

        leds[i+q] = CHSV(hue, 120, 200);       //turn every third pixel off

      }



    }

  }

}



void colorWipe(CRGB c, int speed, int direction){

  for(int i=0; i<NUM_LEDS; i++){

    if(direction == 1){

      leds[i] = c;

    }

    else{

      leds[NUM_LEDS-1-i] = c;

    }

    FastLED.show();

    delay(speed);

  }

}
