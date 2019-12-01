#ifndef MEMORY_MANAGER_H_
#define MEMORY_MANAGER_H_

#include <string>
#include <stdint.h>
#include <WiFi.h>
#include "Users_Utility/User.h"
#include "Peripheral_Utility/Peripheral.h"
#include "System_Utility/Event.h"
#include "Peripheral_Utility/Fingerprint_Scanner.h"
#include "Peripheral_Utility/Rolling_Door.h"

class Memory_Manager
{
public:
  virtual uint8_t initialize_memory() = 0;
  virtual IPAddress get_localIP() = 0;
  virtual IPAddress get_gatewayIP() = 0;
  virtual IPAddress get_subnetIP() = 0;
  virtual IPAddress get_primaryDNS() = 0;
  virtual IPAddress get_secondaryDNS() = 0;
  virtual void set_localIP(const IPAddress &) = 0;
  virtual void set_gatewayIP(const IPAddress &) = 0;
  virtual void set_subnetIP(const IPAddress &) = 0;
  virtual void set_primaryDNS(const IPAddress &) = 0;
  virtual void set_secondaryDNS(const IPAddress &) = 0;

  virtual std::string get_WIFI_SSID() = 0;
  virtual std::string get_WIFI_password() = 0;
  virtual void set_WIFI_SSID(const std::string &) = 0;
  virtual void set_WIFI_password(const std::string &) = 0;

  virtual std::string get_AP_SSID() = 0;
  virtual std::string get_AP_password() = 0;
  virtual void set_AP_SSID(const std::string &) = 0;
  virtual void set_AP_password(const std::string &) = 0;

  virtual uint8_t get_last_state() = 0;
  virtual void set_last_state(uint8_t state) = 0;

  virtual bool set_User(User& user) = 0;
  virtual bool delete_user(uint8_t id) = 0;
  virtual User* find_user(uint8_t id) = 0;
  virtual User* get_users() = 0;
  virtual int get_users_num() = 0;
  virtual uint8_t get_available_User_id() = 0;

  virtual uint8_t add_peripheral(int _peripheral_type, int _peripheral_rx, int _peripheral_tx, int _peripheral_time) = 0;
  virtual bool edit_peripheral(int _peripheral_type, int _peripheral_id, int _peripheral_rx, int _peripheral_tx, int _peripheral_time) = 0;
  virtual bool remove_peripheral(uint8_t _id) = 0;
  virtual Peripheral* get_peripheral(uint8_t _id) = 0;
  virtual Peripheral** get_peripherals() = 0;
  virtual uint8_t get_peripheral_Num() = 0;
  virtual uint16_t find_peripheral(uint8_t _id) = 0;
  virtual void add_event(Event& _peripheral) = 0;
  virtual Event* get_event(uint8_t _id) = 0;
  virtual Event** get_events() = 0;
  virtual uint16_t find_event(uint16_t _id) = 0;
};
#endif /*MEMORY_MANAGER_H_*/
