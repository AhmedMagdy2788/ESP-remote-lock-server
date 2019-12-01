#include "Connectivity_Manager.h"

Connectivity_Manager::Connectivity_Manager(Connectivity_Observer * observer, WIFI_Profile * profile, const char * ap_SSID, const char * ap_Password, uint8_t mode)
{
    m_Observer = observer;
    m_wifi_profile = profile;
    m_AP_SSID = (char*)ap_SSID;
    m_AP_password = (char *)ap_Password;
    m_Connection_Mode = mode;
    if (m_Connection_Mode == WIFI_MODE || m_Connection_Mode == HYPER_MODE)
    {
        connect_to_WIFI();
        m_connectivity_timer.attach(15.0, check_connectivity, this);
    }
    if (m_Connection_Mode == AP_MODE || m_Connection_Mode == HYPER_MODE || m_Connection_Mode == OFF_MODE)
    {
        enable_AP_Mode();
    }
}

Connectivity_Manager::~Connectivity_Manager(){
    delete m_wifi_profile;
    delete[] m_AP_SSID;
    delete[] m_AP_password;
    Serial.println("connection object destroied!!!!!");
}

bool Connectivity_Manager::add_WIFI_Profile(WIFI_Profile* _wifi_profile){
    m_Observer->update_WIFI_Profile(_wifi_profile);
    delete m_wifi_profile;
    m_wifi_profile = _wifi_profile;
    return true;
}

bool Connectivity_Manager::connect_to_WIFI(){
    delay(100);
    // WiFi.softAPdisconnect(true);
    Serial.println("WIFI loading....");
    if (!WiFi.config(m_wifi_profile->get_Local_ip(),
                     m_wifi_profile->get_gateway_ip(),
                     m_wifi_profile->get_subnet_ip(),
                     m_wifi_profile->get_primaryDNS_ip(),
                     m_wifi_profile->get_secondryDNS_ip()))
    {
        Serial.println("STA Failed to configure");
        enable_AP_Mode();
        return false;
    }else{
        Serial.print("Connecting to ");
        Serial.println(m_wifi_profile->get_WIFI_SSID());
        WiFi.begin(m_wifi_profile->get_WIFI_SSID(), m_wifi_profile->get_WIFI_Password());
        int i = 0;
        while (WiFi.status() != WL_CONNECTED)
        {
            delay(120);
            Serial.print(".");
            i++;
            if (i >= 50)
            {
                Serial.println("");
                Serial.print("Connecting to \"");
                Serial.print(m_wifi_profile->get_WIFI_SSID());
                Serial.println("\" fails!!!!");
                break;
            }
        }
        if( is_WIFI_Connected()){
            if (m_Connection_Mode == OFF_MODE || m_Connection_Mode == WIFI_MODE)
            {
                m_Observer->update_Connection_Mode(WIFI_MODE);
                m_Connection_Mode = WIFI_MODE;
                Serial.println("connection mode = WIFI Mode");
            }else{
                m_Observer->update_Connection_Mode(HYPER_MODE);
                m_Connection_Mode = HYPER_MODE;
                Serial.println("connection mode = Hyper Mode");
            }

            return true;
        }else{
            if (m_Connection_Mode == OFF_MODE || m_Connection_Mode == WIFI_MODE)
            {
                m_Observer->update_Connection_Mode(OFF_MODE);
                m_Connection_Mode = OFF_MODE;
                Serial.println("connection mode = OFF_MODE");
                enable_AP_Mode();
            }else{
                m_Observer->update_Connection_Mode(AP_MODE);
                m_Connection_Mode = AP_MODE;
                Serial.println("connection mode = AP_MODE");
            }
            return false;
        }
    }
}

bool Connectivity_Manager::is_WIFI_Connected(){
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("");
        Serial.println("WiFi connected!");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        Serial.print("ESP Mac Address: ");
        Serial.println(WiFi.macAddress());
        Serial.print("Subnet Mask: ");
        Serial.println(WiFi.subnetMask());
        Serial.print("Gateway IP: ");
        Serial.println(WiFi.gatewayIP());
        Serial.print("DNS: ");
        Serial.println(WiFi.dnsIP());
        return true;
    }else{
        Serial.println("there is no connection to wifi!!!");
        return false;
    }
}
//not tested yet........................................
bool Connectivity_Manager::is_WIFI_Available(){
    // Set WiFi to station mode and disconnect from an AP if it was previously connected
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    Serial.println("scan start");

    // WiFi.scanNetworks will return the number of networks found
    int n = WiFi.scanNetworks();
    Serial.println("scan done");
    if (n == 0)
    {
        Serial.println("no networks found");
        return false;
    }
    else
    {
        Serial.print(n);
        Serial.println(" networks found");
        for (int i = 0; i < n; ++i)
        {
            // Print SSID and RSSI for each network found
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(WiFi.SSID(i));
            Serial.print(" (");
            Serial.print(WiFi.RSSI(i));
            Serial.print(")");
            Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
            delay(10);
        }
    }
    return true;
}

