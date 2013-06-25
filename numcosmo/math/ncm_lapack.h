/***************************************************************************
 *            ncm_lapack.h
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

#ifndef _NCM_LAPACK_H_
#define _NCM_LAPACK_H_

#include <glib.h>
#include <numcosmo/build_cfg.h>

G_BEGIN_DECLS

#ifdef NUMCOSMO_HAVE_LAPACK
void dptsv_ (gint *N, gint *NRHS, gdouble *d, gdouble *e, gdouble *b, gint *ldb, gint *info);
G_INLINE_FUNC gint ncm_lapack_dptsv (gdouble *d, gdouble *e, gdouble *b, guint size);
#endif /* NUMCOSMO_HAVE_LAPACK */

#define NCM_LAPACK_CHECK_INFO(func,info) if ((info) != 0) g_error ("Lapack[%s] error %d", func, (info))

G_END_DECLS

#endif /* _NCM_LAPACK_H_ */

#ifndef _NCM_LAPACK_INLINE_H_
#define _NCM_LAPACK_INLINE_H_
#ifdef NUMCOSMO_HAVE_INLINE

G_BEGIN_DECLS

#ifdef NUMCOSMO_HAVE_LAPACK
G_INLINE_FUNC gint
ncm_lapack_dptsv (gdouble *d, gdouble *e, gdouble *b, guint size)
{
	gint N = size;
	gint NRHS = 1;
	gint LDB = N;
	gint info;
	dptsv_ (&N, &NRHS, d, e, b, &LDB, &info);
	return info;
}
#endif /* NUMCOSMO_HAVE_LAPACK */

G_END_DECLS

#endif /* NUMCOSMO_HAVE_INLINE */
#endif /* _NCM_LAPACK_INLINE_H_ */
