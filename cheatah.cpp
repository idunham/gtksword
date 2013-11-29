/*  cheatah
 *  Copyright (C) 1999, CrossWire Bible Society
 *			P. O. Box 2528
 *			Tempe, AZ  85280-2528
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
#include <gdk/gdkkeysyms.h>
#include "cheatsig.h"
#include "cheatah.h"
#include <swmgr.h>
#include <versekey.h>
#include <regex.h>
#include <gbfplain.h>
#include <thmlplain.h>
#include <string.h>


MainWindow *cheatahWindow = NULL;

/*	I'm going to try commenting out get_widget() - it no longer seems to be used	*/
/*	Note: this doesn't seem to have caused any problems so far.			*/
/*
GtkWidget*
get_widget                             (GtkWidget       *widget,
                                        gchar           *widget_name)
{
  GtkWidget *found_widget;

  if (widget->parent)
    widget = gtk_widget_get_toplevel (widget);
  found_widget = (GtkWidget*) gtk_object_get_data (GTK_OBJECT (widget),
                                                   widget_name);
  if (!found_widget)
    g_warning ("Widget not found: %s", widget_name);
  return found_widget;
}		
*/

/* This is an internally used function to set notebook tab widgets. */
void
set_notebook_tab                       (GtkWidget       *notebook,
                                        gint             page_num,
                                        GtkWidget       *widget)
{
  GtkNotebookPage *page;
  GtkWidget *notebook_page;

  page = (GtkNotebookPage*) g_list_nth (GTK_NOTEBOOK (notebook)->children, page_num)->data;
  notebook_page = page->child;
  gtk_widget_ref (notebook_page);
  gtk_notebook_remove_page (GTK_NOTEBOOK (notebook), page_num);
  gtk_notebook_insert_page (GTK_NOTEBOOK (notebook), notebook_page,
                            widget, page_num);
  gtk_widget_unref (notebook_page);
}



MainWindow::MainWindow() {
	searchType_group = NULL;
	mainMgr          = new SWMgr();
	curMod           = NULL;
	chapDisplay      = 0;	// set in create
	entryDisplay     = 0;	// set in create
}


MainWindow::~MainWindow() {
	delete mainMgr;
	if (chapDisplay)
		delete chapDisplay;
	if (entryDisplay)
		delete entryDisplay;
}


GtkWidget* create_mainWindow () {
	GtkWidget *retWidget;
	cheatahWindow = new MainWindow();
	retWidget = cheatahWindow->create();
	cheatahWindow->initSWORD();
	return retWidget;
}

/*								*/
/*	Note: this seems to be a poor choice of a name for this function .	*/
/*	When  I accidentally left off the  parenthesis when trying to call it,	*/
/*	the compiler didn't complain, but it didn't execute, either.		*/
/* 	Could there be a macro or something by the same name?		*/
/*	At any rate, be careful with it.				*/
/*								*/
void destroy_mainWindow () {
	delete cheatahWindow;
}

