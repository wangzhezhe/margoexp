#include <string>
#include <iostream>
#include <vector>
#include <thallium.hpp>
#include "client.h"
#include <unistd.h>

#ifdef USE_GNI
extern "C"
{
#include <rdmacred.h>
}
#include <mercury.h>
#include <margo.h>
#define DIE_IF(cond_expr, err_fmt, ...)                                                                           \
    do                                                                                                            \
    {                                                                                                             \
        if (cond_expr)                                                                                            \
        {                                                                                                         \
            fprintf(stderr, "ERROR at %s:%d (" #cond_expr "): " err_fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
            exit(1);                                                                                              \
        }                                                                                                         \
    } while (0)
#endif

namespace tl = thallium;

tl::engine *globalServerEnginePtr = nullptr;
UniClient *uniClient = nullptr;

const std::string serverCred = "cred_conf";

void callserver1(const tl::request &req, std::string &input)
{
    std::cout << "execute callserver1" << std::endl;
    usleep(1.0 * 1000000);
    req.respond(0);
    return;
}

void callserver2(const tl::request &req, std::string &input)
{

    usleep(1.0 * 1000000);
    std::cout << "execute callserver2 call callserver1" << std::endl;
    req.respond(0);
    //call server1
    uniClient->callserver1();
    return;
}

void runRerver(int serverID, std::string networkingType, std::string globalAddrServer1)
{

#ifdef USE_GNI
    uint32_t drc_credential_id = 0;
    drc_info_handle_t drc_credential_info;
    uint32_t drc_cookie;
    char drc_key_str[256] = {0};
    int ret;

    struct hg_init_info hii;
    memset(&hii, 0, sizeof(hii));

    //create the drc
    if (serverID == 1)
    {
        ret = drc_acquire(&drc_credential_id, DRC_FLAGS_FLEX_CREDENTIAL);
        DIE_IF(ret != DRC_SUCCESS, "drc_acquire");

        ret = drc_access(drc_credential_id, 0, &drc_credential_info);
        DIE_IF(ret != DRC_SUCCESS, "drc_access");
        drc_cookie = drc_get_first_cookie(drc_credential_info);
        sprintf(drc_key_str, "%u", drc_cookie);
        hii.na_init_info.auth_key = drc_key_str;

        ret = drc_grant(drc_credential_id, drc_get_wlm_id(), DRC_FLAGS_TARGET_WLM);
        DIE_IF(ret != DRC_SUCCESS, "drc_grant");

        std::cout << "grant the drc_credential_id: " << drc_credential_id << std::endl;
        std::cout << "use the drc_key_str: " << std::string(drc_key_str) << std::endl;

        //write this cred_id into file that can be shared by clients
        //output the credential id into the config files
        std::ofstream credFile;
        credFile.open(serverCred);
        credFile << drc_credential_id << "\n";
        credFile.close();
    }

    if (serverID == 2)
    {

        //load the drc
        std::ifstream infile(serverCred);
        std::string cred_id;
        std::getline(infile, cred_id);

        std::cout << "load cred_id: " << cred_id << std::endl;

        drc_credential_id = (uint32_t)atoi(cred_id.c_str());

        ret = drc_access(drc_credential_id, 0, &drc_credential_info);
        DIE_IF(ret != DRC_SUCCESS, "drc_access %u", drc_credential_id);
        drc_cookie = drc_get_first_cookie(drc_credential_info);

        sprintf(drc_key_str, "%u", drc_cookie);
        hii.na_init_info.auth_key = drc_key_str;
    }

    margo_instance_id mid;
    //the number here should same with the number of cores used in test scripts
    mid = margo_init_opt("gni", MARGO_SERVER_MODE, &hii, 0, 8);
    tl::engine serverEnginge(mid);
    globalServerEnginePtr = &serverEnginge;

    //margo_instance_id client_mid;
    //client_mid = margo_init_opt("gni", MARGO_CLIENT_MODE, &hii, 0, 8);
    //tl::engine clientEnginge(client_mid);

#else

    std::cout << "use the protocol other than gni: " << networkingType << std::endl;

    tl::engine serverEnginge(networkingType, THALLIUM_SERVER_MODE, false, 16);
    globalServerEnginePtr = &serverEnginge;

#endif
    if (serverID == 1)
    {
        globalServerEnginePtr->define("callserver1", callserver1);
    }
    if (serverID == 2)
    {
        uniClient = new UniClient(globalServerEnginePtr, globalAddrServer1);
        globalServerEnginePtr->define("callserver2", callserver2);
    }

    std::cout << "Server running at address " << globalServerEnginePtr->self() << std::endl;

#ifdef USE_GNI
    //destructor will not be called if send mid to engine
    margo_wait_for_finalize(mid);
#endif

    return;
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {

        std::cerr << "Usage: unimos_server <networking> <server id> " << std::endl;
        exit(0);
    }

    tl::abt scope;

    std::string networking = std::string(argv[1]);

    int serverID = std::stoi(argv[2]);

    if (serverID == 1)
    {
        runRerver(serverID, networking, "");
    }

    if (serverID == 2)
    {
        if (argc != 4)
        {
            std::cerr << "input the addr to init the client" << std::endl;
            exit(0);
        }
        std::string globalAddrServer1 = argv[3];
        runRerver(serverID, networking, globalAddrServer1);
    }

    return 0;
}
