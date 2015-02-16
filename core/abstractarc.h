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
#ifndef ABSTRACTARC_H
#define ABSTRACTARC_H

#include <QList>
#include "item.h"

class QDomElement;
class Node;
class Place;
class Transition;

/// A list of intermediate points
typedef QList<QPointF*> IntermediatePointList;

class AbstractArc : public Item {
public:
    virtual ~AbstractArc();

	Node* from() const { return mFrom; }
	Node* to() const { return mTo; }
	QPointF* addIntermediatePoint(const QPointF& point, int segmentIndex);
	void removeIntermediatePoint(QPointF* point);
	const IntermediatePointList& intermediatePoints() const { return mIntermediatePoints; }
	virtual void setWeight(uint weight);
	uint weight() const { return mWeight; }
	
	void updateTransitionStatus();
	virtual bool canFireTransition() const = 0;
	virtual void fire() = 0;

	void save(QXmlStreamWriter& out);
	static void load(AbstractArc* arc, QDomElement elem);

protected:	
	AbstractArc(PetriNet* pn, Transition* from, Place* to, const ItemId& id);
	AbstractArc(PetriNet* pn, Place* from, Transition* to, const ItemId& id);
	
	void beforeAdd();
	QLinkedList<Item*> beforeDelete();

	enum ArcDirection {
		At_ToTransition,
		At_FromTransition
	} mDirection;

private:
	

	Node* mFrom;
	Node* mTo;
	uint mWeight;

	IntermediatePointList mIntermediatePoints;

};

#endif
