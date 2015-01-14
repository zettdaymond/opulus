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

#include "mainwindow.h"
#include "miniview.h"
#include "controller.h"
#include "exceptions.h"
#include "propertyeditordelegate.h"
#include "analyser.h"
#include "version.h"

#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QToolBar>
#include <QIcon>
#include <QDockWidget>
#include <QDebug>
#include <QTreeView>
#include <QAbstractItemModel>
#include <QCloseEvent>
#include <QWhatsThis>
#include <QSettings>
#include <QFileDialog>
#include <QTranslator>
#include <QApplication>
#include <QPluginLoader>
#include <QMessageBox>

MainWindow::MainWindow(bool firstWindow) : QMainWindow(0, Qt::Window) {
	setAttribute(Qt::WA_DeleteOnClose);
	if (firstWindow) {
		QSettings settings;
		QString locale = settings.value("language", QLocale::system().name()).toString();
		loadI18n(locale);
	}
	ui.setupUi(this);
	setCentralWidget(ui.petriNetView);
	mController = new Controller(this, ui.petriNetView);
	setupActions();
	setupDockWindows();
	updateRecentFileActions();
	loadPlugins();

	connect(mController, SIGNAL(cleanChanged(bool)), this, SLOT(cleanStateChanged(bool)));
	connect(mController, SIGNAL(netChanged(const PetriNet*)), ui.matrixWidget, SLOT(updateMatrices(const PetriNet*)));
	searchAvailableLanguages();
	show();
}

MainWindow::~MainWindow() {
}

QDir MainWindow::i18nDir() const {
	QDir dir(QApplication::applicationDirPath()+"/../share/opulus/i18n/");
	if (!dir.exists())
		qWarning("i18n directory not found!");
	dir.setFilter(QDir::Files | QDir::NoSymLinks);
	return dir;
}

void MainWindow::loadI18n(const QString& language) {
	// remove all installed translators
	foreach (QTranslator* tr, mTranslators) {
		QApplication::removeTranslator(tr);
		delete tr;
	}
	mTranslators.clear();

	QDir dir = i18nDir();
	dir.setNameFilters(QStringList() << "?*"+language+".qm");

	foreach (QString langFile, dir.entryList()) {
		QTranslator* tr = new QTranslator;
		if (tr->load(dir.absolutePath()+'/'+langFile)) {
			QApplication::installTranslator(tr);
			mTranslators << tr;
		} else {
			qWarning(qPrintable("Error loading translation: "+dir.absolutePath()+'/'+langFile));
			delete tr;
		}
	}
}

void MainWindow::setupActions() {
	// file menu actions
	connect(ui.actionQuit, SIGNAL(triggered(bool)), this, SLOT(close()));

	// tool menu actions
	mToolGroup = new QActionGroup(this);
	mToolGroup->setExclusive(true);
	ui.actionSelect->setActionGroup(mToolGroup);
	ui.actionAddPlace->setActionGroup(mToolGroup);
	ui.actionAddTransition->setActionGroup(mToolGroup);
	ui.actionAddArc->setActionGroup(mToolGroup);
	ui.actionAddInhibitorArc->setActionGroup(mToolGroup);
	ui.actionAddRemoveTokens->setActionGroup(mToolGroup);
	connect(ui.actionSelect, SIGNAL(triggered(bool)), mController, SLOT(useSelectTool()));
	connect(ui.actionAddPlace, SIGNAL(triggered(bool)), mController, SLOT(usePlaceTool()));
	connect(ui.actionAddTransition, SIGNAL(triggered(bool)), mController, SLOT(useTransitionTool()));
	connect(ui.actionAddArc, SIGNAL(triggered(bool)), mController, SLOT(useArcTool()));
	connect(ui.actionAddInhibitorArc, SIGNAL(triggered(bool)), mController, SLOT(useInhibitorArcTool()));
	connect(ui.actionAddRemoveTokens, SIGNAL(triggered(bool)), mController, SLOT(useTokenTool()));

	connect(ui.actionWhatsThis, SIGNAL(triggered(bool)), this, SLOT(enterWhatIsThisMode()));
	connect(ui.actionAboutQt, SIGNAL(triggered(bool)), qApp, SLOT(aboutQt()));
	connect(ui.actionExportToPNG, SIGNAL(triggered(bool)), mController, SLOT(exportToPNG()));
	connect(ui.actionExportToPS, SIGNAL(triggered(bool)), mController, SLOT(exportToPS()));
	connect(ui.actionExportToSVG, SIGNAL(triggered(bool)), mController, SLOT(exportToSVG()));
	connect(ui.actionNew, SIGNAL(triggered(bool)), this, SLOT(newPetriNet()));
	connect(ui.actionOpen, SIGNAL(triggered(bool)), this, SLOT(openPetriNet()));
	connect(ui.actionSave, SIGNAL(triggered(bool)), this, SLOT(savePetriNet()));
	connect(ui.actionSaveAs, SIGNAL(triggered(bool)), this, SLOT(savePetriNetAs()));
	connect(ui.actionAbout, SIGNAL(triggered(bool)), this, SLOT(about()));
	connect(ui.actionZoomIn, SIGNAL(triggered(bool)),
			mController, SLOT(zoomIn()));
	connect(ui.actionZoomOut, SIGNAL(triggered(bool)),
			mController, SLOT(zoomOut()));

	// open recent file actions
	for (int i = 0; i < MAX_RECENT_FILES; ++i) {
		mRecentFiles[i] = ui.menuRecentPetriNets->addAction("");
		mRecentFiles[i]->setVisible(false);
		connect(mRecentFiles[i], SIGNAL(triggered(bool)), this, SLOT(openRecentPetriNet()));
	}

	// simulation menu actions
	mSimulationGroup = new QActionGroup(this);
	mSimulationGroup->addAction(ui.actionFireRandomTransition);
	mSimulationGroup->addAction(ui.actionFireNRandomTransitions);
	mSimulationGroup->setDisabled(true);
	connect(ui.actionStartSimulation, SIGNAL(toggled(bool)),
		this, SLOT(changeSimulationMode(bool)));
	connect(ui.actionFireRandomTransition, SIGNAL(triggered(bool)),
		mController, SLOT(fireRandomTransition()));
	connect(ui.actionFireNRandomTransitions, SIGNAL(triggered(bool)),
		mController, SLOT(fireNRandomTransitions()));

	mNonSimulationGroup = new QActionGroup(this);
	QAction* undoAction = mController->createUndoAction();
	ui.undoToolBar->addAction(undoAction);
	mNonSimulationGroup->addAction(undoAction);
	QAction* redoAction = mController->createRedoAction();
	ui.undoToolBar->addAction(redoAction);
	mNonSimulationGroup->addAction(redoAction);
	mNonSimulationGroup->addAction(ui.actionSave);
	mNonSimulationGroup->addAction(ui.actionSaveAs);
	mNonSimulationGroup->addAction(ui.actionExportToPNG);
	mNonSimulationGroup->addAction(ui.actionExportToPS);
	mNonSimulationGroup->addAction(ui.actionExportToSVG);
}

