#include "Requests.h"
#include "../System_Utility/State.h"

const char* Request::REQUEST_WIFI_INIT = "init_wifi";
const char* Request::REQUEST_WIFI_ENABLE = "enable_wifi";
const char* Request::REQUEST_WIFI_DISABLE = "disable_wifi";
const char* Request::REQUEST_CHANGE_IP = "change_ip";
const char* Request::REQUEST_AP_INIT = "init_ap";
const char* Request::REQUEST_AP_ENABLE = "enable_ap";
const char* Request::REQUEST_AP_DISABLE = "disable_ap";
const char* Request::REQUEST_SERVER_ACCESS = "server_access";
const char* Request::REQUEST_SCAN_TO_ACCESS = "scan_to_access";
const char* Request::REQUEST_ENROLL = "enroll_user";
const char* Request::REQUEST_DELETE = "delete_user";
const char* Request::REQUEST_ADD_USER = "add_user";
const char* Request::REQUEST_REMOVE_USER = "remove_user";
const char* Request::REQUEST_EDIT_USER = "edit_user";
const char* Request::REQUEST_GET_USERS = "get_users";
const char* Request::REQUEST_ADD_PERIPHERAL = "add_peripheral";
const char* Request::REQUEST_EDIT_PERIPHERAL = "edit_peripheral";
const char* Request::REQUEST_REMOVE_PERIPHERAL = "remove_peripheral";
const char* Request::REQUEST_GET_PERIPHERALS = "get_peripherals";
const char* Request::REQUEST_ROLLING_STATE = "get_rolling_state";
const char* Request::REQUEST_ROLLING_OPEN = "open_rolling";
const char* Request::REQUEST_ROLLING_CLOSE = "close_rolling";
const char* Request::REQUEST_UPDATES = "get_updates";
const char* Request::REQUEST_ACCESSABILTY = "accessabilty";
const char* Request::REQUEST_AUTHONTICATION = "authontication";

const char* Request::REQUEST_INVALID = "invalid_request";
const char* Request::REQUEST_VOID = "";

Request::Request(){}

Request::~Request(){
    delete[] m_User_Name;
    delete[] m_User_Password;
    delete[] m_Request_Type;
    if(m_user != NULL)
        delete m_user;
}

Request::Request(WiFiClient* client, std::string* _request):client(client), m_str_request(_request), request_is_done(false), m_user(NULL){
    validate_request();
}

Request::Request(Request* other){
    this->client = other->client;
    this->m_str_request = other->m_str_request;
    this->m_User_ID = other->m_User_ID;

    this->m_User_Name = new char[strlen(other->m_User_Name) + 1];
    strcpy(this->m_User_Name, other->m_User_Name);

    this->m_User_Password = new char[strlen(other->m_User_Password) + 1];
    strcpy(this->m_User_Password, other->m_User_Password);

    this->m_Request_Type = new char[strlen(other->m_Request_Type) + 1];
    strcpy(this->m_Request_Type, other->m_Request_Type);

    this->vaild_request = other->vaild_request;
    this->request_is_done = other->request_is_done;
    if(other->m_user == NULL)
        this->m_user = NULL;
    else
        this->m_user = new User(*(other->m_user));
}

void Request::set_user_name(const char* _user_name){
    m_User_Name = new char [strlen(_user_name)+1];
    strcpy(m_User_Name, _user_name);
}

void Request::set_User_password(const char* _user_password){
    m_User_Password = new char [strlen(_user_password)+1];
    strcpy(m_User_Password, _user_password);
}

void Request::set_request_type(const char* _request_type){
    m_Request_Type = new char [strlen(_request_type)+1];
    strcpy(m_Request_Type, _request_type);
}

void Request::validate_request(){
    m_str_request = new std::string(get_Pair_Of(m_str_request, "GET /", " HTTP/1.1"));
    Serial.print("main Request = ");
    Serial.println(m_str_request->c_str());

    if (m_str_request->compare("_not_found_") == 0 || m_str_request->compare("favicon.ico") == 0)
    {
        vaild_request = false;
        m_User_ID = 0;
        set_user_name("NULL");
        set_User_password("NULL");
        set_request_type(Request::REQUEST_INVALID);
    }else{
        get_User_ID();
        get_User_Name();
        get_User_Password();
        get_Request_Type();
        vaild_request = true;
        if(m_User_ID == 0){
            vaild_request = false;
            Serial.print("invalid user id = ");Serial.println(m_User_ID);
        }
        if(strcmp(m_User_Name, "_not_found_") == 0){
            vaild_request = false;
            Serial.print("invalid user name = ");Serial.println(m_User_Name);
        }
        if(strcmp(m_User_Password, "_not_found_") == 0){
            vaild_request = false;
            Serial.print("invalid user password = ");Serial.println(m_User_Password);
        }
        if(strcmp(m_Request_Type, "_not_found_") == 0){
            vaild_request = false;
            Serial.print("invalid request type = ");Serial.println(m_Request_Type);
        }
    }
}

