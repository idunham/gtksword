/*  Note: You are free to use whatever license you want.
    Eventually you will be able to edit it within Glade. */

/*  cheatah
 *  Copyright (C) <YEAR> <AUTHORS>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#include <gtk/gtk.h>


void
on_Exit3_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_View3_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_Help_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_viewMod_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

char *
str_fixup_format		(const char *string);

void
on_About_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_lookupText_changed                  (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_searchButton_clicked                (GtkButton       *button,
                                        gpointer         user_data);

void
on_resultList_selection_changed        (GtkWidget *clist, gint row, gint column, GdkEventButton *event, gpointer data);

void
on_arrow1_button_press_event          (GtkButton       *button,
                                        gpointer         user_data);

void
on_arrow2_button_press_event          (GtkButton       *button,
                                        gpointer         user_data);