void MainWindow::setupDockWindows() {
	// setup miniature view
	ui.miniatureView->setSource(ui.petriNetView);

	// setup property editor
	ui.propEditor->setItemDelegate(new PropertyEditorDelegate(ui.propEditor));
	ui.propEditor->setModel(mController->propertyEditorModel());
}

void MainWindow::changeSimulationMode(bool mode) {
	mToolGroup->setDisabled(mode);
	mSimulationGroup->setDisabled(!mode);
	mNonSimulationGroup->setDisabled(mode);
	ui.propEditor->setDisabled(mode);
	if (mode) {
		ui.petriNetView->scene()->clearSelection();
		mController->startSimulation();
		ui.actionStartSimulation->setText(tr("Stop simulation"));
	} else {
		mController->stopSimulation();
		ui.actionSelect->trigger();
		mController->useSelectTool();
		ui.actionStartSimulation->setText(tr("Start simulation"));
	}
}

void MainWindow::closeEvent(QCloseEvent* event) {
	event->accept();
}

void MainWindow::enterWhatIsThisMode() {
	QWhatsThis::enterWhatsThisMode();
}

void MainWindow::newPetriNet() {
	new MainWindow();
}

void MainWindow::openPetriNet() {
	QString filter(tr("Petri Net (*.xml)"));
	QString fileName = QFileDialog::getOpenFileName(static_cast<QWidget*>(parent()),
								QString(), QString(), filter);
	if (!fileName.isNull())
		loadPetriNet(fileName);
}

void MainWindow::openRecentPetriNet() {
	QAction *action = qobject_cast<QAction *>(sender());
	if (action)
		loadPetriNet(action->data().toString());
}

void MainWindow::loadPetriNet(const QString& fileName) {
	if (mController->hasPetriNet()) {
		(new MainWindow())->loadPetriNet(fileName);
		return;
	}

	mController->loadPetriNet(fileName);
	setWindowTitle(fileName);

	QSettings settings;
	QStringList files = settings.value("recentFileList").toStringList();
	files.removeAll(fileName);
	files.prepend(fileName);
	while (files.size() > MAX_RECENT_FILES)
		files.removeLast();
	settings.setValue("recentFileList", files);

	foreach (QWidget* widget, QApplication::topLevelWidgets()) {
		MainWindow* wnd = qobject_cast<MainWindow*>(widget);
		if (wnd)
			wnd->updateRecentFileActions();
	}
}

void MainWindow::savePetriNet() {
	QString filter(tr("Petri Net (*.xml)"));
	QString	fileName;
	if (mController->fileName().isNull())
		fileName = QFileDialog::getSaveFileName(static_cast<QWidget*>(parent()),
										QString(), QString(), filter);
	else
		fileName = mController->fileName();
	if (!fileName.isNull()) {
		mController->savePetriNet(fileName);
		setWindowTitle(fileName);
	}
}

void MainWindow::savePetriNetAs() {
	QString filter(tr("Petri Net (*.xml)"));
	QString	fileName = QFileDialog::getSaveFileName(static_cast<QWidget*>(parent()),
										QString(), QString(), filter);
	if (!fileName.isNull()) {
		mController->savePetriNet(fileName);
		setWindowTitle(fileName);
	}
}

