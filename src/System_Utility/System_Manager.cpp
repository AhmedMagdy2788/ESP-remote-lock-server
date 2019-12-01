#include "System_Manager.h"
#include "State.h"
#include <sstream>

Requrests_Factory* Requrests_Factory::m_factory = NULL;
const uint8_t Peripheral::FINGERPRINT_TYPE = 0;
const uint8_t Peripheral::ROLLINGDOOR_TYPE = 1;

System_Manager::System_Manager(){
    m_memory = new EEPROM_Manager();

    string WIFI_SSID  =  m_memory->get_WIFI_SSID();
    string WIFI_password  =  m_memory->get_WIFI_password();
    WIFI_Profile* wifi_profile = new WIFI_Profile();
    wifi_profile->set_WIFI_SSID(WIFI_SSID.c_str());
    wifi_profile->set_WIFI_Password(WIFI_password.c_str());
    wifi_profile->set_Local_IP(m_memory->get_localIP());
    wifi_profile->set_gateway_IP(m_memory->get_gatewayIP());
    wifi_profile->set_subnet_IP(m_memory->get_subnetIP());
    wifi_profile->set_primaryDNS_IP(m_memory->get_primaryDNS());
    wifi_profile->set_secondryDNS_IP(m_memory->get_secondaryDNS());

    string str_AP_SSID = m_memory->get_AP_SSID();
    char* AP_SSID = new char[str_AP_SSID.length()];
    strcpy(AP_SSID, str_AP_SSID.c_str());
    Serial.print("AP SSID = ");Serial.println(AP_SSID);

    string str_AP_Password = m_memory->get_AP_password();
    char* AP_password = new char[str_AP_Password.length()];
    strcpy(AP_password, str_AP_Password.c_str());
    Serial.print("AP Password = ");Serial.println(AP_password);

    uint8_t Connection_Mode = m_memory->get_last_state();
    m_connection = new Connectivity_Manager(this, wifi_profile, AP_SSID, AP_password, Connection_Mode);

    m_console = new Console();
    m_http_manager = new HTTP_Manager(this);
    m_system_time = new DateTime();
    m_peripheralList = m_memory->get_peripherals();
    m_peripheral_Num = m_memory->get_peripheral_Num();
    Serial.print("peripherals number = ");Serial.println(m_peripheral_Num);
    for(int i = 0; i < m_peripheral_Num; i++){
        Serial.print("peripheral id = ");Serial.print(m_peripheralList[i]->m_id);
        Serial.print(", peripheral Type = ");Serial.print(m_peripheralList[i]->m_type);
        Serial.print(", peripheral Rx = ");Serial.print(m_peripheralList[i]->m_input_pin);
        Serial.print(", peripheral Tx = ");Serial.print(m_peripheralList[i]->m_output_pin);
        Serial.print(" and peripheral time = ");Serial.println(m_peripheralList[i]->m_fullTime_ms);
    }
    getTimeServer(2);
    m_Current_State = new Default_State(this);
}

void System_Manager::getTimeServer(int time_zone){
    if(m_connection->is_WIFI_Connected()){
        struct tm timeinfo;
        if(!getLocalTime(&timeinfo)){
            Serial.println("Failed to obtain time");
            // return "";
        }else{
            char output[80];
            strftime(output, 80, "dayName=%A,monthNum=%m,dayNum=%d,year=%Y,hour=%H,minute=%M,", &timeinfo);
            configTime(time_zone*3600, 0*3600, "pool.ntp.org", "time.nist.gov");
            std::string str_date_Time = std::string(output);
            Serial.println(str_date_Time.c_str());
            delete m_system_time;
            m_system_time = new DateTime(str_date_Time);
            m_system_time->print(m_console);
        }
    }else{
        Serial.println("wifi isn't connected!!!");
    }
}

void System_Manager::requestHandler(Request* _request){
    State* temp_state = m_Current_State->doAction(_request);
    delete m_Current_State;
    m_Current_State = temp_state;
    if(m_Current_State != NULL){
        requestHandler(_request);
    }else{
        delete _request;
        m_Current_State = new Default_State(this);
    }
}

