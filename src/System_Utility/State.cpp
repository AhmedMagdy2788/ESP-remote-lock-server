#include "State.h"
#include "System_Manager.h"

State::State(System_Manager* _system):m_system(_system){}

State* State::init_wifi(Request_wifi_init* _request_wifi_init){
    //get the user authuntication to initialize the wifi or not
    if(_request_wifi_init->m_user->get_auth_to(User::MODIFY_WIFI_OR_AP_SETTINGS)){
        //user has th authontication to modify th wifi
        //make the system to initialize the wifi with the request wifi profile
        if(m_system->initialize_wifi(_request_wifi_init->m_wifi_profile)){
            //return Approved_State if the imitialization secceeded
            _request_wifi_init->client->print("initialization succeeded");
            return new Approved_State(m_system);
        }else{
            //or Not_Approved_State if failed
            _request_wifi_init->client->print("initialization failed");
            return new Not_Approved_State(m_system);
        }
    }else{
        //user has no authontication to initialize the wifi
        _request_wifi_init->client->print("user has no authontication");
        return new Not_Approved_State(m_system);
    }
}

State* State::enable_wifi(Request_wifi_enable* _request){
    if(_request->m_user->get_auth_to(User::MODIFY_WIFI_OR_AP_SETTINGS)){
        m_system->enable_wifi();
        return new Approved_State(m_system);
    }else{
        //user has no authontication to initialize the wifi
        _request->client->print("user has no authontication");
        return new Not_Approved_State(m_system);
    }
}

State* State::disable_wifi(Request_wifi_disable* _request){
    if(_request->m_user->get_auth_to(User::MODIFY_WIFI_OR_AP_SETTINGS)){
        m_system->disable_wifi();
        return new Approved_State(m_system);
    }else{
        //user has no authontication to initialize the wifi
        _request->client->print("user has no authontication");
        return new Not_Approved_State(m_system);
    }
}

State* State::init_AP(Request_ap_init* _request){
    if(_request->m_user->get_auth_to(User::MODIFY_WIFI_OR_AP_SETTINGS)){
        m_system->init_AP(_request->str_AP_SSID, _request->str_AP_Password);
        return new Approved_State(m_system);
    }else{
        //user has no authontication to initialize the wifi
        _request->client->print("user has no authontication");
        return new Not_Approved_State(m_system);
    }
}

State* State::enable_AP(Request_ap_enable* _request){
    if(_request->m_user->get_auth_to(User::MODIFY_WIFI_OR_AP_SETTINGS)){
        m_system->enable_AP();
        return new Approved_State(m_system);
    }else{
        //user has no authontication to initialize the wifi
        _request->client->print("user has no authontication");
        return new Not_Approved_State(m_system);
    }
}

State* State::disable_AP(Request_ap_disable* _request){
    if(_request->m_user->get_auth_to(User::MODIFY_WIFI_OR_AP_SETTINGS)){
        m_system->disable_AP();
        return new Approved_State(m_system);
    }else{
        //user has no authontication to initialize the wifi
        _request->client->print("user has no authontication to perform this request");
        return new Not_Approved_State(m_system);
    }
}

State* State::server_accesse(Request_Server_Access* _request){
    _request->client->print("server_approved");
    Serial.println("server_approved");
    return new Approved_State(m_system);
}

State* State::add_user(Request_add_user* _request){
    if(_request->m_user->get_auth_to(User::ADD_REMOVE_USERS)){
        if(m_system->add_new_user(_request->m_added_user)){
            //_request->client->print("new user id = ");
            _request->client->print("new user id = ");
            _request->client->print((int)_request->m_added_user.get_id());
            Serial.print("new user id = ");Serial.println((int)_request->m_added_user.get_id());
            return new Approved_State(m_system);
        }else{
            _request->client->print("full users");
            Serial.println("the memory is full users");
            return new Not_Approved_State(m_system);
        }
    }else{
        //user has no authontication to initialize the wifi
        _request->client->print("user has no authontication to add new user");
        Serial.println("user has no authontication to add new user");
        return new Not_Approved_State(m_system);
    }
}

State* State::remove_user(Request_remove_user* _request){
    if(_request->m_user->get_auth_to(User::ADD_REMOVE_USERS)){
        if(m_system->remove_user(_request->m_removed_user)){
            //_request->client->print("new user id = ");
            _request->client->print("userID \"");
            _request->client->print((int)_request->m_removed_user);
            _request->client->print("\" removed");
            Serial.print("userID \" ");
            Serial.print((int)_request->m_removed_user);
            Serial.print("\" removed");
            return new Approved_State(m_system);
        }else{
            _request->client->print("did not found user");
            Serial.println("did not found user");
            return new Not_Approved_State(m_system);
        }
    }else{
        //user has no authontication to initialize the wifi
        _request->client->print("user has no authontication to remove users");
        Serial.println("user has no authontication to remove users");
        return new Not_Approved_State(m_system);
    }
}

