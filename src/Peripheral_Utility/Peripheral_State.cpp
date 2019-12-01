#include "Peripheral_State.h"
/*
Peripheral_State::Peripheral_State(Peripheral_Observer* _system):m_system(_system){}

Enroll_State::Enroll_State(){}
Peripheral_State* Enroll_State::doAction(Request* _request){
    // uint8_t user_id = m_system->scan_New_User();
    // _request->client->print("the new user id = ");
    // _request->client->print(user_id);
    // m_system->m_console->println(user_id);
    // _request->request_is_done = true;
    // return new Default_State(m_system);
    return NULL;
}

Scan_State::Scan_State(){}
Peripheral_State* Scan_State::doAction(Request* _request){
    // uint8_t user_id = m_system->scan_New_User();
    // _request->client->print("the new user id = ");
    // _request->client->print(user_id);
    // m_system->m_console->println(user_id);
    // _request->request_is_done = true;
    // return new Default_State(m_system);
    return NULL;
}

Delete_State::Delete_State(){}
Peripheral_State* Delete_State::doAction(Request* _request){
    // uint8_t user_id = m_system->scan_New_User();
    // _request->client->print("the new user id = ");
    // _request->client->print(user_id);
    // m_system->m_console->println(user_id);
    // _request->request_is_done = true;
    // return new Default_State(m_system);
    return NULL;
}

Connected_State::Connected_State(){}
Peripheral_State* Connected_State::doAction(Request* _request){
    // uint8_t user_id = m_system->scan_New_User();
    // _request->client->print("the new user id = ");
    // _request->client->print(user_id);
    // m_system->m_console->println(user_id);
    // _request->request_is_done = true;
    // return new Default_State(m_system);
    return NULL;
}

Disconnected_State::Disconnected_State(){}
Peripheral_State* Disconnected_State::doAction(Request* _request){
    // uint8_t user_id = m_system->scan_New_User();
    // _request->client->print("the new user id = ");
    // _request->client->print(user_id);
    // m_system->m_console->println(user_id);
    // _request->request_is_done = true;
    // return new Default_State(m_system);
    return NULL;
}

Configured_State::Configured_State(){}
Peripheral_State* Configured_State::doAction(Request* _request){
    // uint8_t user_id = m_system->scan_New_User();
    // _request->client->print("the new user id = ");
    // _request->client->print(user_id);
    // m_system->m_console->println(user_id);
    // _request->request_is_done = true;
    // return new Default_State(m_system);
    return NULL;
}

Opening_State::Opening_State(){}
Peripheral_State* Opening_State::doAction(Request* _request){
    Request_Rolling_Open * cast_request = (Request_Rolling_Open*) _request;
    m_system->open_Door(cast_request->m_rolling_percentage);
    return new Peripherals_State(m_system);
}

Closing_State::Closing_State(){}
Peripheral_State* Closing_State::doAction(Request* _request){
    m_system->close_Door();
    return new Peripherals_State(m_system);
}
*/
