#include <assert.h>
#include <stdio.h>
#include <margo.h>
#include "types.h"
#include "common.h"

typedef struct
{
    int max_rpcs;
    int num_rpcs;
} server_data;

static void sum(hg_handle_t h);
static void testbbx(hg_handle_t h);
static void testobjlist(hg_handle_t h);

DECLARE_MARGO_RPC_HANDLER(sum)
DECLARE_MARGO_RPC_HANDLER(testbbx)
DECLARE_MARGO_RPC_HANDLER(testobjlist)

int main(int argc, char **argv)
{
    margo_instance_id mid = margo_init("tcp", MARGO_SERVER_MODE, 0, 0);
    assert(mid);

    server_data svr_data = {
        .max_rpcs = 4,
        .num_rpcs = 0};

    hg_addr_t my_address;
    margo_addr_self(mid, &my_address);
    char addr_str[128];
    size_t addr_str_size = 128;
    margo_addr_to_string(mid, addr_str, &addr_str_size, my_address);
    margo_addr_free(mid, my_address);
    printf("Server running at address %s\n", addr_str);

    hg_id_t rpc_id = MARGO_REGISTER(mid, "sum", sum_in_t, sum_out_t, sum);
    hg_id_t bbx_rpc_id = MARGO_REGISTER(mid, "testbbx", obj_in_t, obj_out_t, testbbx);
    hg_id_t objlist_rpc_id = MARGO_REGISTER(mid, "test_objlist", objlist_in_t, objlist_out_t, testobjlist);

    margo_register_data(mid, rpc_id, &svr_data, NULL);
    margo_register_data(mid, bbx_rpc_id, &svr_data, NULL);
    margo_register_data(mid, objlist_rpc_id, &svr_data, NULL);
    fflush(stdout);
    margo_wait_for_finalize(mid);

    return 0;
}

static void sum(hg_handle_t h)
{
    hg_return_t ret;

    sum_in_t in;
    sum_out_t out;

    margo_instance_id mid = margo_hg_handle_get_instance(h);

    const struct hg_info *info = margo_get_info(h);
    server_data *svr_data = (server_data *)margo_registered_data(mid, info->id);

    ret = margo_get_input(h, &in);
    assert(ret == HG_SUCCESS);

    out.ret = in.x + in.y;
    printf("Computed %d + %d = %d\n", in.x, in.y, out.ret);

    ret = margo_respond(h, &out);
    assert(ret == HG_SUCCESS);

    ret = margo_free_input(h, &in);
    assert(ret == HG_SUCCESS);

    ret = margo_destroy(h);
    assert(ret == HG_SUCCESS);

    svr_data->num_rpcs += 1;
    //if(svr_data->num_rpcs == svr_data->max_rpcs) {
    //    margo_finalize(mid);
    //}
}

static void testbbx(hg_handle_t h)
{
    hg_return_t ret;

    obj_in_t in;
    obj_out_t out;

    margo_instance_id mid = margo_hg_handle_get_instance(h);

    const struct hg_info *info = margo_get_info(h);
    server_data *svr_data = (server_data *)margo_registered_data(mid, info->id);

    ret = margo_get_input(h, &in);
    assert(ret == HG_SUCCESS);

    printbbx((bbx_t *)(in.obj.raw_obj));

    out.ret = 123321;

    ret = margo_respond(h, &out);
    assert(ret == HG_SUCCESS);

    ret = margo_free_input(h, &in);
    assert(ret == HG_SUCCESS);

    ret = margo_destroy(h);
    assert(ret == HG_SUCCESS);

    fflush(stdout);

    svr_data->num_rpcs += 1;
    if (svr_data->num_rpcs == svr_data->max_rpcs)
    {
        margo_finalize(mid);
    }
}

static void testobjlist(hg_handle_t h)
{
    hg_return_t ret;

    objlist_in_t inlist;
    objlist_out_t out;

    margo_instance_id mid = margo_hg_handle_get_instance(h);

    const struct hg_info *info = margo_get_info(h);
    server_data *svr_data = (server_data *)margo_registered_data(mid, info->id);

    ret = margo_get_input(h, &inlist);
    assert(ret == HG_SUCCESS);

    //check the results
    int num = 0;
    //what if non elem?
    //Try to use array in most cases
    fprintf(stdout, "current id %d\n", 0);
    printbbx((bbx_t *)inlist.obj_list->value.raw_obj);

    obj_list_entry_ptr curr_entry = inlist.obj_list;
    int i=0;
    //for(i=0;i<5;i++)
    while(curr_entry!=NULL)
    {
        num++;
        fprintf(stdout, "current id %d\n", num);
        printbbx((bbx_t *)curr_entry->value.raw_obj);
        curr_entry=curr_entry->next;
    }

    out.ret = 0;

    ret = margo_respond(h, &out);
    assert(ret == HG_SUCCESS);

    ret = margo_free_input(h, &inlist);
    assert(ret == HG_SUCCESS);

    ret = margo_destroy(h);
    assert(ret == HG_SUCCESS);

    fflush(stdout);
    svr_data->num_rpcs += 1;
    if (svr_data->num_rpcs == svr_data->max_rpcs)
    {
        margo_finalize(mid);
    }
}

DEFINE_MARGO_RPC_HANDLER(sum)
DEFINE_MARGO_RPC_HANDLER(testbbx)
DEFINE_MARGO_RPC_HANDLER(testobjlist)