State* State::edit_user(Request_edit_user* _request){
    if(_request->m_user->get_auth_to(User::ADD_REMOVE_USERS)){
        if(m_system->edit_user(_request->m_edited_user)){
            //_request->client->print("new user id = ");
            _request->client->print("userID \"");
            _request->client->print((int)_request->m_edited_user.get_id());
            _request->client->print("\" Edited");
            Serial.print("userID \" ");
            Serial.print((int)_request->m_edited_user.get_id());
            Serial.print("\" Edited");
            return new Approved_State(m_system);
        }else{
            _request->client->print("did not found user");
            Serial.println("did not found user");
            return new Not_Approved_State(m_system);
        }
    }else{
        //user has no authontication to initialize the wifi
        _request->client->print("user has no authontication to edit users");
        Serial.println("user has no authontication to edit users");
        return new Not_Approved_State(m_system);
    }
}

State* State::get_Users(Request_get_users* _request){
    if(_request->m_user->get_auth_to(User::ADD_REMOVE_USERS)){
        char* responce = m_system->get_Users();
        _request->client->print(responce);
        Serial.print("users = ");Serial.println(responce);
        delete[] responce;
        return new Approved_State(m_system);
    }else{
        //user has no authontication to initialize the wifi
        _request->client->print("user has no authontication to get users information");
        return new Not_Approved_State(m_system);
    }
}

State* State::add_peripheral(Request_add_peripheral* _request){
    if(_request->m_user->get_auth_to(User::ADD_REMOVE_PERIPHERALS)){
        uint8_t peripheral_ID = m_system->add_peripheral(_request->m_peripheral_type, _request->m_peripheral_rx, _request->m_peripheral_tx, _request->m_peripheral_time);
        if(peripheral_ID != 0){
            _request->client->print(_request->m_peripheral_type);
            _request->client->print(" has been added with id = ");
            _request->client->print(peripheral_ID);
            _request->client->print(", Rx = ");
            _request->client->print(_request->m_peripheral_rx);
            _request->client->print(", Tx = ");
            _request->client->print(_request->m_peripheral_tx);
            _request->client->print(" and timing = ");
            _request->client->print(_request->m_peripheral_time);

            Serial.print(_request->m_peripheral_type);
            Serial.print(" has been added with id = ");
            Serial.print(peripheral_ID);
            Serial.print(", Rx = ");
            Serial.print(_request->m_peripheral_rx);
            Serial.print(", Tx = ");
            Serial.print(_request->m_peripheral_tx);
            Serial.print("and timing = ");
            Serial.println(_request->m_peripheral_time);
            return new Approved_State(m_system);
        }else{
            _request->client->print("can not add this peripheral even for no space for new one or it conflict with existing peripheral pins");
            Serial.println("can not add this peripheral even for no space for new one or it conflict with existing peripheral pins");
            return new Not_Approved_State(m_system);
        }
    }else{
        _request->client->print("user has no authontication to add peripheral");
        return new Not_Approved_State(m_system);
    }
}

State* State::edit_peripheral(Request_edit_peripheral* _request){
    if(_request->m_user->get_auth_to(User::ADD_REMOVE_PERIPHERALS)){
        if(m_system->edit_peripheral(_request->m_peripheral_type, _request->m_peripheral_id, _request->m_peripheral_rx, _request->m_peripheral_tx, _request->m_peripheral_time)){
            _request->client->print("peripheral with id  = ");
            _request->client->print(_request->m_peripheral_id);
            _request->client->print(" has been edited with type = ");
            _request->client->print(_request->m_peripheral_type);
            _request->client->print(" , Rx = ");
            _request->client->print(_request->m_peripheral_rx);
            _request->client->print(", Tx = ");
            _request->client->print(_request->m_peripheral_tx);
            _request->client->print("and timing = ");
            _request->client->print(_request->m_peripheral_time);

            Serial.print("peripheral with id  = ");
            Serial.print(_request->m_peripheral_id);
            Serial.print(" has been edited with type = ");
            Serial.print(_request->m_peripheral_type);
            Serial.print(" , Rx = ");
            Serial.print(_request->m_peripheral_rx);
            Serial.print(", Tx = ");
            Serial.print(_request->m_peripheral_tx);
            Serial.print("and timing = ");
            Serial.println(_request->m_peripheral_time);
            return new Approved_State(m_system);
        }else{
            _request->client->print("can not edit this peripheral even for no existing peripheral with this id or it conflict with existing peripheral pins");
            Serial.println("can not edit this peripheral even for no existing peripheral with this id or it conflict with existing peripheral pins");
            return new Not_Approved_State(m_system);
        }
    }else{
        _request->client->print("user has no authontication to edit peripheral");
        return new Not_Approved_State(m_system);
    }
}