GtkWidget *MainWindow::create() {
	mainWindow = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_object_set_data (GTK_OBJECT (mainWindow), "mainWindow", mainWindow);
	gtk_window_set_title (GTK_WINDOW (mainWindow), "Cheatah - SWORD Quick Lookup / Search");
	gtk_window_set_policy (GTK_WINDOW (mainWindow), TRUE, TRUE, FALSE);
	gtk_widget_set_usize(GTK_WIDGET(mainWindow), 580, 360);
	gtk_signal_connect(GTK_OBJECT(mainWindow),
		       "destroy",
		       GTK_SIGNAL_FUNC(gtk_main_quit),
		       NULL);


	mainPanel = gtk_vbox_new (FALSE, 0);
  gtk_object_set_data (GTK_OBJECT (mainWindow), "mainPanel", mainPanel);
  gtk_widget_show (mainPanel);
  gtk_container_add (GTK_CONTAINER (mainWindow), mainPanel);

  headerPanel = gtk_hbox_new (FALSE, 0);
  gtk_object_set_data (GTK_OBJECT (mainWindow), "headerPanel", headerPanel);
  gtk_widget_show (headerPanel);
  gtk_box_pack_start (GTK_BOX (mainPanel), headerPanel, FALSE, TRUE, 0);

  handlebox3 = gtk_handle_box_new ();
  gtk_object_set_data (GTK_OBJECT (mainWindow), "handlebox3", handlebox3);
  gtk_widget_show (handlebox3);
  gtk_box_pack_start (GTK_BOX (headerPanel), handlebox3, TRUE, TRUE, 0);

  menubar4 = gtk_menu_bar_new ();
  gtk_object_set_data (GTK_OBJECT (mainWindow), "menubar4", menubar4);
  gtk_widget_show (menubar4);
  gtk_container_add (GTK_CONTAINER (handlebox3), menubar4);

  File = gtk_menu_item_new_with_label ("File");
  gtk_object_set_data (GTK_OBJECT (mainWindow), "File", File);
  gtk_widget_show (File);
  gtk_container_add (GTK_CONTAINER (menubar4), File);

  menuFile = gtk_menu_new ();
  gtk_object_set_data (GTK_OBJECT (mainWindow), "menuFile", menuFile);
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (File), menuFile);

  Exit3 = gtk_menu_item_new_with_label ("Exit");
  gtk_object_set_data (GTK_OBJECT (mainWindow), "Exit3", Exit3);
  gtk_widget_show (Exit3);
  gtk_container_add (GTK_CONTAINER (menuFile), Exit3);
  gtk_signal_connect (GTK_OBJECT (Exit3), "activate",
                      GTK_SIGNAL_FUNC (gtk_main_quit),
                      NULL);

  
  View3 = gtk_menu_item_new_with_label ("View");
  gtk_object_set_data (GTK_OBJECT (mainWindow), "View3", View3);
  gtk_widget_show (View3);
  gtk_container_add (GTK_CONTAINER (menubar4), View3);
  gtk_signal_connect (GTK_OBJECT (View3), "activate",
                      GTK_SIGNAL_FUNC (on_View3_activate),
                      NULL);

  menuView = gtk_menu_new ();
  gtk_object_set_data (GTK_OBJECT (mainWindow), "menuView", menuView);
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (View3), menuView);

  commentaries = gtk_menu_item_new_with_label ("Commentaries");
  gtk_object_set_data (GTK_OBJECT (mainWindow), "commentaries", commentaries);
  gtk_widget_show (commentaries);

  menuCommentaries = gtk_menu_new ();
  gtk_object_set_data (GTK_OBJECT (mainWindow), "menuCommentaries", menuCommentaries);
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (commentaries), menuCommentaries);

  dictionaries = gtk_menu_item_new_with_label ("Dictionaries");
  gtk_object_set_data (GTK_OBJECT (mainWindow), "Dictionaries", dictionaries);
  gtk_widget_show (dictionaries);

  menuDictionaries = gtk_menu_new ();
  gtk_object_set_data (GTK_OBJECT (mainWindow), "menuDictionaries", menuDictionaries);
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (dictionaries), menuDictionaries);



  Help = gtk_menu_item_new_with_label ("Help");
  gtk_object_set_data (GTK_OBJECT (mainWindow), "Help", Help);
  gtk_widget_show (Help);
  gtk_container_add (GTK_CONTAINER (menubar4), Help);
  gtk_signal_connect (GTK_OBJECT (Help), "activate",
                      GTK_SIGNAL_FUNC (on_Help_activate),
                      NULL);
  gtk_menu_item_right_justify (GTK_MENU_ITEM (Help));

  menuHelp = gtk_menu_new ();
  gtk_object_set_data (GTK_OBJECT (mainWindow), "menuHelp", menuHelp);
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (Help), menuHelp);

  About = gtk_menu_item_new_with_label ("About");
  gtk_object_set_data (GTK_OBJECT (mainWindow), "About", About);
  gtk_widget_show (About);
  gtk_container_add (GTK_CONTAINER (menuHelp), About);
  gtk_signal_connect (GTK_OBJECT (About), "activate",
                      GTK_SIGNAL_FUNC (on_About_activate),
                      NULL);

  handlebox4 = gtk_handle_box_new ();
  gtk_object_set_data (GTK_OBJECT (mainWindow), "handlebox4", handlebox4);
  gtk_widget_show (handlebox4);
  gtk_box_pack_start (GTK_BOX (headerPanel), handlebox4, TRUE, TRUE, 0);

  hbox6 = gtk_hbox_new (FALSE, 0);
  gtk_object_set_data (GTK_OBJECT (mainWindow), "hbox6", hbox6);
  gtk_widget_show (hbox6);
  gtk_container_add (GTK_CONTAINER (handlebox4), hbox6);

  label2 = gtk_label_new ("FreeForm Lookup:");
  gtk_object_set_data (GTK_OBJECT (mainWindow), "label2", label2);
  gtk_widget_show (label2);
  gtk_box_pack_start (GTK_BOX (hbox6), label2, FALSE, TRUE, 0);
  gtk_label_set_justify (GTK_LABEL (label2), GTK_JUSTIFY_RIGHT);
  gtk_misc_set_padding (GTK_MISC (label2), 2, 0);

  lookupText = gtk_entry_new ();
  gtk_object_set_data (GTK_OBJECT (mainWindow), "lookupText", lookupText);
  gtk_widget_show (lookupText);
  gtk_box_pack_start (GTK_BOX (hbox6), lookupText, FALSE, TRUE, 0);
  gtk_signal_connect (GTK_OBJECT (lookupText), "changed",
                      GTK_SIGNAL_FUNC (on_lookupText_changed),
                      NULL);

  hbox2 = gtk_hbox_new (FALSE, 0);
  gtk_object_set_data (GTK_OBJECT (mainWindow), "hbox2", hbox2);
  gtk_widget_show (hbox2);
  gtk_box_pack_start (GTK_BOX (mainPanel), hbox2, TRUE, TRUE, 0);


