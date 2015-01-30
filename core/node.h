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

#ifndef NODE_H
#define NODE_H

#include "item.h"
#include <QSet>
#include <QDomElement>

class AbstractArc;

/// A collection of arcs.
typedef QSet<AbstractArc*> ArcCollection;

/**
* A node in the graph
* @ingroup core
*/
class Node : public Item {
public:
	/// Constructs a new Item at the position \p pos.
	Node(PetriNet* pn, const QPointF& pos, ItemId id) : Item(pn, id), mPos(pos), mNumber(-1) {}
	/// Returns the node name.
	const QString& name() const { return mName; }
	/// Sets the node name.
	void setName(const QString& name);
	/// Returns node number.
	int number() const { return mNumber; }
	/// Sets node number.
	void setNumber(int number);
	/// Increments node number.
	int incrementNumber();
	/// Decrements node number, result must be nonnegative.
	int decrementNumber();
	/// Returns the current item position.
	const QPointF& pos() const { return mPos; }
	/// Sets the item position.
	void setPos(const QPointF& pos);
	/// Adds an arc to this node.
	void addInputArc(AbstractArc* arc);
	/// Adds an arc from this node.
	void addOutputArc(AbstractArc* arc);
	/// Removes an input arc.
	void removeInputArc(AbstractArc* arc);
	/// Removes an output arc.
	void removeOutputArc(AbstractArc* arc);
	/// Returns number of arcs to this node.
	int numInputArcs() const { return mInput.count(); }
	/// Returns number of arcs from this node.
	int numOutputArcs() const { return mOutput.count(); }
	/// Returns arc from this node to node \p to, NULL if not found.
	AbstractArc* findArcTo(Node* to);
	/// Returns arc to this node from node\p from, NULL if not found.
	AbstractArc* findArcFrom(Node* from);

	/// Returns all arcs to this node.
	const ArcCollection& inputArcs() { return mInput; }
	/// Returns all arcs from this node.
	const ArcCollection& outputArcs() { return mOutput; }

	void save(QXmlStreamWriter& out);
	static void load(Node* node, QDomElement elem);

protected:
	void beforeAdd() { }
	QLinkedList<Item*> beforeDelete();

private:
	bool updateDefaultName();
	QString mName;
	QPointF mPos;
	ArcCollection mInput;
	ArcCollection mOutput;
	int mNumber;
};

#endif
