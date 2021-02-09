/***************************************************************************
 *   Copyright (C) 2007 by Hugo Parente Lima <hugo.pl@gmail.com>           *
 *                                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 ***************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QLinkedList>
#include <QSettings>
#include <QTranslator>

#include "pluginhandler.h"

class PetriNetView;
class Controller;
class QActionGroup;
class Analyser;
class PetriNetViewZoom;

namespace Ui {
    class MainWindow;
}

/**
* Graphic user interface.
* @defgroup gui GUI
*/

/**
* Main window.
* @note Do not create this window on stack, because Qt::WA_DeleteOnClose is set!
* @ingroup gui
*/
class MainWindow : public QMainWindow {
Q_OBJECT
public:
	MainWindow(bool firstWindow = false);
	~MainWindow();

	void setWindowTitle(const QString& document);
private slots:
	void changeSimulationMode(bool mode);
	void enterWhatIsThisMode();
	void newPetriNet();
	void openPetriNet();
	void openRecentPetriNet();
	void savePetriNet();
	void savePetriNetAs();
	void executeAnalyser();
	void about();
	void cleanStateChanged(bool);
	void changeLanguage();
	void setupFusionDarkTheme();
	void setupFusionTheme();
	void setupSystemTheme();
protected:
	void closeEvent(QCloseEvent* event);
private:
    Ui::MainWindow& ui;
	Controller* mController;
	QHash<QAction*, Analyser*> mAnalysers;
	QLinkedList<QTranslator*> mTranslators;
	PetriNetViewZoom* mZoomController;

	QActionGroup* mToolGroup;
	QActionGroup* mNonSimulationGroup;
	QActionGroup* mSimulationGroup;

	QSettings settings;

	enum { MAX_RECENT_FILES = 7 };
	QAction* mRecentFiles[MAX_RECENT_FILES];

	QDir i18nDir() const;
	void loadI18n(const QString& language);
	void setupActions();
	void setupDockWindows();
	void updateRecentFileActions();
	void loadPetriNet(const QString& fileName);
	void loadPlugins();
	void searchAvailableLanguages();
	void setupProperties();
	void saveProperties();

	PluginHandler mStaticPlugins;

	QString mSystemStyleName;

};

#endif
