/*
 * Copyright (C) 2019 Alexandros Theodotou <alex at zrythm dot org>
 *
 * This file is part of Zrythm
 *
 * Zrythm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Zrythm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Zrythm.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "audio/chord_descriptor.h"
#include "gui/widgets/chord_pad.h"
#include "utils/gtk.h"
#include "utils/resources.h"

G_DEFINE_TYPE (ChordPadWidget,
               chord_pad_widget,
               GTK_TYPE_GRID)

static void
chord_pad_widget_init (
  ChordPadWidget * self)
{
  gtk_widget_init_template (GTK_WIDGET (self));
}

static void
chord_pad_widget_class_init (
  ChordPadWidgetClass * _klass)
{
  GtkWidgetClass * klass = GTK_WIDGET_CLASS (_klass);
  resources_set_class_template (
    klass, "chord_pad.ui");

  gtk_widget_class_set_css_name (
    klass, "chord-pad");

  gtk_widget_class_bind_template_child (
    klass,
    ChordPadWidget,
    pads_overlay);
}
