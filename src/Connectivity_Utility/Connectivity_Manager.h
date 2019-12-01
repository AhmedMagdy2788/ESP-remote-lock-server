#ifndef CONNECTIVITY_MANAGER_H_
#define CONNECTIVITY_MANAGER_H_

#include <WiFi.h>
#include <Ticker.h>
#include "WIFI_Profile.h"
#include "Connectivity_Observer.h"
class Connectivity_Manager{
public:
  static const uint8_t AP_MODE = 0;
  static const uint8_t WIFI_MODE = 1;
  static const uint8_t HYPER_MODE = 2;
  static const uint8_t OFF_MODE = 3;
  WIFI_Profile* m_wifi_profile;
private:
  Ticker m_connectivity_timer;
  uint8_t m_Connection_Mode; // 0->AP, 1->wifi and 2->wifi&ap
  Connectivity_Observer* m_Observer;
  char *m_AP_SSID;
  char *m_AP_password;

  Connectivity_Manager();
public:
  Connectivity_Manager(Connectivity_Observer*, WIFI_Profile *, const char *, const char *, uint8_t);
  ~Connectivity_Manager();

  inline uint8_t get_Connectivity_Mode() { return m_Connection_Mode; }
  void set_Connectivity_Mode(uint8_t connectivity_Mode) { m_Connection_Mode = connectivity_Mode; }

  bool add_WIFI_Profile(WIFI_Profile *wifi_profile);
  bool connect_to_WIFI();
  bool is_WIFI_Connected();
  bool is_WIFI_Available();
  bool enable_WIFI_Mode();
  void disable_WIFI_Mode();
  static void check_connectivity(Connectivity_Manager* _connectivity_Manager);
  bool change_AP_Profile(char *SSID, char *password);
  bool enable_AP_Mode();
  void disable_AP_Mode();
  friend class HTTP_Manager;
};
#endif  /*CONNECTIVITY_MANAGER_H_*/
