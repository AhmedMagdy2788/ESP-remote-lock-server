#ifndef SYSTEM_MANAGER_H_
#define SYSTEM_MANAGER_H_

#include "Memory_Utility/EEPROM_Manager.h"
#include "Console_Utility/Console.h"
#include "Connectivity_Utility/Connectivity_Manager.h"
#include "Connectivity_Utility/Connectivity_Observer.h"
#include "HTTP_Utility/HTTP_Observer.h"
#include "HTTP_Utility/HTTP_Manager.h"
#include "Peripheral_Utility/Peripheral_Observer.h"
#include "Event.h"
#include "time.h"

class State;

class System_Manager : public HTTP_Observer, public Connectivity_Observer, public Peripheral_Observer
{
public:
    Memory_Manager *m_memory;
    MyPrintable *m_console;
    Connectivity_Manager *m_connection;
    HTTP_Manager* m_http_manager;
    State* m_Current_State;
    Peripheral** m_peripheralList;
    uint8_t m_peripheral_Num;
    DateTime* m_system_time;

    System_Manager();
    void getTimeServer(int time_zone);
    //the following functions if for __HTTP_Observer__
    virtual void requestHandler(Request*);
    //virtual void httpResponse(WiFiClient client);
    //the following functions if for __Connectivity_Observer__
    virtual void update_WIFI_Profile(WIFI_Profile *) override;
    virtual bool initialize_wifi(WIFI_Profile*) override;
    virtual bool enable_wifi() override;
    virtual void disable_wifi() override;
    virtual void init_AP(char *SSID, char *password) override;
    virtual void enable_AP() override;
    virtual void disable_AP() override;
    virtual void update_AP_Profile(char *, char *) override;
    virtual void update_Connection_Mode(uint8_t) override;
    //the following functions if for __FingerPrint Sensor__
    uint8_t scan_New_User();
    uint8_t scan_to_Access();
    //the following functions if for __EEPROM Memory__
    char *get_Updates();
    bool add_new_user(User& user);
    bool remove_user(uint8_t user_id);
    bool edit_user(User& user);
    char *get_Users();
    //the following functions if for __Control Peripherals__
    uint8_t add_peripheral(int _peripheral_type, int _peripheral_rx, int _peripheral_tx, int _peripheral_time);
    bool edit_peripheral(int _peripheral_type, int _peripheral_id, int _peripheral_rx, int _peripheral_tx, int _peripheral_time);
    bool remove_peripheral(int _peripheral_id);
    char* get_peripherals();
    int getPeripheral_index(uint8_t _peripheral_id);
    void update_Peripherals_List();
    void open_Door(uint8_t percentage);
    void close_Door();
    uint8_t get_Door_state();
};

#endif /*SYSTEM_MANAGER_H_*/
