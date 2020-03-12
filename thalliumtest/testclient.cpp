#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include <mpi.h>
#include "client.h"



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

const std::string serverCred = "cred_conf";

int main(int argc, char **argv)
{

    MPI_Init(&argc, &argv);
    int rank, procs;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &procs);

    if (argc != 3)
    {
        if (rank == 0)
        {
            std::cerr << "Too few arguments" << std::endl;
            std::cerr << "Usage: testclient protocol serverAddr" << std::endl;
        }
        MPI_Abort(MPI_COMM_WORLD, -1);
    }

    std::string protocol = argv[1];
    std::string serverAddr = argv[2];


#ifdef USE_GNI
    //get the drc id from the shared file
    std::ifstream infile(serverCred);
    std::string cred_id;
    std::getline(infile, cred_id);
    if (rank == 0)
    {
        std::cout << "load cred_id: " << cred_id << std::endl;
    }

    struct hg_init_info hii;
    memset(&hii, 0, sizeof(hii));
    char drc_key_str[256] = {0};
    uint32_t drc_cookie;
    uint32_t drc_credential_id;
    drc_info_handle_t drc_credential_info;
    int ret;
    drc_credential_id = (uint32_t)atoi(cred_id.c_str());

    ret = drc_access(drc_credential_id, 0, &drc_credential_info);
    DIE_IF(ret != DRC_SUCCESS, "drc_access %u", drc_credential_id);
    drc_cookie = drc_get_first_cookie(drc_credential_info);

    sprintf(drc_key_str, "%u", drc_cookie);
    hii.na_init_info.auth_key = drc_key_str;
    //printf("use the drc_key_str %s\n", drc_key_str);

    margo_instance_id mid;
    mid = margo_init_opt("gni", MARGO_CLIENT_MODE, &hii, 0, 1);
    tl::engine globalclientEngine(mid);
#else
    tl::engine globalclientEngine(protocol, THALLIUM_CLIENT_MODE);
#endif

    //generate client
    UniClient* clientEnginePtr = new UniClient(&globalclientEngine, serverAddr);

    for (int i = 0; i < 10; i++)
    {
        clientEnginePtr->callserver2();
    }

    //free the endpoint and remote procesures
    delete clientEnginePtr;
    MPI_Finalize();
}

