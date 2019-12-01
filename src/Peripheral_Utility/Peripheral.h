#ifndef PERIPHERAL_H_
#define PERIPHERAL_H_
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Ticker.h>

typedef enum
{
    State_Enroll,
    State_Scan,
    State_Delete,
    State_Connected,
    State_Disconnected,
    State_Configured,
    State_Opening,
    State_Closing,
    State_Adjested_Open,
    State_Adjested_Close
} State_Peripheral;

class Peripheral{
public:
    static const uint8_t FINGERPRINT_TYPE;//0
    static const uint8_t ROLLINGDOOR_TYPE;//1
    uint8_t m_id;
    State_Peripheral m_state;
    uint8_t m_input_pin;
    uint8_t m_output_pin;
    uint8_t m_type;
    int m_fullTime_ms;
    Ticker m_timer;
    Peripheral(uint8_t _id, uint8_t _input_pin, uint8_t _output_pin, uint8_t _type, int _fullTime_ms):m_id(_id), m_input_pin(_input_pin), m_output_pin(_output_pin), m_type(_type), m_fullTime_ms(_fullTime_ms){}
    virtual ~Peripheral(){}
    virtual State_Peripheral Initialize_periphral() = 0;
    void print(){
        Serial.print("peripheral id = ");Serial.print(m_id);
        Serial.print(", Type = ");Serial.print(m_type);
        Serial.print(", Rx = ");Serial.print(m_input_pin);
        Serial.print(", Tx = ");Serial.print(m_output_pin);
        Serial.print(" and time = ");Serial.println(m_fullTime_ms);
    }
};

class Serial_Peripheral: public Peripheral{
public:
    HardwareSerial* m_HSerial;
    // SoftwareSerial* m_SSerial;
    uint8_t& m_RX_pin;
    uint8_t& m_TX_pin;
    Serial_Peripheral(uint8_t _id, uint8_t _RX_pin, uint8_t _TX_pin, uint8_t _type, int _fullTime_ms, HardwareSerial* _serial):Peripheral(_id, _RX_pin, _TX_pin, _type, _fullTime_ms), m_HSerial(_serial), m_RX_pin(m_input_pin), m_TX_pin(m_output_pin){}
    // Serial_Peripheral(uint8_t _serial_number){
    //     m_HSerial = new HardwareSerial(_serial_number);
    // }
};

class IO_Peripheral: public Peripheral{
public:
    uint8_t& m_open_pin;
    uint8_t& m_close_pin;
    IO_Peripheral(uint8_t _id, uint8_t _input_pin, uint8_t _output_pin, uint8_t _type, int _fullTime_ms):Peripheral(_id, _input_pin, _output_pin, _type, _fullTime_ms),
     m_open_pin(m_input_pin), m_close_pin(m_output_pin){}
    virtual State_Peripheral Initialize_periphral() = 0;
    virtual State_Peripheral get_Rolling_Door_State() = 0;
    virtual State_Peripheral open_door(uint8_t _percentage) = 0;
    virtual State_Peripheral close_door(uint8_t _percentage) = 0;
};
#endif
