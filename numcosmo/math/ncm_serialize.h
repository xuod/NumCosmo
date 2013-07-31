/***************************************************************************
 *            ncm_serialize.h
 *
 *  Mon August 26 13:37:39 2013
 *  Copyright  2013  Sandro Dias Pinto Vitenti
 *  <sandro@isoftware.com.br>
 ****************************************************************************/
/*
 * ncm_serialize.h
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

#ifndef _NCM_SERIALIZE_H_
#define _NCM_SERIALIZE_H_

#include <glib.h>
#include <glib/gstdio.h>
#include <glib-object.h>

G_BEGIN_DECLS

#define NCM_TYPE_SERIALIZE             (ncm_serialize_get_type ())
#define NCM_SERIALIZE(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), NCM_TYPE_SERIALIZE, NcmSerialize))
#define NCM_SERIALIZE_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), NCM_TYPE_SERIALIZE, NcmSerializeClass))
#define NCM_IS_SERIALIZE(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), NCM_TYPE_SERIALIZE))
#define NCM_IS_SERIALIZE_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), NCM_TYPE_SERIALIZE))
#define NCM_SERIALIZE_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), NCM_TYPE_SERIALIZE, NcmSerializeClass))

typedef struct _NcmSerializeClass NcmSerializeClass;
typedef struct _NcmSerialize NcmSerialize;
typedef struct _NcmSerializePrivate NcmSerializePrivate;

/**
 * NcmSerializeOpt:
 * @NCM_SERIALIZE_NONE: Use default serialization.
 * @NCM_SERIALIZE_EXPAND_ALL: Whether to exand all objects during serialization.
 * @NCM_SERIALIZE_EXPLICIT_VM: Whether to use explicit vectors and matrices instead of array (or array of array) of doubles.
 * 
 * Options for serialization.
 * 
 */
typedef enum _NcmSerializeOpt
{
  NCM_SERIALIZE_NONE        = 0,
  NCM_SERIALIZE_EXPAND_ALL  = 1 << 0,
  NCM_SERIALIZE_EXPLICIT_VM = 1 << 1,
} NcmSerializeOpt;

struct _NcmSerialize
{
  /*< private >*/
  GObject parent_instance;
  GHashTable *name_ptr;
  GHashTable *ptr_name;
  GRegex *is_named_regex;
  GRegex *parse_obj_regex;
  NcmSerializeOpt opts;
};

struct _NcmSerializeClass
{
  /*< private >*/
  GObjectClass parent_class;
};

GType ncm_serialize_get_type (void) G_GNUC_CONST;

NcmSerialize *ncm_serialize_new (void);
NcmSerialize *ncm_serialize_ref (NcmSerialize *ser);
void ncm_serialize_free (NcmSerialize *ser);
void ncm_serialize_unref (NcmSerialize *ser);
void ncm_serialize_clear (NcmSerialize **ser);
void ncm_serialize_reset (NcmSerialize *ser);

gboolean ncm_serialize_contain_instance (NcmSerialize *ser, gpointer obj);
gboolean ncm_serialize_contain_name (NcmSerialize *ser, gchar *name);
guint ncm_serialize_count_instances (NcmSerialize *ser);
gpointer ncm_serialize_get_by_name (NcmSerialize *ser, gchar *name);
gchar *ncm_serialize_peek_name (NcmSerialize *ser, gpointer obj);
void ncm_serialize_set (NcmSerialize *ser, gpointer obj, gchar *name, gboolean overwrite);
gboolean ncm_serialize_is_named (NcmSerialize *ser, const gchar *serobj, gchar **name);

void ncm_serialize_set_property (NcmSerialize *ser, GObject *obj, const gchar *prop_str);
GObject *ncm_serialize_create_from_name_params (NcmSerialize *ser, const gchar *obj_name, GVariant *params);
GObject *ncm_serialize_create_from_string (NcmSerialize *ser, const gchar *obj_ser);
GVariant *ncm_serialize_gvalue_to_gvariant (NcmSerialize *ser, GValue *val);
GVariant *ncm_serialize_to_variant (NcmSerialize *ser, GObject *obj);
gchar *ncm_serialize_to_string (NcmSerialize *ser, GObject *obj, gboolean valid_variant);

/* Global NcmSerialize object */

NcmSerialize *ncm_serialize_global (void);
void ncm_serialize_global_reset (void);
gboolean ncm_serialize_global_contain_instance (gpointer obj);
gboolean ncm_serialize_global_contain_name (gchar *name);
guint ncm_serialize_global_count_instances (void);
gpointer ncm_serialize_global_get_by_name (gchar *name);
gchar *ncm_serialize_global_global_peek_name (gpointer obj);
void ncm_serialize_global_set (gpointer obj, gchar *name, gboolean overwrite);
gboolean ncm_serialize_global_is_named (const gchar *serobj, gchar **name);

void ncm_serialize_global_set_property (GObject *obj, const gchar *prop_str);
GObject *ncm_serialize_global_create_from_name_params (const gchar *obj_name, GVariant *params);
GObject *ncm_serialize_global_create_from_string (const gchar *obj_ser);
GVariant *ncm_serialize_global_gvalue_to_gvariant (GValue *val);
GVariant *ncm_serialize_global_to_variant (GObject *obj);
gchar *ncm_serialize_global_to_string (GObject *obj, gboolean valid_variant);

#define NCM_SERIALIZE_PROPERTY_TYPE "{sv}"
#define NCM_SERIALIZE_PROPERTIES_TYPE "a"NCM_SERIALIZE_PROPERTY_TYPE
#define NCM_SERIALIZE_OBJECT_TYPE "{s"NCM_SERIALIZE_PROPERTIES_TYPE"}"
#define NCM_SERIALIZE_OBJECT_FORMAT "{s@"NCM_SERIALIZE_PROPERTIES_TYPE"}"
#define NCM_SERIALIZE_VECTOR_TYPE "ad"
#define NCM_SERIALIZE_MATRIX_TYPE "aad"

G_END_DECLS

#endif /* _NCM_SERIALIZE_H_ */