void Request::get_User_ID(){
    //get user id which found after "GET /?userid=" until "&" then add it into m_User_ID
    char* charptr_userid = get_Pair_Of(m_str_request, "?userid=", "&");
    sscanf(charptr_userid, "%d", &m_User_ID);
    delete[] charptr_userid;
    Serial.print("User id is ");
    Serial.println(m_User_ID);
}

void Request::get_User_Name(){
    //get user name which found after "&username=" until "&" then add it into m_User_Name
    m_User_Name = get_Pair_Of(m_str_request, "&username=", "&");
    Serial.print("User Name is ");
    Serial.println(m_User_Name);
}

void Request::get_User_Password(){
    //get user password which found after "&password=" until "&" then add it into m_User_Password
    m_User_Password = get_Pair_Of(m_str_request, "&userpassword=", "&");
    Serial.print("User Password is ");
    Serial.println(m_User_Password);
}

void Request::get_Request_Type(){
    m_Request_Type = get_Pair_Of(m_str_request, "&requesttype=", "&");
    Serial.print("Request Type is ");
    Serial.println(m_Request_Type);
}

char *Request::get_Pair_Of(std::string* statment, const char *key, const char *end_statment){
    std::size_t key_pos;
    std::size_t key_size;
    std::size_t end_pos;
    std::size_t pair_length;
    std::size_t pair_pos;
    char * pair_Value;
    Serial.println("");
    key_pos = statment->find(key);
    if (key_pos != std::string::npos)
    {
        key_size = strlen(key);
        // end_pos = statment->find(end_statment, key_pos + 1);
        end_pos = statment->find(end_statment, key_pos + key_size);
        if (end_pos == std::string::npos)
        {
            end_pos = statment->size();
            Serial.print("does not found ");
            Serial.println(end_statment);
        }
        pair_length = end_pos - key_pos - key_size;
        pair_pos = key_pos + key_size;
        /*Serial.print("key_pos = ");
        Serial.println(key_pos);
        Serial.print("key_size = ");
        Serial.println(key_size);
        Serial.print("end_pos = ");
        Serial.println(end_pos);
        Serial.print("pair_length = ");
        Serial.println(pair_length);
        Serial.print("pair_pos = ");
        Serial.println(pair_pos);*/
        std::string str_pair_value = statment->substr(pair_pos, pair_length);

        pair_Value = new char [str_pair_value.length()+1];
        strcpy(pair_Value, str_pair_value.c_str());
        // char *pair_Value = (char *)malloc(pair_length + 1);
        // memcpy(pair_Value, str_pair_value.c_str(), pair_length + 1);
        return pair_Value;
    }
    else
    {
        pair_Value = new char [strlen("_not_found_")+1];
        strcpy(pair_Value, "_not_found_");
        return pair_Value;
    }
}

/////////////////////////////////////////////////////////////////////////////////

//GET /?userid=1&username=admin&userpassword=admin&requesttype=init_wifi&wifissid=tedata-11910&wifipassword=TEData@91011&local_ip=192.168.1.111&gateway=192.168.1.1&subnet=255.255.255.0&primaryDNS=165.120.123.135&secondryDNS=165.120.123.134& HTTP/1.1
Request_wifi_init::Request_wifi_init(Request* _super):Request(_super){
    get_WIFI_Profile();
    Serial.print("Request_wifi_init created with WIFI SSID = ");
    Serial.println(m_wifi_profile->get_WIFI_SSID());
}

