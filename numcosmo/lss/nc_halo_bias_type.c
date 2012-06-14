/***************************************************************************
 *            nc_halo_bias_type.c
 *
 *  Tue June 28 15:41:57 2011
 *  Copyright  2011  Mariana Penna Lima
 *  <pennalima@gmail.com>
 ****************************************************************************/
/*
 * numcosmo
 * Copyright (C) Mariana Penna Lima 2012 <pennalima@gmail.com>
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

/**
 * SECTION:nc_halo_bias_type
 * @title: Halo Bias Function Type
 * @short_description: FIXME
 *
 * FIXME
 */
  
#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif /* HAVE_CONFIG_H */
#include <numcosmo/numcosmo.h>
  
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <glib.h>
#include <gsl/gsl_integration.h>
#include <gsl/gsl_const_mksa.h>
#include <glib.h>

G_DEFINE_TYPE (NcHaloBiasType, nc_halo_bias_type, G_TYPE_OBJECT);

/**
 * nc_halo_bias_type_new_from_name:
 * @bias_name: string which specifies the multiplicity function type.
 * 
 * This function returns a new #NcMultiplicityFunc whose type is defined by @multiplicity_name.
 * 
 * Returns: A new #NcHaloBiasType.
 */
NcHaloBiasType *
nc_halo_bias_type_new_from_name (gchar *bias_name)
{
  GType bias_type = g_type_from_name (bias_name);
  if (bias_type == 0)
  {
	g_message ("# Invalid name of halo bias type %s\n", bias_name);
	g_error ("Aborting...");
  }
  else if (!g_type_is_a (bias_type, NC_TYPE_HALO_BIAS_TYPE))
	g_error ("nc_halo_bias_type_new_from_name: NcHaloBiasType %s do not descend from %s\n", bias_name, g_type_name (NC_TYPE_HALO_BIAS_TYPE));
  return g_object_new (bias_type, NULL);
}

/**
 * nc_halo_bias_type_eval:
 * @biasf: a #NcHaloBiasType.
 * @sigma: FIXME
 * @z: redshift.
 *
 * FIXME
 * 
 * Returns: FIXME 
*/ 
gdouble
nc_halo_bias_type_eval (NcHaloBiasType *biasf, gdouble sigma, gdouble z)
{
  return NC_HALO_BIAS_TYPE_GET_CLASS (biasf)->eval (biasf, sigma, z);
}

/**
 * nc_halo_bias_type_free: 
 * @biasf: a #NcHaloBiasType.
 *
 * Atomically decrements the reference count of @biasf by one. If the reference count drops to 0, 
 * all memory allocated by @biasf is released.
 *
*/
void
nc_halo_bias_type_free (NcHaloBiasType *biasf)
{
  g_clear_object (&biasf);
}

static void
nc_halo_bias_type_init (NcHaloBiasType *nc_halo_bias_type)
{
  /* TODO: Add initialization code here */
}

static void
_nc_halo_bias_type_finalize (GObject *object)
{
  /* TODO: Add deinitalization code here */

  G_OBJECT_CLASS (nc_halo_bias_type_parent_class)->finalize (object);
}

static void
nc_halo_bias_type_class_init (NcHaloBiasTypeClass *klass)
{
  GObjectClass* object_class = G_OBJECT_CLASS (klass);
  //GObjectClass* parent_class = G_OBJECT_CLASS (klass);

  object_class->finalize = _nc_halo_bias_type_finalize;
}