void System_Manager::update_WIFI_Profile(WIFI_Profile * wifi_profile){
    m_memory->set_WIFI_SSID(wifi_profile->get_WIFI_SSID());
    m_memory->set_WIFI_password(wifi_profile->get_WIFI_Password());
    m_memory->set_localIP(wifi_profile->get_Local_ip());
    m_memory->set_gatewayIP(wifi_profile->get_gateway_ip());
    m_memory->set_subnetIP(wifi_profile->get_subnet_ip());
    m_memory->set_primaryDNS(wifi_profile->get_primaryDNS_ip());
    m_memory->set_secondaryDNS(wifi_profile->get_secondryDNS_ip());
}

bool System_Manager::initialize_wifi(WIFI_Profile* _wifi_profile) {
    Serial.print("initializing the wifi into ..");
    Serial.println(_wifi_profile->get_WIFI_SSID());
    delete m_connection->m_wifi_profile;
    m_connection->m_wifi_profile = _wifi_profile;
    if(m_connection->connect_to_WIFI()){
        update_WIFI_Profile(_wifi_profile);
        Serial.print("wifi intialized into ");
        Serial.println(_wifi_profile->get_WIFI_SSID());
        return true;
    }else{
        Serial.print("wifi intialization failed ");
        return false;
    }
}

bool System_Manager::enable_wifi(){
    return m_connection->enable_WIFI_Mode();
}

void System_Manager::disable_wifi(){
    m_connection->disable_WIFI_Mode();
}

void System_Manager::init_AP(char *SSID, char *password){
    m_connection->change_AP_Profile(SSID, password);
}

void System_Manager::enable_AP(){
    m_connection->enable_AP_Mode();
}

void System_Manager::disable_AP(){
    m_connection->disable_AP_Mode();
}

void System_Manager::update_AP_Profile(char *SSID, char *password){
    m_memory->set_AP_SSID(SSID);
    m_memory->set_AP_password(password);
}

void System_Manager::update_Connection_Mode(uint8_t mode){
    m_memory->set_last_state(mode);
}

uint8_t System_Manager::scan_New_User(){
    return 0;
}

uint8_t System_Manager::scan_to_Access(){
  return 0;
}

char* System_Manager::get_Updates(){
  return (char*)"";
}

bool System_Manager::add_new_user(User& user){
    user.set_id(m_memory->get_available_User_id());
    return m_memory->set_User(user);
}

bool System_Manager::remove_user(uint8_t user_id){
    return m_memory->delete_user(user_id);
}

bool System_Manager::edit_user(User& user){
    if(m_memory->delete_user(user.get_id())){
        m_memory->set_User(user);
        return true;
    }
    return false;
}

char* System_Manager::get_Users(){
    std::stringstream sstream_json_Users;
    sstream_json_Users << "[";
    int users_num = m_memory->get_users_num();
    Serial.print("number of users = ");Serial.println(users_num);
    User* users = m_memory->get_users();
    for(int i = 0; i < users_num; i++){
        users[i].set_printable(m_console);
        Serial.print("user in userslist = ");users[i].print();
        sstream_json_Users << "{\"userID\":";
        sstream_json_Users << (int)users[i].get_id();
        sstream_json_Users << ",\"userName\":\"";
        sstream_json_Users << std::string(users[i].get_name());
        sstream_json_Users << "\",\"auth\":";
        sstream_json_Users << (int)users[i].get_auth_level();
        sstream_json_Users << "},";
    }
    sstream_json_Users << "]";
    std::string str_json_Users = sstream_json_Users.str();
    char* json_Users = new char[str_json_Users.length()+ 1];
    strcpy(json_Users, str_json_Users.c_str());
  return json_Users;
}

