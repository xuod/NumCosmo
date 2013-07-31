/***************************************************************************
 *            ncm_cfg.h
 *
 *  Wed Aug 13 20:58:50 2008
 *  Copyright  2008  Sandro Dias Pinto Vitenti
 *  <sandro@isoftware.com.br>
 ****************************************************************************/
/*
 * numcosmo
 * Copyright (C) Sandro Dias Pinto Vitenti 2012 <sandro@isoftware.com.br>
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

#ifndef _NCM_CFG_H
#define _NCM_CFG_H

#include <glib.h>
#include <glib/gstdio.h>
#include <glib-object.h>
#include <numcosmo/build_cfg.h>
#include <stdio.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_spline.h>
#include <gsl/gsl_rng.h>
#ifdef NUMCOSMO_HAVE_SQLITE3
#include <sqlite3.h>
#endif
#include <numcosmo/math/ncm_spline.h>

G_BEGIN_DECLS

#ifndef __STDC_IEC_559__
#error "Error: "PACKAGE_NAME" requires IEEE 754 floating point."
#endif /* __STDC_IEC_559__ */

void ncm_cfg_init (void);
void ncm_cfg_enable_gsl_err_handler (void);
void ncm_cfg_register_obj (GType obj);
gchar *ncm_cfg_get_fullpath (const gchar *filename, ...);
void ncm_cfg_keyfile_to_arg (GKeyFile *kfile, gchar *group_name, GOptionEntry *entries, gchar **argv, gint *argc);
void ncm_cfg_entries_to_keyfile (GKeyFile *kfile, gchar *group_name, GOptionEntry *entries);
gchar *ncm_cfg_string_to_comment (const gchar *str);
const GEnumValue *ncm_cfg_get_enum_by_id_name_nick (GType enum_type, const gchar *id_name_nick);
void ncm_cfg_enum_print_all (GType enum_type, gchar *header);

gboolean ncm_cfg_load_fftw_wisdom (gchar *filename, ...);
gboolean ncm_cfg_save_fftw_wisdom (gchar *filename, ...);
gboolean ncm_cfg_exists (gchar *filename, ...);

void ncm_cfg_set_logfile (gchar *filename);
void ncm_cfg_logfile (gboolean on);
void ncm_cfg_logfile_flush (gboolean on);

void ncm_message (gchar *msg, ...);
gchar *ncm_string_ww (const gchar *msg, const gchar *first, const gchar *rest, guint ncols);
void ncm_message_ww (const gchar *msg, const gchar *first, const gchar *rest, guint ncols);
void ncm_cfg_msg_sepa (void);

FILE *ncm_cfg_fopen (gchar *filename, gchar *mode, ...);
FILE *ncm_cfg_vfopen (gchar *filename, gchar *mode, va_list ap);

gboolean ncm_cfg_load_spline (gchar *filename, const gsl_interp_type *stype, NcmSpline **s, ...);
gboolean ncm_cfg_save_spline (gchar *filename, NcmSpline *s, ...);

gboolean ncm_cfg_load_vector (gchar *filename, gsl_vector *v, ...);
gboolean ncm_cfg_save_vector (gchar *filename, gsl_vector *v, ...);
gboolean ncm_cfg_load_matrix (gchar *filename, gsl_matrix *M, ...);
gboolean ncm_cfg_save_matrix (gchar *filename, gsl_matrix *M, ...);

#define LOAD_SAVE_VECTOR_MATRIX_DEF(typen) \
gboolean ncm_cfg_load_vector_##typen (gchar *filename, gsl_vector_##typen *v, ...); \
gboolean ncm_cfg_save_vector_##typen (gchar *filename, gsl_vector_##typen *v, ...); \
gboolean ncm_cfg_load_matrix_##typen (gchar *filename, gsl_matrix_##typen *M, ...); \
gboolean ncm_cfg_save_matrix_##typen (gchar *filename, gsl_matrix_##typen *M, ...);

LOAD_SAVE_VECTOR_MATRIX_DEF(int)
LOAD_SAVE_VECTOR_MATRIX_DEF(float)
LOAD_SAVE_VECTOR_MATRIX_DEF(complex)

#ifdef NUMCOSMO_HAVE_SQLITE3
sqlite3 *ncm_cfg_get_default_sqlite3 (void);
#endif

typedef union _NcmDoubleInt64
{
  gint64 i;
  gdouble x;
} NcmDoubleInt64;

gchar *ncm_cfg_command_line (gchar *argv[], gint argc);

/* Macros */

#define NCM_GARRAY_MEMCPY(dest,src) \
G_STMT_START { \
g_assert_cmpuint ((src)->len, ==, (dest)->len); \
g_assert_cmpuint (g_array_get_element_size (src), ==, g_array_get_element_size (dest)); \
memcpy ((dest)->data, (src)->data, (src)->len * g_array_get_element_size (src)); \
} G_STMT_END

