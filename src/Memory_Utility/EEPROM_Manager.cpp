#include <Arduino.h>
#include "EEPROM_Manager.h"
#include "../Connectivity_Utility/Connectivity_Manager.h"

void swapNums(uint8_t& first, uint8_t& sec){
    first ^= sec;
    sec ^= first;
    first ^= sec;
}

void sort_IDs(uint8_t* usersIDs_ptr, uint8_t usersNum){
    for(uint8_t j = 0; j < usersNum-1; j++){
        for(uint8_t i = j+1; i < usersNum; i++){
            if(usersIDs_ptr[j] > usersIDs_ptr[i]){
                swapNums(usersIDs_ptr[j], usersIDs_ptr[i]);
            }
        }
    }
}

EEPROM_Manager::EEPROM_Manager() {
  Serial.println("EEPROM Memory Instant Created...");
  EEPROM.begin(EEPROM_USED_SIZE);
  delay(100);
  Serial.print("INit Flage = ");
  Serial.println(EEPROM_Read(INITIALIZATION_FLAGE));
  if(EEPROM_Read(INITIALIZATION_FLAGE)!= 201){
      initialize_memory();
  }
}

uint8_t EEPROM_Manager::initialize_memory() {
  Serial.println("initialization start...");
  Serial.println("EEPROM start...");
  for (uint16_t i = 0; i < 120; i++)
  {
  	EEPROM_Write(i, 0);
  }
  for(uint16_t i = USERS_ARRAY_EEPROM + 42; i < (USERS_ARRAY_EEPROM + (MAX_USERS_NUMBER * USER_SIZE)); i += USER_SIZE){
    // Log("i = "); Log(i); Log(" -> "); Logln(EEPROM_Read(i));
    EEPROM_Write(i, 0);
  }
  set_AP_SSID("ESP32-AP");
  set_AP_password("0000");
  set_last_state(Connectivity_Manager::AP_MODE);
  User default_User(1, "admin", "admin", 255);
  set_User(default_User);
  EEPROM_Write(INITIALIZATION_FLAGE, 201);
  Serial.println("intialization ended.");
  return 0;
}

IPAddress EEPROM_Manager::get_localIP(){
return get_IPAdress(IPs_START_ADDRESS_EEPROM);
}

IPAddress EEPROM_Manager::get_gatewayIP(){
return get_IPAdress(IPs_START_ADDRESS_EEPROM + 4);
}

IPAddress EEPROM_Manager::get_subnetIP(){
return get_IPAdress(IPs_START_ADDRESS_EEPROM + 8);
}

IPAddress EEPROM_Manager::get_primaryDNS(){
return get_IPAdress(IPs_START_ADDRESS_EEPROM + 12);
}

IPAddress EEPROM_Manager::get_secondaryDNS(){
return get_IPAdress(IPs_START_ADDRESS_EEPROM + 16);
}

void EEPROM_Manager::set_localIP(const IPAddress & ip){
set_IPAdress(ip, IPs_START_ADDRESS_EEPROM);
}

void EEPROM_Manager::set_gatewayIP(const IPAddress & ip){
set_IPAdress(ip, IPs_START_ADDRESS_EEPROM + 4);
}

void EEPROM_Manager::set_subnetIP(const IPAddress &ip){
set_IPAdress(ip, IPs_START_ADDRESS_EEPROM + 8);
}

void EEPROM_Manager::set_primaryDNS(const IPAddress &ip){
set_IPAdress(ip, IPs_START_ADDRESS_EEPROM + 12);
}

void EEPROM_Manager::set_secondaryDNS(const IPAddress &ip){
set_IPAdress(ip, IPs_START_ADDRESS_EEPROM + 16);
}

std::string EEPROM_Manager::get_WIFI_SSID(){
return get_string(WIFI_SSID_EEPROM, MAX_ELEMENT_SIZE);
}

std::string EEPROM_Manager::get_WIFI_password(){
return get_string(WIFI_PASSWORD_EEPROM, MAX_ELEMENT_SIZE);
}

void EEPROM_Manager::set_WIFI_SSID(const std::string & wifi_ssid){
    set_string(wifi_ssid, WIFI_SSID_EEPROM, MAX_ELEMENT_SIZE);
}

