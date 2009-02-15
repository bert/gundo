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

#include "gundo-tool.h"

struct _GUndoToolPrivate {
        GundoHistory * history;
};

#define PRIV(i) (((GUndoTool*)(i))->_private)

enum {
        PROP_0,
        PROP_HISTORY
};

G_DEFINE_ABSTRACT_TYPE_WITH_CODE (GUndoTool, gundo_tool, GTK_TYPE_TOOL_ITEM,
                                  G_IMPLEMENT_INTERFACE (GUNDO_TYPE_HISTORY_VIEW, NULL));

static void
gundo_tool_init (GUndoTool* self)
{
  PRIV (self) = G_TYPE_INSTANCE_GET_PRIVATE (self, GUNDO_TYPE_TOOL, GUndoToolPrivate);
}

static void
tool_finalize (GObject* object)
{
  if (PRIV (object)->history)
    {
      gundo_history_view_unregister (GUNDO_HISTORY_VIEW (object), PRIV (object)->history);
      g_object_unref (PRIV (object)->history);
      PRIV (object)->history = NULL;
    }

  if (G_OBJECT_CLASS (gundo_tool_parent_class)->finalize)
    {
      G_OBJECT_CLASS (gundo_tool_parent_class)->finalize (object);
    }
}

static void
tool_get_property (GObject   * object,
                   guint       prop_id,
                   GValue    * value,
                   GParamSpec* pspec)
{
  switch (prop_id)
    {
      case PROP_HISTORY:
        g_value_set_object (value, gundo_tool_get_history (GUNDO_TOOL (object)));
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
        break;
    }
}

static void
tool_set_property (GObject     * object,
                   guint         prop_id,
                   GValue const* value,
                   GParamSpec  * pspec)
{
  switch (prop_id)
    {
      case PROP_HISTORY:
        gundo_tool_set_history (GUNDO_TOOL (object), g_value_get_object (value));
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
        break;
    }
}

static void
gundo_tool_class_init (GUndoToolClass* self_class)
{
  GObjectClass* object_class = G_OBJECT_CLASS (self_class);

  object_class->finalize     = tool_finalize;
  object_class->get_property = tool_get_property;
  object_class->set_property = tool_set_property;

  _gundo_history_view_install_properties(object_class, PROP_HISTORY);

  g_type_class_add_private (self_class, sizeof (GUndoToolPrivate));
}

GundoHistory*
gundo_tool_get_history (GUndoTool* self)
{
  g_return_val_if_fail (GUNDO_IS_TOOL (self), NULL);

  return PRIV (self)->history;
}

void
gundo_tool_set_history (GUndoTool   * self,
                        GundoHistory* history)
{
  g_return_if_fail (GUNDO_IS_TOOL (self));
  g_return_if_fail (!history || GUNDO_IS_HISTORY (history));

  if(PRIV (self)->history) {
          gundo_history_view_unregister(GUNDO_HISTORY_VIEW(self), PRIV (self)->history);
          g_object_unref(PRIV (self)->history);
          PRIV (self)->history = NULL;
  }

  if(history) {
          PRIV (self)->history = g_object_ref(history);
          gundo_history_view_register(GUNDO_HISTORY_VIEW(self), PRIV (self)->history);
  } else {
          gtk_widget_set_sensitive(GTK_WIDGET(self), FALSE);
  }

  g_object_notify(G_OBJECT(self), "history");
}

