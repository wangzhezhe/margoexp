#include <assert.h>
#include <stdio.h>
#include <margo.h>
#include "types.h"
#include "common.h"

margo_instance_id global_id;

void testsum(char *addr_str)
{
    printf("---testsum---\n");

    hg_id_t sum_rpc_id = MARGO_REGISTER(global_id, "sum", sum_in_t, sum_out_t, NULL);

    hg_addr_t svr_addr;
    margo_addr_lookup(global_id, addr_str, &svr_addr);

    int i;
    sum_in_t args;
    for (i = 0; i < 4; i++)
    {
        args.x = 42 + i * 2;
        args.y = 42 + i * 2 + 1;

        hg_handle_t h;
        margo_create(global_id, svr_addr, sum_rpc_id, &h);
        margo_forward(h, &args);

        sum_out_t resp;
        margo_get_output(h, &resp);

        printf("Got response: %d+%d = %d\n", args.x, args.y, resp.ret);

        margo_free_output(h, &resp);
        margo_destroy(h);
    }

    margo_addr_free(global_id, svr_addr);

    return;
}

void testrawobj1(char *addr_str)
{

    printf("---testrawobj1---\n");

    hg_id_t bbx_rpc_id = MARGO_REGISTER(global_id, "testbbx", obj_in_t, obj_out_t, NULL);

    hg_addr_t svr_addr;
    margo_addr_lookup(global_id, addr_str, &svr_addr);

    bbx_t *bbx = (bbx_t *)malloc(sizeof(bbx_t));
    bbx->m_dims = 3;
    int i;
    //init array
    for (i = 0; i < DEFAULT_MAX_DIM; i++)
    {

        bbx->m_lb[i] = 0;
        bbx->m_ub[i] = 0;
    }

    for (i = 0; i < 3; i++)
    {

        bbx->m_lb[i] = 0;
        bbx->m_ub[i] = 256;
    }

    obj_in_t bbx_in_arg;
    bbx_in_arg.obj.size = sizeof(bbx_t);
    bbx_in_arg.obj.raw_obj = (char *)bbx;

    hg_handle_t h;
    margo_create(global_id, svr_addr, bbx_rpc_id, &h);
    margo_forward(h, &bbx_in_arg);

    obj_out_t resp;
    margo_get_output(h, &resp);

    printf("Got response: %d\n", resp.ret);
    fflush(stdout);
    margo_free_output(h, &resp);
    margo_destroy(h);

    margo_addr_free(global_id, svr_addr);
}

void testobjlist(char *addr_str)
{

    printf("---testobjlist---\n");

    hg_id_t bbx_rpc_id = MARGO_REGISTER(global_id, "test_objlist", objlist_in_t, objlist_out_t, NULL);

    hg_addr_t svr_addr;
    margo_addr_lookup(global_id, addr_str, &svr_addr);

    //init array
    objlist_in_t objlist_in_header;
    obj_list_entry_ptr prev = NULL;
    int i,j;
    for (i = 0; i < 10; i++)
    {

        bbx_t *bbx = (bbx_t *)malloc(sizeof(bbx_t));
        bbx->m_dims = 3;
        int j;
        for (j = 0; j < DEFAULT_MAX_DIM; j++)
        {

            bbx->m_lb[j] = 0;
            bbx->m_ub[j] = 0;
        }

        for (j = 0; j < 3; j++)
        {

            bbx->m_lb[j] = i;
            bbx->m_ub[j] = 1 + i;
        }

        obj_list_entry_ptr entry_value = (obj_list_entry_ptr)malloc(sizeof(struct obj_list_entry));

        entry_value->next = NULL;
        entry_value->value.size = sizeof(bbx_t);
        entry_value->value.raw_obj = (char *)bbx;

        if (i == 0)
        {
            objlist_in_header.obj_list=entry_value;
            prev=entry_value;
            continue;
        }

        prev->next = entry_value;
        prev = entry_value;
    }

    hg_handle_t h;
    margo_create(global_id, svr_addr, bbx_rpc_id, &h);

    //check the input
    obj_list_entry_ptr currentry = objlist_in_header.obj_list;
    int id = 0;
    while (currentry != NULL)
    {
        fprintf(stdout, "id %d\n", id);
        printbbx((bbx_t *)currentry->value.raw_obj);
        id++;
        currentry = currentry->next;
    }

    margo_forward(h, &objlist_in_header);

    obj_out_t resp;
    margo_get_output(h, &resp);

    printf("Got response: %d\n", resp.ret);
    fflush(stdout);
    margo_free_output(h, &resp);
    margo_destroy(h);

    margo_addr_free(global_id, svr_addr);
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <server address>\n", argv[0]);
        exit(0);
    }

    global_id = margo_init("tcp", MARGO_CLIENT_MODE, 0, 0);

    //testsum(argv[1]);

    testrawobj1(argv[1]);

    testobjlist(argv[1]);

    margo_finalize(global_id);

    return 0;
}
