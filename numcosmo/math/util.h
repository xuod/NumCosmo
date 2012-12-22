/***************************************************************************
 *            util.h
 *
 *  Mon Jul 16 18:02:22 2007
 *  Copyright  2007  Sandro Dias Pinto Vitenti
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

#ifndef _NCM_UTIL_H_
#define _NCM_UTIL_H_

#include <glib.h>
#include <glib-object.h>

#include <numcosmo/math/ncm_model.h>
#include <numcosmo/math/ncm_data.h>
#include <numcosmo/nc_distance.h>
#include <numcosmo/math/ncm_mset_func.h>

#include <gsl/gsl_rng.h>
#include <gsl/gsl_min.h>
#ifndef NUMCOSMO_GIR_SCAN
#include <gmp.h>
#include <mpfr.h>
#endif

G_BEGIN_DECLS

gulong ncm_random_seed (void);

void ncm_finite_diff_calc_J (NcmModel *model, NcmData *data, NcmMatrix *jac);

gdouble *ncm_smoothd (gdouble *in, size_t N, size_t points, size_t pass);
gboolean ncm_get_uniform_sample (NcmMSet *mset, NcmMSetFunc *func, gdouble x0, gdouble x1, NcmVector *sample);
#ifdef NUMCOSMO_HAVE_FFTW3
gboolean ncm_get_smoothed_uniform_sample (NcmMSet *mset, NcmMSetFunc *func, gdouble x0, gdouble x1, gdouble delta, NcmVector *sample);
#endif /* NUMCOSMO_HAVE_FFTW3 */

gdouble ncm_topology_comoving_a0_lss (guint n, gdouble alpha);
gdouble ncm_topology_sigma_comoving_a0_lss (guint n, gdouble alpha, gdouble sigma_alpha);

void ncm_rational_coarce_double (gdouble x, mpq_t q);

gdouble ncm_sphPlm_x (gint l, gint m, gint order);
gdouble ncm_sphPlm_test_theta (gdouble theta, gint lmax, gint *lmin_data);

gsize ncm_mpfr_out_raw (FILE *stream, mpfr_t op);
gsize ncm_mpfr_inp_raw (mpfr_t rop, FILE *stream);
gsize ncm_mpq_out_raw (FILE *f, mpq_t q);
gsize ncm_mpq_inp_raw (mpq_t q, FILE *f);

void ncm_mpz_inits (mpz_t z, ...);
void ncm_mpz_clears (mpz_t z, ...);

gdouble ncm_sum (gdouble *d, gulong n);
gdouble ncm_numdiff_1 (gsl_function *F, const gdouble x, const gdouble ho, gdouble *err);
gdouble ncm_numdiff_2 (gsl_function *F, gdouble *ofx, const gdouble x, const gdouble ho, gdouble *err);
gdouble ncm_numdiff_2_err (gsl_function *F, gdouble *ofx, const gdouble x, const gdouble ho, gdouble err, gdouble *ferr);
gdouble ncm_sqrt1px_m1 (gdouble x);
gint ncm_cmp (gdouble x, gdouble y, gdouble reltol);
void _ncm_assertion_message_cmpdouble (const gchar *domain, const gchar *file, gint line, const gchar *func, const gchar *expr, gdouble arg1, const gchar *cmp, gdouble arg2);

#define ncm_assert_cmpdouble(n1,cmp,n2) \
do { \
  if (ncm_cmp ((n1), (n2), GSL_DBL_EPSILON) cmp 0) ; else \
    _ncm_assertion_message_cmpdouble (G_LOG_DOMAIN, __FILE__, __LINE__, G_STRFUNC, \
                                      #n1 " " #cmp " " #n2, (n1), #cmp, (n2)); \
} while (0)

#define ncm_assert_cmpdouble_e(n1,cmp,n2,epsilon) \
do { \
  if (ncm_cmp ((n1), (n2), (epsilon)) cmp 0) ; else \
    _ncm_assertion_message_cmpdouble (G_LOG_DOMAIN, __FILE__, __LINE__, G_STRFUNC, \
                                      #n1 " " #cmp " " #n2, (n1), #cmp, (n2)); \
} while (0)

G_END_DECLS

#endif /* _NCM_UTIL_H_ */
