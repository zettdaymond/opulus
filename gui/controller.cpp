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

#include <QWidget>
#include <QAction>
#include <QGraphicsView>
#include <QDir>
#include <QFileDialog>
#include <QPrinter>
#include <QSvgGenerator>
#include <QApplication>
#include <QUndoStack>
#include <QMessageBox>
#include <QInputDialog>
#include <QGraphicsView>
#include <cstdlib>
#include <memory>
#include <stdexcept>

#include "controller.h"
#include "petrinet.h"
#include "simulation.h"
#include "place.h"
#include "transition.h"
#include "arc.h"
#include "exceptions.h"
#include "petrinetscene.h"
#include "propertyeditormodel.h"
#include "messagewidget.h"
#include "analyser.h"
#include "analysisrunner.h"
#include "analyserstatusdlg.h"

// commands
#include "commands/cmdcreateplace.h"
#include "commands/cmdcreatetransition.h"
#include "commands/cmdcreatearc.h"
#include "commands/cmdcreateinhibitorarc.h"
#include "commands/cmdremoveitem.h"
#include "commands/cmdmovenode.h"

Controller::Controller(QWidget* parent, QGraphicsView* view) : QObject(parent), mView(view), mParentWidget(parent), mAnalyserStatusDlg(0), mUndoStack(0), mSimulation(0) {
	mUndoStack = new QUndoStack(this);
	mPetriNet = new PetriNet(this);
	mScene = new PetriNetScene(this);
	mView->setScene(mScene);
	mMessage = new MessageWidget(mView);
	mPropEditorModel = new PropertyEditorModel(this);
	mAnalysisRunner = new AnalysisRunner(this);

	// runner notifications
	connect(mAnalysisRunner, SIGNAL(finished()), this, SLOT(analysisFinished()));
	// prop editor notifications
	connect(mPetriNet, SIGNAL(itemModified(Item*)),
			mPropEditorModel, SLOT(itemModified(Item*)));
	// scene notifications
	connect(mPetriNet, SIGNAL(placeCreated(Place*)),
			mScene, SLOT(createPlace(Place*)));
	connect(mPetriNet, SIGNAL(transitionCreated(Transition*)),
			mScene, SLOT(createTransition(Transition*)));
	connect(mPetriNet, SIGNAL(arcCreated(Arc*)),
			mScene, SLOT(createArc(Arc*)));
	connect(mPetriNet, SIGNAL(inhibitorArcCreated(InhibitorArc*)),
			mScene, SLOT(createInhibitorArc(InhibitorArc*)));
	connect(mPetriNet, SIGNAL(itemModified(Item*)),
			mScene, SLOT(updateItem(Item*)));
	connect(mPetriNet, SIGNAL(itemRemoved(Item*)),
			mScene, SLOT(removeItem(Item*)));
	connect(mScene, SIGNAL(itemSelected(Item*)),
			mPropEditorModel, SLOT(setModelSource(Item*)));

	connect(mUndoStack, SIGNAL(cleanChanged(bool)), this, SIGNAL(cleanChanged(bool)));
	qsrand(0);

    connect(mPropEditorModel, SIGNAL(modelReset()),this,SLOT(undoRedoReaction()));
    //FIXME: change name to slot undoRedoReation();
}

Controller::~Controller() {
	stopSimulation();
}

QGraphicsScene* Controller::scene() {
	return mScene;
}

QAbstractItemModel* Controller::propertyEditorModel() {
	return mPropEditorModel;
}

bool Controller::hasPetriNet() const {
	return mPetriNet->itemCount() || mUndoStack->count();
}

QAction* Controller::createRedoAction() {
	QAction* action = mUndoStack->createRedoAction(this);
    connect(action, SIGNAL(triggered()),this,SLOT(undoRedoReaction()));
	action->setIcon(QIcon(":/redo"));
	action->setShortcut(QKeySequence::Redo);
	return action;
}