void EEPROM_Manager::set_WIFI_password(const std::string & wifi_password){
set_string(wifi_password, WIFI_PASSWORD_EEPROM, MAX_ELEMENT_SIZE);
}

std::string EEPROM_Manager::get_AP_SSID(){
return get_string(AP_SSID_EEPROM, MAX_ELEMENT_SIZE);
}

std::string EEPROM_Manager::get_AP_password(){
return get_string(AP_PASSWORD_EEPROM, MAX_ELEMENT_SIZE);
}

void EEPROM_Manager::set_AP_SSID(const std::string & ap_ssid){
set_string(ap_ssid, AP_SSID_EEPROM, MAX_ELEMENT_SIZE);
}

void EEPROM_Manager::set_AP_password(const std::string & ap_password){
set_string(ap_password, AP_PASSWORD_EEPROM, MAX_ELEMENT_SIZE);
}

uint8_t EEPROM_Manager::get_last_state(){
return EEPROM_Read(ESP_MODE_ADDRESS_EEPROM);
}

void EEPROM_Manager::set_last_state(uint8_t state){
EEPROM_Write(ESP_MODE_ADDRESS_EEPROM, state);
}

bool EEPROM_Manager::set_User(User& user){
    if(user.get_id() != 0 && search_user_by_id(user.get_id()) == USERS_META_DATA_EEPROM){
        uint16_t address = get_available_User_address();
        if(address !=  USERS_META_DATA_EEPROM){
            Log("the user will stored in address = ");Logln(address);
            set_User(user, address);
            return true;
        }
    }
    Logln("User didnot added !!!!!!!!!!!!!!![E : 110]");
    return false;
}

bool EEPROM_Manager::delete_user(uint8_t id){
for(int i = 0; i < MAX_USERS_NUMBER; i++){
 if(id == EEPROM_Read(USERS_ARRAY_EEPROM + (i * USER_SIZE) + 42)){
   Serial.print("deleting user at index = ");
   Serial.println(i);
   update_Users_Meta_Data(USERS_ARRAY_EEPROM + (i * USER_SIZE), false);
   EEPROM_Write(USERS_ARRAY_EEPROM + (i * USER_SIZE) + 42, 0);
  return true;
 }
}
return false;
}

User* EEPROM_Manager::find_user(uint8_t id){
    for(int i = 0; i < MAX_USERS_NUMBER; i++){
        if(id == EEPROM_Read(USERS_ARRAY_EEPROM + (i * USER_SIZE) + 42)){
            Serial.print("found user at index = ");
            Serial.println(i);
            return get_User(i);
        }
    }
    User* dummy = new User();
    return dummy;
}

User* EEPROM_Manager::get_users(){
    User* users = new User[EEPROM_Read(USERS_META_DATA_EEPROM)];
    uint8_t byteNum;
    uint8_t bitNum;
    uint8_t metaByte;
    int i = 0;
    for(int index = 0; index < MAX_USERS_NUMBER; index++){
        byteNum = index / 8;
        bitNum = 7 - (index % 8);
        metaByte = EEPROM_Read(USERS_META_DATA_EEPROM + 1 + byteNum);
        if(((metaByte&(1<<bitNum))>>bitNum)){
            User* user = get_User(index);
            users[i++] = *user;
            delete user;
        }
    }
    return users;
}

int EEPROM_Manager::get_users_num(){
    return EEPROM_Read(USERS_META_DATA_EEPROM);
}

uint8_t EEPROM_Manager::get_available_User_id(){
    uint8_t users_number = EEPROM_Read(USERS_META_DATA_EEPROM);
    uint8_t* users_IDs = get_users_IDs();
    Serial.print("users number = ");Serial.println((int)users_number);
    Serial.print("users id = {");
    for(int i = 0; i < users_number-1; i++){
        Serial.print((int)users_IDs[i]);
        Serial.print(", ");
    }
    Serial.print((int)users_IDs[users_number-1]);
    Serial.println("}");
    //return 10;
    //make to variable to capture tow values that could be the needed id located in between them
    if(users_number == 1){
        return ++users_IDs[0];
    }
    sort_IDs(users_IDs, users_number);
    int i = 0;
    while(1){
        if(i == (users_number -1)){
            return users_IDs[i] + 1;
        }
        if(users_IDs[i] == (users_IDs[i+1] -1)){
            i++;
        }else{
            return users_IDs[i] + 1;
        }
    }
}

