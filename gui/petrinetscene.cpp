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

#include "petrinetscene.h"
#include "place.h"
#include "transition.h"
#include "arc.h"
#include "inhibitorarc.h"
#include "graphicplace.h"
#include "graphictransition.h"
#include "graphicarc.h"
#include "graphicinhibitorarc.h"
#include "controller.h"
#include "messagewidget.h"

#include <QVector>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QGraphicsLineItem>
#include <QPen>
#include <QColor>
#include <QLineF>
#include <QDesktopWidget>
#include <cmath>

PetriNetScene::PetriNetScene(Controller* controller) : mController(controller), mResquestCallBack(0) {
	setItemIndexMethod(BspTreeIndex);
	connect(this, SIGNAL(selectionChanged()), this, SLOT(emitItemSelected()));
	mArcLine = new QGraphicsLineItem;
	mArcLine->hide();
	QGraphicsScene::addItem(mArcLine);
	useSelectTool();
	
	setSceneRect(QDesktopWidget().screenGeometry());
}

void PetriNetScene::moveNode(Node* node, const QPointF& pos) {
	mController->moveNode(node, pos);
}

void PetriNetScene::moveNodeGroup(QVector<Node*> &nodes, const QVector<QPointF> &poses) {
	mController->moveNodeGroup(nodes,poses);
}

QPointF* PetriNetScene::addIntermediatePoint(AbstractArc* arc, const QPointF& point, int segmentIndex) {
	return mController->addIntermediatePoint(arc, point, segmentIndex);
}

void PetriNetScene::addItem(Item* item, GraphicItem* gitem) {
	mItemMap.insert(item, gitem);
	mItems.insert(gitem);
	QGraphicsScene::addItem(gitem);
}

void PetriNetScene::createPlace(Place* place) {
	GraphicPlace* g = new GraphicPlace(place);
	addItem(place, g);
}

void PetriNetScene::createTransition(Transition* transition) {
	GraphicTransition* g = new GraphicTransition(transition);
	addItem(transition, g);
}

void PetriNetScene::createArc(Arc* arc) {
	GraphicArc* g = new GraphicArc(arc);
	addItem(arc, g);
	g->itemChanged();
}

void PetriNetScene::createInhibitorArc(InhibitorArc* arc) {
	GraphicInhibitorArc* g = new GraphicInhibitorArc(arc);
	addItem(arc, g);
	g->itemChanged();
}

void PetriNetScene::createItemGroup(QVector<Item*> items) {
	for (auto* i : items) {
		if (i->isA<Place>()) {
			Place* place = static_cast<Place*>(i);
			createPlace(place);
		}
		if (i->isA<Transition>()) {
			Transition* tr = static_cast<Transition*>(i);
			createTransition(tr);
		}
		if (i->isA<Arc>()) {
			Arc* arc = static_cast<Arc*>(i);
			createArc(arc);
		}
		if (i->isA<InhibitorArc>()) {
			InhibitorArc* arc = static_cast<InhibitorArc*>(i);
			createInhibitorArc(arc);
		}
	}
}

void PetriNetScene::updateItem(Item* item) {
	ItemMap::iterator it = mItemMap.find(item);
	if (it != mItemMap.end()) {
		it.value()->itemChanged();
	}
}

void PetriNetScene::removeItem(Item* item) {
	GraphicItem* gitem = mItemMap.take(item);
	if (gitem) {
		QGraphicsScene::removeItem(gitem);
		mItems.remove(gitem);
		delete gitem;
	}
}

void PetriNetScene::removeItemGroup(QVector<Item*> items) {
	for(auto* i : items) {
		removeItem(i);
	}
}

GraphicItem* PetriNetScene::graphicItem(Item* item) const {
	ItemMap::const_iterator it = mItemMap.find(item);
	if (it != mItemMap.end())
		return it.value();
	qWarning("GraphicItem not found at graphics scene!");
	return 0;
}

void PetriNetScene::keyPressEvent(QKeyEvent * event) {
	if (event->key() == Qt::Key_Delete) {
		//If there is only one node to remove,
		//then use specialized method on efficiency reason.
		auto selectedItems = this->selectedItems();
		if (selectedItems.size() == 1) {
			QGraphicsItem* g = selectedItems[0];
			if (mItems.contains(g)) {
				GraphicItem* gItem = dynamic_cast<GraphicItem*>(g);
				if (gItem) {
					mController->removeItem(gItem->item());
				}
			}
		} else {
			QVector<Item*> itemsToRemove;
			itemsToRemove.reserve(selectedItems.size());

			foreach(QGraphicsItem* g, selectedItems) {
				if (!mItems.contains(g)) {
					continue;
				}
				GraphicItem* gItem = dynamic_cast<GraphicItem*>(g);
				if (gItem) {
					itemsToRemove.push_back(gItem->item());
				}
			}
			if (itemsToRemove.size() > 0) {
				mController->removeItemGroup(itemsToRemove);
			}
		}
	}
	QGraphicsScene::keyPressEvent(event);
}

