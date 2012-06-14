/***************************************************************************
 *            ncm_lapack.c
 *
 *  Sun March 18 22:33:15 2012
 *  Copyright  2012  Sandro Dias Pinto Vitenti
 *  <sandro@isoftware.com.br>
 ****************************************************************************/
/*
 * numcosmo
 * Copyright (C) Sandro Dias Pinto Vitenti 2012 <sandro@lapsandro>
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
 * SECTION:ncm_lapack
 * @title: Helper C functions to call lapack
 * @short_description: FIXME
 *
 * FIXME
 */

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif /* HAVE_CONFIG_H */
#include <numcosmo/numcosmo.h>

#include <glib.h>

/**
 * dptsv_: (skip)
 * @N: FIXME
 * @NRHS: FIXME
 * @d: FIXME
 * @e: FIXME
 * @b: FIXME
 * @ldb: FIXME
 * @info: FIXME
 *
 * FIXME
 *
 */

/**
 * ncm_lapack_dptsv:
 * @d: FIXME
 * @e: FIXME
 * @b: FIXME
 * @size: FIXME
 *
 * FIXME
 *
 * Returns: FIXME
 */
#ifndef NUMCOSMO_HAVE_INLINE
#define NUMCOSMO_HAVE_INLINE
#undef _NCM_LAPACK_INLINE_H_
#define G_INLINE_FUNC
#include "ncm_lapack.h"
#undef _NCM_LAPACK_INLINE_H_
#undef NUMCOSMO_HAVE_INLINE
#endif /* NUMCOSMO_HAVE_INLINE */