QAction* Controller::createUndoAction() {
    QAction* action = mUndoStack->createUndoAction(this);
    connect(action, SIGNAL(triggered()),this,SLOT(undoRedoReaction()));
	action->setIcon(QIcon(":/undo"));
	action->setShortcut(QKeySequence::Undo);
	return action;
}

void Controller::showWarningMessage(const QString& message, int durationMs) {
	mMessage->showText(message, MessageWidget::Warning, durationMs);
}

void Controller::showErrorMessage(const QString& message, int durationMs) {
	mMessage->showText(message, MessageWidget::Error, durationMs);
}

void Controller::showInfoMessage(const QString& message, int durationMs) {
	mMessage->showText(message, MessageWidget::Info, durationMs);
}

void Controller::useSelectTool() {
	mMessage->hide();
	mScene->useSelectTool();
}

void Controller::usePlaceTool() {
	mScene->usePlaceTool();
}

void Controller::useTransitionTool() {
	mScene->useTransitionTool();
}

void Controller::useArcTool() {
	mScene->useArcTool();
}

void Controller::useInhibitorArcTool() {
	mScene->useInhibitorArcTool();
}

void Controller::useTokenTool() {
	mScene->useTokenTool();
}

void Controller::useFireTransitionTool() {
	mScene->useFireTransitionTool();
}

void Controller::matrixResized(int rows, int cols)
{
	const int transitions_size = mPetriNet->transitions().size();
	if(rows < transitions_size) {
		showWarningMessage(tr("Please select and delete transition manually."), MessageWidget::Forever);
		emit netChanged(mPetriNet);
		return;
	}

	if(cols < mPetriNet->placeCount()) {
		showWarningMessage(tr("Please select and delete place manually."), MessageWidget::Forever);
		emit netChanged(mPetriNet);
		return;
	}
	if(rows > transitions_size) {
		for(int i = transitions_size; i < rows; ++i) {
			int x = qrand() % 200 - 100;
			int y = qrand() % 200 - 100;
			QGraphicsView *v = scene()->views().takeFirst();
			QPointF center = v->mapToScene(v->viewport()->rect().center());
			addTransition(center+QPointF(x,y));
		}
	}

	if(cols > mPetriNet->placeCount()) {
		for(int i = mPetriNet->placeCount(); i < cols; ++i) {
			int x = qrand() % 200 - 100;
			int y = qrand() % 200 - 100;
			QGraphicsView *v = scene()->views().takeFirst();
			QPointF center = v->mapToScene(v->viewport()->rect().center());
			addPlace(center+QPointF(x,y));

		}

	}
}

void Controller::matrixUpdate(char matrix, int row, int col, int val)
{
	if(row >= mPetriNet->transitions().size() || col >= mPetriNet->placeCount()) {
		bool st = this->blockSignals(true);
		matrixResized(row+1,col+1);
		this->blockSignals(st);
	}
	if(row < 0 || col < 0)
		throw std::invalid_argument("row or col < 0");
	if(!(matrix == '-' || matrix == '+'))
		throw std::invalid_argument("incorrect matrix: must be - or +");

	Transition* tr = NULL;
	Place *pl = NULL;
	foreach (Transition* t, mPetriNet->transitions()) {
		QString tmp = t->name();
		if(tmp.startsWith('T')) {
			int num; bool ok;
			if(((num = tmp.remove(0,1).toInt(&ok)) == row) && ok) {
				tr = t;
				break;
			}
		}
	}
	foreach (Place* p, mPetriNet->places()) {
		QString tmp = p->name();
		if(tmp.startsWith('P')) {
			int num; bool ok;
			if(((num = tmp.remove(0,1).toInt(&ok)) == col) && ok) {
				pl = p;
				break;
			}
		}
	}

	if(tr && pl) {
		//qDebug() << "found" << from->name() << to->name();
		if(matrix == '-') {
			bool found = false;
			foreach (AbstractArc* arc, pl->outputArcs()) {
				if(arc->to() == static_cast<Node*>(tr)) {
					found = true;
					if(val == 0) {
						removeItem(arc);
						break;
					}
					arc->setWeight(val);
				}
			}
			if(!found)
				addArc(pl,tr);

		} else if (matrix == '+') {
			bool found = false;
			foreach (AbstractArc* arc, tr->outputArcs()) {
				if(arc->to() == static_cast<Node*>(pl)) {
					found = true;
					if(val == 0) {
						removeItem(arc);
						break;
					}
					arc->setWeight(val);
				}
			}
			if(!found)
				addArc(tr,pl);

		}
	}
}