uint8_t System_Manager::add_peripheral(int _peripheral_type, int _peripheral_rx, int _peripheral_tx, int _peripheral_time){
    uint8_t peripheral_ID = m_memory->add_peripheral(_peripheral_type, _peripheral_rx, _peripheral_tx, _peripheral_time);
    if(peripheral_ID == 0){
        return 0;
    }
    //create peripheral object and add it to system peripheral list
    if(_peripheral_type == Peripheral::ROLLINGDOOR_TYPE){
        m_peripheralList[m_peripheral_Num++] = new Rolling_Door(peripheral_ID, _peripheral_rx, _peripheral_tx, _peripheral_time);
    }else if(_peripheral_type == Peripheral::FINGERPRINT_TYPE){
        if(_peripheral_rx == 6 && _peripheral_tx == 7){
            m_peripheralList[m_peripheral_Num++]  = new Fingerprint_Scanner(peripheral_ID, _peripheral_rx, _peripheral_tx, _peripheral_time, new HardwareSerial(2));
        }else if(_peripheral_rx == 12 && _peripheral_tx == 13){
            m_peripheralList[m_peripheral_Num++]  = new Fingerprint_Scanner(peripheral_ID, _peripheral_rx, _peripheral_tx, _peripheral_time, new HardwareSerial(0));
        }
    }
    return peripheral_ID;
}

bool System_Manager::edit_peripheral(int _peripheral_type, int _peripheral_id, int _peripheral_rx, int _peripheral_tx, int _peripheral_time){
    if(m_memory->edit_peripheral(_peripheral_type, _peripheral_id, _peripheral_rx, _peripheral_tx, _peripheral_time)){
        //edit the peripheral object in system peripheral list
        update_Peripherals_List();
        return true;
    }
    return false;
}

bool System_Manager::remove_peripheral(int _peripheral_id){
    if(m_memory->remove_peripheral(_peripheral_id)){
        //remove the peripheral from system peripheral list
        update_Peripherals_List();
        return true;
    }
    return false;
}

void System_Manager::update_Peripherals_List(){
    for(int i = 0; i < m_peripheral_Num; i++){
        delete m_peripheralList[i];
    }
    m_peripheralList = m_memory->get_peripherals();
    m_peripheral_Num = m_memory->get_peripheral_Num();
}

char* System_Manager::get_peripherals(){
    std::stringstream sstream_json_peripherals;
    sstream_json_peripherals << "[";
    Serial.println("looping throw the peripherals list");
    for(int i = 0; i < m_peripheral_Num; i++){
        // peripherals[i].set_printable(m_console);
        // Serial.print("user in userslist = ");peripherals[i].print();
        sstream_json_peripherals << "{\"peripheral_ID\":";
        sstream_json_peripherals << (int)m_peripheralList[i]->m_id;
        sstream_json_peripherals << ",\"type\":";
        sstream_json_peripherals << (int)m_peripheralList[i]->m_type;
        sstream_json_peripherals << ",\"rx\":";
        sstream_json_peripherals << (int)m_peripheralList[i]->m_input_pin;
        sstream_json_peripherals << ",\"tx\":";
        sstream_json_peripherals << (int)m_peripheralList[i]->m_output_pin;
        sstream_json_peripherals << ",\"fulltime\":";
        sstream_json_peripherals << (int)m_peripheralList[i]->m_fullTime_ms;
        sstream_json_peripherals << "},";
    }
    sstream_json_peripherals << "]";
    Serial.println("finish looping throw the peripherals list");
    std::string str_json_peripherals = sstream_json_peripherals.str();
    char* json_peripherals = new char[str_json_peripherals.length()+ 1];
    strcpy(json_peripherals, str_json_peripherals.c_str());
    Serial.println("peripherals = ");Serial.println(json_peripherals);
  return json_peripherals;
}

int System_Manager::getPeripheral_index(uint8_t _peripheral_id){
    for(int i = 0; i < m_peripheral_Num; i++){
        if(m_peripheralList[i]->m_id == _peripheral_id){
            return i;
        }
    }
    return -1;
}

void System_Manager::open_Door(uint8_t percentage){
    m_console->print("door opend at ");
    m_console->println(percentage);
}

void System_Manager::close_Door(){

}

uint8_t System_Manager::get_Door_state(){
  return 0;
}
