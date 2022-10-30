// SPDX-FileCopyrightText: © 2020, 2022 Alexandros Theodotou <alex@zrythm.org>
// SPDX-License-Identifier: LicenseRef-ZrythmLicense

#include "gui/backend/editor_settings.h"
#include "gui/widgets/arranger.h"
#include "gui/widgets/center_dock.h"
#include "gui/widgets/main_notebook.h"
#include "gui/widgets/timeline_panel.h"
#include "gui/widgets/tracklist.h"
#include "project.h"
#include "utils/debug.h"
#include "zrythm_app.h"

void
editor_settings_init (EditorSettings * self)
{
  self->schema_version = EDITOR_SETTINGS_SCHEMA_VERSION;
  self->hzoom_level = 1.0;
}

void
editor_settings_set_scroll_start_x (
  EditorSettings * self,
  int              x,
  bool             validate)
{
  self->scroll_start_x = MAX (x, 0);
  if (validate)
    {
      if (self == &PRJ_TIMELINE->editor_settings)
        {
        }
    }
  g_debug (
    "scrolled horizontally to %d", self->scroll_start_x);
}

void
editor_settings_set_scroll_start_y (
  EditorSettings * self,
  int              y,
  bool             validate)
{
  self->scroll_start_y = MAX (y, 0);
  if (validate)
    {
      if (self == &PRJ_TIMELINE->editor_settings)
        {
          int tracklist_height =
            gtk_widget_get_allocated_height (
              GTK_WIDGET (MW_TRACKLIST->unpinned_box));
          int tracklist_scroll_height =
            gtk_widget_get_allocated_height (
              GTK_WIDGET (MW_TRACKLIST->unpinned_scroll));
          int diff =
            (self->scroll_start_y + tracklist_scroll_height)
            - tracklist_height;
          if (diff > 0)
            {
              self->scroll_start_y -= diff;
            }
        }
    }
  g_debug ("scrolled vertically to %d", self->scroll_start_y);
}

/**
 * Appends the given deltas to the scroll x/y values.
 */
void
editor_settings_append_scroll (
  EditorSettings * self,
  int              dx,
  int              dy,
  bool             validate)
{
  editor_settings_set_scroll_start_x (
    self, self->scroll_start_x + dx, validate);
  editor_settings_set_scroll_start_y (
    self, self->scroll_start_y + dy, validate);
  g_debug (
    "scrolled to (%d, %d)", self->scroll_start_x,
    self->scroll_start_y);
}
