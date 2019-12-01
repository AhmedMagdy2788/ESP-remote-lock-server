#ifndef WIFI_PROFILE_H_
#define WIFI_PROFILE_H_

#include <Arduino.h>
#include <IPAddress.h>
#include <string>

class WIFI_Profile
{
    const char *WIFI_SSID;
    const char *WIFI_password;
    IPAddress local;
    IPAddress gateway;
    IPAddress subnet;
    IPAddress primaryDNS;
    IPAddress secondryDNS;
public:
    WIFI_Profile(){
        WIFI_SSID = new char[1];
        WIFI_password = new char[1];
    }

    ~WIFI_Profile(){
        delete[] WIFI_SSID;
        delete[] WIFI_password;
    }

    void set_WIFI_SSID(const char *wifi_ssid)
    {
        delete[] WIFI_SSID;
		char* temp = new char[strlen(wifi_ssid) + 1];
		strcpy(temp, wifi_ssid);
		WIFI_SSID = temp;
        //////////////////////////////////////////////
        // unsigned int size = strlen(wifi_ssid) + 1;
        // WIFI_SSID = (char *)malloc(size);
        // memcpy(WIFI_SSID, wifi_ssid, size);
    }

    const char* get_WIFI_SSID(){return WIFI_SSID;}

    void set_WIFI_Password(const char *wifi_password)
    {
        delete[] WIFI_password;
		char* temp = new char[strlen(wifi_password) + 1];
		strcpy(temp, wifi_password);
		WIFI_password = temp;
        //////////////////////////////////////////////
        // unsigned int size = strlen(wifi_password) + 1;
        // WIFI_password = (char *)malloc(size);
        // memcpy(WIFI_password, wifi_password, size);
    }

    const char* get_WIFI_Password(){return WIFI_password;}

    bool set_Local_IP(const char *str_address)
    {
        return set_address(str_address, local);
    }

    void set_Local_IP(IPAddress _Local)
    {
        local = _Local;
    }

    IPAddress& get_Local_ip(){return local;}

    bool set_gateway_IP(const char *str_address)
    {
        return set_address(str_address, gateway);
    }

    void set_gateway_IP(IPAddress _gateway)
    {
        gateway = _gateway;
    }

    IPAddress& get_gateway_ip(){return gateway;}

    bool set_subnet_IP(const char *str_address)
    {
        return set_address(str_address, subnet);
    }

    void set_subnet_IP(IPAddress _subnet)
    {
        subnet = _subnet;
    }

    IPAddress& get_subnet_ip(){return subnet;}

    bool set_primaryDNS_IP(const char *str_address)
    {
        return set_address(str_address, primaryDNS);
    }

    void set_primaryDNS_IP(IPAddress _primaryDNS)
    {
        primaryDNS = _primaryDNS;
    }

    IPAddress& get_primaryDNS_ip(){return primaryDNS;}

    bool set_secondryDNS_IP(const char *str_address)
    {
        return set_address(str_address, secondryDNS);
    }

    void set_secondryDNS_IP(IPAddress _secondryDNS)
    {
        secondryDNS = _secondryDNS;
    }

    IPAddress& get_secondryDNS_ip(){return secondryDNS;}

  private:
    bool set_address(const char *ptr_address, IPAddress &_address)
    {
        std::string str_address(ptr_address);
        std::size_t dot_pos = -1;
        std::size_t temp = 0;
        for (int i = 0; i < 4; i++)
        {
            temp = str_address.find(".", dot_pos + 1);
            if (temp != std::string::npos)
            {
                std::string str_byte = str_address.substr(dot_pos + 1, temp - dot_pos - 1);
                // Serial.println(str_byte.c_str());
                _address[i] = (uint8_t)str_into_intager(str_byte.c_str());
                dot_pos = temp;
                if (i == 3)
                    return 1;
            }
            else if (i == 3)
            {
                temp = str_address.size();
                std::string str_byte = str_address.substr(dot_pos + 1, temp - dot_pos - 1);
                // Serial.println(str_byte.c_str());
                _address[i] = (uint8_t)str_into_intager(str_byte.c_str());
                return 1;
            }
            else
            {
                return 0;
            }
        }
        return 0;
    }

    int str_into_intager(const char *str_num)
    {
        int num = 0;
        std::size_t str_size = strlen(str_num);
        for (int i = 0, j = str_size - 1; i < str_size; i++)
        {
            num += ((str_num[i] - '0') * pow(10, j));
            j--;
        }
        // Serial.print("converted num = ");Serial.println(num);
        return num;
    }
};
#endif /*WIFI_PROFILE_H_*/