uint8_t* EEPROM_Manager::get_users_IDs(){
    Serial.println("------------------------------------------------------------------------");
    Serial.println("inside get users id function...");

    uint8_t* usersIDs = new uint8_t[EEPROM_Read(USERS_META_DATA_EEPROM)];
    uint8_t metaByte;
    uint8_t array_index = 0;
    for(int i = 0; i < 5; i++ ){
        metaByte = EEPROM_Read(USERS_META_DATA_EEPROM + 1 + i);
        Serial.print("first loop byte number = ");Serial.println(i);
        if(metaByte > 0){
            uint16_t address;
            for(uint8_t bitNum = 7; (7 >= bitNum && bitNum >= 0); bitNum--){
                Serial.print("second loop bit number = ");Serial.println(bitNum);
                if((metaByte & (1<<bitNum)) == (1<<bitNum)){
                    Serial.print("user found at bit number = ");Serial.print(bitNum);
                    Serial.print(", and byte number = ");Serial.println(metaByte);
                    address = (((i* 8) + 7 - bitNum) * USER_SIZE) + USERS_ARRAY_EEPROM + 42;
                    usersIDs[array_index++] = EEPROM_Read(address);
                }
            }
        }
    }
    return usersIDs;
}

uint8_t EEPROM_Manager::add_peripheral(int _peripheral_type, int _peripheral_rx, int _peripheral_tx, int _peripheral_time){
    uint8_t peripheral_ID = check_peripheral(_peripheral_rx, _peripheral_tx);
    if(peripheral_ID)
    {
        uint16_t address = get_available_Peripheral_address();
        if(address != PERIPHERALS_META_DATA_EEPROM){
            EEPROM_Write(address, peripheral_ID);
            EEPROM_Write(address + 1, _peripheral_type);
            EEPROM_Write(address + 2, _peripheral_rx);
            EEPROM_Write(address + 3, _peripheral_tx);
            EEPROM_Write_Integer(address + 4, _peripheral_time);
            update_Peripheral_Meta_Data(address, true);
            return peripheral_ID;//id;
        }
    }
    return 0;
}

bool EEPROM_Manager::edit_peripheral(int _peripheral_type, int _peripheral_id, int _peripheral_rx, int _peripheral_tx, int _peripheral_time){
    uint16_t address = find_peripheral(_peripheral_id);
    if(address != PERIPHERALS_META_DATA_EEPROM){
        EEPROM_Write(address + 1, _peripheral_type);
        EEPROM_Write(address + 2, _peripheral_rx);
        EEPROM_Write(address + 3, _peripheral_tx);
        EEPROM_Write_Integer(address + 4, _peripheral_time);
        return true;
    }
    return false;
}

bool EEPROM_Manager::remove_peripheral(uint8_t _id){
    uint16_t address = find_peripheral(_id);
    if(address != PERIPHERALS_META_DATA_EEPROM){
        EEPROM_Write(address, 0);
        EEPROM_Write(address + 1, 0);
        EEPROM_Write(address + 2, 0);
        EEPROM_Write(address + 3, 0);
        EEPROM_Write_Integer(address + 4, 0);
        return update_Peripheral_Meta_Data(address, false);
    }
    return false;
}

Peripheral* EEPROM_Manager::get_peripheral(uint8_t _id){
    uint16_t address = find_peripheral(_id);
    return get_peripheral_by_address(address);
}

Peripheral** EEPROM_Manager::get_peripherals(){
    const uint8_t peripherals_number = EEPROM_Read(PERIPHERALS_META_DATA_EEPROM);
    Peripheral** peripherals = new Peripheral*[(peripherals_number > 12) ? 12 : peripherals_number];
    uint8_t byteNum;
    uint8_t bitNum;
    uint8_t metaByte;
    uint16_t address;
    int i = 0;
    for(int index = 0; index < MAX_PERIPHERAL_NUM; index++){
        address = PERIPHERALS_ARRAY_EEPROM + (index * PERIPHERAL_SIZE);
        byteNum = index / 8;
        bitNum = 7 - (index % 8);
        metaByte = EEPROM_Read(PERIPHERALS_META_DATA_EEPROM + 1 + byteNum);
        if(((metaByte&(1<<bitNum))>>bitNum)){
            peripherals[i++] = get_peripheral_by_address(address);
            Serial.println("from get peripherals:");
            peripherals[i-1]->print();
        }
    }
    return peripherals;
}

