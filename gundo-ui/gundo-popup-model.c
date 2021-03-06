/* This file is part of gundo
 *
 * AUTHORS
 *     Sven Herzberg  <herzi@lanedo.com>
 *
 * Copyright (C) 2009  Sven Herzberg
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA
 */

#include "gundo-popup-model.h"

/**
 * GUndoPopupModel:
 *
 * A shared base class for #GUndoRedoModel and #GUndoUndoModel.
 */

struct _GUndoPopupModelPrivate {
  GundoHistory* history;
};

#define PRIV(i) (((GUndoPopupModel*)(i))->_private)

enum {
  PROP_0,
  PROP_HISTORY
};

G_DEFINE_ABSTRACT_TYPE (GUndoPopupModel, gundo_popup_model, G_TYPE_OBJECT);

static void
gundo_popup_model_init (GUndoPopupModel* self)
{
  PRIV (self) = G_TYPE_INSTANCE_GET_PRIVATE (self, GUNDO_TYPE_POPUP_MODEL, GUndoPopupModelPrivate);
}

static void
model_finalize (GObject* object)
{
  g_object_unref (PRIV (object)->history);

  G_OBJECT_CLASS (gundo_popup_model_parent_class)->finalize (object);
}

static void
model_get_property (GObject   * object,
                    guint       prop_id,
                    GValue    * value,
                    GParamSpec* pspec)
{
  switch (prop_id)
    {
      case PROP_HISTORY:
        g_value_set_object (value, PRIV (object)->history);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
        break;
    }
}

static void
model_set_property (GObject     * object,
                    guint         prop_id,
                    GValue const* value,
                    GParamSpec  * pspec)
{
  switch (prop_id)
    {
      case PROP_HISTORY:
        g_return_if_fail (!PRIV (object)->history);

        PRIV (object)->history = g_value_dup_object (value);

        g_return_if_fail (PRIV (object)->history);

        g_object_notify (object, "history");
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
        break;
    }
}

static void
gundo_popup_model_class_init (GUndoPopupModelClass* self_class)
{
  GObjectClass* object_class = G_OBJECT_CLASS (self_class);

  object_class->finalize     = model_finalize;
  object_class->get_property = model_get_property;
  object_class->set_property = model_set_property;

  g_object_class_install_property (object_class,
                                   PROP_HISTORY,
                                   g_param_spec_object ("history", "history", "history",
                                                        GUNDO_TYPE_HISTORY,
                                                        G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_type_class_add_private (self_class, sizeof (GUndoPopupModelPrivate));
}

/**
 * gundo_popup_model_get_history:
 * @self: a #GUndoPopupModel
 *
 * Get the #GundoHistory assigned to a #GUndoPopupModel.
 *
 * Returns: the #GundoHistory of @self, %NULL if there is none.
 */
GundoHistory*
gundo_popup_model_get_history (GUndoPopupModel* self)
{
  g_return_val_if_fail (GUNDO_IS_POPUP_MODEL (self), NULL);

  return PRIV (self)->history;
}

