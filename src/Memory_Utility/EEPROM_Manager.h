#ifndef EEPROM_MANAGER_H_
#define EEPROM_MANAGER_H_

#include <EEPROM.h>
#include <algorithm> // std::search
#include <string>
#include <stdint.h>
#include <WiFi.h>
#include "Memory_Manager.h"
#include "Users_Utility/User.h"

using std::count;
using std::search;
using std::string;

class EEPROM_Manager : public Memory_Manager{
  static const uint16_t EEPROM_USED_SIZE = 4096;
  static const uint8_t MAX_USERS_NUMBER = 40;
  static const uint8_t MAX_PERIPHERAL_NUM = 12;
  static const uint8_t MAX_EVENTS_NUM = 255;
  static const uint8_t MAX_ELEMENT_SIZE = 20;
  static const uint8_t PERIPHERAL_SIZE = 8;
  static const uint8_t USER_SIZE = 45;
  static const uint8_t EVENT_SIZE = 8;
  static const uint16_t IPs_START_ADDRESS_EEPROM = 0;
  static const uint16_t AP_SSID_EEPROM = 20;
  static const uint16_t AP_PASSWORD_EEPROM = (AP_SSID_EEPROM + MAX_ELEMENT_SIZE);           //40
  static const uint16_t WIFI_SSID_EEPROM = (AP_PASSWORD_EEPROM + MAX_ELEMENT_SIZE);         //60
  static const uint16_t WIFI_PASSWORD_EEPROM = (WIFI_SSID_EEPROM + MAX_ELEMENT_SIZE);       //80
  static const uint16_t INITIALIZATION_FLAGE = (WIFI_PASSWORD_EEPROM + MAX_ELEMENT_SIZE); //100
  static const uint16_t ESP_MODE_ADDRESS_EEPROM = (INITIALIZATION_FLAGE + 5);             //105
  static const uint16_t USERS_META_DATA_EEPROM = (ESP_MODE_ADDRESS_EEPROM + 5);             //110
  //META Data : 1 Byte [number of users regestered],
  //            5 Bytes [every bit represent every user location in memory is 0 empty 1 used]  for 40 user
  static const uint16_t PERIPHERALS_META_DATA_EEPROM = (USERS_META_DATA_EEPROM + 6);             //116
  //META Data : 1 Byte [number of peripherals regested],
  //            1.5 Bytes [every bit represent every peripheral location in memory is 0 empty 1 used] for 12 peripherals
  //            1.5 Bytes [every bit represent the type of peripheral if 0 io_peripheral or 1 serial_peripheral] for 12 peripherals
  static const uint16_t USERS_ARRAY_EEPROM = (PERIPHERALS_META_DATA_EEPROM + 4);                 //120
  // each user has 45 byte in memory 20 bytes for name, 1 byte 0, 20 bytes _password,
  //     1 byte 0, 1byte ID, 1 byte auth_level, 1 byte 0
  static const uint16_t PERIPHERALS_ARRAY_EEPROM = (USERS_ARRAY_EEPROM + 1800);                 //1920
  // each peripheral has 8 bytes in memory 1 byte for id, 1 byte for the type, 1 byte for input pin, 1 byte for output pin and 4 bytes for time length
  static const uint16_t EVENTES_META_DATA_EEPROM = (PERIPHERALS_ARRAY_EEPROM + 96);                 //2016
  static const uint16_t EVENTES_ARRAY_EEPROM = (EVENTES_META_DATA_EEPROM + 2);                 //2018
  // each event has 8 bytes in memory 2 bytes for id, 1 byte user id, 1 byte for output peripheral id, 1 byte for the type and 3 bytes for date & time
  //date and time : 24 hours -> 5 bits
  //                60 miniuts -> 6 bits
  //                31 days -> 5 bits
  //                12 month -> 4 bits
  //                15 years over 2018 -> 4 bits
public:
  EEPROM_Manager();
  ~EEPROM_Manager(){Serial.println("EEPROM Memory Instant Destroyed.");}
  virtual uint8_t initialize_memory() override;
  virtual IPAddress get_localIP() override;
  virtual IPAddress get_gatewayIP() override;
  virtual IPAddress get_subnetIP() override;
  virtual IPAddress get_primaryDNS() override;
  virtual IPAddress get_secondaryDNS() override;
  virtual void set_localIP(const IPAddress &) override;
  virtual void set_gatewayIP(const IPAddress &) override;
  virtual void set_subnetIP(const IPAddress &) override;
  virtual void set_primaryDNS(const IPAddress &) override;
  virtual void set_secondaryDNS(const IPAddress &) override;

  virtual std::string get_WIFI_SSID() override;
  virtual std::string get_WIFI_password() override;
  virtual void set_WIFI_SSID(const std::string &) override;
  virtual void set_WIFI_password(const std::string &) override;

  virtual std::string get_AP_SSID() override;
  virtual std::string get_AP_password() override;
  virtual void set_AP_SSID(const std::string &) override;
  virtual void set_AP_password(const std::string &) override;

  virtual uint8_t get_last_state() override;
  virtual void set_last_state(uint8_t state) override;

  virtual bool set_User(User& user) override;
  virtual bool delete_user(uint8_t id) override;
  virtual User* find_user(uint8_t id) override;
  virtual User* get_users() override;
  virtual int get_users_num() override;
  virtual uint8_t get_available_User_id() override;

  virtual uint8_t add_peripheral(int _peripheral_type, int _peripheral_rx, int _peripheral_tx, int _peripheral_time) override;
  virtual bool edit_peripheral(int _peripheral_type, int _peripheral_id, int _peripheral_rx, int _peripheral_tx, int _peripheral_time) override;
  virtual bool remove_peripheral(uint8_t _id) override;
  virtual Peripheral* get_peripheral(uint8_t _id) override;
  virtual Peripheral** get_peripherals() override;
  virtual uint8_t get_peripheral_Num() override;
  virtual uint16_t find_peripheral(uint8_t _id) override;
  virtual void add_event(Event& _peripheral) override;
  virtual Event* get_event(uint8_t _id) override;
  virtual Event** get_events() override;
  virtual uint16_t find_event(uint16_t _id) override;
private:
  uint8_t EEPROM_Read(uint16_t address);
  void EEPROM_Write(uint16_t address, uint8_t data);

  int EEPROM_Read_Integer(uint16_t address);
  void EEPROM_Write_Integer(uint16_t address, int data);

  void set_IPAdress(IPAddress ip, const uint16_t address );
  IPAddress get_IPAdress(const uint16_t address );

  void set_string(const std::string str, const uint16_t address, const uint8_t size);
  std::string get_string(const uint16_t address, const uint8_t size);

  void set_User(User& user, const uint16_t address);
  User* get_User(uint8_t index);
  uint16_t search_user_by_id(uint8_t id);
  uint8_t* get_users_IDs();

  void update_Users_Meta_Data(const uint16_t address, bool modeficationType);
  bool update_Peripheral_Meta_Data(const uint16_t address, bool modeficationType);
  uint16_t get_available_User_address();
  uint16_t get_available_Peripheral_address();
  uint8_t check_peripheral(int _peripheral_rx, int _peripheral_tx);
  uint16_t getEventAddress();
  Event* get_event_by_address(uint16_t _address);
  Peripheral* get_peripheral_by_address(uint16_t _address);
};

#endif /*EEPROM_MANAGER_H_*/
