#ifndef OPEN_CLOSE_DOOR_H_
#define OPEN_CLOSE_DOOR_H_
#include "Peripheral.h"

class Open_Close_Door: public IO_Peripheral{
public:
    Open_Close_Door(uint8_t _id, uint8_t _open_pin, uint8_t _close_pin, int _fullTime_ms): IO_Peripheral(_id, _open_pin, _close_pin, _fullTime_ms){
        connected = Initialize_periphral();
    }

    virtual bool Initialize_periphral(){
        pinMode(m_input_pin, OUTPUT);
        pinMode(m_output_pin, OUTPUT);
        return true;
    }

    void open_door(uint8_t _percentage){
        digitalWrite(m_input_pin, HIGH);   // turn the LED on (HIGH is the voltage level)
        int time = (m_fullTime_ms * _percentage / 100);
        //void(*closeFn)()= [this](){digitalWrite(m_input_pin, LOW);};
        m_timer.once_ms(time,Open_Close_Door::end_time_open, m_open_pin);
        // delay(1000);                       // wait for a second
        // digitalWrite(m_input_pin, LOW);    // turn the LED off by making the voltage LOW
        // delay(100);
    }

    void close_door(uint8_t _percentage){
        digitalWrite(m_output_pin, HIGH);   // turn the LED on (HIGH is the voltage level)
        int time = (m_fullTime_ms * _percentage / 100);
        //void(*closeFn)()= [this](){digitalWrite(m_close_pin, LOW);};
        m_timer.once_ms(time,Open_Close_Door::end_time_close, m_close_pin);
        // delay(1000);                       // wait for a second
        // digitalWrite(m_output_pin, LOW);    // turn the LED off by making the voltage LOW
        // delay(100);
    }

    static void end_time_open(uint8_t _open_pin){
        digitalWrite(_open_pin, LOW);
    }

    static void end_time_close(uint8_t _close_pin){
        digitalWrite(_close_pin, LOW);
    }
};
#endif
