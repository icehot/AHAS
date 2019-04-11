#include "App_MFRC522.h"
#include "App_IO.h"
#include "AHAS_Config.h"
#include <SPI.h>
#include <MFRC522.h>

#ifdef USE_MFRC522

MFRC522 mfrc522(PIN_MFRC522_CS_PIN, PIN_MFRC522_RST_PIN);   // Create MFRC522 instance.

MFRC522::MIFARE_Key key;

volatile boolean bNewInt = false;
byte regVal = 0x7F;
void activateRec(MFRC522 mfrc522);
void clearInt(MFRC522 mfrc522);


/**
* MFRC522 interrupt serving routine
*/
void readCard() {
    bNewInt = true;
}

/**
* Helper routine to dump a byte array as hex values to Serial.
*/
void dump_byte_array(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
}

/*
* The function sending to the MFRC522 the needed commands to activate the reception
*/
void activateRec(MFRC522 mfrc522) {
    mfrc522.PCD_WriteRegister(mfrc522.FIFODataReg, mfrc522.PICC_CMD_REQA);
    mfrc522.PCD_WriteRegister(mfrc522.CommandReg, mfrc522.PCD_Transceive);
    mfrc522.PCD_WriteRegister(mfrc522.BitFramingReg, 0x87);
}

/*
* The function to clear the pending interrupt bits after interrupt serving routine
*/
void clearInt(MFRC522 mfrc522) {
    mfrc522.PCD_WriteRegister(mfrc522.ComIrqReg, 0x7F);
}

/**
* Initialize.
*/
void init_MFRC522() {
    
	Serial.print(F("#INIT: MFRC522 => "));
 
    SPI.begin();          // Init SPI bus

    mfrc522.PCD_Init(); // Init MFRC522 card

	

                        /* read and printout the MFRC522 version (valid values 0x91 & 0x92)*/
    // Serial.print(F("Ver: 0x"));
    byte readReg = mfrc522.PCD_ReadRegister(mfrc522.VersionReg);
    //Serial.println(readReg, HEX);

    /* setup the IRQ pin*/
    pinMode(PIN_MFRC522_IRQ_PIN, INPUT_PULLUP);

    /*
    * Allow the ... irq to be propagated to the IRQ pin
    * For test purposes propagate the IdleIrq and loAlert
    */
    regVal = 0xA0; //rx irq
    mfrc522.PCD_WriteRegister(mfrc522.ComIEnReg, regVal);

    bNewInt = false; //interrupt flag

                     /*Activate the interrupt*/
    attachInterrupt(digitalPinToInterrupt(PIN_MFRC522_IRQ_PIN), readCard, FALLING);

    /*
    do { //clear a spourious interrupt at start
        ;
    } while (!bNewInt);
    */

    bNewInt = false;
    

    //Serial.println(F("End setup"));

	Serial.println(F("DONE"));
}

/**
* Main loop.
*/
void cyclicMFRC522() {
    if (bNewInt) { //new read interrupt
        Serial.print(F("#MFRC522: Interrupt. "));
        mfrc522.PICC_ReadCardSerial(); //read the tag data
                                       // Show some details of the PICC (that is: the tag/card)
        Serial.print(F("Card UID:"));
        dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
        Serial.println();

        clearInt(mfrc522);
        mfrc522.PICC_HaltA();
        bNewInt = false;
    }

    // The receiving block needs regular retriggering (tell the tag it should transmit??)
    // (mfrc522.PCD_WriteRegister(mfrc522.FIFODataReg,mfrc522.PICC_CMD_REQA);)
    activateRec(mfrc522);
} 


#endif


