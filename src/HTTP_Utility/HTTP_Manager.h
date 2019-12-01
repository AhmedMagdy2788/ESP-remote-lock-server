#ifndef HTTP_MANAGER_H_
#define HTTP_MANAGER_H_

#include <WiFi.h>
#include "Connectivity_Utility/Connectivity_Manager.h"
#include "System_Utility/State.h"
#include "HTTP_Observable.h"
#include "HTTP_Observer.h"
#include "HTTP_Utility/Requests_Factory.h"

class HTTP_Manager : public HTTP_Observable
{
  HTTP_Observer * m_system_observer;
  Request *m_request;
  WiFiServer* m_server;
public:
  HTTP_Manager(HTTP_Observer* observer);
  virtual void listen() override;
  void clientHandler(WiFiClient& client);
  void serverResponce(WiFiClient& client, std::string& currentLine);
};
#endif  /*CONNECTIVITY_MANAGER_H_*/
