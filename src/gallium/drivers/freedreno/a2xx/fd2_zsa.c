/*
 * Copyright © 2012 Rob Clark <robclark@freedesktop.org>
 * SPDX-License-Identifier: MIT
 *
 * Authors:
 *    Rob Clark <robclark@freedesktop.org>
 */

#include "pipe/p_state.h"
#include "util/u_memory.h"
#include "util/u_string.h"

#include "fd2_context.h"
#include "fd2_util.h"
#include "fd2_zsa.h"

void *
fd2_zsa_state_create(struct pipe_context *pctx,
                     const struct pipe_depth_stencil_alpha_state *cso)
{
   struct fd2_zsa_stateobj *so;

   so = CALLOC_STRUCT(fd2_zsa_stateobj);
   if (!so)
      return NULL;

   so->base = *cso;

   so->rb_depthcontrol |=
      A2XX_RB_DEPTHCONTROL_ZFUNC(cso->depth_func); /* maps 1:1 */

   if (cso->depth_enabled)
      so->rb_depthcontrol |=
         A2XX_RB_DEPTHCONTROL_Z_ENABLE |
         COND(!cso->alpha_enabled, A2XX_RB_DEPTHCONTROL_EARLY_Z_ENABLE);
   if (cso->depth_writemask)
      so->rb_depthcontrol |= A2XX_RB_DEPTHCONTROL_Z_WRITE_ENABLE;

   if (cso->stencil[0].enabled) {
      const struct pipe_stencil_state *s = &cso->stencil[0];

      so->rb_depthcontrol |=
         A2XX_RB_DEPTHCONTROL_STENCIL_ENABLE |
         A2XX_RB_DEPTHCONTROL_STENCILFUNC(s->func) | /* maps 1:1 */
         A2XX_RB_DEPTHCONTROL_STENCILFAIL(fd_stencil_op(s->fail_op)) |
         A2XX_RB_DEPTHCONTROL_STENCILZPASS(fd_stencil_op(s->zpass_op)) |
         A2XX_RB_DEPTHCONTROL_STENCILZFAIL(fd_stencil_op(s->zfail_op));
      so->rb_stencilrefmask |=
         0xff000000 | /* ??? */
         A2XX_RB_STENCILREFMASK_STENCILWRITEMASK(s->writemask) |
         A2XX_RB_STENCILREFMASK_STENCILMASK(s->valuemask);

      if (cso->stencil[1].enabled) {
         const struct pipe_stencil_state *bs = &cso->stencil[1];

         so->rb_depthcontrol |=
            A2XX_RB_DEPTHCONTROL_BACKFACE_ENABLE |
            A2XX_RB_DEPTHCONTROL_STENCILFUNC_BF(bs->func) | /* maps 1:1 */
            A2XX_RB_DEPTHCONTROL_STENCILFAIL_BF(fd_stencil_op(bs->fail_op)) |
            A2XX_RB_DEPTHCONTROL_STENCILZPASS_BF(fd_stencil_op(bs->zpass_op)) |
            A2XX_RB_DEPTHCONTROL_STENCILZFAIL_BF(fd_stencil_op(bs->zfail_op));
         so->rb_stencilrefmask_bf |=
            0xff000000 | /* ??? */
            A2XX_RB_STENCILREFMASK_STENCILWRITEMASK(bs->writemask) |
            A2XX_RB_STENCILREFMASK_STENCILMASK(bs->valuemask);
      }
   }

   if (cso->alpha_enabled) {
      so->rb_colorcontrol = A2XX_RB_COLORCONTROL_ALPHA_FUNC(cso->alpha_func) |
                            A2XX_RB_COLORCONTROL_ALPHA_TEST_ENABLE;
      so->rb_alpha_ref = fui(cso->alpha_ref_value);
   }

   return so;
}
