/***************************************************************************
 *   Copyright (C) 2007 by Hugo Parente Lima <hugo.pl@gmail.com>           *
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

#include <QXmlStreamWriter>
#include "abstractarc.h"
#include "node.h"
#include "place.h"
#include "transition.h"

AbstractArc::AbstractArc(PetriNet* pn, Place* from, Transition* to, const ItemId& id)
	: Item(pn, id), mDirection(At_ToTransition), mFrom(from), mTo(to), mWeight(1) {
	mTo->addInputArc(this);
	mFrom->addOutputArc(this);
}

AbstractArc::AbstractArc(PetriNet* pn, Transition* from, Place* to, const ItemId& id)
	: Item(pn, id), mDirection(At_FromTransition), mFrom(from), mTo(to), mWeight(1) {
	mTo->addInputArc(this);
	mFrom->addOutputArc(this);
}

AbstractArc::~AbstractArc() {
	qDeleteAll(mIntermediatePoints);
}

QPointF* AbstractArc::addIntermediatePoint(const QPointF& point, int segmentIndex) {
	QPointF* p = new QPointF(point);
	mIntermediatePoints.insert(segmentIndex, p);
	return p;
}

void AbstractArc::removeIntermediatePoint(QPointF* point) {
	int idx = mIntermediatePoints.indexOf(point);
	if (idx != -1)
		delete mIntermediatePoints.takeAt(idx);
}

void AbstractArc::setWeight(uint weight) {
	mWeight = weight;
	updateTransitionStatus();
	notifyModification();
}

void AbstractArc::beforeAdd() {
	mTo->addInputArc(this);
	mFrom->addOutputArc(this);
	updateTransitionStatus();
}

QLinkedList<Item*> AbstractArc::beforeDelete() {
	mTo->removeInputArc(this);
	mFrom->removeOutputArc(this);
	updateTransitionStatus();
	return QLinkedList<Item*>() << this;
}

void AbstractArc::updateTransitionStatus() {
	Transition* t = static_cast<Transition*>(mDirection == At_ToTransition ? mTo : mFrom);
	t->updateStatus();
}

void AbstractArc::save(QXmlStreamWriter& out) {
	out.writeAttribute("id", id().toString());
	out.writeAttribute("source", from()->id().toString());
	out.writeAttribute("target", to()->id().toString());
	out.writeAttribute("weight", QString::number(weight()));

	// arc paths
	foreach(QPointF* p, mIntermediatePoints) {
		out.writeEmptyElement("arcpath");
		out.writeAttribute("x", QString::number(p->x()));
		out.writeAttribute("y", QString::number(p->y()));
	}
}

void AbstractArc::load(AbstractArc* arc, QDomElement elem) {
	arc->setWeight(elem.attribute("weight").toUInt());

	QDomNodeList arcpaths = elem.elementsByTagName("arcpath");
	for (int i = 0; i < arcpaths.count(); ++i) {
		QDomElement elem = arcpaths.item(i).toElement();
		QPointF* point = new QPointF;
		point->setX(elem.attribute("x").toDouble());
		point->setY(elem.attribute("y").toDouble());
		arc->mIntermediatePoints << point;
	}
}

