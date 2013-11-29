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
#include "cheatah.h"
#include "cheatsig.h"

int
main (int argc, char *argv[])
{
  GtkWidget *mainWindow;

  gtk_set_locale ();
  gtk_init (&argc, &argv);

  /*
   * The following code was added by Glade to create one of each component
   * (except popup menus), just so that you see something after building
   * the project. Delete any components that you don't want shown initially.
   */
  mainWindow = create_mainWindow ();
  gtk_widget_show (mainWindow);

  gtk_main ();

  destroy_mainWindow();		
  return 0;
}


void
on_Exit3_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_View3_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_Help_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_viewMod_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	cheatahWindow->viewModActivate(menuitem, (gchar *)user_data);
}


/* 
* This function is used to do a quick and dirty translation of the formating
* commands in the module descriptions into reasonable equivalents in
* plain ascii.  (The descriptions look like .rtf-lite, perhaps?)
* It takes a character pointer to the module description and
* returns a pointer to a new string that's been fixed up.  Ultimately, 
* a better solution should be developed, but this improves the 
* appearance of the descriptions significantly for now.
*
* Perhaps this could serve as the basis for a function that would take
* an input string and a text widget, then insert the string into the 
* widget in a formatted manner (good next step?);
*/

char *
str_fixup_format		(const char *string)
{
	char	*strNew;	
	int 	i,j;

	assert(string!=NULL);
	strNew = (char *) malloc(strlen(string));	// First, allocate space for the new string 
	assert(strNew!=NULL);
	if(strNew==NULL) return ((char *)NULL);	// Make sure we actually got it 

	for (i=0,j=0;i<strlen(string);i++) {		// Work through input string char at a time
	    if (string[i]=='\\') {			// Found backslash, probably a command
                           if (string[i+1]=='q') {			// Looks like an alignment command
                               if(string[i+2]=='c') {			// Want to center text 
		strNew[j]='\n';			// - make do with a new line	
		j++;	
		strNew[j]='\t';			//  - and a tab for now
		j++;
	             }
	              i=i+2;				// Assume command and skip past q and ?
	              continue;
	      }
	      else {					// if string[i+1] != 'q'
                             if(string[i+1]=='p' && string[i+2]=='a' && string[i+3]=='r') {	 // "\par" or "\pard"command?
		if(string[i+4]=='d') {		 // "\pard" command - ignore for now
		    i=i+4;				// Skip past the "pard"
		    continue;
                                     }
		else {				// "\par" command
		    strNew[j]='\n';			// - use a new line for now
		    j++;
		    i=i+3;				// Skip past the "par"
		    continue;
		}
	           }
	       }					// End of "else"
	    }					// End of "if(string[i]=='\\')
	    strNew[j]=string[i];			// Don't know what it is, so copy it
	    j++;	
	}					// End of "for"
	strNew[j]='\0';				// Terminate new string

//	fprintf(stderr, "strlen(string) = %i, strlen(strNew) = %i \n", strlen(string), strlen(strNew));	// Used for debugging
//	fprintf(stderr, "string = \"%s\"\n",string);						// Used for debugging
//	fprintf(stderr, "strNew = \"%s\"\n",strNew);						// Used for debugging

	assert(strlen(strNew)<=strlen(string));	// Nothing we have done should have lengthend it
	return(strNew);				// Return fixed-up string		
}

void
on_About_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
 	SWMgr *mainMgr;

	mainMgr=cheatahWindow->getSwordManager();
	int i;
	char *tempString;
	static char *modtypes[] = {"Biblical Texts", "Commentaries", "Lexicons / Dictionaries"};
	string newtext, tmptext;
	ModMap::iterator it;
	SectionMap::iterator sit;
	ConfigEntMap::iterator eit;

 	GtkWidget *dialogAbout;
	GtkWidget *hBoxModDesc;
	GtkWidget *textAboutMods;
	GtkWidget *textScrollbarMods;
	GtkWidget *hBoxAbout;
	GtkWidget *textAbout;
	GtkWidget *textScrollbar;
	GtkWidget *buttonOK;

	dialogAbout=gtk_dialog_new();
	gtk_object_set_data (GTK_OBJECT (dialogAbout), "dialogAbout", dialogAbout);
	gtk_window_set_title (GTK_WINDOW (dialogAbout), "About Cheatah");
	gtk_window_set_policy (GTK_WINDOW (dialogAbout), FALSE, TRUE, FALSE);
	gtk_widget_set_usize(GTK_WIDGET(dialogAbout), 360, 360);
	gtk_window_position(GTK_WINDOW(dialogAbout),GTK_WIN_POS_CENTER);

	buttonOK = gtk_button_new_with_label("O.k.");

