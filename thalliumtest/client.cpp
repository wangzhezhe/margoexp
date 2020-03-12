
#include "client.h"
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include "client.h"

namespace tl = thallium;

tl::endpoint UniClient::lookup(
    const std::string &address) const
{

    auto it = m_endpoint_cache.find(address);
    if (it == m_endpoint_cache.end())
    {
        throw std::runtime_error("cache the endpoint at the constructor\n");
    }

    return it->second;
}

void UniClient::callserver1()
{

    tl::endpoint serverEndpoint = this->lookup(this->m_serverAddr);
    //the parameters here should be consistent with the defination at the server end
    std::string input("testserver1");
    int status = m_remoteserver1.on(serverEndpoint)(input);
    if (status != 0)
    {
        throw std::runtime_error("failed for testserver1");
    }
    return;
}

void UniClient::callserver2()
{
    tl::endpoint serverEndpoint = this->lookup(this->m_serverAddr);
    //the parameters here should be consistent with the defination at the server end
    std::string input("testserver2");
    int status = m_remoteserver2.on(serverEndpoint)(input);
    std::cout << "call callserver2" << std::endl;
    if (status != 0)
    {
        throw std::runtime_error("failed for testserver2");
    }
    return;
}