void Request_wifi_init::get_WIFI_Profile(){
    m_wifi_profile = new WIFI_Profile();
    m_wifi_profile->set_WIFI_SSID(get_Pair_Of(m_str_request, "&wifissid=", "&"));
    Serial.print("WIFI SSID is ");
    Serial.println(m_wifi_profile->get_WIFI_SSID());

    m_wifi_profile->set_WIFI_Password(get_Pair_Of(m_str_request, "&wifipassword=", "&"));
    Serial.print("WIFI password is ");
    Serial.println(m_wifi_profile->get_WIFI_Password());

    m_wifi_profile->set_Local_IP(get_Pair_Of(m_str_request, "&local_ip=", "&"));
    Serial.print("Local IP = ");
    for (int i = 0; i < 3; i++)
    {
        Serial.print(m_wifi_profile->get_Local_ip()[i]);
        Serial.print(".");
    }
    Serial.println(m_wifi_profile->get_Local_ip()[3]);

    m_wifi_profile->set_gateway_IP(get_Pair_Of(m_str_request, "&gateway=", "&"));
    Serial.print("Gateway IP = ");
    for (int i = 0; i < 3; i++)
    {
        Serial.print(m_wifi_profile->get_gateway_ip()[i]);
        Serial.print(".");
    }
    Serial.println(m_wifi_profile->get_gateway_ip()[3]);

    m_wifi_profile->set_subnet_IP(get_Pair_Of(m_str_request, "&subnet=", "&"));
    Serial.print("Subnet IP = ");
    for (int i = 0; i < 3; i++)
    {
        Serial.print(m_wifi_profile->get_subnet_ip()[i]);
        Serial.print(".");
    }
    Serial.println(m_wifi_profile->get_subnet_ip()[3]);

    m_wifi_profile->set_primaryDNS_IP(get_Pair_Of(m_str_request, "&primaryDNS=", "&"));
    Serial.print("PrimaryDNS IP = ");
    for (int i = 0; i < 3; i++)
    {
        Serial.print(m_wifi_profile->get_primaryDNS_ip()[i]);
        Serial.print(".");
    }
    Serial.println(m_wifi_profile->get_primaryDNS_ip()[3]);

    m_wifi_profile->set_secondryDNS_IP(get_Pair_Of(m_str_request, "&secondryDNS=", "&"));
    Serial.print("SecondryDNS IP = ");
    for (int i = 0; i < 3; i++)
    {
        Serial.print(m_wifi_profile->get_secondryDNS_ip()[i]);
        Serial.print(".");
    }
    Serial.println(m_wifi_profile->get_secondryDNS_ip()[3]);
}

State* Request_wifi_init::perform_Request(State* _state){
    return _state->init_wifi(this);
}

//GET /?userid=1&username=admin&userpassword=admin751486923&requesttype=enable_wifi& HTTP/1.1
Request_wifi_enable::Request_wifi_enable(Request* _super):Request(_super){

}

State* Request_wifi_enable::perform_Request(State* _state){
    return _state->enable_wifi(this);
}

//GET /?userid=1&username=admin&userpassword=admin&requesttype=disable_wifi& HTTP/1.1
Request_wifi_disable::Request_wifi_disable(Request* _super):Request(_super){

}

State* Request_wifi_disable::perform_Request(State* _state){
    return _state->disable_wifi(this);
}

//GET /?userid=1&username=admin&userpassword=admin751486923&requesttype=change_ip&wifissid=AIMDs&wifipassword=751486923@AIDM$&local_ip=192.168.1.111&gateway=192.168.1.1&subnet=255.255.255.0&primaryDNS=165.120.123.135&secondryDNS=165.120.123.134& HTTP/1.1
Request_change_ip::Request_change_ip(Request* _super):Request_wifi_init(_super){}

State* Request_change_ip::perform_Request(State* _state){
    return _state->init_wifi(this);
}

//GET /?userid=1&username=admin&userpassword=admin751486923&requesttype=init_ap&ap_ssid=ESP32-AP&ap_password=751486923@ESP& HTTP/1.1
Request_ap_init::Request_ap_init(Request* _super):Request(_super){
    get_AP_SSID();
    get_AP_password();
}

void Request_ap_init::get_AP_SSID(){
    str_AP_SSID = get_Pair_Of(m_str_request, "&ap_ssid=", "&");
    Serial.print("AP SSID = ");
    Serial.println(str_AP_SSID);
}

void Request_ap_init::get_AP_password(){
    str_AP_Password = get_Pair_Of(m_str_request, "&ap_password=", "&");
    Serial.print("AP Password = ");
    Serial.println(str_AP_Password);
}

State* Request_ap_init::perform_Request(State* _state){
    return _state->init_AP(this);
}

//GET /?userid=1&username=admin&userpassword=admin751486923&requesttype=enable_ap& HTTP/1.1
Request_ap_enable::Request_ap_enable(Request* _super):Request(_super){}

