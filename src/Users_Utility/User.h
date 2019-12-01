#ifndef USER_H_
#define USER_H_

#include <Arduino.h>
#include <stdint.h>
#include "Console_Utility/MyPrintable.h"

#define Logln(x) Serial.println(x)
#define Log(x) Serial.print(x)

class User {
	uint8_t m_id;//must be > 0
	const char* m_name;
	const char* m_password;
	uint8_t m_auth_level;//
	MyPrintable* m_printable;
public:
	static const uint8_t ADD_REMOVE_USERS = 1;
	static const uint8_t CHANGE_USERS_AUTHONTICATIONS = 2;
	static const uint8_t ADD_REMOVE_PERIPHERALS = 4;
	static const uint8_t USING_PERIPHERALS = 8;
	static const uint8_t MODIFY_WIFI_OR_AP_SETTINGS = 16;

	bool get_auth_to(uint8_t _service){
		return ((m_auth_level&_service)==_service)?true:false;
	}

	User(){
		m_id = ((uint8_t)0);
		char* temp = new char[strlen("dummy")+1];
	  strcpy(temp, "dummy");
		m_name = temp;
		temp = new char[strlen("dummy")+1];
		strcpy(temp, "dummy");
		m_password = temp;
		m_auth_level = ((uint8_t)0);
	  Log("user ");Log(m_name);Logln(" created with default parameters...");
	}

	User(uint8_t id, const char* name, const char* password, uint8_t auth_level): m_id(id), m_auth_level(auth_level) {
		char* temp = new char[strlen(name) + 1];
		strcpy(temp, name);
		m_name = temp;
		temp = new char[strlen(password)+1];
		strcpy(temp, password);
		m_password = temp;
		Log("user ");Log(m_name);Logln(" created with intialization parameter...");
	}

	User(const User& copy) : m_id(copy.m_id), m_auth_level(copy.m_auth_level) {
		Logln("copy constructor created");
		char* temp = new char[strlen(copy.m_name) + 1];
		strcpy(temp, copy.m_name);
		m_name = temp;
		temp = new char[strlen(copy.m_password)+1];
		strcpy(temp, copy.m_password);
		m_password = temp;
		/*
		unsigned int size = strlen(copy.m_name) + 1;
		m_name = (char*)malloc(size);
		memcpy(m_name, copy.m_name, size);
   		//Log("copied ");Logln(m_name);
		size = strlen(copy.m_password) + 1;
		m_password = (char*)malloc(size);
		memcpy(m_password, copy.m_password, size);*/
    	//Log("copied ");Logln(m_password);
	}

	~User() {
		delete[] m_name;
		delete[] m_password;
		Log("destroy ");Logln(m_name);
	}
	User& operator = (User& rhs){
		if(this != &rhs){
			this->set_id(rhs.get_id());
			this->set_name(rhs.get_name());
			this->set_password(rhs.get_password());
			this->set_auth_level(rhs.get_auth_level());
			Serial.print("user assignment ...");
		}
		return *this;
	}
	uint8_t get_id() {
		return m_id;
	}

	const char* get_name() {
		return m_name;
	}

	const char* get_password() {
		return m_password;
	}

	uint8_t get_auth_level() {
		return m_auth_level;
	}

	void set_id(uint8_t id) { m_id = id; }

	void set_name(const char* _name) {
		if(m_name != NULL)delete[] m_name;
		char* temp = new char[strlen(_name) + 1];
		strcpy(temp, _name);
		m_name = temp;
		// unsigned int size = strlen(str) + 1;
		// m_name = (char *)malloc(size);
		// memcpy(m_name, str, size);
	}

	void set_password(const char* _password) {
		if(m_password != NULL)delete[] m_password;
		char* temp = new char[strlen(_password) + 1];
		strcpy(temp, _password);
		m_password = temp;
		// unsigned int size = strlen(password) + 1;
		// m_password = (char*)malloc(size);
		// memcpy(m_password, password, size);
	}

	void set_auth_level(uint8_t auth_level) { m_auth_level = auth_level; }

	void set_printable(MyPrintable* printable) {
		m_printable = printable;
	}

	void print() {
		m_printable->print("user \""); m_printable->print(m_name);
		m_printable->print("\" has password \""); m_printable->print(m_password);
		m_printable->print("\", ID \""); m_printable->print(m_id);
		m_printable->print("\"and Auth Level \""); m_printable->print(m_auth_level); m_printable->println("\"");
	}
};
#endif //OCSYSTEM_OOP_USER_H
