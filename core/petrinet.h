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

#ifndef PETRINET_H
#define PETRINET_H

#include <QObject>
#include <QList>
#include <QLinkedList>
#include <QHash>
#include <QSet>
#include <QPointF>
#include <QIODevice>
#include "item.h"
#include "marking.h"

class QPointF;
class QDomDocument;

class Item;
class Place;
class Transition;
class Arc;
class InhibitorArc;

/**
* GUI independent core lib to simulate petri nets.
* @defgroup core Core
*/

/**
* A Petri net.
* @ingroup core
*/
class PetriNet : public QObject {
Q_OBJECT
public:
	/// Constructs a new petri net.
	explicit PetriNet(QObject* parent = 0);
	/// Destroy this petri net.
	~PetriNet();

	/// Create a new place in the petri net at position \p pos.
	Place* createPlace(const QPointF& pos = QPointF(), const ItemId& = ItemId());
	/// Create a new transition in the petri net at position \p pos.
	Transition* createTransition(const QPointF& pos = QPointF(), const ItemId& = ItemId());
	/// Create an arc from a \p place to a \p transition.
	Arc* createArc(Place* place, Transition* transition, const ItemId& = ItemId());
	/// Create an arc from a \p transition to a \p place.
	Arc* createArc(Transition* transition, Place* place, const ItemId& = ItemId());
	/// Create an inhibitor arc from a \p transition to a \p place.
	InhibitorArc* createInhibitorArc(Place* place, Transition* transition, const ItemId& = ItemId());
	/**
	* Remove an item and all it's dependences from the petri net.
	* Places or transitions will have their numbers corrected, if necessary.
	* @return A list of all items removed. \p item will be the last item in the list.
	*/
	QLinkedList<Item*> removeItem(Item* item);
	QLinkedList<Item*> removeItem(const ItemId& id);
	/// Check if the \p item belong to this petri net.
	bool contains(Item* item) const;
	/// Returns the item with id \p id.
	Item* item(const ItemId& id) const;
	/// emits itemModified signal if ModifyNotification is enabled.
	void notifyAboutItemModification(Item* item);
	/// emits arcWeightChanged signal if ModifyNotification is enabled.
	void notifyAboutArcWeightChanged(Arc* arc);
	/// Returns all petri net transitions.
	const QSet<Transition*>& transitions() const { return mTransitions; }
	/// Returns the number of petri net transitions.
	int transitionCount() const { return mTransitions.count(); }
	/// Returns transition with number \p number, or nullptr if not found.
	Transition* findTransitionWithNumber(int number);

	/// Returns all petri net places.
	const QSet<Place*>& places() const { return mPlaces; }
	/// Returns the number of petri net places.
	int placeCount() const { return mPlaces.count(); }
	/// Returns place with number \p number, or nullptr if not found.
	Place* findPlaceWithNumber(int number);

	/// Set the current PetriNet marking.
	void setCurrentMarking(const Marking& marking);
	/// Returns the current PetriNet marking.
	Marking& currentMarking() { return mCurrentMarking; }
	/// Returns the current PetriNet marking.
	const Marking& currentMarking() const { return mCurrentMarking; }

	/// Save petri net on \p device.
	void save(QIODevice* device);
	/**
	* Clear this petri net and load a new petri net from
	* the XML description on \p device.
	*/
	void load(QIODevice* device);

	void setModifyNotificationEnabled(bool value) {
		mModifyNotificationEnabled = value;
	}

	bool isModifyNotificationEnabled() const { return mModifyNotificationEnabled; }
	/// Add a transition to the active transitions list.
	void addActiveTransition(Transition* t);
	/// Remove a transition from the active transitions list.
	void removeActiveTransition(Transition* t);
	/// Returns all active transitions.
	const QSet<Transition*>& activeTransitions() const { return mActiveTransitions; }
	/// Returns the number of active transitions.
	int activeTransitionsCount() const { return mActiveTransitions.count(); }
	/// Fire a random transition.
	void fireRandomTransition();

	/// Add an item to the petri net. Places or transitions will have their numbers corrected, if necessary.
	void addItem(Item* item);
	/// Returns the number of items in this petri net.
	int itemCount() const { return mItems.count(); }

signals:
	void placeCreated(Place* place);
	void transitionCreated(Transition* transition);
	void arcCreated(Arc* arc);
	void arcWeightChanged(Arc* arc);
	void inhibitorArcCreated(InhibitorArc* arc);
	void itemModified(Item* item);
	void itemRemoved(Item* item);

private:
	QSet<Transition*> mTransitions;
	QSet<Place*> mPlaces;
	QHash<ItemId, Item*> mItems;
	uint mNextId;
	Marking mCurrentMarking;
	bool mModifyNotificationEnabled;
	QSet<Transition*> mActiveTransitions;

	ItemId nextId();
	template <typename T>
	void loadItem(QDomDocument& doc, const QString& tagName);

};

#endif