void PetriNetScene::emitItemSelected() {
	QList<QGraphicsItem*> items = selectedItems();
	Item* item = 0;
	if (items.size() == 1) {
		GraphicItem* gitem = dynamic_cast<GraphicItem*>(items.first());
		if (gitem)
			item = gitem->item();
	}
	emit itemSelected(item);
}

void PetriNetScene::mousePressEvent(QGraphicsSceneMouseEvent* event) {
	if (mCurrentRequest != CR_NONE) {
         QTransform  deviceTransform;
        //TODO //QGraphicsItem* item = itemAt(event->scenePos());
        QGraphicsItem* item = itemAt(event->scenePos(),deviceTransform);
		mCurrentMouseButton = event->button();
		if (mCurrentRequest == CR_POINT) {
			if (item) {
				clearSelection();
				item->setSelected(true);
			} else {
				mRequestedObjects << event->scenePos();
				(this->*mResquestCallBack)();
			}
		} else if (mCurrentRequest == CR_ITEM && item) {
			mRequestedObjects << QVariant::fromValue(item);
			(this->*mResquestCallBack)();
		}
	} else {
		QGraphicsScene::mousePressEvent(event);
	}
}

void PetriNetScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) {
	mousePressEvent(event);
}

void PetriNetScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
	if (mArcLine->isVisible())
		mArcLine->setLine(QLineF(mArcLine->line().p1(), event->scenePos()));
	else
		QGraphicsScene::mouseMoveEvent(event);
}

void PetriNetScene::useSelectTool() {
	cleanTool(&PetriNetScene::useSelectTool);
	mCurrentRequest = CR_NONE;
	mRequestedObjects.clear();
}

void PetriNetScene::usePlaceTool() {
	cleanTool(&PetriNetScene::usePlaceTool);
	if (mRequestedObjects.count() == 0) {
		mCurrentRequest = CR_POINT;
		mController->showInfoMessage(tr("Choose a location to put the place."), MessageWidget::Forever);
		mResquestCallBack = &PetriNetScene::usePlaceTool;
	} else if (mRequestedObjects.count() == 1 && mCurrentMouseButton == Qt::LeftButton) {
		mController->addPlace(mRequestedObjects.takeLast().toPointF());
	} else
		mRequestedObjects.removeLast();
}

void PetriNetScene::useTransitionTool() {
	cleanTool(&PetriNetScene::useTransitionTool);
	if (mRequestedObjects.count() == 0) {
		mCurrentRequest = CR_POINT;
		mController->showInfoMessage(tr("Choose a location to put the transition."), MessageWidget::Forever);
		mResquestCallBack = &PetriNetScene::useTransitionTool;
	} else if (mRequestedObjects.count() == 1 && mCurrentMouseButton == Qt::LeftButton) {
		mController->addTransition(mRequestedObjects.takeLast().toPointF());
	} else
		mRequestedObjects.removeLast();
}

void PetriNetScene::useArcTool() {
	cleanTool(&PetriNetScene::useArcTool);
	if (mRequestedObjects.count() == 0) {
		mCurrentRequest = CR_ITEM;
		mController->showInfoMessage(tr("Choose a transition or place."), MessageWidget::Forever);
		mResquestCallBack = &PetriNetScene::useArcTool;
	} else if (mRequestedObjects.count() == 1 && mCurrentMouseButton == Qt::LeftButton) {
		QGraphicsItem* item = mRequestedObjects.last().value<QGraphicsItem*>();
		switch(item->type()) {
			case GraphicPlace::Type:
				mController->showInfoMessage(tr("Choose a transition."), MessageWidget::Forever);
				break;
			case GraphicTransition::Type:
				mController->showInfoMessage(tr("Choose a place."), MessageWidget::Forever);
				break;
			default:
				mController->showInfoMessage(tr("You must choose a transition or place, not this!"), MessageWidget::Forever);
				mRequestedObjects.clear();
				return;
		}
		mArcLine->setLine(QLineF(item->pos(), item->pos()));
		mArcLine->show();
	} else if (mRequestedObjects.count() == 2 && mCurrentMouseButton == Qt::LeftButton) {
		QGraphicsItem* second = mRequestedObjects.takeLast().value<QGraphicsItem*>();
		QGraphicsItem* first = mRequestedObjects.takeLast().value<QGraphicsItem*>();
		int expectedType = first->type() == GraphicPlace::Type ? int(GraphicTransition::Type) : int(GraphicPlace::Type);
		if (second->type() != expectedType) {
			mController->showErrorMessage(tr("You can not create an arc between these items."));
		} else {
			GraphicItem* gitem1 = static_cast<GraphicItem*>(first);
			GraphicItem* gitem2 = static_cast<GraphicItem*>(second);
			if (expectedType == GraphicPlace::Type) {
				mController->addArc(static_cast<Transition*>(gitem1->item()), static_cast<Place*>(gitem2->item()));
			} else {
				mController->addArc(static_cast<Place*>(gitem1->item()), static_cast<Transition*>(gitem2->item()));
			}
		}
		mArcLine->hide();
		useArcTool();
	} else
		mRequestedObjects.removeLast();
}

