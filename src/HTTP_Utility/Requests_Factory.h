#ifndef _REQUESTS_FACTORY_H_
#define _REQUESTS_FACTORY_H_
#include "Requests.h"
class Requrests_Factory{
    static Requrests_Factory* m_factory;
    Requrests_Factory(){}
public:
    static Requrests_Factory* get_factory(){
        if(m_factory == NULL){
            m_factory = new Requrests_Factory();
        }
        return m_factory;
    }

    Request* create_Request(WiFiClient* client, std::string* _str_request){
        Request* request = new Request(client, _str_request);
        Request* sub_Request;
        if(!request->vaild_request){
            sub_Request = new Request_invalid(request);
        }
        else if (strcmp(request->m_Request_Type, Request::REQUEST_WIFI_INIT) == 0){
            sub_Request = new Request_wifi_init(request);
        }
        else if (strcmp(request->m_Request_Type, Request::REQUEST_WIFI_ENABLE) == 0){
            sub_Request = new Request_wifi_enable(request);
        }
        else if (strcmp(request->m_Request_Type, Request::REQUEST_WIFI_DISABLE) == 0){
            sub_Request = new Request_wifi_disable(request);
        }
        else if (strcmp(request->m_Request_Type, Request::REQUEST_CHANGE_IP) == 0){
            sub_Request = new Request_change_ip(request);
        }
        else if (strcmp(request->m_Request_Type, Request::REQUEST_AP_INIT) == 0){
            sub_Request = new Request_ap_init(request);
        }
        else if (strcmp(request->m_Request_Type, Request::REQUEST_AP_ENABLE) == 0){
            sub_Request = new Request_ap_enable(request);
        }
        else if (strcmp(request->m_Request_Type, Request::REQUEST_AP_DISABLE) == 0){
            sub_Request = new Request_ap_disable(request);
        }
        else if (strcmp(request->m_Request_Type, Request::REQUEST_SERVER_ACCESS) == 0){
            sub_Request = new Request_Server_Access(request);
        }
        else if (strcmp(request->m_Request_Type, Request::REQUEST_SCAN_TO_ACCESS) == 0){
            sub_Request = new Request_scan_to_access(request);
        }
        else if (strcmp(request->m_Request_Type, Request::REQUEST_ENROLL) == 0){
            sub_Request = new Request_enroll(request);
        }
        else if (strcmp(request->m_Request_Type, Request::REQUEST_DELETE) == 0){
            sub_Request = new Request_delete(request);
        }
        else if (strcmp(request->m_Request_Type, Request::REQUEST_ADD_USER) == 0){
            sub_Request = new Request_add_user(request);
        }
        else if (strcmp(request->m_Request_Type, Request::REQUEST_REMOVE_USER) == 0){
            sub_Request = new Request_remove_user(request);
        }
        else if (strcmp(request->m_Request_Type, Request::REQUEST_EDIT_USER) == 0){
            sub_Request = new Request_edit_user(request);
        }
        else if (strcmp(request->m_Request_Type, Request::REQUEST_GET_USERS) == 0){
            sub_Request = new Request_get_users(request);
        }
        else if (strcmp(request->m_Request_Type, Request::REQUEST_ADD_PERIPHERAL) == 0){
            sub_Request = new Request_add_peripheral(request);
        }
        else if (strcmp(request->m_Request_Type, Request::REQUEST_EDIT_PERIPHERAL) == 0){
            sub_Request = new Request_edit_peripheral(request);
        }
        else if (strcmp(request->m_Request_Type, Request::REQUEST_REMOVE_PERIPHERAL) == 0){
            sub_Request = new Request_remove_peripheral(request);
        }
        else if (strcmp(request->m_Request_Type, Request::REQUEST_GET_PERIPHERALS) == 0){
            sub_Request = new Request_get_peripherals(request);
        }
        else if (strcmp(request->m_Request_Type, Request::REQUEST_ROLLING_STATE) == 0){
            sub_Request = new Request_Rolling_State(request);
        }
        else if (strcmp(request->m_Request_Type, Request::REQUEST_ROLLING_OPEN) == 0){
            sub_Request = new Request_Rolling_Open(request);
        }
        else if (strcmp(request->m_Request_Type, Request::REQUEST_ROLLING_CLOSE) == 0){
            sub_Request = new Request_Rolling_Close(request);
        }
        else if (strcmp(request->m_Request_Type, Request::REQUEST_UPDATES) == 0){
            sub_Request = new Request_updates(request);
        }
        else if (strcmp(request->m_Request_Type, Request::REQUEST_ACCESSABILTY) == 0){
            sub_Request = new Request_Accessabilty(request);
        }
        else if (strcmp(request->m_Request_Type, Request::REQUEST_AUTHONTICATION) == 0){
            sub_Request = new Request_authontication(request);
        }
        else{
            sub_Request = new Request_void(request);
        }
        delete request;
        return sub_Request;
    }
};
#endif /*_REQUESTS_FACTORY_H_*/