/*
  textScrolledWindow = gtk_scrolled_window_new (NULL, NULL);
  gtk_object_set_data (GTK_OBJECT (mainWindow), "textScrolledWindow", textScrolledWindow);
  gtk_widget_show (textScrolledWindow);
  gtk_box_pack_start (GTK_BOX (hbox2), textScrolledWindow, FALSE, FALSE, 0);
*/

  moduleText = gtk_text_new (NULL, NULL);
  gtk_object_set_data (GTK_OBJECT (mainWindow), "moduleText", moduleText);
  gtk_widget_show (moduleText);
//  gtk_container_add (GTK_CONTAINER (textScrolledWindow), moduleText);
  gtk_box_pack_start (GTK_BOX (hbox2), moduleText, TRUE, TRUE, 0);
  gtk_text_set_editable (GTK_TEXT (moduleText), FALSE);
  gtk_text_set_word_wrap (GTK_TEXT (moduleText), TRUE);
  gtk_widget_realize (moduleText);

	/* Add a vertical scrollbar to the GtkText widget */
	textScrollbar = gtk_vscrollbar_new (GTK_TEXT (moduleText)->vadj);
	gtk_box_pack_start (GTK_BOX (hbox2), textScrollbar, FALSE, FALSE, 0);
	gtk_widget_show (textScrollbar);


/*
  handlebox5 = gtk_handle_box_new ();
  gtk_object_set_data (GTK_OBJECT (mainWindow), "handlebox5", handlebox5);
  gtk_widget_show (handlebox5);
  gtk_box_pack_start (GTK_BOX (hbox2), handlebox5, FALSE, FALSE, 0);
  gtk_container_border_width (GTK_CONTAINER (handlebox5), 5);
*/

  searchPanel = gtk_vbox_new (FALSE, 0);
  gtk_object_set_data (GTK_OBJECT (mainWindow), "searchPanel", searchPanel);
  gtk_widget_show (searchPanel);
  gtk_box_pack_start (GTK_BOX (hbox2), searchPanel, FALSE, FALSE, 0);