void PetriNetScene::useInhibitorArcTool() {
	cleanTool(&PetriNetScene::useInhibitorArcTool);
	if (mRequestedObjects.count() == 0) {
		mCurrentRequest = CR_ITEM;
		mController->showInfoMessage(tr("Choose a place."), MessageWidget::Forever);
		mResquestCallBack = &PetriNetScene::useInhibitorArcTool;
	} else if (mRequestedObjects.count() == 1 && mCurrentMouseButton == Qt::LeftButton) {
		QGraphicsItem* item = mRequestedObjects.last().value<QGraphicsItem*>();
		if (item->type() == GraphicPlace::Type) {
			mController->showInfoMessage(tr("Choose a transition."), MessageWidget::Forever);
			mArcLine->setLine(QLineF(item->pos(), item->pos()));
			mArcLine->show();
		} else {
			mController->showInfoMessage(tr("You must choose a place, not this!"), MessageWidget::Forever);
			mRequestedObjects.clear();
			return;
		}
	} else if (mRequestedObjects.count() == 2 && mCurrentMouseButton == Qt::LeftButton) {
		QGraphicsItem* second = mRequestedObjects.takeLast().value<QGraphicsItem*>();
		QGraphicsItem* first = mRequestedObjects.takeLast().value<QGraphicsItem*>();
		if (second->type() != GraphicTransition::Type) {
			mController->showErrorMessage(tr("Inhibitor arcs must be from a place to a transition."));
		} else {
			Place* item1 = static_cast<Place*>(static_cast<GraphicItem*>(first)->item());
			Transition* item2 = static_cast<Transition*>(static_cast<GraphicItem*>(second)->item());
			mController->addInhibitorArc(item1, item2);
		}
		mArcLine->hide();
		useInhibitorArcTool();
	} else
		mRequestedObjects.removeLast();
}

void PetriNetScene::useTokenTool() {
	cleanTool(&PetriNetScene::useTokenTool);
	if (mRequestedObjects.count() == 0) {
		mCurrentRequest = CR_ITEM;
		mController->showInfoMessage(tr("Use the right mouse button to add a token and the left mouse button to remove a token."), MessageWidget::Forever);
		mResquestCallBack = &PetriNetScene::useTokenTool;
	} else if (mRequestedObjects.count() == 1) {
		QGraphicsItem* gitem = mRequestedObjects.takeLast().value<QGraphicsItem*>();
		if (gitem->type() == GraphicPlace::Type) {
			GraphicPlace* gplace = static_cast<GraphicPlace*>(gitem);
			switch (mCurrentMouseButton) {
				case Qt::LeftButton:
					mController->addToken(gplace->place());
					break;
				case Qt::RightButton:
					mController->removeToken(gplace->place());
					break;
				default:
					break;
			}
		}
	}
}

void PetriNetScene::useFireTransitionTool() {
	cleanTool(&PetriNetScene::useFireTransitionTool);
	if (mRequestedObjects.count() == 0) {
		mCurrentRequest = CR_ITEM;
		mController->showInfoMessage(tr("Select a transition to fire."), MessageWidget::Forever);
		mResquestCallBack = &PetriNetScene::useFireTransitionTool;
	} else if (mRequestedObjects.count() == 1) {
		QGraphicsItem* gitem = mRequestedObjects.takeLast().value<QGraphicsItem*>();
		if (gitem->type() != GraphicTransition::Type)
			mController->showErrorMessage(tr("You need to choose a transition!"));
		else {
			mController->fireTransition(static_cast<GraphicTransition*>(gitem)->transition());
		}
	}
}

void PetriNetScene::cleanTool(const MethodPointer& method) {
	if (method != mResquestCallBack) {
		mArcLine->hide();
		mRequestedObjects.clear();
	}
}

//#include "petrinetscene.moc"
