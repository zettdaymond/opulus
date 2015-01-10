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

#ifndef PETRINETSCENE_H
#define PETRINETSCENE_H

#include <QGraphicsScene>
#include <QHash>
#include <QSet>

class PetriNet;
class Item;
class Place;
class Transition;
class AbstractArc;
class Arc;
class InhibitorArc;
class Node;
class GraphicItem;
class Controller;

class QGraphicsLineItem;

/**
* @ingroup gui
*/
class PetriNetScene : public QGraphicsScene {
Q_OBJECT
public:
	explicit PetriNetScene(Controller* controller);

	GraphicItem* graphicItem(Item* item) const;

	void moveNode(Node* node, const QPointF& pos);
	QPointF* addIntermediatePoint(AbstractArc* arc, const QPointF& point, int segmentIndex);

	void useSelectTool();
	void usePlaceTool();
	void useTransitionTool();
	void useArcTool();
	void useInhibitorArcTool();
	void useTokenTool();
	void useFireTransitionTool();

public slots:
	void updateItem(Item* item);
	void createPlace(Place* place);
	void createTransition(Transition* transition);
	void createArc(Arc* arc);
	void createInhibitorArc(InhibitorArc* arc);
	void removeItem(Item* item);

signals:
	void itemSelected(Item* item);

protected:
	void keyPressEvent(QKeyEvent * event);
	void mousePressEvent(QGraphicsSceneMouseEvent* event);
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
private slots:
	void emitItemSelected();
private:
	typedef QHash<Item*, GraphicItem*> ItemMap;
	ItemMap mItemMap;
	typedef QSet<QGraphicsItem*> ItemSet;
	ItemSet mItems;
	Controller* mController;

	// the type os the current request
	enum CurrentRequest {
		CR_ITEM, CR_POINT, CR_NONE
	} mCurrentRequest;
	Qt::MouseButton mCurrentMouseButton;
	typedef void (PetriNetScene::*MethodPointer)(); // a typedef to a method pointer
	MethodPointer mResquestCallBack;               // callback to the next request
	QList<QVariant> mRequestedObjects;             // requested objects
	QGraphicsLineItem* mArcLine;

	void cleanTool(const MethodPointer& method);
	void addItem(Item* item, GraphicItem* gitem);
};

#endif