//  gtk_container_add (GTK_CONTAINER (handlebox5), searchPanel);

  hbox4 = gtk_hbox_new (FALSE, 0);
  gtk_object_set_data (GTK_OBJECT (mainWindow), "hbox4", hbox4);
  gtk_widget_show (hbox4);
  gtk_box_pack_start (GTK_BOX (searchPanel), hbox4, FALSE, FALSE, 0);

  searchText = gtk_entry_new ();
  gtk_object_set_data (GTK_OBJECT (mainWindow), "searchText", searchText);
  gtk_widget_show (searchText);
  gtk_box_pack_start (GTK_BOX (hbox4), searchText, TRUE, TRUE, 0);

  searchButton = gtk_button_new_with_label ("Search");
  gtk_object_set_data (GTK_OBJECT (mainWindow), "searchButton", searchButton);
  gtk_widget_show (searchButton);
  gtk_box_pack_start (GTK_BOX (hbox4), searchButton, FALSE, TRUE, 0);
  gtk_signal_connect (GTK_OBJECT (searchButton), "clicked",
                      GTK_SIGNAL_FUNC (on_searchButton_clicked),
                      NULL);

  searchSettingsPanel = gtk_hbox_new (FALSE, 0);
  gtk_object_set_data (GTK_OBJECT (mainWindow), "searchSettingsPanel", searchSettingsPanel);
  gtk_widget_show (searchSettingsPanel);
  gtk_box_pack_start (GTK_BOX (searchPanel), searchSettingsPanel, FALSE, FALSE, 0);

  searchTypePanel = gtk_vbox_new (FALSE, 0);
  gtk_object_set_data (GTK_OBJECT (mainWindow), "searchTypePanel", searchTypePanel);
  gtk_widget_show (searchTypePanel);
  gtk_box_pack_start (GTK_BOX (searchSettingsPanel), searchTypePanel, FALSE, FALSE, 0);

  multiWordSearch = gtk_radio_button_new_with_label (searchType_group, "Multi Word");
  searchType_group = gtk_radio_button_group (GTK_RADIO_BUTTON (multiWordSearch));
  gtk_object_set_data (GTK_OBJECT (mainWindow), "multiWordSearch", multiWordSearch);
  gtk_widget_show (multiWordSearch);
  gtk_box_pack_start (GTK_BOX (searchTypePanel), multiWordSearch, FALSE, FALSE, 0);

  phraseSearch = gtk_radio_button_new_with_label (searchType_group, "Exact Phrase");
  searchType_group = gtk_radio_button_group (GTK_RADIO_BUTTON (phraseSearch));
  gtk_object_set_data (GTK_OBJECT (mainWindow), "phraseSearch", phraseSearch);
  gtk_widget_show (phraseSearch);
  gtk_box_pack_start (GTK_BOX (searchTypePanel), phraseSearch, FALSE, FALSE, 0);

  regexSearch = gtk_radio_button_new_with_label (searchType_group, "Regular Expression");
  searchType_group = gtk_radio_button_group (GTK_RADIO_BUTTON (regexSearch));
  gtk_object_set_data (GTK_OBJECT (mainWindow), "regexSearch", regexSearch);
  gtk_widget_show (regexSearch);
  gtk_box_pack_start (GTK_BOX (searchTypePanel), regexSearch, FALSE, FALSE, 0);


  searchOptionsPanel = gtk_vbox_new (FALSE, 0);
  gtk_object_set_data (GTK_OBJECT (mainWindow), "searchOptionsPanel", searchOptionsPanel);
  gtk_widget_show (searchOptionsPanel);
  gtk_box_pack_start (GTK_BOX (searchSettingsPanel), searchOptionsPanel, FALSE, FALSE, 0);

  caseSensitive = gtk_check_button_new_with_label ("Case Sensitive");
  gtk_object_set_data (GTK_OBJECT (mainWindow), "caseSensitive", caseSensitive);
  gtk_widget_show (caseSensitive);
  gtk_box_pack_start (GTK_BOX (searchOptionsPanel), caseSensitive, FALSE, FALSE, 0);

  GtkWidget *resultPanel = gtk_hbox_new (FALSE, 0);
  gtk_object_set_data (GTK_OBJECT (mainWindow), "resultPanel", resultPanel);
  gtk_widget_show (resultPanel);
  gtk_box_pack_start (GTK_BOX (searchPanel), resultPanel, TRUE, TRUE, 0);
  resultList = gtk_clist_new(1);
  gtk_object_set_data(GTK_OBJECT (mainWindow), "resultList", resultList);
