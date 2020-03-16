#pragma once

#include "Core/AppIncludes.hpp"

class AppResources
{
public:
    AppResources(class Application &app);
    ~AppResources();
    void Draw();
    void Update();

private:
    class Application &app;

private:
    //Write here
    int port;
    Server m_server;
    Client m_client;
};