void Controller::addPlace(const QPointF& position) {
	pushCommand(new CmdCreatePlace(mPetriNet, position));
}

void Controller::addTransition(const QPointF& position) {
	pushCommand(new CmdCreateTransition(mPetriNet, position));
}

void Controller::addArc(Place* from, Transition* to) {
	pushCommand(new CmdCreateArc(mPetriNet, from, to));
}
void Controller::addArc(Transition* from, Place* to) {
	pushCommand(new CmdCreateArc(mPetriNet, from, to));
}

void Controller::addInhibitorArc(Place* from, Transition* to) {
	pushCommand(new CmdCreateInhibitorArc(mPetriNet, from, to));
}

void Controller::addToken(Place* place) {
	try {
		place->addToken();
	} catch(Exception& e) {
		showErrorMessage(e.message());
	}
}

void Controller::removeToken(Place* place) {
	try {
		place->removeToken();
	} catch(Exception& e) {
		showErrorMessage(e.message());
	}
}

void Controller::moveNode(Node* node, const QPointF& pos) {
	pushCommand(new CmdMoveNode(node, pos));
}

QPointF* Controller::addIntermediatePoint(AbstractArc* arc, const QPointF& point, int segmentIndex) {
	return arc->addIntermediatePoint(point, segmentIndex);
}

void Controller::fireTransition(Transition* t) {
	try {
		if (mSimulation)
			mSimulation->fireTransition(t);
	} catch (Exception& e) {
		showErrorMessage(e.message());
	}
}

void Controller::fireRandomTransition() {
	try {
		if (mSimulation)
			mSimulation->fireRandomTransition();
	} catch (Exception& e) {
		showErrorMessage(e.message());
	}
}

void Controller::fireNRandomTransitions() {
	try {
		if (mSimulation){
	    int n = QInputDialog::getInt(mParentWidget, tr("Fire N Random Transitons"), tr("How many times do you want to fire random transitions?"), 1,1);
			mSimulation->fireNRandomTransitions(n);
		}
	} catch (Exception& e) {
		showErrorMessage(e.message());
	}
}

void Controller::removeItem(Item* item) {
	mUndoStack->push(new CmdRemoveItem(item));
    emit netChanged(mPetriNet);
}

void Controller::startSimulation() {
	try {
		mSimulation = new Simulation(mPetriNet);
		useFireTransitionTool();
	} catch (Exception& e) {
		showErrorMessage(e.message());
	}
}

void Controller::stopSimulation() {
	delete mSimulation;
	mSimulation = 0;
}

void Controller::renameNode(Node* node, const QString& newName) {
	node->setName(newName);
}

QString Controller::showExportFileDialog(const QString& filter, const QString& defaultSuffix) {
	QFileDialog dlg(static_cast<QWidget*>(parent()), tr("Export"));
	dlg.setAcceptMode(QFileDialog::AcceptSave);
	dlg.setNameFilter(filter);
	dlg.setDefaultSuffix(defaultSuffix);
	if (dlg.exec() == QDialog::Rejected)
		return QString::null;
	return dlg.selectedFiles().first();
}

