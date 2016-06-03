/***************************************************************************
 *   Copyright (C) 2007 by Hugo Parente Lima <hugo.pl@gmail.com>           *
 *                 2015-2016 by Zett Daymond <zettday@gmail.com>           *
 *                 2015-2016 by Catgirl      <captainslowpoke@gmail.com>   *
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
#include "commands/cmdcreatearcwithweight.h"
#include "commands/cmdcreateinhibitorarc.h"
#include "commands/cmdremoveitem.h"
#include "commands/cmdremoveitemgroup.h"
#include "commands/cmdmovenode.h"
#include "commands/cmdmovenodegroup.h"
#include "commands/cmdpack.h"

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
    connect(mPetriNet, SIGNAL(itemGroupRemoved(QVector<Item*>)),
            mScene, SLOT(removeItemGroup(QVector<Item*>)));
    connect(mPetriNet, SIGNAL(itemGroupCreated(QVector<Item*>)),
            mScene, SLOT(createItemGroup(QVector<Item*>)));


	connect(mPetriNet, SIGNAL(placeCreated(Place*)), this, SLOT(netUpdated()));
	connect(mPetriNet, SIGNAL(transitionCreated(Transition*)), this, SLOT(netUpdated()));
	connect(mPetriNet, SIGNAL(arcCreated(Arc*)), this, SLOT(netUpdated()));
	connect(mPetriNet, SIGNAL(arcWeightChanged(Arc*)), this, SLOT(netUpdated()));
	connect(mPetriNet, SIGNAL(itemRemoved(Item*)), this, SLOT(netUpdated()));
	connect(mPetriNet, SIGNAL(itemGroupRemoved(QVector<Item*>)), this, SLOT(netUpdated()));
	connect(mPetriNet, SIGNAL(itemGroupCreated(QVector<Item*>)), this, SLOT(netUpdated()));


	connect(mUndoStack, SIGNAL(cleanChanged(bool)), this, SIGNAL(cleanChanged(bool)));
	qsrand(0);
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
	action->setIcon(QIcon(":/redo"));
	action->setShortcut(QKeySequence::Redo);
	return action;
}

QAction* Controller::createUndoAction() {
	QAction* action = mUndoStack->createUndoAction(this);
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

QUndoCommand* Controller::createResizeMatrixCmds(int rows, int cols) {
	const int transitions_size = mPetriNet->transitionCount();

	QVector<QUndoCommand*> cmdPack;

	QVector<Item*> items;
	if(rows < transitions_size) {
		// TODO: move this into a function
		foreach (Transition* t, mPetriNet->transitions()) {
			if(t->number() >= rows)
				items.push_back(t);
		}
	}

	if(cols < mPetriNet->placeCount()) {
		// TODO: move this into a function
		foreach (Place* p, mPetriNet->places()) {
			if(p->number() >= cols)
				items.push_back(p);
		}
	}

	if (items.size() > 0) {
		cmdPack.push_back(new CmdRemoveItemGroup(items));
	}

	QVector<QPointF> positions;
	if(rows > transitions_size) {
		for(int i = transitions_size; i < rows; ++i) {
			int x = qrand() % 200 - 100;
			int y = qrand() % 200 - 100;
			QGraphicsView *v = scene()->views().takeFirst();
			QPointF center = v->mapToScene(v->viewport()->rect().center());
			//TODO: cange to CmdCreateTransitionGROUP
			cmdPack.push_back(new CmdCreateTransition(mPetriNet, center+QPointF(x,y)));
		}
	}

	if(cols > mPetriNet->placeCount()) {
		for(int i = mPetriNet->placeCount(); i < cols; ++i) {
			int x = qrand() % 200 - 100;
			int y = qrand() % 200 - 100;
			QGraphicsView *v = scene()->views().takeFirst();
			QPointF center = v->mapToScene(v->viewport()->rect().center());
			//TODO: change to CmdCreatePlaceGroup
			cmdPack.push_back(new CmdCreatePlace(mPetriNet, center+QPointF(x,y)));
		}

	}

	if (cmdPack.size() > 1) {
		CmdPack* pack = new CmdPack(this, cmdPack);
		return pack;
	}
	if (cmdPack.size() == 1) {
		return cmdPack[0];
	}

	return nullptr;
}

void Controller::matrixResized(int rows, int cols) {
	QUndoCommand* cmd = createResizeMatrixCmds(rows,cols);
	if (cmd != nullptr) {
		pushCommand(cmd);
	}
}

void Controller::updateMatrixValue(MatrixType which, int row, int col, int val) {
	Q_ASSERT(row >= 0 && col >= 0 && val >= 0);
	QUndoCommand* resizeCmd = nullptr;

	if(row >= mPetriNet->transitionCount() || col >= mPetriNet->placeCount()) {
		bool st = this->blockSignals(true);
		resizeCmd = createResizeMatrixCmds(
					std::max(row+1, mPetriNet->transitionCount()),
					std::max(col+1, mPetriNet->placeCount()));
		this->blockSignals(st);
	}

	QUndoCommand* updateCmd = createUpdateMatrixCmds(which, row, col, val);

	if (resizeCmd != nullptr && updateCmd != nullptr) {
		CmdPack* pack = new CmdPack(this);
		pack->pushBack(resizeCmd);
		pack->pushBack(updateCmd);
		pushCommand(pack);
	} else if (resizeCmd == nullptr && updateCmd != nullptr) {
		pushCommand( updateCmd );
	}

	return;
}

QUndoCommand* Controller::createUpdateMatrixCmds(MatrixType which, int row, int col, int val)
{
	Q_ASSERT(row < mPetriNet->transitionCount() && col < mPetriNet->placeCount());

	Transition* tr = mPetriNet->findTransitionWithNumber(row);
	Place *pl = mPetriNet->findPlaceWithNumber(col);

	if(tr && pl) {
		if(which == MatrixType::dMinusMatrix) {
			AbstractArc* arc = pl->findArcTo(static_cast<Node*>(tr));
			if(arc) {
				if(val == 0) {
					return new CmdRemoveItem(arc);
				} else {
					//Set weight
					if (val != arc->weight()) {
						return createItemAttributeCmd (arc, &AbstractArc::setWeight,
								(uint)val, arc->weight());
					} else {
						return nullptr;
					}
				}
			} else if(val) {
				if (val == 1) {
					return createAddArcCmd(pl,tr);
				} else {
					return createAddArcWithWeightCmd(pl,tr, val);
				}
				arc = pl->findArcTo(static_cast<Node*>(tr));
				if(!arc)
					throw Exception("Unable to find added arc");
			}

		} else if (which == MatrixType::dPlusMatrix) {
			AbstractArc* arc = tr->findArcTo(static_cast<Node*>(pl));
			if(arc) {
				if(val == 0) {
					return new CmdRemoveItem(arc);
				} else {
					//Set weight
					if( val != arc->weight()) {
						return createItemAttributeCmd(arc, &AbstractArc::setWeight,
								(uint)val, arc->weight());
					} else {
						return nullptr;
					}
				}
			} else if(val) {
				if (val == 1) {
					return createAddArcCmd(tr,pl);
				} else {
					return createAddArcWithWeightCmd(tr,pl, val);
				}
				arc = tr->findArcTo(static_cast<Node*>(pl));
				if(!arc) {
					throw Exception("Unable to find added arc");
				}
			}
		}
	}
	return nullptr;
}

void Controller::updateBasedOnMatrices(Eigen::MatrixXi dMinus, Eigen::MatrixXi dPlus)
{
	QUndoCommand* resizeCommands = createResizeMatrixCmds(dMinus.rows(), dMinus.cols());

	mPetriNet->blockSignals(true);
	this->blockSignals(true);
	if (resizeCommands != nullptr) {
		resizeCommands->redo();
	}

	CmdPack* updateCommands = new CmdPack(this);

	for(int i = 0; i < dMinus.rows(); i++) {
		for(int j = 0; j < dMinus.cols(); j++) {
			auto val = dMinus(i,j);
			auto cmd = createUpdateMatrixCmds(MatrixType::dMinusMatrix, i, j, val);
			//changes happens
			if (cmd) {
				updateCommands->pushBack(cmd);
			}
			val = dPlus(i,j);
			cmd = createUpdateMatrixCmds(MatrixType::dPlusMatrix, i, j, val);
			//changes happens
			if (cmd) {
				updateCommands->pushBack(cmd);
			}
		}
	}

	if (resizeCommands != nullptr) {
		resizeCommands->undo();
	}
	mPetriNet->blockSignals(false);
	this->blockSignals(false);

	CmdPack* cmdPack = new CmdPack(this);
	if (resizeCommands != nullptr) {
		if (updateCommands->size() > 0) {
			cmdPack->pushBack(resizeCommands);
			cmdPack->pushBack(updateCommands);
			pushCommand(cmdPack);
		} else {
			pushCommand(resizeCommands);
		}
	} else if (updateCommands->size() > 0) {
		pushCommand(updateCommands);
	} else {
		showWarningMessage(tr("Nothing to update"));
	}
}

void Controller::netUpdated() {
	if (mIsGuiNotificationEnabled == 0) {
		emit netChanged(PetriNetMatrices(d_minus_matrix(mPetriNet), d_plus_matrix(mPetriNet), d_matrix(mPetriNet)));
	}
}

void Controller::addPlace(const QPointF& position) {
	pushCommand(new CmdCreatePlace(mPetriNet, position));
}

void Controller::addTransition(const QPointF& position) {
	pushCommand(new CmdCreateTransition(mPetriNet, position));
}

void Controller::addArc(Place* from, Transition* to) {
	pushCommand( createAddArcCmd(from, to) );
}

void Controller::addArc(Place *from, Transition *to, uint weight) {
	pushCommand( createAddArcWithWeightCmd(from, to, weight) );
}
void Controller::addArc(Transition* from, Place* to) {
	pushCommand( createAddArcCmd(from, to) );
}

void Controller::addArc(Transition *from, Place *to, uint weight) {
	pushCommand( createAddArcWithWeightCmd(from, to, weight) );
}

void Controller::addInhibitorArc(Place* from, Transition* to) {
	pushCommand(new CmdCreateInhibitorArc(mPetriNet, from, to));
}

void Controller::addToken(Place* place) {
	if (place->numTokens() != Marking::OMEGA) {
		setItemAttribute(place, &Place::setNumTokens, place->numTokens() + 1, place->numTokens());
	}
}

void Controller::removeToken(Place* place) {
	Q_ASSERT(place);
	if(place->numTokens() == 0) {
		showErrorMessage(NoMoreTokensException().message());
	} else if (place->numTokens() != Marking::OMEGA) {
		setItemAttribute(place, &Place::setNumTokens, place->numTokens() - 1, place->numTokens());
	}
}

void Controller::moveNode(Node* node, const QPointF& pos) {
	pushCommand(new CmdMoveNode(node, pos));
}

void Controller::moveNodeGroup(QVector<Node*> &nodes, const QVector<QPointF>& poses) {
	pushCommand(new CmdMoveNodeGroup(nodes, poses));
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
	//FIXME: Does need change to pushCommand()?
	mUndoStack->push(new CmdRemoveItem(item));
}

void Controller::removeItemGroup(QVector<Item*>& items) {
	//FIXME: Does need change to CmdPack()?
	pushCommand(new CmdRemoveItemGroup(items));
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
	QPrinter printer(QPrinter::ScreenResolution);
	printer.setOutputFormat(QPrinter::PdfFormat);
	printer.setOutputFileName(fileName);
	paintScene(&printer);
}

void Controller::exportToSVG() {
	QString fileName = showExportFileDialog(tr("Scalable Vector Graphics (*.svg)"), "svg");
	if (fileName.isNull())
		return;
	QSvgGenerator svgGenerator;
	svgGenerator.setSize(mScene->sceneRect().size().toSize());
	svgGenerator.setTitle(tr("Petri Net Graph"));
	svgGenerator.setDescription(tr("Generated by Opulus Petri Net Editor"));
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
}

void Controller::savePetriNet(const QString& fileName) {
	QFile file(fileName);
	mPetriNet->save(&file);
	mFileName = fileName;
	mUndoStack->setClean();
}

QUndoCommand* Controller::createAddArcWithWeightCmd(auto from, auto to, uint weight) {
	AbstractArc* arc = from->findArcTo(static_cast<Node*>(to));
	if(!arc) {
		return new CmdCreateArcWithWeight(mPetriNet, from, to, weight);
	} else {
		return createItemAttributeCmd(arc, &AbstractArc::setWeight,
		arc->weight() + weight, arc->weight());
	}
}

QUndoCommand*Controller::createAddArcCmd(auto from, auto to) {
	AbstractArc* arc = from->findArcTo(static_cast<Node*>(to));
	if(!arc) {
		return new CmdCreateArc(mPetriNet, from, to);
	} else {
		return createItemAttributeCmd(arc, &AbstractArc::setWeight,
		arc->weight() + 1, arc->weight());
	}
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

void Controller::zoomIn() {
	mView->scale(1.1, 1.1);
}

void Controller::zoomOut() {
	mView->scale(0.9, 0.9);
}

void Controller::pushCommandNoCatch(QUndoCommand* cmd) {
	std::unique_ptr<QUndoCommand> ptr(cmd);
	mUndoStack->push(ptr.get());
	ptr.release();
}

bool Controller::pushCommand(QUndoCommand* cmd) {
	try {
		pushCommandNoCatch(cmd);
		return true;
	} catch (Exception& e) {
		showErrorMessage(e.message(), 10000);
	}
	return false;
}

void Controller::enableGuiNotifications()
{
	if (mIsGuiNotificationEnabled > 0) {
		mIsGuiNotificationEnabled -= 1;
	}
	if (mIsGuiNotificationEnabled == 0) {
		netUpdated();
	}
}

void Controller::disableGuiNotifications()
{
	mIsGuiNotificationEnabled += 1;
}


//#include "controller.moc"
