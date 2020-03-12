#ifndef PARAM_H
#define PARAM_H

#include <mercury.h>
#include <mercury_macros.h>

typedef struct{
        int64_t size;
        char *raw_obj;
}obj_t;

static inline hg_return_t hg_proc_obj_t(hg_proc_t proc, void *arg)
{
  hg_return_t ret;
  obj_t *in = (obj_t*)arg;
  ret = hg_proc_hg_size_t(proc, &in->size);
  if(ret != HG_SUCCESS) return ret;
  if (in->size) {
    switch (hg_proc_get_op(proc)) {
    case HG_ENCODE:
        ret = hg_proc_raw(proc, in->raw_obj, in->size);
        if(ret != HG_SUCCESS) return ret;
      break;
    case HG_DECODE:
      in->raw_obj = (char*)malloc(in->size);
      ret = hg_proc_raw(proc, in->raw_obj, in->size);
      if(ret != HG_SUCCESS) return ret;
      break;
    case HG_FREE:
      free(in->raw_obj);
      break;
    default:
      break;
    }
  }
  return HG_SUCCESS;
}


MERCURY_GEN_PROC(sum_in_t,
        ((int32_t)(x))\
        ((int32_t)(y)))

MERCURY_GEN_PROC(sum_out_t, ((int32_t)(ret)))

MERCURY_GEN_PROC(obj_in_t, ((obj_t)(obj)))

MERCURY_GEN_PROC(obj_out_t, ((int32_t)(ret)))

#endif
