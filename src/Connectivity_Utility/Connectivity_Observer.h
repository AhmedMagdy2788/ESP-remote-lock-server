#ifndef CONNECTIVITY_OBSERVER_H_
#define CONNECTIVITY_OBSERVER_H_

#include "Connectivity_Utility/WIFI_Profile.h"

class Connectivity_Observer{
public:
  virtual void update_WIFI_Profile(WIFI_Profile*) = 0;
  virtual bool initialize_wifi(WIFI_Profile*) = 0;
  virtual bool enable_wifi() = 0;
  virtual void disable_wifi() = 0;
  virtual void init_AP(char *SSID, char *password) = 0;
  virtual void enable_AP() = 0;
  virtual void disable_AP() = 0;
  virtual void update_AP_Profile(char*, char*) = 0;
  virtual void update_Connection_Mode(uint8_t) = 0;
};
#endif /*CONNECTIVITY_OBSERVER_H_*/