uint8_t EEPROM_Manager::get_peripheral_Num(){
    return EEPROM_Read(PERIPHERALS_META_DATA_EEPROM);
}

uint16_t EEPROM_Manager::find_peripheral(uint8_t _id){
    uint16_t address;
    for(int i = 0; i <= MAX_PERIPHERAL_NUM; i += 1){
        address = PERIPHERALS_ARRAY_EEPROM + PERIPHERAL_SIZE * i;
        if(EEPROM_Read(address) == _id){
            return address;
        }
    }
    return PERIPHERALS_META_DATA_EEPROM;
}

void EEPROM_Manager::add_event(Event& _peripheral){
    uint16_t address = getEventAddress();
    EEPROM_Write(address, (uint8_t)_peripheral.m_id>>8);
    EEPROM_Write(address + 1, (uint8_t)_peripheral.m_id);
    EEPROM_Write(address + 2, _peripheral.m_user_id);
    EEPROM_Write(address + 3, _peripheral.m_peripheral_id);
    EEPROM_Write(address + 4, _peripheral.m_type);
    EEPROM_Write(address + 5, (uint8_t)(_peripheral.m_date_time.m_date_and_time >> 16));
    EEPROM_Write(address + 6, (uint8_t)(_peripheral.m_date_time.m_date_and_time >> 8));
    EEPROM_Write(address + 7, (uint8_t)_peripheral.m_date_time.m_date_and_time);
    uint16_t event_number = (EEPROM_Read(EVENTES_META_DATA_EEPROM)<<8)|(EEPROM_Read(EVENTES_META_DATA_EEPROM+1));
    event_number++;
    EEPROM_Write(EVENTES_META_DATA_EEPROM, (uint8_t)(event_number>>8));
    EEPROM_Write(EVENTES_META_DATA_EEPROM+1, (uint8_t)event_number);
}

Event* EEPROM_Manager::get_event(uint8_t _id){
    uint16_t address = find_event(_id);
    return get_event_by_address(address);
}

Event* EEPROM_Manager::get_event_by_address(uint16_t _address){
    Event* event = new Event();
    event->m_id = (uint16_t)(EEPROM_Read(_address)<<8);
    event->m_id = (event->m_id|EEPROM_Read(_address + 1));
    event->m_user_id = EEPROM_Read(_address + 2);
    event->m_peripheral_id = EEPROM_Read(_address + 3);
    event->m_type = EEPROM_Read(_address + 4);
    event->m_date_time.m_date_and_time = (EEPROM_Read(_address + 5)<<16);
    event->m_date_time.m_date_and_time = event->m_date_time.m_date_and_time|(EEPROM_Read(_address + 6)<<8);
    event->m_date_time.m_date_and_time = event->m_date_time.m_date_and_time|(EEPROM_Read(_address + 7)<<0);
    return event;
}

Event** EEPROM_Manager::get_events(){
    Event** events = new Event*[MAX_EVENTS_NUM];
    for(int i = 0; i < MAX_EVENTS_NUM; i++){
        events[i] = get_event_by_address(EVENTES_ARRAY_EEPROM + (EVENT_SIZE*i));
    }
    return events;
}

uint16_t EEPROM_Manager::find_event(uint16_t _id){
    uint16_t address;
    uint16_t id;
    for(int i = 0; i < MAX_EVENTS_NUM; i++){
        address = EVENTES_ARRAY_EEPROM +(EVENT_SIZE * i);
        id = (EEPROM_Read(address)<<8)|(EEPROM_Read(address + 1));
        if(_id == id){
            return address;
        }
    }
    return EVENTES_META_DATA_EEPROM;
}

uint8_t EEPROM_Manager::EEPROM_Read(uint16_t address){
    uint8_t data = EEPROM.read(address);
    data ^= 255;
    return data;
}