/*								*/
/*	Set it up so that the dialog box and all it's children are automatically	*/	
/*	destroyed when the user is done looking at it.			*/
/*	Note: Use gtk_signal_connect_object() rather than		*/
/*	gtk_signal_connect(), because we want the "destroy" 		*/
/*	connected to the dialog box, not just the button.			*/
/*								*/
 	gtk_signal_connect_object(GTK_OBJECT(buttonOK),
		       "clicked",
		       GTK_SIGNAL_FUNC(gtk_widget_destroy),
		       GTK_OBJECT(dialogAbout));

	gtk_container_add (GTK_CONTAINER (GTK_DIALOG(dialogAbout)->action_area), buttonOK);

 	hBoxModDesc = gtk_hbox_new (FALSE,0);
  	gtk_object_set_data (GTK_OBJECT (dialogAbout), "hBoxModDesc", hBoxModDesc);
  	gtk_box_pack_end (GTK_BOX (GTK_DIALOG(dialogAbout)->vbox), hBoxModDesc, TRUE, TRUE, 0);
	textAboutMods = gtk_text_new (NULL, NULL);
 	gtk_object_set_data (GTK_OBJECT (dialogAbout), "textAboutMods", textAboutMods);
   	gtk_box_pack_start (GTK_BOX (hBoxModDesc), textAboutMods, TRUE, TRUE, 0);
  	gtk_text_set_editable (GTK_TEXT (textAboutMods), FALSE);
  	gtk_text_set_word_wrap (GTK_TEXT (textAboutMods), TRUE);

	gtk_text_freeze (GTK_TEXT(textAboutMods));
	gtk_text_set_point(GTK_TEXT(textAboutMods), 0);
	gtk_text_forward_delete (GTK_TEXT (textAboutMods), gtk_text_get_length((GTK_TEXT(textAboutMods))));

	if (mainMgr->Modules.empty()==TRUE) {
		gtk_text_freeze (GTK_TEXT(textAboutMods));
		gtk_text_insert(GTK_TEXT(textAboutMods), NULL, &textAboutMods->style->black, NULL, "\nCheatah was unable to find any books installed!\n\n", -1);
		gtk_text_insert(GTK_TEXT(textAboutMods), NULL, &textAboutMods->style->black, NULL, "They should be listed in a configuration file named either: ", -1);
		gtk_text_insert(GTK_TEXT(textAboutMods), NULL, &textAboutMods->style->black, NULL, "'mods.conf' or 'mods.d'. \n\n", -1);
		gtk_text_insert(GTK_TEXT(textAboutMods), NULL, &textAboutMods->style->black, NULL, "Try setting:\n\tSWORD_PATH=<directory containing mods.conf>\n\t",-1);
		gtk_text_insert(GTK_TEXT(textAboutMods), NULL, &textAboutMods->style->black, NULL, "Or see the README file for a full description of setup options.\n", -1);
	}
	else {
		gtk_text_insert(GTK_TEXT(textAboutMods), NULL, &textAboutMods->style->black, NULL, "\tInstalled Book Modules", -1);
		for (i = 0; i < 3; i++) {
			gtk_text_insert(GTK_TEXT(textAboutMods), NULL, &textAboutMods->style->black, NULL, "\n\n\t", -1);
			gtk_text_insert(GTK_TEXT(textAboutMods), NULL, &textAboutMods->style->black, NULL, modtypes[i], -1);
			gtk_text_insert(GTK_TEXT(textAboutMods), NULL, &textAboutMods->style->black, NULL, "\n\n", -1);
			for (it = mainMgr->Modules.begin(); it != mainMgr->Modules.end(); it++) {
				if (!strcmp((*it).second->Type(), modtypes[i])) {
					sit = mainMgr->config->Sections.find((*it).second->Name());
					if (sit != mainMgr->config->Sections.end()) {
						gtk_text_insert(GTK_TEXT(textAboutMods), NULL, &textAboutMods->style->black, NULL, (*it).second->Name() ,-1);
						gtk_text_insert(GTK_TEXT(textAboutMods), NULL, &textAboutMods->style->black, NULL, ": " ,-1);
						gtk_text_insert(GTK_TEXT(textAboutMods), NULL, &textAboutMods->style->black, NULL, (*it).second->Description() ,-1);
						gtk_text_insert(GTK_TEXT(textAboutMods), NULL, &textAboutMods->style->black, NULL, "\n\n", -1);
						eit = (*sit).second.find("About");
						if (eit != (*sit).second.end()) {
//							gtk_text_insert(GTK_TEXT(textAboutMods), NULL, &textAboutMods->style->black, NULL, (*eit).second.c_str() ,-1);
							tempString=str_fixup_format((*eit).second.c_str() );
							gtk_text_insert(GTK_TEXT(textAboutMods), NULL, &textAboutMods->style->black, NULL, tempString, -1);
							free(tempString);
							gtk_text_insert(GTK_TEXT(textAboutMods), NULL, &textAboutMods->style->black, NULL, "\n\n" ,-1);
						}
//						eit = (*sit).second.find("SourceType");
//						if (eit != (*sit).second.end()) {
//							fprintf(stderr,"SourceType: %s\n", (*eit).second.c_str() );
//						}
					}
				}
			}
		}
	}

	gtk_text_set_point(GTK_TEXT(textAboutMods), 0);
	gtk_text_thaw(GTK_TEXT(textAboutMods));

	/* Add a vertical scrollbar to the textAboutMods GtkText widget */
	textScrollbarMods = gtk_vscrollbar_new (GTK_TEXT (textAboutMods)->vadj);
	gtk_box_pack_end (GTK_BOX (hBoxModDesc), textScrollbarMods, FALSE, FALSE, 0);
 
	hBoxAbout = gtk_hbox_new (FALSE,0);
  	gtk_object_set_data (GTK_OBJECT (dialogAbout), "hBoxAbout", hBoxAbout);
  	gtk_box_pack_start (GTK_BOX (GTK_DIALOG(dialogAbout)->vbox), hBoxAbout, FALSE, TRUE, 0);
	textAbout = gtk_text_new (NULL, NULL);
 	gtk_object_set_data (GTK_OBJECT (dialogAbout), "textAbout", textAbout);
   	gtk_box_pack_start (GTK_BOX (hBoxAbout), textAbout, TRUE, TRUE, 0);
  	gtk_text_set_editable (GTK_TEXT (textAbout), FALSE);
  	gtk_text_set_word_wrap (GTK_TEXT (textAbout), TRUE);

	gtk_text_freeze (GTK_TEXT(textAbout));
	gtk_text_set_point(GTK_TEXT(textAbout), 0);
	gtk_text_forward_delete (GTK_TEXT (textAbout), gtk_text_get_length((GTK_TEXT(textAbout))));
	gtk_text_insert(GTK_TEXT(textAbout), NULL, &textAbout->style->black, NULL, "Cheatah is a quick and dirty GTK/Linux front end to the Sword software.", -1);
	gtk_text_insert(GTK_TEXT(textAbout), NULL, &textAbout->style->black, NULL, "\n\nSword is developed by the CrossWire Software & Bible Society:",-1);
	gtk_text_insert(GTK_TEXT(textAbout), NULL, &textAbout->style->black, NULL, "\n\tP. O. Box 2528",-1);
	gtk_text_insert(GTK_TEXT(textAbout), NULL, &textAbout->style->black, NULL, "\n\tTempe, AZ  85280-2528 ", -1);
	gtk_text_insert(GTK_TEXT(textAbout), NULL, &textAbout->style->black, NULL, "\n\thttp://www.crosswire.org ", -1);
	gtk_text_insert(GTK_TEXT(textAbout), NULL, &textAbout->style->black, NULL, "\n\nThis software is provided free for the study of God and His Word.",-1);

	gtk_text_set_point(GTK_TEXT(textAbout), 0);
	gtk_text_thaw(GTK_TEXT(textAbout));

	/* Add a vertical scrollbar to the textAbout GtkText widget */
	textScrollbar = gtk_vscrollbar_new (GTK_TEXT (textAbout)->vadj);
	gtk_box_pack_end (GTK_BOX (hBoxAbout), textScrollbar, FALSE, FALSE, 0);

	gtk_widget_show_all(dialogAbout);		// "show_all" makes all the children visable too

/*								*/
/*	No need to clean up the various widgets - we've arranged		*/
/*	for that to happen automatically when the user presses the		*/
/*	"o.k." button.						*/
/*								*/
  
	return;
}


void
on_lookupText_changed                  (GtkEditable     *editable,
                                        gpointer         user_data)
{
	cheatahWindow->lookupTextChanged();
}


void
on_searchButton_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
	cheatahWindow->searchButtonClicked();
}


void
on_resultList_selection_changed        (GtkWidget *clist, gint row, gint column, GdkEventButton *event, gpointer data)
{
	cheatahWindow->resultListSelectionChanged(clist, row, column, event, data);
}


void on_arrow1_button_press_event(GtkButton       *button, gpointer         user_data) {
	cheatahWindow->navigateButtonClicked(0);
}


void on_arrow2_button_press_event(GtkButton       *button, gpointer         user_data) {
	cheatahWindow->navigateButtonClicked(1);
}





