#ifndef ROLLING_DOOR_H_
#define ROLLING_DOOR_H_
#include "Peripheral.h"

class Rolling_Door: public IO_Peripheral{
    static const int PERIOD = 500;
public:
    int m_current_ticks;
    int m_Requested_Ticks;
    int m_percentage;
    Rolling_Door(uint8_t _id, uint8_t _open_pin, uint8_t _close_pin, int _fullTime_ms): IO_Peripheral(_id, _open_pin, _close_pin, Peripheral::ROLLINGDOOR_TYPE, _fullTime_ms){
        m_state = Initialize_periphral();
    }

    virtual State_Peripheral Initialize_periphral(){
        pinMode(m_input_pin, OUTPUT);
        pinMode(m_output_pin, OUTPUT);
        //get_Rolling_Door_State();
        m_current_ticks = (m_fullTime_ms / PERIOD);
        Serial.print("current ticks = ");
        Serial.println(m_current_ticks);
        m_percentage = 100;
        Serial.print("Percentage = ");
        Serial.println(m_percentage);
        return State_Configured;
    }

    State_Peripheral get_Rolling_Door_State(){
        //read the door state from sensors
        return m_state;
    }

    State_Peripheral open_door(uint8_t _percentage){
        int Requested_Ticks = ((m_fullTime_ms / PERIOD)*_percentage)/100;
        Serial.print("Requested ticks = ");
        Serial.println(m_Requested_Ticks);
        if(m_state == State_Configured){
            if(Requested_Ticks > m_current_ticks){
                m_Requested_Ticks = Requested_Ticks;
                digitalWrite(m_input_pin, HIGH);
                m_timer.attach_ms(PERIOD,Rolling_Door::end_time_open, this);
                m_state = State_Opening;
            }
        }else if(m_state == State_Closing){
            if(Requested_Ticks >= m_current_ticks){
                m_Requested_Ticks = Requested_Ticks;
                m_state = State_Adjested_Open;
            }
        }else if(m_state == State_Opening){
            if(Requested_Ticks >= m_current_ticks){
                m_Requested_Ticks = Requested_Ticks;
            }
        }
        return m_state;
    }

    static void end_time_open(Rolling_Door* _rolling_door){
        (_rolling_door->m_current_ticks)++;
        _rolling_door->m_percentage = ((_rolling_door->m_current_ticks) * PERIOD * 100) /(_rolling_door->m_fullTime_ms);
        if(_rolling_door->m_state == State_Adjested_Close){
            digitalWrite(_rolling_door->m_input_pin, LOW);
            delay(100);
            digitalWrite(_rolling_door->m_close_pin, HIGH);
            _rolling_door->m_timer.detach();
            _rolling_door->m_timer.attach_ms(PERIOD,Rolling_Door::end_time_close, _rolling_door);
        }else if(_rolling_door->m_current_ticks >= _rolling_door->m_Requested_Ticks){
            digitalWrite(_rolling_door->m_open_pin, LOW);
            _rolling_door->m_state = State_Configured;
            _rolling_door->m_timer.detach();
            Serial.print("current ticks = ");
            Serial.println(_rolling_door->m_current_ticks);
            Serial.print("Percentage = ");
            Serial.println(_rolling_door->m_percentage);
        }
    }

    State_Peripheral close_door(uint8_t _percentage){
        Serial.print("the peripheral Full Time in ms = ");
        Serial.println(m_fullTime_ms);
        int Requested_Ticks = ((m_fullTime_ms / PERIOD)*_percentage)/100;
        Serial.print("Requested ticks = ");
        Serial.println(Requested_Ticks);
        if(m_state == State_Configured){
            if(Requested_Ticks < m_current_ticks){
                m_Requested_Ticks = Requested_Ticks;
                digitalWrite(m_output_pin, HIGH);
                m_state = State_Closing;
                m_timer.attach_ms(PERIOD,Rolling_Door::end_time_close, this);
            }
        }else if(m_state == State_Closing){
            if(Requested_Ticks <= m_current_ticks){
                m_Requested_Ticks = Requested_Ticks;
            }
        }else if(m_state == State_Opening){
            if(Requested_Ticks <= m_current_ticks){
                m_Requested_Ticks = Requested_Ticks;
                m_state = State_Adjested_Close;
            }
        }
        return m_state;
    }

    static void end_time_close(Rolling_Door* _rolling_door){
        (_rolling_door->m_current_ticks)--;
        _rolling_door->m_percentage = ((_rolling_door->m_current_ticks) * PERIOD * 100) /(_rolling_door->m_fullTime_ms);
        if(_rolling_door->m_state == State_Adjested_Open){
            digitalWrite(_rolling_door->m_close_pin, LOW);
            delay(100);
            digitalWrite(_rolling_door->m_input_pin, HIGH);
            _rolling_door->m_timer.detach();
            _rolling_door->m_timer.attach_ms(PERIOD,Rolling_Door::end_time_open, _rolling_door);
            _rolling_door->m_state == State_Opening;
        }else if(_rolling_door->m_current_ticks <= _rolling_door->m_Requested_Ticks){
            digitalWrite(_rolling_door->m_close_pin, LOW);
            _rolling_door->m_state = State_Configured;
            _rolling_door->m_timer.detach();
            Serial.print("current ticks = ");
            Serial.println(_rolling_door->m_current_ticks);
            Serial.print("Percentage = ");
            Serial.println(_rolling_door->m_percentage);
        }
    }
};
#endif
