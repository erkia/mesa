/*
 * Copyright © 2013 Rob Clark <robclark@freedesktop.org>
 * SPDX-License-Identifier: MIT
 *
 * Authors:
 *    Rob Clark <robclark@freedesktop.org>
 */

#include "pipe/p_state.h"
#include "util/u_memory.h"
#include "util/u_string.h"

#include "fd3_context.h"
#include "fd3_format.h"
#include "fd3_zsa.h"

void *
fd3_zsa_state_create(struct pipe_context *pctx,
                     const struct pipe_depth_stencil_alpha_state *cso)
{
   struct fd3_zsa_stateobj *so;

   so = CALLOC_STRUCT(fd3_zsa_stateobj);
   if (!so)
      return NULL;

   so->base = *cso;

   so->rb_depth_control |=
      A3XX_RB_DEPTH_CONTROL_ZFUNC(cso->depth_func); /* maps 1:1 */

   if (cso->depth_enabled)
      so->rb_depth_control |=
         A3XX_RB_DEPTH_CONTROL_Z_TEST_ENABLE | A3XX_RB_DEPTH_CONTROL_Z_READ_ENABLE;

   if (cso->depth_writemask)
      so->rb_depth_control |= A3XX_RB_DEPTH_CONTROL_Z_WRITE_ENABLE;

   if (cso->stencil[0].enabled) {
      const struct pipe_stencil_state *s = &cso->stencil[0];

      so->rb_stencil_control |=
         A3XX_RB_STENCIL_CONTROL_STENCIL_READ |
         A3XX_RB_STENCIL_CONTROL_STENCIL_ENABLE |
         A3XX_RB_STENCIL_CONTROL_FUNC(s->func) | /* maps 1:1 */
         A3XX_RB_STENCIL_CONTROL_FAIL(fd_stencil_op(s->fail_op)) |
         A3XX_RB_STENCIL_CONTROL_ZPASS(fd_stencil_op(s->zpass_op)) |
         A3XX_RB_STENCIL_CONTROL_ZFAIL(fd_stencil_op(s->zfail_op));
      so->rb_stencilrefmask |=
         0xff000000 | /* ??? */
         A3XX_RB_STENCILREFMASK_STENCILWRITEMASK(s->writemask) |
         A3XX_RB_STENCILREFMASK_STENCILMASK(s->valuemask);

      if (cso->stencil[1].enabled) {
         const struct pipe_stencil_state *bs = &cso->stencil[1];

         so->rb_stencil_control |=
            A3XX_RB_STENCIL_CONTROL_STENCIL_ENABLE_BF |
            A3XX_RB_STENCIL_CONTROL_FUNC_BF(bs->func) | /* maps 1:1 */
            A3XX_RB_STENCIL_CONTROL_FAIL_BF(fd_stencil_op(bs->fail_op)) |
            A3XX_RB_STENCIL_CONTROL_ZPASS_BF(fd_stencil_op(bs->zpass_op)) |
            A3XX_RB_STENCIL_CONTROL_ZFAIL_BF(fd_stencil_op(bs->zfail_op));
         so->rb_stencilrefmask_bf |=
            0xff000000 | /* ??? */
            A3XX_RB_STENCILREFMASK_STENCILWRITEMASK(bs->writemask) |
            A3XX_RB_STENCILREFMASK_STENCILMASK(bs->valuemask);
      }
   }

   if (cso->alpha_enabled) {
      so->rb_render_control =
         A3XX_RB_RENDER_CONTROL_ALPHA_TEST |
         A3XX_RB_RENDER_CONTROL_ALPHA_TEST_FUNC(cso->alpha_func);
      so->rb_alpha_ref = A3XX_RB_ALPHA_REF_UINT(cso->alpha_ref_value * 255.0f) |
                         A3XX_RB_ALPHA_REF_FLOAT(cso->alpha_ref_value);
      so->rb_depth_control |= A3XX_RB_DEPTH_CONTROL_EARLY_Z_DISABLE;
   }

   return so;
}
