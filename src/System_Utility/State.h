#ifndef _STATE_H_
#define _STATE_H_

#include <WiFi.h>
#include <string>
#include "../HTTP_Utility/Requests.h"
#include "../Peripheral_Utility/Peripheral_State.h"
class System_Manager;

class State{
protected:
    System_Manager* m_system;
    State(){}
public:
    State(System_Manager* _system);
    virtual State* doAction(Request* _request) = 0;
    State* init_wifi(Request_wifi_init* _request_wifi_init);
    State* enable_wifi(Request_wifi_enable* _request);
    State* disable_wifi(Request_wifi_disable* _request);
    State* init_AP(Request_ap_init* _request);
    State* enable_AP(Request_ap_enable* _request);
    State* disable_AP(Request_ap_disable* _request);
    State* server_accesse(Request_Server_Access* _request);
    State* add_user(Request_add_user* _request);
    State* remove_user(Request_remove_user* _request);
    State* edit_user(Request_edit_user* _request);
    State* get_Users(Request_get_users* _request);
    State* add_peripheral(Request_add_peripheral* _request);
    State* edit_peripheral(Request_edit_peripheral* _request);
    State* remove_peripheral(Request_remove_peripheral* _request);
    State* get_peripheral(Request_get_peripherals* _request);
    State* get_Rolling_State(Request_Rolling_State* _request);
    State* open_rolling_peripheral(Request_Rolling_Open* _request);
    State* close_rolling_peripheral(Request_Rolling_Close* _request);
    virtual ~State(){}
};

///////////////////////////////////////////////////////////////////////////////////////////
class Default_State : public State{
public:
    Default_State(System_Manager* _system);
    virtual State* doAction(Request* _request);
};

class Match_State : public State{
public:
    Match_State(System_Manager* _system);
    virtual State* doAction(Request* _request);
};

class Not_Match_State : public State{
public:
    Not_Match_State(System_Manager* _system);
    virtual State* doAction(Request* _request);
};

class Invalid_Request_State : public State{
public:
    Invalid_Request_State(System_Manager* _system);
    virtual State* doAction(Request* _request);
};

class Approved_State : public State{
public:
    Approved_State(System_Manager* _system);
    virtual State* doAction(Request* _request);
};

class Not_Approved_State : public State{
public:
    Not_Approved_State(System_Manager* _system);
    virtual State* doAction(Request* _request);
};

class Server_Access_State : public State{
public:
    Server_Access_State(System_Manager* _system);
    virtual State* doAction(Request* _request);
};

class Update_State : public State{
public:
    Update_State(System_Manager* _system);
    virtual State* doAction(Request* _request);
};

class Users_State : public State{
public:
    Users_State(System_Manager* _system);
    virtual State* doAction(Request* _request);
};

class Change_Ip_State : public State{
public:
    Change_Ip_State(System_Manager* _system);
    virtual State* doAction(Request* _request);
};

class AP_Enable_State : public State{
public:
    AP_Enable_State(System_Manager* _system);
    virtual State* doAction(Request* _request);
};

class AP_Disable_state : public State{
public:
    AP_Disable_state(System_Manager* _system);
    virtual State* doAction(Request* _request);
};

class WIFI_Enable_State : public State{
public:
    WIFI_Enable_State(System_Manager* _system);
    virtual State* doAction(Request* _request);
};

class WIFI_Disable_State : public State{
public:
    WIFI_Disable_State(System_Manager* _system);
    virtual State* doAction(Request* _request);
};

class Accessabilty_State : public State{
public:
    Accessabilty_State(System_Manager* _system);
    virtual State* doAction(Request* _request);
};

class Authontication_State : public State{
public:
    Authontication_State(System_Manager* _system);
    virtual State* doAction(Request* _request);
};

class No_Access_State : public State{
public:
    No_Access_State(System_Manager* _system);
    virtual State* doAction(Request* _request);
};

class Delete_State : public State{
public:
    Delete_State(System_Manager* _system);
    virtual State* doAction(Request* _request);
};

class AP_Approvale_State : public State{
public:
    AP_Approvale_State(System_Manager* _system);
    virtual State* doAction(Request* _request);
};

class AP_Not_Approvale_State : public State{
public:
    AP_Not_Approvale_State(System_Manager* _system);
    virtual State* doAction(Request* _request);
};

class Fail_Config_State : public State{
public:
    Fail_Config_State(System_Manager* _system);
    virtual State* doAction(Request* _request);
};

class Fail_Connect_State : public State{
public:
    Fail_Connect_State(System_Manager* _system);
    virtual State* doAction(Request* _request);
};

class AP_Init_State : public State{
public:
    AP_Init_State(System_Manager* _system);
    virtual State* doAction(Request* _request);
};

class Server_Init_State : public State{
public:
    Server_Init_State(System_Manager* _system);
    virtual State* doAction(Request* _request);
};

class Void_State : public State{
public:
    Void_State(System_Manager* _system);
    virtual State* doAction(Request* _request);
};
#endif
/*typedef enum
{
    STATE_SLEEP,
    STATE_SCAN,
    STATE_NOT_MATCH,
    STATE_MATCH,
    STATE_ACCESSABILTY,
    STATE_AUTHONTICATION,
    STATE_APPROVED,
    STATE_NOT_APPROVED,
    STATE_ACCES,
    STATE_NO_ACCESS,
    STATE_DOOR_STATE,
    STATE_OPEN,
    STATE_CLOSE,
    STATE_UPDATES,
    STATE_ENROLL,
    STATE_DELETE,
    STATE_USERS,
    STATE_VOID,
    STATE_AP_APPROVALE,
    STATE_AP_NOT_APPROVALE,
    STATE_CONFIG_FAIL,
    STATE_CONNECT_FAIL,
    STATE_CHANGE_IP,
    STATE_AP_INIT,
    STATE_SERVER_INIT,
    STATE_SERVER_ACCESS,
    STATE_AP_ENABLE,
    STATE_AP_DISABLE,
    STATE_WIFI_ENABLE,
    STATE_WIFI_DISABLE,
    STATE_NOT_VAILD_REQUEST
} Const_State;*/
