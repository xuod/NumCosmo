/***************************************************************************
 *            ncm_rng.h
 *
 *  Sat August 17 12:39:57 2013
 *  Copyright  2013  Sandro Dias Pinto Vitenti
 *  <sandro@isoftware.com.br>
 ****************************************************************************/
/*
 * ncm_rng.h
 * Copyright (C) 2013 Sandro Dias Pinto Vitenti <sandro@isoftware.com.br>
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

#ifndef _NCM_RNG_H_
#define _NCM_RNG_H_

#include <glib.h>
#include <glib-object.h>
#include <numcosmo/build_cfg.h>

#include <gsl/gsl_randist.h>

G_BEGIN_DECLS

#define NCM_TYPE_RNG             (ncm_rng_get_type ())
#define NCM_RNG(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), NCM_TYPE_RNG, NcmRNG))
#define NCM_RNG_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), NCM_TYPE_RNG, NcmRNGClass))
#define NCM_IS_RNG(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), NCM_TYPE_RNG))
#define NCM_IS_RNG_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), NCM_TYPE_RNG))
#define NCM_RNG_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), NCM_TYPE_RNG, NcmRNGClass))

typedef struct _NcmRNGClass NcmRNGClass;
typedef struct _NcmRNG NcmRNG;

struct _NcmRNG
{
  /*< private >*/
  GObject parent_instance;
  gsl_rng *r;
  GMutex *lock;
#if !((GLIB_MAJOR_VERSION == 2) && (GLIB_MINOR_VERSION < 32))
  GMutex lock_m;
#endif
};

struct _NcmRNGClass
{
  /*< private >*/
  GObjectClass parent_class;
};

GType ncm_rng_get_type (void) G_GNUC_CONST;

NcmRNG *ncm_rng_new (const gchar *algo);
NcmRNG *ncm_rng_ref (NcmRNG *rng);
void ncm_rng_free (NcmRNG *rng);
void ncm_rng_clear (NcmRNG **rng);

void ncm_rng_lock (NcmRNG *rng);
void ncm_rng_unlock (NcmRNG *rng);

const gchar *ncm_rng_get_algo (NcmRNG *rng);
gchar *ncm_rng_get_state (NcmRNG *rng);
void ncm_rng_set_algo (NcmRNG *rng, const gchar *algo);
void ncm_rng_set_state (NcmRNG *rng, const gchar *state);

NcmRNG *ncm_rng_pool_get (const gchar *name);

G_END_DECLS

#endif /* _NCM_RNG_H_ */