//  gtk_clist_set_policy(GTK_CLIST(resultList), GTK_POLICY_AUTOMATIC, 0);
//  gtk_clist_set_border(GTK_CLIST(resultList), GTK_SHADOW_ETCHED_IN);
  gtk_widget_show (resultList);
  gtk_box_pack_start (GTK_BOX (resultPanel), resultList, TRUE, TRUE, 0);
  gtk_signal_connect (GTK_OBJECT (resultList), "select_row",
                      GTK_SIGNAL_FUNC (on_resultList_selection_changed),
                      NULL);
	/* Add a vertical scrollbar to the GtkCList widget */
	textScrollbar = gtk_vscrollbar_new (NULL);
	gtk_clist_set_vadjustment (GTK_CLIST(resultList), gtk_range_get_adjustment(GTK_RANGE(textScrollbar)));
	gtk_box_pack_start (GTK_BOX (resultPanel), textScrollbar, FALSE, FALSE, 0);
	gtk_widget_show (textScrollbar);


/*
  statusbar1 = gtk_statusbar_new ();
  gtk_object_set_data (GTK_OBJECT (mainWindow), "statusbar1", statusbar1);
  gtk_widget_show (statusbar1);
  gtk_box_pack_start (GTK_BOX (mainPanel), statusbar1, FALSE, TRUE, 0);
*/

  arrow1 = gtk_button_new_with_label ("[<]");
  gtk_object_set_data (GTK_OBJECT (mainWindow), "arrow1", arrow1);
  gtk_widget_show (arrow1);
  gtk_box_pack_start (GTK_BOX (headerPanel), arrow1, FALSE, FALSE, 0);
  gtk_signal_connect (GTK_OBJECT (arrow1), "clicked",
                      GTK_SIGNAL_FUNC (on_arrow1_button_press_event),
                      NULL);

  arrow2 = gtk_button_new_with_label ("[>]");
  gtk_object_set_data (GTK_OBJECT (mainWindow), "arrow2", arrow2);
  gtk_widget_show (arrow2);
  gtk_box_pack_start (GTK_BOX (headerPanel), arrow2, FALSE, FALSE, 0);
  gtk_signal_connect (GTK_OBJECT (arrow2), "clicked",
                      GTK_SIGNAL_FUNC (on_arrow2_button_press_event),
                      NULL);


	statusbar1 = gtk_label_new ("");
	gtk_object_set_data (GTK_OBJECT (mainWindow), "statusbar1", statusbar1);
	gtk_widget_show (statusbar1);
	gtk_box_pack_start (GTK_BOX (mainPanel), statusbar1, FALSE, FALSE, 0);
	gtk_label_set_justify (GTK_LABEL (statusbar1), GTK_JUSTIFY_LEFT);
	gtk_misc_set_padding (GTK_MISC (statusbar1), 2, 0);


	GTKEntryDisp::__initialize();
	chapDisplay = new GTKChapDisp(moduleText);
	entryDisplay = new GTKEntryDisp(moduleText);

	return mainWindow;
}