#define NCM_GARRAY_DUP(dest,src) \
G_STMT_START { \
dest = g_array_sized_new (FALSE, FALSE, g_array_get_element_size (src), (src)->len); \
g_array_set_size ((dest), (src)->len); \
memcpy ((dest)->data, (src)->data, (src)->len * g_array_get_element_size (src)); \
} G_STMT_END

#ifdef SUNDIALS_USES_LONG_INT
#define _NCM_SUNDIALS_INT_TYPE glong
#else
#define _NCM_SUNDIALS_INT_TYPE gint
#endif

#if (GLIB_MAJOR_VERSION == 2) && (GLIB_MINOR_VERSION < 32)
#define _NCM_MUTEX_LOCK(l) g_static_mutex_lock (l)
#define _NCM_MUTEX_UNLOCK(l) g_static_mutex_unlock (l)
#define _NCM_MUTEX_TRYLOCK(l) g_static_mutex_trylock (l)
#define _NCM_MUTEX_TYPE GStaticMutex
#define _NCM_STATIC_MUTEX_DECL(l) static GStaticMutex l = G_STATIC_MUTEX_INIT
#define _NCM_MUTEX_INIT(l) g_static_mutex_init (l)
#define _NCM_MUTEX_CLEAR(l) g_static_mutex_free (l)
#else
#define _NCM_MUTEX_LOCK(l) g_mutex_lock (l)
#define _NCM_MUTEX_UNLOCK(l) g_mutex_unlock (l)
#define _NCM_MUTEX_TRYLOCK(l) g_mutex_trylock (l)
#define _NCM_MUTEX_TYPE GMutex
#define _NCM_STATIC_MUTEX_DECL(l) static GMutex l
#define _NCM_MUTEX_INIT(l) g_mutex_init (l)
#define _NCM_MUTEX_CLEAR(l) g_mutex_clear (l)
#endif

#define NCM_RETURN_IF_INF(a) if (gsl_isinf(a)) return a

#define NCM_FLOOR_TRUNC(a,b) (floor ((b) * (a)) / (b))
#define NCM_CEIL_TRUNC(a,b) (ceil ((b) * (a)) / (b))
#define NCM_ROUND_TRUNC(a,b) (round ((b) * (a)) / (b))

#define NCM_TEST_GSL_RESULT(func,ret) if (ret != GSL_SUCCESS) g_error ("%s: %s", func, gsl_strerror (ret))

#define NCM_ZERO_LIMIT 1e-13
#define NCM_DEFAULT_PRECISION 1e-7

#ifndef NCM_THREAD_POOL_MAX
#define NCM_THREAD_POOL_MAX 5
#endif

#ifndef mpz_inits
#define mpz_inits ncm_mpz_inits
#endif /* mpz_inits */

#ifndef mpz_clears
#define mpz_clears ncm_mpz_clears
#endif /* mpz_inits */

#define NCM_COMPLEX_INC_MUL_REAL_TEST(a,b,c) \
((fabs(GSL_REAL((b))*(c)/GSL_REAL((a))) < 1e-16) && (fabs(GSL_IMAG((b))*(c)/GSL_IMAG((a))) < 1e-16))

#define NCM_COMPLEX_INC_MUL_REAL(a,b,c) \
G_STMT_START { \
  GSL_REAL((a)) += GSL_REAL((b))*(c); \
  GSL_IMAG((a)) += GSL_IMAG((b))*(c); \
} G_STMT_END

#define NCM_COMPLEX_INC_MUL(a,b,c) \
G_STMT_START { \
  GSL_REAL((a)) += GSL_REAL((b)) * GSL_REAL((c)) - GSL_IMAG((b)) * GSL_IMAG((c)); \
  GSL_IMAG((a)) += GSL_REAL((b)) * GSL_IMAG((c)) + GSL_IMAG((b)) * GSL_REAL((c)); \
} G_STMT_END

#define NCM_COMPLEX_INC_MUL_MUL_REAL(a,b,c,d) \
G_STMT_START { \
  GSL_REAL((a)) += (GSL_REAL((b)) * GSL_REAL((c)) - GSL_IMAG((b)) * GSL_IMAG((c))) * (d); \
  GSL_IMAG((a)) += (GSL_REAL((b)) * GSL_IMAG((c)) + GSL_IMAG((b)) * GSL_REAL((c))) * (d); \
} G_STMT_END

#define NCM_COMPLEX_MUL_REAL(a,b,c) \
G_STMT_START { \
  GSL_REAL((a)) = GSL_REAL((b)) * (c); \
  GSL_IMAG((a)) = GSL_IMAG((b)) * (c); \
} G_STMT_END

#define NCM_COMPLEX_MUL(a,b) \
G_STMT_START { \
  gdouble temp = GSL_REAL((a)) * GSL_REAL((b)) - GSL_IMAG((a)) * GSL_IMAG((b)); \
  GSL_IMAG(a) = GSL_REAL((a)) * GSL_IMAG((b)) + GSL_IMAG((a)) * GSL_REAL((b)); \
  GSL_REAL(a) = temp; \
} G_STMT_END

