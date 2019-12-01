#ifndef FINGERPRINT_SCANNER_H_
#define FINGERPRINT_SCANNER_H_
#include "Peripheral.h"
#include <Adafruit_Fingerprint.h>

class Fingerprint_Scanner: public Serial_Peripheral{
public:
    Adafruit_Fingerprint finger;
    Fingerprint_Scanner(uint8_t _id, uint8_t _RX_pin, uint8_t _TX_pin, int _fullTime_ms, HardwareSerial* _serial):Serial_Peripheral(_id, _RX_pin, _TX_pin, Peripheral::FINGERPRINT_TYPE, _fullTime_ms, _serial)
    , finger(&(*m_HSerial)){
        m_state = Initialize_periphral();
    }

    virtual State_Peripheral Initialize_periphral(){
        finger.begin(57600);
        if (finger.verifyPassword()) {
        	Serial.println("Found fingerprint sensor!");
            return State_Connected;
        } else {
        	Serial.println("Did not find fingerprint sensor :(");
            return State_Disconnected;
        }
    }

    // returns -1 if failed, otherwise returns ID #
    int getFingerprintIDez() {
      uint8_t p = finger.getImage();
      if (p != FINGERPRINT_OK) {
        Serial.println("no finger on scanner");
        return -1;
      }
      p = finger.image2Tz();
      if (p != FINGERPRINT_OK) {
        Serial.println("couldn't convert the image");
        return -2;
      }
      p = finger.fingerFastSearch();
      if (p != FINGERPRINT_OK) {
        Serial.println("couldn't found that finger print");
        return -3;
      }
      // found a match!
      Serial.print("Found ID #");
      Serial.print(finger.fingerID);
      Serial.print(" with confidence of ");
      Serial.println(finger.confidence);
      return finger.fingerID;
    }

    uint8_t getFingerprintEnroll(uint8_t id) {
        int p = -1;
        Serial.print("Waiting for valid finger to enroll as #");
        Serial.println(id);
        while (p != FINGERPRINT_OK) {
            p = finger.getImage();
            switch (p) {
            case FINGERPRINT_OK:
                Serial.println("Image taken");
                break;
            case FINGERPRINT_NOFINGER:
                Serial.println(".");
                break;
            case FINGERPRINT_PACKETRECIEVEERR:
                Serial.println("Communication error");
                break;
            case FINGERPRINT_IMAGEFAIL:
                Serial.println("Imaging error");
                break;
            default:
                Serial.println("Unknown error");
                break;
            }
        }
        // OK success!
        p = finger.image2Tz(1);
        switch (p) {
        case FINGERPRINT_OK:
            Serial.println("Image converted");
            break;
        case FINGERPRINT_IMAGEMESS:
            Serial.println("Image too messy");
            return p;
        case FINGERPRINT_PACKETRECIEVEERR:
            Serial.println("Communication error");
            return p;
        case FINGERPRINT_FEATUREFAIL:
            Serial.println("Could not find fingerprint features");
            return p;
        case FINGERPRINT_INVALIDIMAGE:
            Serial.println("Could not find fingerprint features");
            return p;
        default:
            Serial.println("Unknown error");
            return p;
        }
        Serial.println("Remove finger");
        delay(2000);
        p = 0;
        while (p != FINGERPRINT_NOFINGER) {
            p = finger.getImage();
        }
        Serial.print("ID ");
        Serial.println(id);
        p = -1;
        Serial.println("Place same finger again");
        while (p != FINGERPRINT_OK) {
            p = finger.getImage();
            switch (p) {
            case FINGERPRINT_OK:
              Serial.println("Image taken");
              break;
            case FINGERPRINT_NOFINGER:
              Serial.print(".");
              break;
            case FINGERPRINT_PACKETRECIEVEERR:
              Serial.println("Communication error");
              break;
            case FINGERPRINT_IMAGEFAIL:
              Serial.println("Imaging error");
              break;
            default:
              Serial.println("Unknown error");
              break;
            }
        }
        // OK success!
        p = finger.image2Tz(2);
        switch (p) {
        case FINGERPRINT_OK:
            Serial.println("Image converted");
            break;
        case FINGERPRINT_IMAGEMESS:
            Serial.println("Image too messy");
            return p;
        case FINGERPRINT_PACKETRECIEVEERR:
            Serial.println("Communication error");
            return p;
        case FINGERPRINT_FEATUREFAIL:
            Serial.println("Could not find fingerprint features");
            return p;
        case FINGERPRINT_INVALIDIMAGE:
            Serial.println("Could not find fingerprint features");
            return p;
        default:
            Serial.println("Unknown error");
            return p;
        }
        // OK converted!
        Serial.print("Creating model for #");
        Serial.println(id);
        p = finger.createModel();
        if (p == FINGERPRINT_OK) {
            Serial.println("Prints matched!");
        } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
            Serial.println("Communication error");
            return p;
        } else if (p == FINGERPRINT_ENROLLMISMATCH) {
            Serial.println("Fingerprints did not match");
            return p;
        } else {
            Serial.println("Unknown error");
            return p;
        }
        Serial.print("ID ");
        Serial.println(id);
        p = finger.storeModel(id);
        if (p == FINGERPRINT_OK) {
            Serial.println("Stored!");
            Serial.print("p = ");
            Serial.println(p);
            return id;
        } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
            Serial.println("Communication error");
            return p;
        } else if (p == FINGERPRINT_BADLOCATION) {
            Serial.println("Could not store in that location");
            return p;
        } else if (p == FINGERPRINT_FLASHERR) {
            Serial.println("Error writing to flash");
            return p;
        } else {
            Serial.println("Unknown error");
            return p;
        }
    }

    uint8_t deleteFingerPrintById(uint8_t id) {
      uint8_t p = -1;
      p = finger.deleteModel(id);
      if (p == FINGERPRINT_OK) {
        Serial.println("Deleted!");
        return p;
      } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
        Serial.println("Communication error");
        return p;
      } else if (p == FINGERPRINT_BADLOCATION) {
        Serial.println("Could not delete in that location");
        return p;
      } else if (p == FINGERPRINT_FLASHERR) {
        Serial.println("Error writing to flash");
        return p;
      } else {
        Serial.print("Unknown error: 0x");
        Serial.println(p, HEX);
        return p;
      }
    }

    uint8_t deleteFingerPrintByToutsh() {
        //scan the user finger and get the user id
        //delete the user using the id from scan
        return 0;
    }
};

#endif
