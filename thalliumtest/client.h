#ifndef __UNIMOSCLIENT_H__
#define __UNIMOSCLIENT_H__


#include <vector>
#include <iostream>
#include <string>
#include <array>
#include <string>
#include <fstream>
#include <map>
#include <thallium.hpp>
#include <thallium/serialization/stl/string.hpp>

namespace tl = thallium;

struct UniClient
{
    //for server enginge, the client ptr is the pointer to the server engine
    //for the client code, the client engine is the pointer to the engine with the client mode
    UniClient(tl::engine *clientEnginePtr, std::string serverAddr)
    :m_remoteserver1(clientEnginePtr->define("callserver1"))
    ,m_remoteserver2(clientEnginePtr->define("callserver2"))
    {
        m_clientEnginePtr = clientEnginePtr;
        m_serverAddr = serverAddr;
        std::cout << "load server Addr: " << m_serverAddr << std::endl;

        //Attention:
        //look up all the potential server addresses to avoid the race condition of mercury
        auto endpoint = this->m_clientEnginePtr->lookup(serverAddr);
        m_endpoint_cache[serverAddr] = endpoint;
    };

    std::string m_serverAddr = "";
    tl::engine *m_clientEnginePtr = NULL;
    
    //the remote procedure is supposed to be called only once
    tl::remote_procedure m_remoteserver1;
    tl::remote_procedure m_remoteserver2;

    mutable std::unordered_map<std::string, tl::endpoint> m_endpoint_cache;

    ~UniClient(){
        std::cout<<"destroy client"<<std::endl;
    };

    tl::endpoint lookup(const std::string& address) const;

    void callserver1();

    void callserver2();


};

#endif