#define NCM_COMPLEX_ADD(a,b) \
G_STMT_START { \
  GSL_REAL(a) += GSL_REAL((b)); \
  GSL_IMAG(a) += GSL_IMAG((b)); \
} G_STMT_END

#define NCM_COMPLEX_MUL_CONJUGATE(a,b) \
G_STMT_START { \
  gdouble temp = GSL_REAL((a)) * GSL_REAL((b)) + GSL_IMAG((a)) * GSL_IMAG((b)); \
  GSL_IMAG(a) = - GSL_REAL((a)) * GSL_IMAG((b)) + GSL_IMAG((a)) * GSL_REAL((b)); \
  GSL_REAL(a) = temp; \
} G_STMT_END

#define NCM_CFG_DEFAULT_SQLITE3_FILENAME "data_observation.sqlite3"

#define NCM_WRITE_INT32(_ff,_ii) G_STMT_START { gint32 _temp_i = GINT32_TO_BE ((_ii)); if (fwrite (&_temp_i, sizeof(gint32), (1), _ff) != 1) g_error ("NCM_WRITE_INT32: io error"); } G_STMT_END
#define NCM_WRITE_UINT32(_ff,_ii) G_STMT_START { guint32 _temp_i = GUINT32_TO_BE ((_ii)); if (fwrite (&_temp_i, sizeof(guint32), (1), _ff) != 1) g_error ("NCM_WRITE_UINT32: io error"); } G_STMT_END

#define NCM_WRITE_INT64(_ff,_ii) G_STMT_START { gint64 _temp_i = GINT64_TO_BE ((_ii)); if (fwrite (&_temp_i, sizeof(gint64), (1), _ff) != 1) g_error ("NCM_WRITE_INT64: io error"); } G_STMT_END
#define NCM_WRITE_UINT64(_ff,_ii) G_STMT_START { guint64 _temp_i = GUINT64_TO_BE ((_ii)); if (fwrite (&_temp_i, sizeof(guint64), (1), _ff) != 1) g_error ("NCM_WRITE_INT64: io error"); } G_STMT_END

#define NCM_WRITE_DOUBLE(_ff,_ii) G_STMT_START { NcmDoubleInt64 _iii; _iii.x = _ii; _iii.i = GINT64_TO_BE ((_iii.i)); if (fwrite (&_iii.i, sizeof(gint64), (1), _ff) != 1) g_error ("NCM_WRITE_DOUBLE: io error"); } G_STMT_END

#define NCM_READ_INT32(_ff,_ii) G_STMT_START { gint32 _temp_i; if (fread (&_temp_i, sizeof(gint32), (1), _ff) != 1) g_error ("NCM_READ_INT32: io error"); _ii = GINT32_FROM_BE (_temp_i); } G_STMT_END
#define NCM_READ_UINT32(_ff,_ii) G_STMT_START { guint32 _temp_i; if (fread (&_temp_i, sizeof(guint32), (1), _ff) != 1) g_error ("NCM_READ_UINT32: io error"); _ii = GUINT32_FROM_BE (_temp_i); } G_STMT_END

#define NCM_READ_INT64(_ff,_ii) G_STMT_START { gint64 _temp_i; if (fread (&_temp_i, sizeof(gint64), (1), _ff) != 1) g_error ("NCM_READ_INT64: io error"); _ii = GINT64_FROM_BE (_temp_i); } G_STMT_END
#define NCM_READ_UINT64(_ff,_ii) G_STMT_START { guint64 _temp_i; if (fread (&_temp_i, sizeof(guint64), (1), _ff) != 1) g_error ("NCM_READ_UINT64: io error"); _ii = GUINT64_FROM_BE (_temp_i); } G_STMT_END

#define NCM_READ_DOUBLE(_ff,_ii) G_STMT_START { NcmDoubleInt64 _iii; if (fread (&_iii.i, sizeof(gint64), (1), _ff) != 1) g_error ("NCM_READ_DOUBLE: io error"); _iii.i = GINT64_FROM_BE (_iii.i); _ii = _iii.x; } G_STMT_END

/* Workaround on g_clear_pointer */
#if ((GLIB_MAJOR_VERSION == 2) && (GLIB_MINOR_VERSION < 34))
#define g_clear_pointer(ptr,freefunc) G_STMT_START if (*(ptr) != NULL) { *(freefunc) (*(ptr)); *(ptr) = NULL; } G_STMT_END
#endif /* Glib version < 2.34 */

/* Workaround on g_clear_object. */
#if ((GLIB_MAJOR_VERSION == 2) && (GLIB_MINOR_VERSION < 28))
#define g_clear_object(obj) g_clear_pointer(ptr,g_object_unref)
#endif /* Glib version < 2.28 */

#define ncm_g_string_clear(s) G_STMT_START if (*(s) != NULL) { g_string_free (*(s), TRUE); *(s) = NULL; } G_STMT_END

G_END_DECLS

#endif /* _NCM_CFG_H */