void Controller::paintScene(QPaintDevice* device) {
	QPainter painter(device);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
	mScene->render(&painter, QRectF(), mScene->itemsBoundingRect().toRect());
}

void Controller::exportToPNG() {
	QString fileName = showExportFileDialog(tr("Image PNG (*.png)"), "png");
	if (fileName.isNull())
		return;
	QRect sceneRect = mScene->itemsBoundingRect().toRect();
	sceneRect.adjust(-5, -5, 5, 5); // small border
	QImage img(sceneRect.size(), QImage::Format_ARGB32);
	img.fill(0);
	paintScene(&img);
	img.save(fileName, "PNG");
}

void Controller::exportToPS() {
	QString fileName = showExportFileDialog(tr("PDF (*.pdf)"), "pdf");
	if (fileName.isNull())
		return;
	QPrinter printer(QPrinter::HighResolution);
	printer.setOutputFormat(QPrinter::PdfFormat);
	printer.setOutputFileName(fileName);
	paintScene(&printer);
}

void Controller::exportToSVG() {
	QString fileName = showExportFileDialog(tr("Scalable Vector Graphics (*.svg)"), "svg");
	if (fileName.isNull())
		return;
	QSvgGenerator svgGenerator;
	svgGenerator.setFileName(fileName);
	paintScene(&svgGenerator);
}

void Controller::loadPetriNet(const QString& fileName) {
	QApplication::setOverrideCursor(Qt::WaitCursor);
	try {
		QFile file(fileName);
		mPetriNet->load(&file);
		mFileName = fileName;
	} catch (Exception& e) {
		showErrorMessage(e.message());
	}
    QApplication::restoreOverrideCursor();
    emit netChanged(mPetriNet);
}

void Controller::savePetriNet(const QString& fileName) {
	QFile file(fileName);
	mPetriNet->save(&file);
	mFileName = fileName;
	mUndoStack->setClean();
}

void Controller::executeAnalyser(Analyser* analyser) {
	if (!mAnalyserStatusDlg) {
		mAnalyserStatusDlg = new AnalyserStatusDlg(mParentWidget);
		connect(mAnalysisRunner, SIGNAL(analysisStatusMessageChanged(const QString&)),
			mAnalyserStatusDlg, SLOT(setStatusMessage(const QString&)));
		connect(mAnalysisRunner, SIGNAL(analysisPercentageChanged(int)),
			mAnalyserStatusDlg, SLOT(setPercenage(int)));
		connect(mAnalysisRunner, SIGNAL(analysisFatalErrorOccurred(const QString&)),
			this, SLOT(analysisFatalError(const QString&)));
	}

	if (analyser->setup(mParentWidget))
		mAnalysisRunner->executeAnalysis(analyser, mPetriNet);
	else
		mAnalyserStatusDlg->hide();
}

void Controller::analysisFinished() {
	mAnalyserStatusDlg->hide();
	mAnalysisRunner->analyser()->finish(mParentWidget);
}

void Controller::analysisFatalError(const QString& msg) {
	mAnalyserStatusDlg->hide();
	QMessageBox::critical(mParentWidget,
                          mAnalysisRunner->analyser()->name(), msg);
}

void Controller::undoRedoReaction()
{
    emit netChanged(mPetriNet);
}

void Controller::zoomIn() {
	mView->scale(1.1, 1.1);
}

void Controller::zoomOut() {
	mView->scale(0.9, 0.9);
}

void Controller::pushCommandNoCatch(QUndoCommand* cmd) {
	std::auto_ptr<QUndoCommand> ptr(cmd);
	mUndoStack->push(ptr.get());
	ptr.release();
}

bool Controller::pushCommand(QUndoCommand* cmd) {
	try {
		pushCommandNoCatch(cmd);
        emit netChanged(mPetriNet);
		return true;
	} catch (Exception& e) {
		showErrorMessage(e.message(), 10000);
	}
	return false;
}


#include "controller.moc"