State* State::remove_peripheral(Request_remove_peripheral* _request){
    if(_request->m_user->get_auth_to(User::ADD_REMOVE_PERIPHERALS)){
        if(m_system->remove_peripheral(_request->m_peripheral_id)){
            _request->client->print("peripheral with id  = ");
            _request->client->print(_request->m_peripheral_id);
            _request->client->print(" has been removed");

            Serial.print("peripheral with id  = ");
            Serial.print(_request->m_peripheral_id);
            Serial.println(" has been removed");
            return new Approved_State(m_system);
        }else{
            _request->client->print("there is no existing peripheral with this id ");
            Serial.println("there is no existing peripheral with this id ");
            return new Not_Approved_State(m_system);
        }
    }else{
        _request->client->print("user has no authontication to remove peripheral");
        Serial.print("user has no authontication to remove peripheral");
        return new Not_Approved_State(m_system);
    }
}

State* State::get_peripheral(Request_get_peripherals* _request){
    if(_request->m_user->get_auth_to(User::USING_PERIPHERALS)){
        char* responce = m_system->get_peripherals();
        _request->client->print(responce);
        Serial.print("peripherals = ");Serial.println(responce);
        delete[] responce;
        return new Approved_State(m_system);
    }else{
        //user has no authontication to initialize the wifi
        _request->client->print("user has no authontication to get peripheral information");
        return new Not_Approved_State(m_system);
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////
State* State::get_Rolling_State(Request_Rolling_State* _request){
    if(_request->m_user->get_auth_to(User::USING_PERIPHERALS)){
        uint8_t peripheral_index = m_system->getPeripheral_index(_request->m_rolling_id);
        Rolling_Door* rolling_peripheral =
            (Rolling_Door*)m_system->m_peripheralList[peripheral_index];
        rolling_peripheral->get_Rolling_Door_State();
        _request->client->print("Door percentage = ");
        _request->client->print(rolling_peripheral->m_percentage);
        Serial.print("Door percentage = ");Serial.println(rolling_peripheral->m_percentage);
        return new Approved_State(m_system);
    }else{
        //user has no authontication to initialize the wifi
        _request->client->print("user has no authontication to get peripheral state");
        Serial.println("user has no authontication to get peripheral state");
        return new Not_Approved_State(m_system);
    }
}
/*
1-in the beginning the peripheral will be in the Configured_State
2- if the peripheral get open request it will make the open pin HIGH and start the timer with time equivalent to the requested percentage and make the state opening_state
3- when the timer rech the end it will make the open pen LOW and regestered the percentage in the peripheral object
*/
State* State::open_rolling_peripheral(Request_Rolling_Open* _request){
    if(_request->m_user->get_auth_to(User::USING_PERIPHERALS)){
        //get the peripheral from system peripheral list
        uint8_t peripheral_index =
            m_system->getPeripheral_index(_request->m_rolling_id);
        Rolling_Door* rolling_peripheral =
            (Rolling_Door*)m_system->m_peripheralList[peripheral_index];
        rolling_peripheral->print();
        //perform the request to open the door
        if(rolling_peripheral->open_door(_request->m_rolling_percentage) == State_Opening || rolling_peripheral->open_door(_request->m_rolling_percentage) == State_Adjested_Open){
            _request->client->print("Door is Opening to percentage = ");
            _request->client->print(_request->m_rolling_percentage);
            Serial.print("Door is Opening to percentage = ");
            Serial.println(_request->m_rolling_percentage);
            return new Approved_State(m_system);
        }else {
            _request->client->print("incorrect percentage to open, close the door to rech the requesred percentage");
            Serial.println("incorrect percentage to open, close the door to rech the requesred percentage");
            return new Not_Approved_State(m_system);
        }
    }else{
        _request->client->print("user has no authontication to use this peripheral");
        Serial.println("user has no authontication to use this peripheral");
        return new Not_Approved_State(m_system);
    }
}

State* State::close_rolling_peripheral(Request_Rolling_Close* _request){
    if(_request->m_user->get_auth_to(User::USING_PERIPHERALS)){
        //get the peripheral from system peripheral list
        uint8_t peripheral_index =
            m_system->getPeripheral_index(_request->m_rolling_id);
        Rolling_Door* rolling_peripheral =
            (Rolling_Door*)m_system->m_peripheralList[peripheral_index];
        rolling_peripheral->print();
        //perform the request to close the door
        if(rolling_peripheral->close_door(_request->m_rolling_percentage) == State_Closing || rolling_peripheral->open_door(_request->m_rolling_percentage) == State_Adjested_Close){
            _request->client->print("Door is Closing to percentage = ");
            _request->client->print(_request->m_rolling_percentage);
            Serial.print("Door is Closing to percentage = ");
            Serial.println(_request->m_rolling_percentage);
            return new Approved_State(m_system);
        }else{
            _request->client->print("incorrect percentage to close, open the door to rech the requesred percentage");
            Serial.println("incorrect percentage to close, open the door to rech the requesred percentage");
            return new Not_Approved_State(m_system);
        }
    }else{
        _request->client->print("user has no authontication to use this peripheral");
        Serial.println("user has no authontication to use this peripheral");
        return new Not_Approved_State(m_system);
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////
Default_State::Default_State(System_Manager* _system):State(_system){}
State* Default_State::doAction(Request* _request){
    if(_request->request_is_done){return NULL;}
    if(_request->vaild_request){
        User *mem_User = m_system->m_memory->find_user(_request->m_User_ID);
        mem_User->set_printable(m_system->m_console);
        mem_User->print();
        if (mem_User->get_id() != 255 && strcmp(mem_User->get_name(), _request->m_User_Name) == 0 && strcmp(mem_User->get_password(), _request->m_User_Password) == 0){
            _request->m_user = mem_User;
            return new Match_State(m_system);
        }else{
            delete mem_User;
            _request->request_is_done = true;
            return new Not_Match_State(m_system);
        }
    }else{
        _request->request_is_done = true;
        return new Invalid_Request_State(m_system);
    }
    /*
        request_type = m_request->m_Request_Type;
        Serial.println(request_type);

    */
    return new Void_State(m_system);
}

Match_State::Match_State(System_Manager* _system):State(_system){}
State* Match_State::doAction(Request* _request){
    if (m_system->m_connection->get_Connectivity_Mode() == m_system->m_connection->WIFI_MODE){
        if (strcmp(_request->m_Request_Type, Request::REQUEST_WIFI_DISABLE) == 0)
        {
            m_system->m_console->println("can not disable WIFI while AP is disable");
            return new Not_Approved_State(m_system);
        }
    }else if (m_system->m_connection->get_Connectivity_Mode() == m_system->m_connection->AP_MODE){
        if (strcmp(_request->m_Request_Type, Request::REQUEST_AP_DISABLE) == 0)
        {
            m_system->m_console->println("can not disable AP while WIFI is disable");
            return new Not_Approved_State(m_system);
        }
    }
    return _request->perform_Request(this);
}

Not_Match_State::Not_Match_State(System_Manager* _system):State(_system){}
State* Not_Match_State::doAction(Request* _request){
    _request->client->print("user_not_exist");
    m_system->m_console->println("user_not_exist");
    return NULL;
}

Invalid_Request_State::Invalid_Request_State(System_Manager* _system):State(_system){}
State* Invalid_Request_State::doAction(Request* _request){
    _request->client->print(Request::REQUEST_INVALID);
    m_system->m_console->println(Request::REQUEST_INVALID);
    return NULL;
}

Server_Access_State::Server_Access_State(System_Manager* _system):State(_system){}
State* Server_Access_State::doAction(Request* _request){
    _request->client->print("server_approved");
    m_system->m_console->println("server_approved");
    _request->request_is_done = true;
    return new Default_State(m_system);
}

Approved_State::Approved_State(System_Manager* _system):State(_system){}
State* Approved_State::doAction(Request* _request){
    //create event object and store the request user and the action that happend in memory
    return NULL;
}

Not_Approved_State::Not_Approved_State(System_Manager* _system):State(_system){}
State* Not_Approved_State::doAction(Request* _request){
    //create event object and store the request user and the not approved notation in memory
    return NULL;
}

Update_State::Update_State(System_Manager* _system):State(_system){}
State* Update_State::doAction(Request* _request){
    char *updates = m_system->get_Updates();
    _request->client->print("here is the last update eventes :DDD");
    _request->client->print(updates);
    m_system->m_console->println(updates);
    _request->request_is_done = true;
    return new Default_State(m_system);
}

Users_State::Users_State(System_Manager* _system):State(_system){}
State* Users_State::doAction(Request* _request){
    char *users = m_system->get_Users();
    _request->client->print("here is the user regestered list");
    _request->client->print(users);
    m_system->m_console->println(users);
    _request->request_is_done = true;
    return new Default_State(m_system);
}

Change_Ip_State::Change_Ip_State(System_Manager* _system):State(_system){}
State* Change_Ip_State::doAction(Request* _request){
    Request_change_ip* cast_request = (Request_change_ip*) _request;
    if (m_system->m_connection->add_WIFI_Profile(cast_request->m_wifi_profile))
    {
        if (m_system->m_connection->connect_to_WIFI())
        {
            string responce = "wifi conected to ";
            responce += m_system->m_connection->m_wifi_profile->get_WIFI_SSID();
            _request->client->print(responce.c_str());
            m_system->m_console->print(responce.c_str());
        }
        else
        {
            _request->client->print("server connection failed");
            m_system->m_console->println("server connection failed");
        }
    }
    _request->request_is_done = true;
    return new Default_State(m_system);
}

AP_Enable_State::AP_Enable_State(System_Manager* _system):State(_system){}
State* AP_Enable_State::doAction(Request* _request){
    m_system->m_connection->enable_AP_Mode();
    _request->client->print("AP_Enabled");
    m_system->m_console->println("AP_Enabled");
    _request->request_is_done = true;
    return new Default_State(m_system);
}

AP_Disable_state::AP_Disable_state(System_Manager* _system):State(_system){}
State* AP_Disable_state::doAction(Request* _request){
    m_system->m_connection->disable_AP_Mode();
    _request->client->print("AP_Disabled");
    m_system->m_console->println("AP_Disabled");
    _request->request_is_done = true;
    return new Default_State(m_system);
}

WIFI_Enable_State::WIFI_Enable_State(System_Manager* _system):State(_system){}
State* WIFI_Enable_State::doAction(Request* _request){
    m_system->m_connection->connect_to_WIFI();
    _request->client->print("WIFI_Connected");
    m_system->m_console->println("WIFI_Connected");
    _request->request_is_done = true;
    return new Default_State(m_system);
}

WIFI_Disable_State::WIFI_Disable_State(System_Manager* _system):State(_system){}
State* WIFI_Disable_State::doAction(Request* _request){
    m_system->m_connection->disable_WIFI_Mode();
    _request->client->print("WIFI Disabled");
    m_system->m_console->println("WIFI Disabled");
    _request->request_is_done = true;
    return new Default_State(m_system);
}

Accessabilty_State::Accessabilty_State(System_Manager* _system):State(_system){}
State* Accessabilty_State::doAction(Request* _request){
    return NULL;
}

Authontication_State::Authontication_State(System_Manager* _system):State(_system){}
State* Authontication_State::doAction(Request* _request){
    return NULL;
}

No_Access_State::No_Access_State(System_Manager* _system):State(_system){}
State* No_Access_State::doAction(Request* _request){
    return NULL;
}

Delete_State::Delete_State(System_Manager* _system):State(_system){}
State* Delete_State::doAction(Request* _request){
    return NULL;
}

AP_Approvale_State::AP_Approvale_State(System_Manager* _system):State(_system){}
State* AP_Approvale_State::doAction(Request* _request){
    return NULL;
}

AP_Not_Approvale_State::AP_Not_Approvale_State(System_Manager* _system):State(_system){}
State* AP_Not_Approvale_State::doAction(Request* _request){
    return NULL;
}

Fail_Config_State::Fail_Config_State(System_Manager* _system):State(_system){}
State* Fail_Config_State::doAction(Request* _request){
    return NULL;
}

Fail_Connect_State::Fail_Connect_State(System_Manager* _system):State(_system){}
State* Fail_Connect_State::doAction(Request* _request){
    return NULL;
}

AP_Init_State::AP_Init_State(System_Manager* _system):State(_system){}
State* AP_Init_State::doAction(Request* _request){
    return NULL;
}

Server_Init_State::Server_Init_State(System_Manager* _system):State(_system){}
State* Server_Init_State::doAction(Request* _request){
    return NULL;
}

Void_State::Void_State(System_Manager* _system):State(_system){}
State* Void_State::doAction(Request* _request){
    string responce = "\"";
    responce += _request->m_Request_Type;
    responce += "\" this functionalty does not implemented yet:(";
    _request->client->print(responce.c_str());
    m_system->m_console->println(responce.c_str());
    return NULL;
}