State* Request_ap_enable::perform_Request(State* _state){
    return _state->enable_AP(this);
}

//GET /?userid=1&username=admin&userpassword=admin751486923&requesttype=disable_ap& HTTP/1.1
Request_ap_disable::Request_ap_disable(Request* _super):Request(_super){}

State* Request_ap_disable::perform_Request(State* _state){
    return _state->disable_AP(this);
}

//GET /?userid=1&username=admin&userpassword=admin751486923&requesttype=server_access& HTTP/1.1
Request_Server_Access::Request_Server_Access(Request* _super):Request(_super){}

State* Request_Server_Access::perform_Request(State* _state){
    return _state->server_accesse(this);
}

//GET /?userid=1&username=admin&userpassword=admin751486923&requesttype=scan_to_access& HTTP/1.1
Request_scan_to_access::Request_scan_to_access(Request* _super):Request(_super){}

State* Request_scan_to_access::perform_Request(State* _state){
    return NULL;
}

//GET /?userid=1&username=admin&userpassword=admin751486923&requesttype=enroll_user& HTTP/1.1
Request_enroll::Request_enroll(Request* _super):Request(_super){

}

State* Request_enroll::perform_Request(State* _state){
    return NULL;
}

//GET /?userid=1&username=admin&userpassword=admin751486923&requesttype=delete_user&user_id=11& HTTP/1.1
Request_delete::Request_delete(Request* _super):Request(_super){
    get_user_id();
}

State* Request_delete::perform_Request(State* _state){
    return NULL;
}

void Request_delete::get_user_id(){
    sscanf(get_Pair_Of(m_str_request, "&user_id=", "&"), "%d", &user_id);
    Serial.print("delete id = ");
    Serial.println(user_id);
}

//GET /?userid=1&username=admin&userpassword=admin751486923&requesttype=add_user&name_user=Bojy&password_user=751486923@Aa&auth_user=31& HTTP/1.1
Request_add_user::Request_add_user(Request* _super):Request(_super){
    get_added_user();
}

State* Request_add_user::perform_Request(State * _state){
    return _state->add_user(this);
}

void Request_add_user::get_added_user(){
    m_added_user.set_name(get_Pair_Of(m_str_request, "&name_user=", "&"));
    Serial.print("added user name = ");Serial.println(m_added_user.get_name());
    m_added_user.set_password(get_Pair_Of(m_str_request, "&password_user=", "&"));
    Serial.print("added User Password = ");Serial.println(m_added_user.get_password());
    int auth = 0;
    sscanf(get_Pair_Of(m_str_request, "&auth_user=", "&"), "%d", &auth);
    m_added_user.set_auth_level(auth);
    Serial.print("added user authontication = ");Serial.println(m_added_user.get_auth_level());
}
//GET /?userid=1&username=admin&userpassword=admin751486923&requesttype=remove_user&removed_user_id=10& HTTP/1.1
Request_remove_user::Request_remove_user(Request* _super):Request(_super){
    get_removed_user();
}

State* Request_remove_user::perform_Request(State * _state){
    return _state->remove_user(this);
}

void Request_remove_user::get_removed_user(){
    int removed_user_id = 0;
    sscanf(get_Pair_Of(m_str_request, "&removed_user_id=", "&"), "%d", &removed_user_id);
    m_removed_user = removed_user_id;
}
//GET /?userid=1&username=admin&userpassword=admin751486923&requesttype=edit_user&id_user=10&name_user=Bojy&password_user=751486923@Aa&auth_user=31& HTTP/1.1
Request_edit_user::Request_edit_user(Request* _super):Request(_super){
    get_edited_user();
}

State* Request_edit_user::perform_Request(State * _state){
    return _state->edit_user(this);
}

void Request_edit_user::get_edited_user(){
    int edited_user_id = 0;
    sscanf(get_Pair_Of(m_str_request, "&id_user=", "&"), "%d", &edited_user_id);
    m_edited_user.set_id(edited_user_id);
    m_edited_user.set_name(get_Pair_Of(m_str_request, "&name_user=", "&"));
    Serial.print("added user name = "); Serial.println(m_edited_user.get_name());
    m_edited_user.set_password(get_Pair_Of(m_str_request, "&password_user=", "&"));
    Serial.print("added User Password = "); Serial.println(m_edited_user.get_password());
    int auth = 0;
    sscanf(get_Pair_Of(m_str_request, "&auth_user=", "&"), "%d", &auth);
    m_edited_user.set_auth_level(auth);
    Serial.print("added user authontication = "); Serial.println(m_edited_user.get_auth_level());
}
//GET /?userid=1&username=admin&userpassword=admin751486923&requesttype=get_users& HTTP/1.1
Request_get_users::Request_get_users(Request* _super):Request(_super){}

