#ifndef REQUESTS_H_
#define REQUESTS_H_
#include <Arduino.h>
#include <string>
#include "../Connectivity_Utility/WIFI_Profile.h"
#include "../Users_Utility/User.h"
#include "../Peripheral_Utility/Peripheral.h"
#include <wifi.h>
class State;
class Request{
public:
    static const char* REQUEST_WIFI_INIT;
    static const char* REQUEST_WIFI_ENABLE;
    static const char* REQUEST_WIFI_DISABLE;
    static const char* REQUEST_CHANGE_IP;
    static const char* REQUEST_AP_INIT;
    static const char* REQUEST_AP_ENABLE;
    static const char* REQUEST_AP_DISABLE;
    static const char* REQUEST_SERVER_ACCESS;
    static const char* REQUEST_SCAN_TO_ACCESS;
    static const char* REQUEST_ENROLL;
    static const char* REQUEST_DELETE;
    static const char* REQUEST_ADD_USER;
    static const char* REQUEST_REMOVE_USER;
    static const char* REQUEST_EDIT_USER;
    static const char* REQUEST_GET_USERS;
    static const char* REQUEST_ADD_PERIPHERAL;
    static const char* REQUEST_EDIT_PERIPHERAL;
    static const char* REQUEST_REMOVE_PERIPHERAL;
    static const char* REQUEST_GET_PERIPHERALS;
    static const char* REQUEST_ROLLING_STATE;
    static const char* REQUEST_ROLLING_OPEN;
    static const char* REQUEST_ROLLING_CLOSE;
    static const char* REQUEST_UPDATES;
    static const char* REQUEST_ACCESSABILTY;
    static const char* REQUEST_AUTHONTICATION;

    static const char* REQUEST_INVALID;
    static const char* REQUEST_VOID;

    WiFiClient* client;
    std::string* m_str_request;
    int m_User_ID;
    char* m_User_Name;
    char* m_User_Password;
    char* m_Request_Type;
    bool vaild_request;
    bool request_is_done;
    User* m_user;
    Request(WiFiClient* client, std::string* _request);
    Request(Request* other);
    virtual ~Request();
    void set_user_name(const char*);
    void set_User_password(const char*);
    void set_request_type(const char*);
    virtual State* perform_Request(State* _state){return NULL;}
protected:
    Request();
    void validate_request();
    void get_User_ID();
    void get_User_Name();
    void get_User_Password();
    void get_Request_Type();
    char *get_Pair_Of(std::string* statment, const char *key, const char *end_statment);
};

class Request_wifi_init: public Request{
public:
    WIFI_Profile* m_wifi_profile;
    Request_wifi_init(Request* _super);
    State* perform_Request(State * _state) override;
    ~Request_wifi_init(){}
    void get_WIFI_Profile();
};

class Request_wifi_enable: public Request{
public:
    Request_wifi_enable(Request* _super);
    State* perform_Request(State * _state) override;
};

class Request_wifi_disable: public Request{
public:
    Request_wifi_disable(Request* _super);
    State* perform_Request(State * _state) override;
};

class Request_change_ip: public Request_wifi_init{
public:
    Request_change_ip(Request* _super);
    State* perform_Request(State * _state) override;
};

class Request_ap_init: public Request{
public:
    char* str_AP_Password;
    char* str_AP_SSID;
    Request_ap_init(Request* _super);
    State* perform_Request(State * _state) override;
    void get_AP_SSID();
    void get_AP_password();
};

class Request_ap_enable: public Request{
public:
    Request_ap_enable(Request* _super);
    State* perform_Request(State * _state) override;
};

class Request_ap_disable: public Request{
public:
    Request_ap_disable(Request* _super);
    State* perform_Request(State * _state) override;
};

class Request_Server_Access: public Request{
public:
    Request_Server_Access(Request* _super);
    State* perform_Request(State * _state) override;
};

class Request_scan_to_access: public Request{
public:
    int m_scanner_id;
    Request_scan_to_access(Request* _super);
    State* perform_Request(State * _state) override;
    void get_scanner_id();
};

class Request_enroll: public Request{
public:
    int m_scanner_id;
    Request_enroll(Request* _super);
    State* perform_Request(State * _state) override;
    void get_scanner_id();
};

class Request_delete: public Request{
public:
    int user_id;
    int m_scanner_id;
    Request_delete(Request* _super);
    State* perform_Request(State * _state) override;
    void get_user_id();
    void get_scanner_id();
};

class Request_add_user: public Request{
public:
    User m_added_user;
    Request_add_user(Request* _super);
    State* perform_Request(State * _state) override;
    void get_added_user();
};

class Request_remove_user: public Request{
public:
    int m_removed_user;
    Request_remove_user(Request* _super);
    State* perform_Request(State * _state) override;
    void get_removed_user();
};

class Request_edit_user: public Request{
public:
    User m_edited_user;
    Request_edit_user(Request* _super);
    State* perform_Request(State * _state) override;
    void get_edited_user();
};

class Request_get_users: public Request{
public:
    Request_get_users(Request* _super);
    State* perform_Request(State * _state) override;
};

class Request_add_peripheral: public Request{
public:
    int m_peripheral_type;
    int m_peripheral_rx;
    int m_peripheral_tx;
    int m_peripheral_time;
    Request_add_peripheral(Request* _super);
    State* perform_Request(State * _state) override;
    void get_peripheral();
};

class Request_edit_peripheral: public Request{
public:
    int m_peripheral_type;
    int m_peripheral_id;
    int m_peripheral_rx;
    int m_peripheral_tx;
    int m_peripheral_time;
    Request_edit_peripheral(Request* _super);
    State* perform_Request(State * _state) override;
    void get_peripheral();
};

class Request_remove_peripheral: public Request{
public:
    int m_peripheral_id;
    Request_remove_peripheral(Request* _super);
    State* perform_Request(State * _state) override;
    void get_peripheral_id();
};

class Request_get_peripherals: public Request{
public:
        Request_get_peripherals(Request* _super);
        State* perform_Request(State * _state) override;
};

class Request_Rolling_State: public Request{
public:
    int m_rolling_id;
    Request_Rolling_State(Request* _super);
    State* perform_Request(State * _state) override;
    void get_rolling_id();
};

class Request_Rolling_Open: public Request{
public:
    int m_rolling_id;
    int m_rolling_percentage;
    Request_Rolling_Open(Request* _super);
    State* perform_Request(State * _state) override;
    void get_rolling_id();
    void get_rolling_Percentage();
};

class Request_Rolling_Close: public Request{
public:
    int m_rolling_id;
    int m_rolling_percentage;
    Request_Rolling_Close(Request* _super);
    State* perform_Request(State * _state) override;
    void get_rolling_id();
    void get_rolling_percentage();
};

class Request_updates: public Request{
public:
    Request_updates(Request* _super);
    State* perform_Request(State * _state) override;
};

class Request_Accessabilty: public Request{
public:
    Request_Accessabilty(Request* _super);
    State* perform_Request(State * _state) override;
};

class Request_authontication: public Request{
public:
    Request_authontication(Request* _super);
    State* perform_Request(State * _state) override;
};

class Request_invalid: public Request{
public:
    Request_invalid(Request* _super);
    State* perform_Request(State * _state) override;
};

class Request_void: public Request{
public:
    Request_void(Request* _super);
    State* perform_Request(State * _state) override;
};

#endif
