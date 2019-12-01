#include "HTTP_Manager.h"

HTTP_Manager::HTTP_Manager(HTTP_Observer *observer)
{
    m_system_observer = observer;
    m_server = new WiFiServer(8080);
    m_server->begin();
    Serial.println("");
    Serial.println("");
    Serial.println("server created..");
    Serial.println("");
    Serial.println("");

}

void HTTP_Manager::listen()
{
    WiFiClient client = m_server->available(); // listen for incoming clients
    if (client)
    { // if you get a client,
        Serial.println("connected to client");
        clientHandler(client);
        client.stop();
    }else{
        // Serial.println("No client");
    }
}

void HTTP_Manager::clientHandler(WiFiClient& client)
{
    Serial.println("New Client."); // print a message out the serial port
    std::string currentLine = "";  // make a std::string to hold incoming data from the client
    int i = 0;
    int flage = 0;
    while (client.connected())
    { // loop while the client's connected
        //Serial.println("access while ");
        if (i++ > 100)
        {
            break;
        }
        if (client.available())
        { // if there's bytes to read from the client,
            i = 0;
            char c = client.read(); // read a byte, then
            Serial.write(c);        // print it out the serial monitor
            if (c == '\n')
            {
                flage++;
                if(flage > 1){
                    // Serial.println("Entered the if statment");
                    serverResponce(client, currentLine);
                    break;
                }else{
                    // Serial.println("Entered the else statment");
                    currentLine += c;
                }
            }
            else if (c != '\r')
            {                     // if you got anything else but a carriage return character,
                currentLine += c; // add it to the end of the currentLine
                flage = 0;
            }
        }
        if (i > 0)
        {
            delay(20);
        }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
}

void HTTP_Manager::serverResponce(WiFiClient& client, std::string& clientRequest)
{
    Serial.println("creating Request...");
    m_request = (Requrests_Factory::get_factory())->create_Request(&client, &clientRequest);
    Serial.println("start the response ...");
    client.println("HTTP/1.1 200 OK");
    Serial.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    Serial.println("Content-type:text/html");
    client.println();
    Serial.println();
    m_system_observer->requestHandler(m_request);
    // m_system_observer->httpResponse(client);

    // The HTTP response ends with another blank line:
    client.println();
    Serial.println();
    Serial.println("response ended");
}