void MainWindow::initSWORD() {

	GtkWidget *menuChoice;
	int viewNumber = 1;
	char menuName[64];
	ModMap::iterator it;
	SectionMap::iterator sit;
	ConfigEntMap::iterator eit;
	char *font;
	SWModule *curMod;

/*	First, check and see whether SWMgr found any installed books.  If not, tell the user what the problem is.	*/
/*	Print a short notice to stderr, for use when the program is invoked from the command line,		*/
/*	and also put  a bit longer message into the spot where bible text would otherwise show up. 		*/
/*	Then, return out of this function, because there's nothing left to do.					*/

	if (mainMgr->Modules.empty()==TRUE) {
		fprintf(stderr,"SWMgr: Can't find 'mods.conf' or 'mods.d'.  Try setting:\n\tSWORD_PATH=<directory containing mods.conf>\n\tOr see the README file for a full description of setup options.\n\t");

		gtk_text_set_point(GTK_TEXT(moduleText), 0);
		gtk_text_forward_delete (GTK_TEXT (moduleText), gtk_text_get_length((GTK_TEXT(moduleText))));
		int curPos = 0;
		gtk_text_freeze (GTK_TEXT(moduleText));
		gtk_text_insert(GTK_TEXT(moduleText), NULL, &moduleText->style->black, NULL, "\nCheatah was unable to find any books installed!\n\n", -1);
		gtk_text_insert(GTK_TEXT(moduleText), NULL, &moduleText->style->black, NULL, "They should be listed in a configuration file named either: ", -1);
		gtk_text_insert(GTK_TEXT(moduleText), NULL, &moduleText->style->black, NULL, "'mods.conf' or 'mods.d'. \n\n", -1);
		gtk_text_insert(GTK_TEXT(moduleText), NULL, &moduleText->style->black, NULL, "Try setting:\n\tSWORD_PATH=<directory containing mods.conf>\n\t",-1);
		gtk_text_insert(GTK_TEXT(moduleText), NULL, &moduleText->style->black, NULL, "Or see the README file for a full description of setup options.\n", -1);
		gtk_text_set_point(GTK_TEXT(moduleText), curPos);
		gtk_text_thaw(GTK_TEXT(moduleText));
						
		return;
	}

/*
* Create filters to use, depending on the needs of the installed modules.
* Right now, basically just handling GBF files and converting them to plain
* text for display.  So far, I haven't found any modules that specify
* anything other than GBF, but I've dropped in code to 
* install the thtml to plain filter just in case, since it's available.
* Question: I'm installing this as a display filter - would it make more
* sense to install it is a raw filter, so that I could use "plain" as the
* base for any strip filters we may need?  Or is that used for something else,
* like compression or decyphering locked books?
*/

	gbftoplain = new GBFPlain();
	thmlplain = new ThMLPlain();

/*
*	Next, if SWMgr did find  some book modules installed , hook them up to any filters needed
*	and set up the menues to reflect them.
*	Pick a default module to show on startup.
*/
		
	for (it = mainMgr->Modules.begin(); it != mainMgr->Modules.end(); it++) {
		curMod = (*it).second;
		menuChoice = gtk_menu_item_new_with_label (curMod->Name());
		sprintf(menuName, "viewMod%d", viewNumber++);
		gtk_object_set_data (GTK_OBJECT (mainWindow), menuName, menuChoice);
		gtk_widget_show (menuChoice);
		gtk_signal_connect(GTK_OBJECT (menuChoice), "activate",
			      GTK_SIGNAL_FUNC (on_viewMod_activate),
			      g_strdup(curMod->Name()));

		if ((sit = mainMgr->config->Sections.find(curMod->Name())) != mainMgr->config->Sections.end()) {
			if ((eit = (*sit).second.find("SourceType")) != (*sit).second.end()) {
				if (!strcasecmp((*eit).second.c_str(), "GBF")) {
					curMod->AddRenderFilter(gbftoplain);
				}
				else if (!strcasecmp((*eit).second.c_str(), "ThML")) {
					curMod->AddRenderFilter(thmlplain);
				}
			}
		}

		if (!strcmp((*it).second->Type(), "Biblical Texts")) {
			gtk_container_add (GTK_CONTAINER (menuView), menuChoice);
			font = 0;
			if ((sit = mainMgr->config->Sections.find(curMod->Name())) != mainMgr->config->Sections.end()) {
				if ((eit = (*sit).second.find("Font")) != (*sit).second.end()) {
					font = (char *)(*eit).second.c_str();
				}
			}
//			CreateTextPane((*it).second, font);
			curMod->Disp(chapDisplay);	// set our GTKChapDisp object up for the diplayer of each Biblical Text module
			if (!this->curMod) 	// set currently selected module for app to first module from SWMgr (Bible Texts get first preference
				this->curMod = curMod;
		}
		else	{
			curMod->Disp(entryDisplay);	// set our GTKEntryDisp object up for the diplayer of each module other than Biblical Texts
			if (!strcmp(curMod->Type(), "Commentaries")) {
				gtk_container_add (GTK_CONTAINER (menuCommentaries), menuChoice);
	//			CreateCommentPane((*it).second);
			}
			if (!strcmp(curMod->Type(), "Lexicons / Dictionaries")) {
				gtk_container_add (GTK_CONTAINER (menuDictionaries), menuChoice);
	//			CreateLDPane((*it).second);
			}
		}


	}
	gtk_container_add (GTK_CONTAINER (menuView), commentaries);
	gtk_container_add (GTK_CONTAINER (menuView), dictionaries);

	if (!this->curMod)			// We didn't find a bible text, so set currently selected module for app to first module from SWMgr
		this->curMod = curMod;	// Should be o.k. to do this, because we've already tested to see if SWMgr thinks it found something
	assert(this->curMod!=NULL);
	if(this->curMod!=NULL)		// Go ahead and display default module
		lookupTextChanged();
}


