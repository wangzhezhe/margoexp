#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <margo.h>

int main(int argc, char** argv)
{
    
    //init
    char *proto;
    char *colon;
    margo_instance_id mid;


    proto = strdup(argv[1]);
    assert(proto);
    colon = strchr(proto, ':');
    if(colon)
        *colon = '\0';

    /* actually start margo -- margo_init() encapsulates the Mercury &
     * Argobots initialization, so this step must precede their use. */
    /* Use main process to drive progress (it will relinquish control to
     * Mercury during blocking communication calls). No RPC threads are
     * used because this is a pure client that will not be servicing
     * rpc requests.
     */
    /***************************************/
    mid = margo_init(proto, MARGO_CLIENT_MODE, 0, 0);
    free(proto);
    if(mid == MARGO_INSTANCE_NULL)
    {
        fprintf(stderr, "Error: margo_init()\n");
        return(-1);
    }
    margo_diag_start(mid);

    hg_id_t hello_rpc_id = MARGO_REGISTER(mid, "hello", void, void, NULL);
    //tell mercury there is no respond value for this function
    margo_registered_disable_response(mid, hello_rpc_id, HG_TRUE);
    hg_addr_t svr_addr;
    int ret;
    ret = margo_addr_lookup(mid, argv[1], &svr_addr);
    hg_handle_t handle;
    ret = margo_create(mid, svr_addr, hello_rpc_id, &handle);
    ret = margo_forward(handle, NULL);
    ret = margo_destroy(handle);
    ret = margo_addr_free(mid, svr_addr);

    margo_finalize(mid);

    return 0;
}