#ifndef HTTP_OBSERVER_H_
#define HTTP_OBSERVER_H_
#include <string>
#include <wifi.h>
#include "Requests.h"

class HTTP_Observer
{
public:
  virtual void requestHandler(Request*) = 0;
  //virtual void httpResponse(WiFiClient client) = 0;
};

#endif /*HTTP_OBSERVER_H_*/