void MainWindow::lookupTextChanged() {
	string keyText;
	gchar *entryText;
	char tmpBuf[255];

	entryText = gtk_entry_get_text(GTK_ENTRY(lookupText));
	keyText = entryText;

	if (curMod) {
		curMod->SetKey(keyText.c_str());
		curMod->Display();
		sprintf(tmpBuf, "%s (%s)", curMod->KeyText(), curMod->Name());
		gtk_label_set(GTK_LABEL(statusbar1), tmpBuf);
	}
}

void MainWindow::viewModActivate(GtkMenuItem *sender, gchar *modName) {
	ModMap::iterator it;

	it = mainMgr->Modules.find(modName);
	if (it != mainMgr->Modules.end()) {
		curMod = (*it).second;
		lookupTextChanged();
	}
	else	curMod = 0;
}


void MainWindow::navigateButtonClicked(int direction) {
	if (curMod) {
		if (direction)
			(*curMod)++;
		else	(*curMod)--;
		(const char *)*curMod;  // snap to closest locations
		const gchar *text = curMod->KeyText();
		gtk_entry_set_text(GTK_ENTRY(lookupText), text);
//		curMod->Display();
	}
}


void MainWindow::searchButtonClicked() {
	string srchText;
	gchar *entryText;
	const gchar *resultText;
	gchar **clistText = (gchar **)&resultText;


	gtk_clist_clear(GTK_CLIST(resultList));

	entryText = gtk_entry_get_text(GTK_ENTRY(searchText));
	srchText = entryText;

	if (curMod) {
		int searchType = GTK_TOGGLE_BUTTON(regexSearch)->active ? 0 : GTK_TOGGLE_BUTTON(phraseSearch)->active ? -1 : -2;
		int searchParams = GTK_TOGGLE_BUTTON(caseSensitive)->active ? 0 : REG_ICASE;
		gtk_clist_freeze(GTK_CLIST(resultList));
		for (ListKey searchResults = curMod->Search(srchText.c_str(), searchType, searchParams); !searchResults.Error(); searchResults++) {
			resultText = (const char *)searchResults;
			gtk_clist_append(GTK_CLIST(resultList), clistText);
		}
		gtk_clist_thaw(GTK_CLIST(resultList));
	}
}


void MainWindow::resultListSelectionChanged(GtkWidget *clist, gint row, gint column, GdkEventButton *event, gpointer data) {
	gchar *text;

	gtk_clist_get_text(GTK_CLIST(resultList), row, column, &text);

	if (text) {
		gtk_entry_set_text(GTK_ENTRY(lookupText), text);
	}
}

