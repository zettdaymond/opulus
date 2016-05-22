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

#include "graphicnode.h"
#include "petrinetscene.h"
#include "arc.h"
#include "node.h"
#include <QGraphicsSimpleTextItem>

GraphicNode::GraphicNode(Node* node) : mNode(node) {
	setFlags(ItemIsMovable | ItemIsSelectable);
	setPos(node->pos());
	mLabel = new QGraphicsSimpleTextItem(mNode->name(), this);
	mLabel->setPos(-20, 20);
}

Item* GraphicNode::item() const {
	return mNode;
}

void GraphicNode::itemChanged() {
	if(mNode->hasCustomName())
		mLabel->setText(mNode->name() + " ("+QString::number(mNode->number()) +")");
	else	mLabel->setText(mNode->name());

	setPos(mNode->pos());
	updateArcs();
	update();
}

void GraphicNode::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
	if (pos() != mNode->pos()) {
		PetriNetScene* sc = static_cast<PetriNetScene*>(scene());

		// move all selected nodes
		//If there is only one node to move,
		//then use specialized method on efficiency reason.
		auto selectedItems = sc->selectedItems();
		if (selectedItems.size() == 1) {
			GraphicNode* gnode = dynamic_cast<GraphicNode*>(selectedItems[0]);
			if (gnode) {
				sc->moveNode(gnode->mNode, gnode->pos());
			}
		} else {
			QVector<Node*> nodes;
			nodes.reserve(selectedItems.size());
			QVector<QPointF> poses;
			poses.reserve(selectedItems.size());

			foreach(QGraphicsItem* i, selectedItems) {
				GraphicNode* gnode = dynamic_cast<GraphicNode*>(i);
				if (gnode) {
					nodes.push_back(gnode->mNode);
					poses.push_back(gnode->pos());
				}
			}

			if (nodes.size() != 0) {
				sc->moveNodeGroup(nodes, poses);
			}
		}
	}
	QGraphicsItem::mouseReleaseEvent(event);
}

void GraphicNode::updateArcs() {
	PetriNetScene* pnScene = static_cast<PetriNetScene*>(scene());
	foreach (AbstractArc* arc, mNode->inputArcs())
		pnScene->updateItem(arc);
	foreach (AbstractArc* arc, mNode->outputArcs())
		pnScene->updateItem(arc);
}

QVariant GraphicNode::itemChange(GraphicsItemChange change, const QVariant & value) {
	if (change == ItemPositionHasChanged && scene())
		updateArcs();
	return value;
}