void MainWindow::updateRecentFileActions() {
	QSettings settings;
	QStringList files = settings.value("recentFileList").toStringList();

	ui.menuRecentPetriNets->setEnabled(!files.isEmpty());

	int numRecentFiles = qMin(files.size(), (int)MAX_RECENT_FILES);
	for (int i = 0; i < numRecentFiles; ++i) {
		mRecentFiles[i]->setText(files[i]);
		mRecentFiles[i]->setData(files[i]);
		mRecentFiles[i]->setVisible(true);
	}
	for (int j = numRecentFiles; j < MAX_RECENT_FILES; ++j)
		mRecentFiles[j]->setVisible(false);
}

void MainWindow::loadPlugins() {
	QDir pluginsDir(QApplication::applicationDirPath()+"/../share/opulus/plugins");
	foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
		QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
		QObject* plugin = loader.instance();
		if (!plugin) {
			qWarning(qPrintable("Error loading plugin: "
					+loader.errorString()));
		} else {
			Analyser* analyser = qobject_cast<Analyser*>(plugin);
			if (analyser) {
				QAction* action = ui.menuAnalysis->addAction(analyser->name(),
								this, SLOT(executeAnalyser()));
				mAnalysers.insert(action, analyser);
				mNonSimulationGroup->addAction(action);
			}
		}
	}

}

void MainWindow::executeAnalyser() {
	QAction* a = qobject_cast<QAction*>(sender());
	Q_ASSERT(a);
	mController->executeAnalyser(mAnalysers[a]);
}

void MainWindow::about() {
	QMessageBox::about(this, tr("About"),
			tr("<h1>Opulus version %1</h1>\n"
			"<p>Copyright 2007-2008 Opulus developers. Opulus is distributed under the terms of GPL v2, see the COPYING file for details or visit <a href=\"http://opulus.sf.net\">http://opulus.sf.net</a>.</p>\n"
			"<h2>Authors</h2>\n"
			"<dl>\n"
			"<dt>Hugo Parente Lima</dt>\n"
			"<dd><a href=\"mailto:hugo.pl@gmail.com\">hugo.pl@gmail.com</a><br>\n"
			"Developer, original author.</dd>\n"
			"<dt>Adauto Trigueiro de Almeida Filho</dt>\n"
			"<dd><a href=\"mailto:adautofilho@gmail.com\">adautofilho@gmail.com</a><br>\n"
			"Developer.</dd>\n"
			"<dt>Clerton Ribeiro de Ara\xFAjo Filho</dt>\n"
			"<dd><a href=\"mailto:clertonfilho@gmail.com\">clertonfilho@gmail.com</a><br>\n"
			"Developer.</dd>\n"
			"</dl>\n"
			"<h2>Contributors</h2>\n"
			"<dl>\n"
			"<dt>Carlos Sanchis</dt>\n"
			"<dd><a href=\"csanchisb@yahoo.es\">csanchisb@yahoo.es</a><br>\n"
			"Spanish translation.</dd>\n"
			"</dl>"
			).arg(opulusVersion()));
}

void MainWindow::setWindowTitle(const QString& document) {
	QMainWindow::setWindowTitle(QApplication::applicationName()+" - "+document+"[*]");
}

void MainWindow::cleanStateChanged(bool value) {
	setWindowModified(!value);
}

void MainWindow::searchAvailableLanguages() {
	QDir dir = i18nDir();
	dir.setNameFilters(QStringList() << "opulus_*.qm");

	QString currentLanguage = QSettings().value("language", QLocale::system().name()).toString();

	QActionGroup* languages = new QActionGroup(this);
	QAction* action = ui.menuChangeLanguage->addAction("English/United States");
	action->setData("en_US");
	action->setCheckable(true);
	action->setChecked(currentLanguage == "en_US");
	languages->addAction(action);
	connect(action, SIGNAL(triggered(bool)), this, SLOT(changeLanguage()));

	QRegExp regex("opulus_(.*)\\.qm$");
	foreach (QString lang, dir.entryList()) {
		if (regex.exactMatch(lang)) {
			QLocale locale(regex.cap(1));
			action = ui.menuChangeLanguage->addAction(QLocale::languageToString(locale.language())+"/"+QLocale::countryToString(locale.country()));
			action->setCheckable(true);
			action->setData(regex.cap(1));
			languages->addAction(action);
			action->setChecked(currentLanguage == regex.cap(1));
			connect(action, SIGNAL(triggered(bool)), this, SLOT(changeLanguage()));
		}
	}
}

void MainWindow::changeLanguage() {
	QAction* action = static_cast<QAction*>(sender());
	QString lang = action->data().toString();
	loadI18n(lang);
	ui.retranslateUi(this);

	// update analyser menu
	QHash<QAction*, Analyser*>::iterator it = mAnalysers.begin();
	for (; it != mAnalysers.end(); ++it) {
		it.key()->setText(it.value()->name());
	}
	QSettings().setValue("language", lang);
}

#include "mainwindow.moc"
