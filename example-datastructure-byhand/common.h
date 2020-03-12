#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DEFAULT_MAX_DIM 9

typedef struct bbx_t
{
    uint16_t m_dims;
    uint64_t m_lb[DEFAULT_MAX_DIM];
    uint64_t m_ub[DEFAULT_MAX_DIM];
} bbx_t;

static void copybbx(bbx_t *destbbx, bbx_t *srcbbx)
{
    destbbx->m_dims = srcbbx->m_dims;
    int i;
    for (i = 0; i < DEFAULT_MAX_DIM; i++)
    {
        destbbx->m_lb[i] = srcbbx->m_lb[i];
        destbbx->m_ub[i] = srcbbx->m_ub[i];
    }
    return;
}
static void printbbx(bbx_t *bbx)
{
    fprintf(stdout, "dims %d\n", bbx->m_dims);
    int i;
    fprintf(stdout, "m_lb\n");

    for (i = 0; i < bbx->m_dims; i++)
    {
        fprintf(stdout, "%d,", bbx->m_lb[i]);
    }

    fprintf(stdout, "\nm_ub\n");

    for (i = 0; i < bbx->m_dims; i++)
    {
        fprintf(stdout, "%d,", bbx->m_ub[i]);
    }

    fprintf(stdout, "\n");
    return;
}

#endif