/***************************************************************************
 *            ncm_mset_func.h
 *
 *  Wed June 06 15:32:36 2012
 *  Copyright  2012  Sandro Dias Pinto Vitenti
 *  <sandro@isoftware.com.br>
 ****************************************************************************/
/*
 * numcosmo
 * Copyright (C) Sandro Dias Pinto Vitenti 2012 <sandro@isoftware.com.br>
 *
 * numcosmo is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * numcosmo is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _NCM_MSET_FUNC_H_
#define _NCM_MSET_FUNC_H_

#include <glib.h>
#include <glib-object.h>
#include <numcosmo/build_cfg.h>
#include <numcosmo/math/ncm_mset.h>

G_BEGIN_DECLS

#define NCM_TYPE_MSET_FUNC             (ncm_mset_func_get_type ())
#define NCM_MSET_FUNC(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), NCM_TYPE_MSET_FUNC, NcmMSetFunc))
#define NCM_MSET_FUNC_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), NCM_TYPE_MSET_FUNC, NcmMSetFuncClass))
#define NCM_IS_MSET_FUNC(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), NCM_TYPE_MSET_FUNC))
#define NCM_IS_MSET_FUNC_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), NCM_TYPE_MSET_FUNC))
#define NCM_MSET_FUNC_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), NCM_TYPE_MSET_FUNC, NcmMSetFuncClass))

typedef struct _NcmMSetFuncClass NcmMSetFuncClass;
typedef struct _NcmMSetFunc NcmMSetFunc;

typedef void (*NcmMSetFuncN) (NcmMSetFunc *func, NcmMSet *mset, const gdouble *x, gdouble *res);

struct _NcmMSetFunc
{
  /*< private >*/
  GObject parent_instance;
  guint nvar;
  guint dim;
  NcmVector *eval_x;
  gchar *name;
  gchar *symbol;
  gchar *ns;
  gchar *desc;
  gchar *uname;
  gchar *usymbol;
};

struct _NcmMSetFuncClass
{
  /*< private >*/
  GObjectClass parent_class;
  NcmMSetFuncN eval;
};

GType ncm_mset_func_get_type (void) G_GNUC_CONST;

NcmMSetFunc *ncm_mset_func_new (NcmMSetFuncN func, guint nvar, guint dim, gpointer obj, GDestroyNotify free);
NcmMSetFunc *ncm_mset_func_ref (NcmMSetFunc *func);

void ncm_mset_func_free (NcmMSetFunc *func);
void ncm_mset_func_clear (NcmMSetFunc **func);

GPtrArray *ncm_mset_func_array_new (void);

G_INLINE_FUNC void ncm_mset_func_eval (NcmMSetFunc *func, NcmMSet *mset, const gdouble *x, gdouble *res);
G_INLINE_FUNC gdouble ncm_mset_func_eval_nvar (NcmMSetFunc *func, NcmMSet *mset, const gdouble *x);
G_INLINE_FUNC gdouble ncm_mset_func_eval0 (NcmMSetFunc *func, NcmMSet *mset);
G_INLINE_FUNC gdouble ncm_mset_func_eval1 (NcmMSetFunc *func, NcmMSet *mset, const gdouble x);
G_INLINE_FUNC void ncm_mset_func_eval_vector (NcmMSetFunc *func, NcmMSet *mset, NcmVector *x_v, NcmVector *res_v);

void ncm_mset_func_set_eval_x (NcmMSetFunc *func, gdouble *x, guint len);

gboolean ncm_mset_func_is_scalar (NcmMSetFunc *func);
gboolean ncm_mset_func_is_vector (NcmMSetFunc *func, guint dim);
gboolean ncm_mset_func_is_const (NcmMSetFunc *func);
gboolean ncm_mset_func_has_nvar (NcmMSetFunc *func, guint nvar);

guint ncm_mset_func_get_nvar (NcmMSetFunc *func);
guint ncm_mset_func_get_dim (NcmMSetFunc *func);

const gchar *ncm_mset_func_peek_name (NcmMSetFunc *func);
const gchar *ncm_mset_func_peek_symbol (NcmMSetFunc *func);
const gchar *ncm_mset_func_peek_ns (NcmMSetFunc *func);
const gchar *ncm_mset_func_peek_desc (NcmMSetFunc *func);

const gchar *ncm_mset_func_peek_uname (NcmMSetFunc *func);
const gchar *ncm_mset_func_peek_usymbol (NcmMSetFunc *func);

NcmVector *ncm_mset_func_numdiff_fparams (NcmMSetFunc *func, NcmMSet *mset, const gdouble *x, NcmVector *out);

G_END_DECLS

#endif /* _NCM_MSET_FUNC_H_ */

#ifndef _NCM_MSET_FUNC_INLINE_H_
#define _NCM_MSET_FUNC_INLINE_H_
#ifdef NUMCOSMO_HAVE_INLINE

G_BEGIN_DECLS

G_INLINE_FUNC void
ncm_mset_func_eval (NcmMSetFunc *func, NcmMSet *mset, const gdouble *x, gdouble *res)
{
  NCM_MSET_FUNC_GET_CLASS (func)->eval (func, mset, x, res);
}

G_INLINE_FUNC gdouble
ncm_mset_func_eval_nvar (NcmMSetFunc *func, NcmMSet *mset, const gdouble *x)
{
  gdouble res;
  NCM_MSET_FUNC_GET_CLASS (func)->eval (func, mset, x, &res);
  return res;
}

G_INLINE_FUNC gdouble
ncm_mset_func_eval0 (NcmMSetFunc *func, NcmMSet *mset)
{
  gdouble res;
#ifdef NCM_MSET_FUNC_CHECK_TYPE
  g_assert_cmpint (func->dim,  ==, 1);
  g_assert ((func->nvar == 0) || (func->eval_x != NULL));
#endif
  
  NCM_MSET_FUNC_GET_CLASS (func)->eval (func, mset, (func->eval_x != NULL) ? ncm_vector_data (func->eval_x) : NULL, &res);

  return res;
}

G_INLINE_FUNC gdouble
ncm_mset_func_eval1 (NcmMSetFunc *func, NcmMSet *mset, const gdouble x)
{
  gdouble res;
#ifdef NCM_MSET_FUNC_CHECK_TYPE
  g_assert_cmpint (func->dim,  ==, 1);
  g_assert_cmpint (func->nvar, ==, 1);  
#endif
  NCM_MSET_FUNC_GET_CLASS (func)->eval (func, mset, &x, &res);
  return res;
}

G_INLINE_FUNC void 
ncm_mset_func_eval_vector (NcmMSetFunc *func, NcmMSet *mset, NcmVector *x_v, NcmVector *res_v)
{
  guint i;
#ifdef NCM_MSET_FUNC_CHECK_TYPE
  g_assert_cmpint (func->dim,  ==, 1);
  g_assert_cmpint (func->nvar, ==, 1);
  g_assert_cmpuint (ncm_vector_len (x_v), <=, ncm_vector_len (res_v));
#endif
  for (i = 0; i < ncm_vector_len (x_v); i++)
  {
    NCM_MSET_FUNC_GET_CLASS (func)->eval (func, mset, ncm_vector_ptr (x_v, i), ncm_vector_ptr (res_v, i));
  }
}

G_END_DECLS

#endif /* NUMCOSMO_HAVE_INLINE */
#endif /* _NCM_MSET_FUNC_INLINE_H_ */
