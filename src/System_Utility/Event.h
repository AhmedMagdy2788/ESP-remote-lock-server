#ifndef EVENT_H_
#define EVENT_H_
#include <stdint.h>
#include <string>
#include "../Console_Utility/MyPrintable.h"

class DateTime{
    static const uint16_t start_year = 2018;
public:
    uint32_t m_date_and_time;

    DateTime(){}

    DateTime(uint32_t _date_and_time):m_date_and_time(_date_and_time){}

    DateTime(uint8_t _hour, uint8_t _minute, uint8_t _day, uint8_t _month, uint16_t _year):m_date_and_time(0){
        set_hour(_hour);
        set_minute(_minute);
        set_day(_day);
        set_month(_month);
        set_year(_year);
    }

    DateTime(std::string _date_and_time):m_date_and_time(0){
      //"dayName=%A,monthNum=%m,dayNum=%d,year=%Y,hour=%H,minute=%M"
      char* charptr_dayNum = get_Pair_Of(&_date_and_time, "dayNum=", ",");
      int dayNum = 0;
      sscanf(charptr_dayNum, "%d", &dayNum);
      set_day((uint8_t)dayNum);
      delete[] charptr_dayNum;
      //----------------------------------------------------------------------
      char* charptr_monthNum = get_Pair_Of(&_date_and_time, "monthNum=", ",");
      int monthNum = 0;
      sscanf(charptr_monthNum, "%d", &monthNum);
      set_month((uint8_t)monthNum);
      delete[] charptr_monthNum;
      //----------------------------------------------------------------------
      char* charptr_year = get_Pair_Of(&_date_and_time, "year=", ",");
      int year = 0;
      sscanf(charptr_year, "%d", &year);
      set_year((uint8_t)year);
      delete[] charptr_year;
      //----------------------------------------------------------------------
      char* charptr_hour = get_Pair_Of(&_date_and_time, "hour=", ",");
      int hour = 0;
      sscanf(charptr_hour, "%d", &hour);
      set_hour((uint8_t)hour);
      delete[] charptr_hour;
      //----------------------------------------------------------------------
      char* charptr_minute = get_Pair_Of(&_date_and_time, "minute=", ",");
      int minute = 0;
      sscanf(charptr_minute, "%d", &minute);
      set_minute((uint8_t)minute);
      delete[] charptr_minute;
    }

    void set_hour(uint8_t _hour){
        m_date_and_time &= 0xffffffe0;
        _hour &= 0x1f;
        m_date_and_time |= (_hour%24);
    }

    uint8_t get_hour(){
        return (uint8_t)(m_date_and_time&0x1f);
    }

    void set_minute(uint8_t _minute){
        m_date_and_time &= 0xfffff81f;
        _minute &= 0x3f;
        _minute %= 60;
        m_date_and_time |= _minute << 5;
    }

    uint8_t get_minute(){
        return (uint8_t)((m_date_and_time&(0x3f<<5))>>5);
    }

    void set_day(uint8_t _day){
        _day--;
        m_date_and_time &= 0xffff07ff;
        _day &= 0x1f;
        _day %= 31;
        m_date_and_time |= _day << 11;
    }

    uint8_t get_day(){
        return (uint8_t)((m_date_and_time&(0x1f<<11))>>11)+1;
    }

    void set_month(uint8_t _month){
        _month--;
        m_date_and_time &= 0xfff0ffff;
        _month &= 0xf;
        _month %= 12;
        m_date_and_time |= _month << 16;
    }

    uint8_t get_month(){
        return (uint8_t)((m_date_and_time&(0xf<<16))>>16)+1;
    }

    void set_year(uint16_t _year){
        _year -= start_year;
        m_date_and_time &= 0x000fffff;
        _year &= 0xf;
        m_date_and_time |= _year << 20;
    }

    uint16_t get_year(){
        return (start_year + (uint8_t)((m_date_and_time&(0xf<<20))>>20));
    }

    char* get_Pair_Of(std::string* statment, const char *key, const char *end_statment){
        std::size_t key_pos;
        std::size_t key_size;
        std::size_t end_pos;
        std::size_t pair_length;
        std::size_t pair_pos;
        char * pair_Value;
        Serial.println("");
        key_pos = statment->find(key);
        if (key_pos != std::string::npos){
            key_size = strlen(key);
            end_pos = statment->find(end_statment, key_pos + key_size);
            if (end_pos == std::string::npos){
                end_pos = statment->size();
                Serial.print("didn't found ");
                Serial.println(end_statment);
            }
            pair_length = end_pos - key_pos - key_size;
            pair_pos = key_pos + key_size;
            std::string str_pair_value = statment->substr(pair_pos, pair_length);

            pair_Value = new char [str_pair_value.length()+1];
            strcpy(pair_Value, str_pair_value.c_str());
            return pair_Value;
        }
        else{
            pair_Value = new char [strlen("_not_found_")+1];
            strcpy(pair_Value, "_not_found_");
            return pair_Value;
        }
    }

    void print(MyPrintable* _printable){
        _printable->print("Date = ");_printable->print((int)get_day());
        _printable->print("/"); _printable->print((int)get_month());
        _printable->print("/"); _printable->println((int)get_year());
        _printable->print("Time = "); _printable->print((int)get_hour());
        _printable->print(":"); _printable->println((int)get_minute());
    }
};

class Event{
public:
    uint16_t m_id;
    uint8_t m_user_id;
    uint8_t m_peripheral_id;
    uint8_t m_type;
    DateTime m_date_time;
    Event(){}
};
#endif
