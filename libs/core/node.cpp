/***************************************************************************
 *   Copyright (C) 2007 by Hugo Parente Lima <hugo.pl@gmail.com>           *
 *                 2015-2016 by Zett Daymond <zettday@gmail.com>           *
 *                 2015-2016 by Cat          <captainslowpoke@gmail.com>   *
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

#include "node.h"
#include "arc.h"
#include "petrinet.h"

#include <QXmlStreamWriter>

void Node::setName(const QString& name) {
	mName = name;
	mHasCustomName = true;
	notifyModification();
}

void Node::updateDefaultName() {
	if(!mHasCustomName && mName.size() > 0) {
		mName = mName[0] + QString::number(mNumber);
	}
}

void Node::setNumber(int number) {
	Q_ASSERT(number >= 0);
	mNumber = number;
	updateDefaultName();
	notifyModification();
}

int Node::incrementNumber() {
	++mNumber;
	updateDefaultName();
	notifyModification();
	return mNumber;
}

int Node::decrementNumber() {
	Q_ASSERT(mNumber > 0);
	--mNumber;
	updateDefaultName();
	notifyModification();
	return mNumber;
}

void Node::setPos(const QPointF& pos) {
	mPos = pos;
	notifyModification();
}

void Node::addInputArc(AbstractArc* arc) {
	mInput.insert(arc);
}

void Node::addOutputArc(AbstractArc* arc) {
	mOutput.insert(arc);
}

void Node::removeInputArc(AbstractArc* arc) {
	mInput.remove(arc);
}

void Node::removeOutputArc(AbstractArc* arc) {
	mOutput.remove(arc);
}

AbstractArc *Node::findArcTo(Node *to) {
	foreach (AbstractArc* arc, mOutput) {
		if(arc->to() == to)
			return arc;
	}
    return nullptr;
}

AbstractArc *Node::findArcFrom(Node *from) {
	foreach (AbstractArc* arc, mInput) {
		if(arc->from() == from)
			return arc;
	}
    return nullptr;
}

std::list<Item*> Node::beforeDelete() {
    std::list<Item*> arcs;

    foreach(AbstractArc* arc, mInput) {
        auto arcList = petriNet()->removeItem(arc);
        arcs.insert(arcs.end(), arcList.begin(), arcList.end());
    }
    foreach(AbstractArc* arc, mOutput) {
        auto arcList = petriNet()->removeItem(arc);
        arcs.insert(arcs.end(), arcList.begin(), arcList.end());
    }
    arcs.push_back(this);
    return arcs;
}

void Node::save(QXmlStreamWriter& out) {
	out.writeAttribute("id", id().toString());
	out.writeAttribute("name", name());
	out.writeAttribute("number",QString::number(mNumber));
	out.writeAttribute("x", QString::number(pos().x()));
	out.writeAttribute("y", QString::number(pos().y()));
}

void Node::load(Node* node, QDomElement elem) {
	QPointF position;
	position.setX(elem.attribute("x").toDouble());
	position.setY(elem.attribute("y").toDouble());
	node->setPos(position);
	node->mName = elem.attribute("name");
	node->setNumber(elem.attribute("number").toInt());
}