GdkColor GTKEntryDisp::colourBlue;
GdkColor GTKEntryDisp::colourGreen;

/*										*/
/*	Note: we have a warning that should be fixed here.  GTKEntryDisp::Display() is defined	*/
/*	as returning char, but doesn't have a return statement				*/
/*	What was intended - a return code?  That doesn't seem right, because those are	*/
/*	usually int's.  But it isn't defined as returning a char*, so it doesn't seem		*/
/*	likely that it's intended to return the text involved.					*/
/*										*/
char GTKEntryDisp::Display(SWModule &imodule) {
	char tmpBuf[255];

	gtk_text_set_point(GTK_TEXT(gtkText), 0);
	gtk_text_forward_delete (GTK_TEXT (gtkText), gtk_text_get_length((GTK_TEXT(gtkText))));
	int curPos = 0;
	(const char *)imodule;	// snap to entry
	gtk_text_freeze (GTK_TEXT(gtkText));
	sprintf(tmpBuf, "[%s] ", imodule.KeyText());
	gtk_text_insert(GTK_TEXT(gtkText), NULL, &colourBlue, NULL, tmpBuf, -1);
	gtk_text_insert(GTK_TEXT(gtkText), NULL, &gtkText->style->black, NULL, (const char *)imodule, -1);
	gtk_text_set_point(GTK_TEXT(gtkText), curPos);
	gtk_text_thaw(GTK_TEXT(gtkText));
}

/*											*/
/*	Note: we have a warning that should be fixed here.  GTKChapDisp::Display() is defined		*/
/*	as returning char, but doesn't have a return statement.					*/
/*	What was intended - a return code?  That doesn't seem right, because those are		*/
/*	usually int's.  But it isn't defined as returning a char*, so it doesn't seem			*/
/*	likely that it's intended to return the text involved.						*/
/*											*/

char GTKChapDisp::Display(SWModule &imodule) {
	char tmpBuf[255];

	gtk_text_set_point(GTK_TEXT(gtkText), 0);
	gtk_text_forward_delete (GTK_TEXT (gtkText), gtk_text_get_length((GTK_TEXT(gtkText))));
	VerseKey *key = (VerseKey *)(SWKey *)imodule;
	int curVerse = key->Verse();
	int curChapter = key->Chapter();
	int curBook = key->Book();
	int curPos = 0;
	gfloat adjVal;
	gtk_text_freeze (GTK_TEXT(gtkText));
	for (key->Verse(1); (key->Book() == curBook && key->Chapter() == curChapter && !imodule.Error()); imodule++) {
		sprintf(tmpBuf, "%d. ", key->Verse());
		gtk_text_insert(GTK_TEXT(gtkText), NULL, &colourBlue, NULL, tmpBuf, -1);
		if (key->Verse() == curVerse) {
			gtk_text_thaw(GTK_TEXT(gtkText));
			adjVal = GTK_TEXT(gtkText)->vadj->upper;
			curPos = gtk_text_get_length(GTK_TEXT(gtkText));
			gtk_text_insert(GTK_TEXT(gtkText), NULL, &colourGreen, NULL, (const char *)imodule, -1);
			gtk_text_freeze (GTK_TEXT(gtkText));
		}
		else {
			gtk_text_insert(GTK_TEXT(gtkText), NULL, &gtkText->style->black, NULL, (const char *)imodule, -1);
		}
		gtk_text_insert(GTK_TEXT(gtkText), NULL, &gtkText->style->black, NULL, "  ", -1);
	}
	gtk_text_set_point(GTK_TEXT(gtkText), curPos);
	gtk_text_thaw(GTK_TEXT(gtkText));
	adjVal -= (GTK_TEXT(gtkText)->vadj->page_size / 2);
	gtk_adjustment_set_value(GTK_TEXT(gtkText)->vadj, (adjVal>0)?adjVal:0);
	key->Verse(1);
	key->Chapter(1);
	key->Book(curBook);
	key->Chapter(curChapter);
	key->Verse(curVerse);
}