State* Request_get_users::perform_Request(State* _state){
    return _state->get_Users(this);
}
//GET /?userid=1&username=admin&userpassword=admin751486923&requesttype=add_peripheral&peripheraltype=1&peripheralrx=4&peripheraltx=2&timing=15000& HTTP/1.1
//GET /?userid=1&username=admin&userpassword=admin751486923&requesttype=add_peripheral&peripheraltype=0&peripheraluart=2&timing=15000& HTTP/1.1
Request_add_peripheral::Request_add_peripheral(Request* _super):Request(_super){
    get_peripheral();
}

State* Request_add_peripheral::perform_Request(State* _state){
    return _state->add_peripheral(this);
}

void Request_add_peripheral::get_peripheral(){
    sscanf(get_Pair_Of(m_str_request, "&peripheraltype=", "&"), "%d", &m_peripheral_type);
    sscanf(get_Pair_Of(m_str_request, "&timing=", "&"), "%d", &m_peripheral_time);
    if(m_peripheral_type == Peripheral::ROLLINGDOOR_TYPE){
        sscanf(get_Pair_Of(m_str_request, "&peripheralrx=", "&"), "%d", &m_peripheral_rx);
        sscanf(get_Pair_Of(m_str_request, "&peripheraltx=", "&"), "%d", &m_peripheral_tx);
    }else if(m_peripheral_type == Peripheral::FINGERPRINT_TYPE){
        int peripheral_uart_num = 0;
        sscanf(get_Pair_Of(m_str_request, "&peripheraluart=", "&"), "%d", &peripheral_uart_num);
        if(peripheral_uart_num == 2){
            m_peripheral_rx = 6;
            m_peripheral_tx = 7;
        }else if(peripheral_uart_num == 0){
            m_peripheral_rx = 12;
            m_peripheral_tx = 13;
        }
    }else{
        m_peripheral_type = 0;
        m_peripheral_rx = 0;
        m_peripheral_tx = 0;
        m_peripheral_time = 0;
    }
    Serial.print("peripheral type = ");Serial.println(m_peripheral_type);
    Serial.print("peripheral RX = ");Serial.println(m_peripheral_rx);
    Serial.print("peripheral TX = ");Serial.println(m_peripheral_tx);
    Serial.print("peripheral Timeing = ");Serial.println(m_peripheral_time);
}
//GET /?userid=1&username=admin&userpassword=admin751486923&requesttype=edit_peripheral&peripheralid=1& HTTP/1.1
Request_edit_peripheral::Request_edit_peripheral(Request* _super):Request(_super){
    get_peripheral();
}

State* Request_edit_peripheral::perform_Request(State* _state){
    return _state->edit_peripheral(this);
}

void Request_edit_peripheral::get_peripheral(){
    sscanf(get_Pair_Of(m_str_request, "&peripheraltype=", "&"), "%d", &m_peripheral_type);
    sscanf(get_Pair_Of(m_str_request, "&peripheralid=", "&"), "%d", &m_peripheral_id);
    sscanf(get_Pair_Of(m_str_request, "&timing=", "&"), "%d", &m_peripheral_time);
    if(m_peripheral_type == Peripheral::ROLLINGDOOR_TYPE){
        sscanf(get_Pair_Of(m_str_request, "&peripheralrx=", "&"), "%d", &m_peripheral_rx);
        sscanf(get_Pair_Of(m_str_request, "&peripheraltx=", "&"), "%d", &m_peripheral_tx);
    }else if(m_peripheral_type == Peripheral::FINGERPRINT_TYPE){
        int peripheral_uart_num = 0;
        sscanf(get_Pair_Of(m_str_request, "&peripheraluart=", "&"), "%d", &peripheral_uart_num);
        if(peripheral_uart_num == 2){
            m_peripheral_rx = 6;
            m_peripheral_tx = 7;
        }else if(peripheral_uart_num == 0){
            m_peripheral_rx = 12;
            m_peripheral_tx = 13;
        }
    }
}
//GET /?userid=1&username=admin&userpassword=admin751486923&requesttype=remove_peripheral&peripheralid=1& HTTP/1.1
Request_remove_peripheral::Request_remove_peripheral(Request* _super):Request(_super){
    get_peripheral_id();
}

