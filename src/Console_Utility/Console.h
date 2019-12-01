#ifndef CONSOL_H_
#define CONSOL_H_

#include <Arduino.h>
#include "MyPrintable.h"

class Console: public MyPrintable{
public:
  Console(){Serial.println("Consol object created");}
  ~Console(){Serial.println("Consol object destroyed");}

  virtual void print(int data){
    Serial.print(data);
  }

  virtual void println(int data){
      Serial.println(data);
  }

  virtual void print(const char* data){
    Serial.print(data);
  }

  virtual void println(const char* data){
      Serial.println(data);
  }
};

#endif
