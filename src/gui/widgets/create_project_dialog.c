/*
 * Copyright (C) 2019 Alexandros Theodotou <alex at zrythm dot org>
 *
 * This file is part of Zrythm
 *
 * Zrythm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Zrythm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with Zrythm.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "gui/widgets/create_project_dialog.h"
#include "utils/io.h"
#include "utils/resources.h"
#include "zrythm.h"
#include "zrythm_app.h"

#include <glib/gi18n.h>

G_DEFINE_TYPE (CreateProjectDialogWidget,
               create_project_dialog_widget,
               GTK_TYPE_DIALOG)

static void
respond (
  CreateProjectDialogWidget * self)
{
  /* get the zrythm project name */
  char * str =
    g_settings_get_string (
      S_GENERAL, "last-project-dir");
  ZRYTHM->create_project_path =
    g_build_filename (
      str,
      gtk_entry_get_text (
        GTK_ENTRY (self->name)),
      NULL);
  g_free (str);

  /* TODO validate */

  gtk_dialog_response (
    GTK_DIALOG (self), GTK_RESPONSE_OK);
}

static void
on_name_activate (
  GtkEntry *entry,
  CreateProjectDialogWidget * self)
{
  respond (self);
}

static void
on_ok_clicked (
  GtkButton * btn,
  CreateProjectDialogWidget * self)
{
  respond (self);
}

static void
on_cancel_clicked (
  GtkButton * btn,
  CreateProjectDialogWidget * self)
{
  gtk_dialog_response (
    GTK_DIALOG (self), GTK_RESPONSE_CANCEL);
}

static void
on_name_changed (
  GtkEditable *editable,
  CreateProjectDialogWidget * self)
{
  gtk_widget_set_sensitive (
    GTK_WIDGET (self->ok),
    strlen (gtk_entry_get_text (self->name)) > 0);
}

static void
on_fc_file_set (
  GtkFileChooserButton *widget,
  CreateProjectDialogWidget * self)
{
  GFile * file =
    gtk_file_chooser_get_file (
      GTK_FILE_CHOOSER (widget));
  char * str =
    g_file_get_path (file);
  g_settings_set_string (
    S_GENERAL, "last-project-dir", str);
  g_free (str);
}

CreateProjectDialogWidget *
create_project_dialog_widget_new ()
{
  CreateProjectDialogWidget * self =
    g_object_new (
      CREATE_PROJECT_DIALOG_WIDGET_TYPE,
      "title", _("Create New Project"),
      "icon-name", "zrythm",
      "window-position", GTK_WIN_POS_CENTER,
      NULL);

  char * str =
    g_settings_get_string (
      S_GENERAL, "last-project-dir");
  gtk_file_chooser_set_current_folder (
    GTK_FILE_CHOOSER (self->fc),
    str);

  /* get next available "Untitled Project" */
  char * untitled_project =
    g_strdup (_("Untitled Project"));
  char * tmp =
    g_build_filename (
      str,
      untitled_project,
      NULL);
  char * dir =
    io_get_next_available_filepath (tmp);
  g_free (tmp);
  g_free (untitled_project);
  untitled_project =
    g_path_get_basename (dir);
  g_free (dir);
  g_free (str);
  gtk_entry_set_text (
    self->name, untitled_project);
  gtk_editable_select_region (
    GTK_EDITABLE (self->name), 0,
    (int) strlen (untitled_project));
  g_free (untitled_project);

  gtk_widget_grab_focus (GTK_WIDGET (self->name));

  return self;
}

static void
create_project_dialog_widget_init (
  CreateProjectDialogWidget * self)
{
  gtk_widget_init_template (GTK_WIDGET (self));
}

static void
create_project_dialog_widget_class_init (
  CreateProjectDialogWidgetClass * _klass)
{
  GtkWidgetClass * klass = GTK_WIDGET_CLASS (_klass);
  resources_set_class_template (
    klass, "create_project_dialog.ui");

#define BIND_CHILD(x) \
  gtk_widget_class_bind_template_child ( \
    klass, \
    CreateProjectDialogWidget, \
    x)

  BIND_CHILD (ok);
  BIND_CHILD (fc);
  BIND_CHILD (name);

  gtk_widget_class_bind_template_callback (
    klass, on_ok_clicked);
  gtk_widget_class_bind_template_callback (
    klass, on_cancel_clicked);
  gtk_widget_class_bind_template_callback (
    klass, on_fc_file_set);
  gtk_widget_class_bind_template_callback (
    klass, on_name_changed);
  gtk_widget_class_bind_template_callback (
    klass, on_name_activate);

#undef BIND_CHILD
}