void EEPROM_Manager::EEPROM_Write(uint16_t address, uint8_t data){
    data ^= 255;
    EEPROM.write(address, data);
    EEPROM.commit();
}

int EEPROM_Manager::EEPROM_Read_Integer(uint16_t _address){
    int rtn =0;
    for(int i = 0; i < 4; i++){
        rtn += (EEPROM_Read(_address + i) << (i * 8));
    }
    return rtn;
}

void EEPROM_Manager::EEPROM_Write_Integer(uint16_t address, int data){
    for(int i = 0; i < 4; i++){
        EEPROM_Write(address + i, ((data>>(i*8)) & 255));
    }
}

void EEPROM_Manager::set_IPAdress(IPAddress ip, const uint16_t address ){
    // Serial.println("setting ip address in EEPROM ...");
    for(int i = 0; i < 4; i++){
      // Serial.print(ip[i]);Serial.print(".");
      EEPROM_Write(i + address, ip[i]);
      // Serial.print(EEPROM_Read(i + address));Serial.print(".");
    }
    // Serial.println("");
}

IPAddress EEPROM_Manager::get_IPAdress(const uint16_t address ){
    // Serial.println("getting ip address in EEPROM ...");
    uint8_t ip_Arr[4];
    for(int i = 0; i < 4; i++){
        ip_Arr[i] = EEPROM_Read(address + i);
        // Serial.print(ip_Arr[i]);Serial.print(".");
    }
    // Serial.println("");
    IPAddress ip(ip_Arr);
    return ip;
}

void EEPROM_Manager::set_string(const std::string str, const uint16_t address, const uint8_t size){
    for(int i = 0; i < size; i++){
        EEPROM_Write(i + address, str[i]);
            if(str[i] == 0){
            break;
        }
        if(i == (size - 2)){
            EEPROM_Write(i + address + 1, 0);
            break;
        }
    }
}

std::string EEPROM_Manager::get_string(const uint16_t address, const uint8_t size){
    std::string str;
    for(int i = 0; i < (size); i++ ){
        str += EEPROM_Read(address + i);
    }
    return str;
}

void EEPROM_Manager::set_User(User& user, const uint16_t address){
    set_string(user.get_name(), address, 21);
    EEPROM_Write(address + 20 , 0);

    set_string(user.get_password(), address + 21, 21);
    EEPROM_Write(address + 41 , 0);

    EEPROM_Write(address + 42 , user.get_id());

    EEPROM_Write(address + 43 , user.get_auth_level());

    EEPROM_Write(address + 44 , 0);

    update_Users_Meta_Data(address, true);
}

User* EEPROM_Manager::get_User(uint8_t index){
    User* user = new User();
    if(index < MAX_USERS_NUMBER ){
        uint16_t user_address = USERS_ARRAY_EEPROM + (index * USER_SIZE);
        std::string str =  get_string(user_address, 21);
        // Serial.println(str.c_str());
        user->set_name(str.c_str());
        user->set_password(get_string(user_address + 21, 21).c_str());
        user->set_id(EEPROM_Read(user_address + 42));
        user->set_auth_level(EEPROM_Read(user_address + 43));
    }
    return user;
}

uint16_t EEPROM_Manager::search_user_by_id(uint8_t id){
    for(int i = 0; i < MAX_USERS_NUMBER; i++){
        if(id == EEPROM_Read(USERS_ARRAY_EEPROM + (i * USER_SIZE) + 42)){
            Serial.print("found user at index = ");
            Serial.println(i);
            return USERS_ARRAY_EEPROM + (i * USER_SIZE);
        }
    }
    Serial.print("did not found user with id = ");Serial.println(id);
    return USERS_META_DATA_EEPROM;
}