bool Connectivity_Manager::enable_WIFI_Mode(){
    m_connectivity_timer.attach(15.0, check_connectivity, this);
    if (m_Connection_Mode == OFF_MODE || m_Connection_Mode == WIFI_MODE)
    {
        m_Observer->update_Connection_Mode(WIFI_MODE);
        m_Connection_Mode = WIFI_MODE;
        Serial.println("connection mode = WIFI_MODE");
        enable_AP_Mode();
    }else{
        m_Observer->update_Connection_Mode(HYPER_MODE);
        m_Connection_Mode = HYPER_MODE;
        Serial.println("connection mode = HYPER_MODE");
    }
    if(connect_to_WIFI()){
        Serial.print("wifi connected into ");
        Serial.println(m_wifi_profile->get_WIFI_SSID());
        return true;
    }else{
        Serial.println("wifi intialization failed ");
        return false;
    }
}

void Connectivity_Manager::disable_WIFI_Mode() {
    WiFi.disconnect(true);
    m_connectivity_timer.detach();
    if (m_Connection_Mode == HYPER_MODE)
    {
        m_Observer->update_Connection_Mode(AP_MODE);
        m_Connection_Mode = AP_MODE;
    }
    else if (m_Connection_Mode == WIFI_MODE)
    {
        m_Observer->update_Connection_Mode(OFF_MODE);
        m_Connection_Mode = OFF_MODE;
        enable_AP_Mode();
    }

    Serial.println("WIFI Disconnected!!!");
}

void Connectivity_Manager::check_connectivity(Connectivity_Manager* _connectivity_Manager){
    if (!(_connectivity_Manager->is_WIFI_Connected()))
    {
        if(_connectivity_Manager->m_Connection_Mode == HYPER_MODE || _connectivity_Manager->m_Connection_Mode == WIFI_MODE){
            if(!_connectivity_Manager->connect_to_WIFI()){
                _connectivity_Manager->set_Connectivity_Mode(HYPER_MODE);
                _connectivity_Manager->m_Observer->update_Connection_Mode(HYPER_MODE);
                Serial.println("connection mode = HYPER_MODE");
            }
        }else{
            Serial.print("connection mode = ");
            Serial.println(_connectivity_Manager->m_Connection_Mode);
        }
    }else{
        Serial.println("wifi connected.");
    }
}

bool Connectivity_Manager::change_AP_Profile(char* SSID, char* password){
    m_Observer->update_AP_Profile(SSID, password);
    delete[] m_AP_SSID;
    m_AP_SSID = SSID;

    delete[] m_AP_password;
    m_AP_password = password;
    return enable_AP_Mode();
}

bool Connectivity_Manager::enable_AP_Mode(){
    Serial.println();
    Serial.println("esp ap setup .....");
    Serial.println();
    Serial.print("Take Online ... ");
    Serial.println(m_AP_SSID);
    WiFi.softAP(m_AP_SSID, m_AP_password, 2, 0);
    Serial.println("");
    Serial.println("WiFi AP online ...");
    if (m_Connection_Mode == OFF_MODE || m_Connection_Mode == AP_MODE)
    {
        m_Observer->update_Connection_Mode(AP_MODE);
        m_Connection_Mode = AP_MODE;
        Serial.println("connection mode = AP Mode");
    }
    else
    {
        m_Observer->update_Connection_Mode(HYPER_MODE);
        m_Connection_Mode = HYPER_MODE;
        Serial.println("connection mode = Hyper Mode");
    }
    delay(50);
    return true;
}

void Connectivity_Manager::disable_AP_Mode(){
    WiFi.softAPdisconnect(true);
    Serial.println("AP Disabled.");
    if (m_Connection_Mode == HYPER_MODE)
    {
        Serial.println("connection mode = WIFI_MODE.");
        m_Observer->update_Connection_Mode(WIFI_MODE);
        m_Connection_Mode = WIFI_MODE;
    }
    else if (m_Connection_Mode == AP_MODE){
        m_Observer->update_Connection_Mode(OFF_MODE);
        Serial.println("connection mode = OFF_MODE.");
        m_Connection_Mode = OFF_MODE;
        connect_to_WIFI();
    }

    Serial.println("AP Disabled");
}
