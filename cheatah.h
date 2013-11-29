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
#include <swmgr.h>
#include <swdisp.h>
#include <swmodule.h>
#include <encfiltmgr.h>
#include <versekey.h>
#include <stdio.h>
#include <string>
#include <assert.h>

using std::string;

using sword::SWMgr;
using sword::SWModule;
using sword::SWDisplay;
using sword::SWFilter;
using sword::ModMap;
using sword::SectionMap;
using sword::ConfigEntMap;
using sword::SWKey;
using sword::ListKey;
using sword::VerseKey;

/*
 * This function returns a widget in a component created by Glade.
 * Call it with the toplevel widget in the component (i.e. a window/dialog),
 * or alternatively any widget in the component, and the name of the widget
 * you want returned.
 */
/*	I'm going to try commenting it out - it no longer seems to be used.	*/
/*	Note: This doesn't seem to have caused any problems so far.		*/
/*
GtkWidget*
get_widget                             (GtkWidget       *widget,
                                        gchar           *widget_name);	
*/

 /*
  * This is an internally used function for setting notebook tabs. It is only
  * included in this header file so you don't get compilation warnings
  */
void
set_notebook_tab                       (GtkWidget       *notebook,
                                        gint             page_num,
                                        GtkWidget       *widget);

GtkWidget* create_mainWindow (void);
void	     destroy_mainWindow (void);

class GTKEntryDisp : public SWDisplay {
protected:
	GtkWidget *gtkText;
public:
	static GdkColor colourGreen;
	static GdkColor colourBlue;
	static void __initialize() {
			GdkColormap *cmap;

			cmap = gdk_colormap_get_system();
			colourGreen.red = 0;
			colourGreen.green = 0x00ff;
			colourGreen.blue = 0xffff;
			colourBlue.red = 0;
			colourBlue.green = 0;
			colourBlue.blue = 0xffff;
			if (!gdk_color_alloc(cmap, &colourGreen)) {
			  g_error("couldn't allocate colour");
			}
			if (!gdk_color_alloc(cmap, &colourBlue)) {
			  g_error("couldn't allocate colour");
			}
	}
	GTKEntryDisp(GtkWidget *gtkText) { this->gtkText = gtkText; }
	virtual char Display(SWModule &imodule);
};

class GTKChapDisp : public GTKEntryDisp {
public:
	GTKChapDisp(GtkWidget *gtkText) : GTKEntryDisp(gtkText) {}
	virtual char Display(SWModule &imodule);
};

class MainWindow {
	GtkWidget *mainWindow;
	GtkWidget *mainPanel;
	GtkWidget *headerPanel;
	GtkWidget *handlebox3;
	GtkWidget *menubar4;
	GtkWidget *File;
	GtkWidget *menuFile;
	GtkWidget *Exit3;
	GtkWidget *View3;
	GtkWidget *Help;
	GtkWidget *menuHelp;
	GtkWidget *menuView;
	GtkWidget *menuCommentaries;
	GtkWidget *menuDictionaries;
	GtkWidget *commentaries;
	GtkWidget *dictionaries;
	GtkWidget *About;
	GtkWidget *handlebox4;
	GtkWidget *hbox6;
	GtkWidget *label2;
	GtkWidget *lookupText;
	GtkWidget *hbox2;
	GtkWidget *moduleText;
	GtkWidget *handlebox5;
	GtkWidget *searchPanel;
	GtkWidget *hbox4;
	GtkWidget *searchText;
	GtkWidget *searchButton;
	GtkWidget *searchSettingsPanel;
	GtkWidget *searchTypePanel;
	GSList *searchType_group;
	GtkWidget *regexSearch;
	GtkWidget *phraseSearch;
	GtkWidget *multiWordSearch;
	GtkWidget *searchOptionsPanel;
	GtkWidget *caseSensitive;
	GtkWidget *resultList;
	GtkWidget *statusbar1;
	GtkWidget *textScrolledWindow;
	GtkWidget *textScrollbar;
	GtkWidget *arrow1;
	GtkWidget *arrow2;


	SWMgr *mainMgr;
	SWModule *curMod;
	SWDisplay *chapDisplay;
	SWDisplay *entryDisplay;
	SWFilter *gbftoplain;	// GBF seems to be the most common format
	SWFilter *thmlplain;	// ThML to Plain filter is also available - put it in just in case
				// Should there be any more added? (Do we need a RWPtoPlain filter?)

public:
	MainWindow();
	~MainWindow();
	GtkWidget *create();
	void initSWORD();
	void lookupTextChanged();
	void viewModActivate(GtkMenuItem *sender, gchar *modName);
	void searchButtonClicked();
	void navigateButtonClicked(int direction);
	void resultListSelectionChanged(GtkWidget *clist, gint row, gint column, GdkEventButton *event, gpointer data);
	SWMgr *getSwordManager(void)	{return(mainMgr);};
};


extern MainWindow *cheatahWindow;