void EEPROM_Manager::update_Users_Meta_Data(const uint16_t address, bool modeficationType){
    //update users meta data
    uint8_t byteNum = ((address - USERS_ARRAY_EEPROM)/ USER_SIZE) / 8;
    uint8_t bitNum = 7 - (((address - USERS_ARRAY_EEPROM)/ USER_SIZE) % 8);
    uint8_t modyfyByte = EEPROM_Read(USERS_META_DATA_EEPROM + 1 + byteNum);
    Log("byteNum = ");Logln(byteNum);
    Log("bit num = ");Logln(bitNum);
    Log("modefyByte before = ");Logln(modyfyByte);
    if(modeficationType){
        EEPROM_Write(USERS_META_DATA_EEPROM, EEPROM_Read(USERS_META_DATA_EEPROM) + 1);
        modyfyByte |= (1<<bitNum);
    }else{
        EEPROM_Write(USERS_META_DATA_EEPROM, EEPROM_Read(USERS_META_DATA_EEPROM) - 1);
        modyfyByte &= ((1<<bitNum)^255);
    }
    EEPROM_Write(USERS_META_DATA_EEPROM + 1 + byteNum, modyfyByte);
    Log("modefyByte after = ");Logln(modyfyByte);
}

bool EEPROM_Manager::update_Peripheral_Meta_Data(const uint16_t address, bool modeficationType){
    //update peripherals meta data
    if(address < (PERIPHERALS_ARRAY_EEPROM + (PERIPHERAL_SIZE * MAX_PERIPHERAL_NUM)) && address >= PERIPHERALS_ARRAY_EEPROM){
        uint8_t byteNum = ((address - PERIPHERALS_ARRAY_EEPROM)/ PERIPHERAL_SIZE) / 8;
        uint8_t bitNum = 7 - (((address - PERIPHERALS_ARRAY_EEPROM)/ PERIPHERAL_SIZE) % 8);
        uint8_t modyfyByte = EEPROM_Read(PERIPHERALS_META_DATA_EEPROM + 1 + byteNum);
        Log("byteNum = ");Logln(byteNum);
        Log("bit num = ");Logln(bitNum);
        Log("modefyByte before = ");Logln(modyfyByte);
        if(modeficationType){
            EEPROM_Write(PERIPHERALS_META_DATA_EEPROM, EEPROM_Read(PERIPHERALS_META_DATA_EEPROM) + 1);
            modyfyByte |= (1<<bitNum);
        }else{
            EEPROM_Write(PERIPHERALS_META_DATA_EEPROM, EEPROM_Read(PERIPHERALS_META_DATA_EEPROM) - 1);
            modyfyByte &= ((1<<bitNum)^255);
        }
        EEPROM_Write(PERIPHERALS_META_DATA_EEPROM + 1 + byteNum, modyfyByte);
        Log("modefyByte after = ");Logln(modyfyByte);
        return true;
    }else{
        return false;
    }
}

uint16_t EEPROM_Manager::get_available_User_address(){
    uint8_t modefyByte;
    for(int i = 0; i < 5; i++ ){
        modefyByte = EEPROM_Read(USERS_META_DATA_EEPROM + 1 + i);
        if(modefyByte < 255){
            uint16_t address;
            modefyByte ^= 255;//flipping the bits so the empty locations become ones and the occubed become zeros
            for(uint8_t bitNum = 7; (7 >= bitNum && bitNum >= 0); bitNum--){
                if((modefyByte & (1<<bitNum)) == (1<<bitNum)){
                    address = (((i* 8) + 7 - bitNum) * USER_SIZE) + USERS_ARRAY_EEPROM;
                    return address;
                }
            }
        }
    }
    return USERS_META_DATA_EEPROM;
}

uint16_t EEPROM_Manager::get_available_Peripheral_address(){
    uint8_t byteNum;
    uint8_t bitNum;
    uint8_t metaByte;
    for(int index = 0; index < MAX_PERIPHERAL_NUM; index++){
        byteNum = index / 8;
        bitNum = 7 - (index % 8);
        metaByte = EEPROM_Read(PERIPHERALS_META_DATA_EEPROM + 1 + byteNum);
        if(!((metaByte&(1<<bitNum))>>bitNum)){
            return PERIPHERALS_ARRAY_EEPROM + (index * PERIPHERAL_SIZE);
        }
    }
    return PERIPHERALS_META_DATA_EEPROM;
}