State* Request_remove_peripheral::perform_Request(State* _state){
    return _state->remove_peripheral(this);
}

void Request_remove_peripheral::get_peripheral_id(){
    int peripheral_id = 0;
    sscanf(get_Pair_Of(m_str_request, "&peripheralid=", "&"), "%d", &peripheral_id);
    m_peripheral_id = peripheral_id;
}
//GET /?userid=1&username=admin&userpassword=admin&requesttype=get_peripherals& HTTP/1.1
Request_get_peripherals::Request_get_peripherals(Request* _super):Request(_super){}

State* Request_get_peripherals::perform_Request(State* _state){
    return _state->get_peripheral(this);
}
//GET /?userid=1&username=admin&userpassword=admin751486923&requesttype=get_rolling_state&peripheralid=1& HTTP/1.1
Request_Rolling_State::Request_Rolling_State(Request* _super):Request(_super){
    get_rolling_id();
}

State* Request_Rolling_State::perform_Request(State* _state){
    return _state->get_Rolling_State(this);
}

void Request_Rolling_State::get_rolling_id(){
    sscanf(get_Pair_Of(m_str_request, "&peripheralid=", "&"), "%d", &m_rolling_id);
    Serial.print("The Rolling Door ID =  ");
    Serial.println(m_rolling_id);
}

//GET /?userid=1&username=admin&userpassword=admin751486923&requesttype=open_rolling&doorpercentage=70& HTTP/1.1
Request_Rolling_Open::Request_Rolling_Open(Request* _super):Request(_super){
    get_rolling_id();
    get_rolling_Percentage();
}

State* Request_Rolling_Open::perform_Request(State* _state){
    return _state->open_rolling_peripheral(this);
}

void Request_Rolling_Open::get_rolling_id(){
    sscanf(get_Pair_Of(m_str_request, "&peripheralid=", "&"), "%d", &m_rolling_id);
    Serial.print("The Rolling Door ID =  ");
    Serial.println(m_rolling_id);
}

void Request_Rolling_Open::get_rolling_Percentage(){
    sscanf(get_Pair_Of(m_str_request, "&doorpercentage=", "&"), "%d", &m_rolling_percentage);
    Serial.print("The Rolling Door ID ");
    Serial.print(m_rolling_id);
    Serial.print(" will be opened at percentage =  ");
    Serial.println(m_rolling_percentage);
}

//GET /?userid=1&username=admin&userpassword=admin751486923&requesttype=close_rolling& HTTP/1.1
Request_Rolling_Close::Request_Rolling_Close(Request* _super):Request(_super){
    get_rolling_id();
    get_rolling_percentage();
}

State* Request_Rolling_Close::perform_Request(State* _state){
    return _state->close_rolling_peripheral(this);
}

void Request_Rolling_Close::get_rolling_id(){
    sscanf(get_Pair_Of(m_str_request, "&peripheralid=", "&"), "%d", &m_rolling_id);
    Serial.print("The Rolling Door ID =  ");
    Serial.println(m_rolling_id);
}

void Request_Rolling_Close::get_rolling_percentage(){
    sscanf(get_Pair_Of(m_str_request, "&doorpercentage=", "&"), "%d", &m_rolling_percentage);
    Serial.print("The Rolling Door ID ");
    Serial.print(m_rolling_id);
    Serial.print(" will be opened at percentage =  ");
    Serial.println(m_rolling_percentage);
}

//GET /?userid=1&username=admin&userpassword=admin751486923&requesttype=get_updates& HTTP/1.1
Request_updates::Request_updates(Request* _super):Request(_super){

}

State* Request_updates::perform_Request(State* _state){
        return NULL;
}
//the application will send that request when the user logup or login in the app to ask for which peripherals he can control
Request_Accessabilty::Request_Accessabilty(Request* _super):Request(_super){

}

State* Request_Accessabilty::perform_Request(State* _state){
    return NULL;
}
//the application will send that request when the user logup or login in the app to ask for which his authontication
Request_authontication::Request_authontication(Request* _super):Request(_super){

}

State* Request_authontication::perform_Request(State* _state){
    return NULL;
}

Request_invalid::Request_invalid(Request* _super):Request(_super){

}

State* Request_invalid::perform_Request(State* _state){
    return NULL;
}

Request_void::Request_void(Request* _super):Request(_super){

}

State* Request_void::perform_Request(State* _state){
    return NULL;
}
