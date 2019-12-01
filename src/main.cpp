#include <Arduino.h>
#include "System_Utility/System_Manager.h"

std::string time_str;
System_Manager* my_system;
std::string getLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return "";
  }
  char output[80];
  strftime(output, 80, "dayName=%A,monthNum=%m,dayNum=%d,year=%Y,hour=%H,minute=%M,", &timeinfo);
  time_str = std::string(output);
  return time_str;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("");
  //initialize system
  my_system = new System_Manager();
  Logln("");
}

void print_Date_Time(DateTime& _date_time){
  Serial.print("Date = ");Serial.print((int)_date_time.get_day());
  Serial.print("/");Serial.print((int)_date_time.get_month());
  Serial.print("/");Serial.println((int)_date_time.get_year());
  Serial.print("Time = ");Serial.print((int)_date_time.get_hour());
  Serial.print(":");Serial.println((int)_date_time.get_minute());
}

void loop() {
    // put your main code here, to run repeatedly:
    my_system->m_http_manager->listen();
/*    configTime(2*3600, 0*3600, "pool.ntp.org", "time.nist.gov");
//    DateTime date_time = getLocalTime();
    std::string str_date_Time = getLocalTime();
    Serial.println(str_date_Time.c_str());
    Serial.println("-----------------------------------");
    DateTime date_time(str_date_Time);
    print_Date_Time(date_time);
    delay(1000);*/
}
/*
  WIFI_Profile* wifi_profile = new WIFI_Profile();
  wifi_profile->set_WIFI_SSID("AIMDs");
  wifi_profile->set_WIFI_Password("751486923@AIMD$");
  wifi_profile->local = IPAddress(192, 168, 1, 111);
  wifi_profile->gateway = IPAddress(192, 168, 1, 1);
  wifi_profile->subnet = IPAddress(255, 255, 255, 0);
  wifi_profile->primaryDNS = IPAddress(192, 168, 1, 1);
  wifi_profile->secondryDNS = IPAddress(163, 121, 128, 134);

  my_system->m_connection->change_AP_Profile((char *)"ESP32-AP", (char *)"espappassword");
  my_system->m_connection->add_WIFI_Profile(wifi_profile);
  my_system->m_connection->connect_to_WIFI();

  User admin(1, "admin", "admin751486923", 254);
  my_system->m_memory->set_User(admin);
*/
//////////////////////////////////...TESTING...////////////////////////////............
  /*{
    //testing user class functionalty
    {
      User dummy(-1, "dummy", "dummy", -1);
      dummy.set_printable(console);
      dummy.print();
      User Ahmed(10, "Ahmed", "751486923", 2);
      Ahmed.set_printable(console);
      Ahmed.print();
      dummy.set_name(Ahmed.get_name());
      dummy.set_password(Ahmed.get_password());
      dummy.set_id(Ahmed.get_id());
      dummy.set_auth_level(Ahmed.get_auth_level());
      dummy.print();
      User Bojy(Ahmed);
      Bojy.set_printable(console);
      Bojy.set_name("Bojy");
      Bojy.print();
    }

    //testing ip addresses read and write into EEPROM...........................
    {
      memory->set_localIP(IPAddress(41, 32, 113, 28));
      memory->set_gatewayIP(IPAddress(41, 32, 113, 25));
      memory->set_subnetIP(IPAddress(255, 255, 255, 248));
      memory->set_primaryDNS(IPAddress(163, 125, 110, 134));
      memory->set_secondaryDNS(IPAddress(163, 125, 110, 135));
      IPAddress local_ip = memory->get_localIP();
      IPAddress gateway_ip = memory->get_gatewayIP();
      IPAddress subnet_ip = memory->get_subnetIP();
      IPAddress primaryDNS_ip = memory->get_primaryDNS();
      IPAddress secondaryDNS_ip = memory->get_secondaryDNS();
      for(int i = 0; i < 3; i++){
        Serial.print(local_ip[i]);
        Serial.print(".");
      }
      Serial.println(local_ip[3]);

      for(int i = 0; i < 3; i++){
        Serial.print(gateway_ip[i]);
        Serial.print(".");
      }
      Serial.println(gateway_ip[3]);

      for(int i = 0; i < 3; i++){
        Serial.print(subnet_ip[i]);
        Serial.print(".");
      }
      Serial.println(subnet_ip[3]);

      for(int i = 0; i < 3; i++){
        Serial.print(primaryDNS_ip[i]);
        Serial.print(".");
      }
      Serial.println(primaryDNS_ip[3]);

      for(int i = 0; i < 3; i++){
        Serial.print(secondaryDNS_ip[i]);
        Serial.print(".");
      }
      Serial.println(secondaryDNS_ip[3]);
    }

    //testing wifi ssid, wifi password, ap ssid and ap password reead and write
    {
      memory->set_WIFI_SSID("AIMD");
      memory->set_WIFI_password("751486923AIMD");

      memory->set_AP_SSID("ESP32-AP");
      memory->set_AP_password("espappassword");

      Logln(memory->get_WIFI_SSID().c_str());
      Logln(memory->get_WIFI_password().c_str());
      Logln(memory->get_AP_SSID().c_str());
      Logln(memory->get_AP_password().c_str());
      memory->set_last_state(0);
      Logln(memory->get_last_state());
    }

    //testing registring users into EEPROM Memory
    {
      User Ahmed(10, "Ahmed", "751486923", 0);
      memory->set_User(Ahmed);
      for(int i = 0; i < 45; i++){
        Log(EEPROM.read(i+120));Log(" ");
      }
      Logln("");
      User* result = memory->find_user(10);
      User admin(*result);
      delete result;
      admin.set_name("admin");
      admin.set_id(11);
      admin.set_printable(console);
      admin.print();
      memory->set_User(admin);
      for(int i = 0; i < 45; i++){
        Log(EEPROM.read(i+165));Log(" ");
      }
      Logln("");
      memory->delete_user(Ahmed.get_id());
      admin.set_id(12);
      memory->set_User(admin);
      for(int i = 0; i < 45; i++){
        Log(EEPROM.read(i+120));Log(" ");
      }
    }
  }*/