uint8_t EEPROM_Manager::check_peripheral(int _peripheral_rx, int _peripheral_tx){
    //check the availability of the peripheral pins
    uint8_t byteNum;
    uint8_t bitNum;
    uint8_t metaByte;
    uint8_t peripherals_number = EEPROM_Read(PERIPHERALS_META_DATA_EEPROM);
    uint8_t peripherals_IDs[peripherals_number];
    uint8_t IDs_index = 0;
    for(int index = 0; index < MAX_PERIPHERAL_NUM; index++){
        byteNum = index / 8;
        bitNum = 7 - (index % 8);
        metaByte = EEPROM_Read(PERIPHERALS_META_DATA_EEPROM + 1 + byteNum);
        if(((metaByte&(1<<bitNum))>>bitNum)){
            uint16_t address = PERIPHERALS_ARRAY_EEPROM + (index * PERIPHERAL_SIZE);
            peripherals_IDs[IDs_index++] = EEPROM_Read(address);
            uint8_t input_pin = EEPROM_Read(address + 2);
            uint8_t output_pin = EEPROM_Read(address + 3);
            if(_peripheral_rx == 0 || _peripheral_tx == 0 || _peripheral_rx == input_pin || _peripheral_rx == output_pin || _peripheral_tx == input_pin || _peripheral_tx == output_pin){
                return false;
            }
        }
    }
    // add available id to the peripheral object
    if(peripherals_number == 0){
        return 1;
    }
    sort_IDs(peripherals_IDs, peripherals_number);
    int i = 0;
    while(1){
        if(i == (peripherals_number -1)){
            return peripherals_IDs[i] + 1;
        }
        if(peripherals_IDs[i] == (peripherals_IDs[i+1] -1)){
            i++;
        }else{
            return  peripherals_IDs[i] + 1;
        }
    }
}

uint16_t EEPROM_Manager::getEventAddress(){
    uint16_t event_number = (EEPROM_Read(EVENTES_META_DATA_EEPROM)<<8)|(EEPROM_Read(EVENTES_META_DATA_EEPROM+1));
    uint16_t address = EVENTES_ARRAY_EEPROM+((event_number%MAX_EVENTS_NUM)*EVENT_SIZE);
    return address;
}

Peripheral* EEPROM_Manager::get_peripheral_by_address(uint16_t _address){
    uint8_t id = EEPROM_Read(_address);
    uint8_t type = EEPROM_Read(_address + 1);
    uint8_t input_pin = EEPROM_Read(_address + 2);
    uint8_t output_pin = EEPROM_Read(_address + 3);
    int time = EEPROM_Read_Integer(_address + 4);
    Peripheral* rtn_per = NULL;
    uint8_t peripheral_index = (_address - PERIPHERALS_ARRAY_EEPROM)/8;//the size of peripheral in memory = 8 Bytes
    if(type == Peripheral::FINGERPRINT_TYPE){
        rtn_per = new Fingerprint_Scanner(id, input_pin, output_pin, time, (input_pin == 6 && output_pin == 7)?new HardwareSerial(2): new HardwareSerial(0));
    }else if(type == Peripheral::ROLLINGDOOR_TYPE){
        rtn_per = new Rolling_Door(id, input_pin, output_pin, time);
    }else{
        Serial.println("the tybe of the peripheral is undefined");
        return rtn_per;
    }
    // uint8_t periph_type ;
    // if(peripheral_index < 4){//                                      0123 4567
    //     //the second half in the third byte of peripherals meta data 1111 1111
    //     periph_type = (EEPROM_Read(PERIPHERALS_META_DATA_EEPROM + 2) & (1<<(3-peripheral_index)))>>(3-peripheral_index);
    // }else{
    //     //the fourth byte of the peripheral meta data
    //     periph_type = (EEPROM_Read(PERIPHERALS_META_DATA_EEPROM + 3) & (11<<(3-peripheral_index)))>>(11-peripheral_index);
    // }
    //
    // if(!periph_type){// serial peripherol
    //     if(type == Peripheral::FINGERPRINT_TYPE)
    //         rtn_per = new Fingerprint_Scanner(id, input_pin, output_pin, time, (input_pin == 6 && output_pin == 7)?new HardwareSerial(2): new HardwareSerial(0));
    // }else{
    //     if(type == Peripheral::ROLLINGDOOR_TYPE)
    //         rtn_per = new Rolling_Door(id, input_pin, output_pin, time);
    // }
    Serial.println("from peripheral by address:");
    rtn_per->print();
    return rtn_per;
}
