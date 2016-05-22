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

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include "exceptions.h"
#include "matrix_util.h"
#include "commands/cmdchangeproperty.hpp"

class QWidget;
class QGraphicsView;
class QPointF;
class QUndoStack;
class QAction;
class PetriNet;
class Simulation;
class Item;
class Node;
class Place;
class Transition;
class AbstractArc;
class Arc;
class PetriNetScene;
class QGraphicsScene;
class PropertyEditorModel;
class QAbstractItemModel;
class QPaintDevice;
class MessageWidget;
class Analyser;
class AnalysisRunner;
class AnalyserStatusDlg;
class QUndoCommand;

/**
* GUI controller, hold the petri net and can send commands to it.
* @ingroup gui
*/
class Controller : public QObject {
Q_OBJECT
public:
	/// Constructs a new Controller with the parent \p parent.
	Controller(QWidget* parent, QGraphicsView* view);
	~Controller();
	/// Returns the PetriNet QGV scene
	QGraphicsScene* scene();
	/// Returns a model with the properties of the current selected item.
	QAbstractItemModel* propertyEditorModel();

	bool hasPetriNet() const;
	const QString& fileName() const { return mFileName; }

	QAction* createRedoAction();
	QAction* createUndoAction();

	void showWarningMessage(const QString& message, int durationMs = 4000);
	void showErrorMessage(const QString& message, int durationMs = 4000);
	void showInfoMessage(const QString& message, int durationMs = 4000);

	void executeAnalyser(Analyser* analyser);
public slots:
	/// Add a place to the petri net.
	void addPlace(const QPointF& position);
	/// Add a transition to the petri net.
	void addTransition(const QPointF& position);
	/// Add an arc to the petri net.
	void addArc(Place* from, Transition* to);
	void addArc(Place* from, Transition* to, uint weight);
	void addArc(Transition* from, Place* to);
	void addArc(Transition* from, Place* to, uint weight);
	void addInhibitorArc(Place* from, Transition* to);
	/// Add a token to a place.
	void addToken(Place* place);
	/// Remove one token from a place.
	void removeToken(Place* place);
	/// Move a node to \p pos.
	void moveNode(Node* node, const QPointF& pos);
	QPointF* addIntermediatePoint(AbstractArc* arc, const QPointF& point, int segmentIndex);
	/// try to fire the transition \p t.
	void fireTransition(Transition* t);
	/// Fire a random transition.
	void fireRandomTransition();
	/// Fire n random transitions
	void fireNRandomTransitions();
	/// Remove the \p item from the petri net.
	void removeItem(Item* item);
	/// Start the petri net simulation.
	void startSimulation();
	/// Stop the petri net simulation.
	void stopSimulation();
	/// Rename a node.
	void renameNode(Node* node, const QString& newName);

	void exportToPNG();
	void exportToPS();
	void exportToSVG();

	void useSelectTool();
	void usePlaceTool();
	void useTransitionTool();
	void useArcTool();
	void useInhibitorArcTool();
	void useTokenTool();
	void useFireTransitionTool();

	void matrixResized(int rows, int cols);
	void matrixUpdate(MatrixType which, int row, int col, int val);
	void netUpdated();

	void zoomIn();
	void zoomOut();

	void loadPetriNet(const QString& fileName);
	void savePetriNet(const QString& fileName);
signals:
	void cleanChanged(bool);
	void netChanged(PetriNetMatrices matrices);
public:
	// TODO: will be possile create such thing in a command when we implement the
	// command pattern o do the undo/redo?
//    template<class Type, typename ParamType>
//	void setItemAttribute(Type* obj, void(Type::* method)(ParamType), ParamType param) {
//		try {
//			(obj->*method)(param);
//		} catch (Exception& e) {
//			showErrorMessage(e.message());
//			throw;
//		}
//	}
	template<typename Type, typename Base, typename ParamType>
	void setItemAttribute(Type* obj, void(Base::* method)(ParamType), ParamType param, ParamType old) {
		pushCommand(new CmdChangeProperty<Type, Base, ParamType> (obj, method, param, old, mPetriNet));
	}
private slots:
	void analysisFinished();
	void analysisFatalError(const QString& msg);

private:
	QString mFileName;
	PetriNet* mPetriNet;
	PetriNetScene* mScene;
	QGraphicsView* mView;
	MessageWidget* mMessage;
	AnalysisRunner* mAnalysisRunner;
	QWidget* mParentWidget;
	AnalyserStatusDlg* mAnalyserStatusDlg;
	QUndoStack* mUndoStack;

	PropertyEditorModel* mPropEditorModel;
	Simulation* mSimulation;

	QString showExportFileDialog(const QString& filter, const QString& defaultSuffix);
	void paintScene(QPaintDevice* device);
	void pushCommandNoCatch(QUndoCommand* cmd);
	bool pushCommand(QUndoCommand* cmd);
};

#endif
