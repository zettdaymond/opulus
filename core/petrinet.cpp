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

#include "petrinet.h"
#include "place.h"
#include "transition.h"
#include "arc.h"
#include "inhibitorarc.h"
#include "exceptions.h"
#include "simulation.h"

#include <cstdlib>
#include <ctime>
#include <QXmlStreamWriter>
#include <QDomDocument>
#include <QDomElement>

PetriNet::PetriNet(QObject* parent) : QObject(parent), mNextId(0), mModifyNotificationEnabled(true) {
	::srand(time(0));
}

PetriNet::~PetriNet() {
	qDeleteAll(mItems);
}

void PetriNet::notifyAboutItemModification(Item* item) {
	Q_ASSERT(item->petriNet() == this);
	if (mModifyNotificationEnabled)
		emit itemModified(item);
}

Place* PetriNet::createPlace(const QPointF& pos, const ItemId& id) {
	Place* place = new Place(this, pos, id.isValid() ? id : nextId(), "P");
	place->setNumber(mPlaces.count());

	addItem(place); // order of these two calls matters, no idea why :D
	mPlaces.insert(place);
	return place;
}

Transition* PetriNet::createTransition(const QPointF& pos, const ItemId& id) {
	Transition* t = new Transition(this, pos, id.isValid() ? id : nextId(), "T");
	t->setNumber(mTransitions.size());

	addItem(t);  // order of these two calls matters, no idea why :D
	mTransitions.insert(t);
	return t;
}

Arc* PetriNet::createArc(Place* place, Transition* transition, const ItemId& id) {
	// NOTE: is this logic correct?
	// If arc between items already exists, increase the weight only.
	AbstractArc* orig = NULL;
	foreach (AbstractArc* a, place->outputArcs()) {
		if(a->to()->isA<Transition>() && static_cast<Transition*>(a->to()) == transition) {
			a->setWeight(a->weight()+1);
			orig = a;
		}
	}
	if(orig)
		return static_cast<Arc*>(orig);

	Arc* arc = new Arc(this, place, transition, id.isValid() ? id : nextId());
	addItem(arc);
	return arc;
}

Arc* PetriNet::createArc(Transition* transition, Place* place, const ItemId& id) {
	// NOTE: is this logic correct?
	// If arc between items already exists, increase the weight only.
	AbstractArc* orig = NULL;
	foreach (AbstractArc* a, transition->outputArcs()) {
		if(a->to()->isA<Place>() && static_cast<Place*>(a->to()) == place) {
			a->setWeight(a->weight()+1);
			orig = a;
		}
	}
	if(orig)
		return static_cast<Arc*>(orig);


	Arc* arc = new Arc(this, transition, place, id.isValid() ? id : nextId());
	addItem(arc);
	return arc;
}

InhibitorArc* PetriNet::createInhibitorArc(Place* place, Transition* transition, const ItemId& id) {
	InhibitorArc* arc = new InhibitorArc(this, place, transition, id.isValid() ? id : nextId());
	addItem(arc);
	return arc;
}

Transition* PetriNet::findTransitionWithNumber(int number) {
	if(number < 0)
		return NULL;

	foreach (Transition* t, mTransitions) {
		if(t->number() == number)
			return t;
	}

	return NULL;
}

Place* PetriNet::findPlaceWithNumber(int number) {
	if(number < 0)
		return NULL;

	foreach (Place* p, mPlaces) {
		if(p->number() == number)
			return p;
	}

	return NULL;
}


void PetriNet::addItem(Item* item) {
	if (mItems.contains(item->id()))
		throw DuplicatedIdException(item->id());
	mItems.insert(item->id(), item);
	item->beforeAdd();
	if (item->isA<Place>()) {
		Place* placeitem = static_cast<Place*>(item);
		// correct other places' numbers
		bool need_to_correct = false;
		foreach (Place* p, mPlaces) {
			if(p->number() == placeitem->number()) {
				need_to_correct = true;
				break;
			}
		}
		if(need_to_correct) {
			foreach (Place* p, mPlaces) {
				if(p->number() >= placeitem->number())
					p->incrementNumber();
			}
		}
		mPlaces.insert(placeitem);
		emit placeCreated(placeitem);
	}
	else if (item->isA<Arc>())
		emit arcCreated(static_cast<Arc*>(item));
	else if (item->isA<Transition>()) {
		Transition* transitem = static_cast<Transition*>(item);
		// correct other transitions' numbers
		bool need_to_correct = false;
		foreach (Transition* tr, mTransitions) {
			if(tr->number() == transitem->number()) {
				need_to_correct = true;
				break;
			}
		}
		if(need_to_correct) {
			foreach (Transition* tr, mTransitions) {
				if(tr->number() >= transitem->number()) {
					tr->incrementNumber();
				}
			}
		}
		mTransitions.insert(static_cast<Transition*>(item));
		emit transitionCreated(static_cast<Transition*>(item));
	}
	else if (item->isA<InhibitorArc>()) {
		emit inhibitorArcCreated(static_cast<InhibitorArc*>(item));
	}
}

ItemId PetriNet::nextId() {
	ItemId id;
	do {
		id = QString::number(++mNextId);
	} while(mItems.contains(id));
	return id;
}

QLinkedList<Item*> PetriNet::removeItem(Item* item) {
	if (!mItems.remove(item->id()))
		return QLinkedList<Item*>(); // FIXME: Throw an exception?
	if (item->isA<Transition>()) {
		Transition* transitem = static_cast<Transition*>(item);
		mTransitions.remove(transitem);
		removeActiveTransition(transitem);
		// correct other transitions' numbers
		foreach (Transition* t, mTransitions) {
			if(t->number() == transitem->number())
				throw DuplicatedNumberException(t->number(), tr("transition"));
			if(t->number() > transitem->number()) {
				t->decrementNumber();
			}
		}
	} else if (item->isA<Place>()) {
		Place* placeitem = static_cast<Place*>(item);
		mPlaces.remove(placeitem);
		// correct other places' numbers
		foreach (Place*p, mPlaces) {
			if(p->number() == placeitem->number())
				throw DuplicatedNumberException(p->number(), tr("place"));
			if(p->number() > placeitem->number()) {
				p->decrementNumber();
			}
		}
	}
	QLinkedList<Item*> res = item->beforeDelete();
	emit itemRemoved(item);
	return res;
}

QLinkedList<Item*> PetriNet::removeItem(const ItemId& id) {
	Item* it = mItems.value(id);
	return it ? removeItem(it) : QLinkedList<Item*>();
}

bool PetriNet::contains(Item* item) const {
	return mItems.value(item->id()) == item;
}

Item* PetriNet::item(const ItemId& id) const {
	Item* it = mItems.value(id);
	if (!it)
		throw ItemNotFoundException(id);
	return it;
}

void PetriNet::setCurrentMarking(const Marking& marking) {
	mCurrentMarking = marking;
	foreach(Transition* t, mTransitions)
		t->updateStatus();
	if (isModifyNotificationEnabled())
		foreach (Place* place, mPlaces)
			place->notifyModification();
}

void PetriNet::save(QIODevice* device){
	device->open(QIODevice::WriteOnly | QIODevice::Text);
	QXmlStreamWriter out(device);
	out.setAutoFormatting(true);
	out.writeStartDocument();
	out.writeStartElement("ptnet");
	foreach (Item* item, mItems)
		item->save(out);
	out.writeEndDocument();
}

void PetriNet::load(QIODevice* device) {
	device->open(QIODevice::ReadOnly | QIODevice::Text);
	QDomDocument doc;
	QString errorMsg;
	int errorLine, errorCol;
	if (!doc.setContent(device, &errorMsg, &errorLine, &errorCol))
		throw IOException(errorLine, errorCol, errorMsg);

	// create all places
	loadItem<Place>(doc, "place");
	// create all transitions
	loadItem<Transition>(doc, "transition");
	// create all arcs
	loadItem<Arc>(doc, "arc");
	// create all arcs
	loadItem<Arc>(doc, "inhibitorarc");
	// force a redraw
	setCurrentMarking(currentMarking());
}

template <typename T>
void PetriNet::loadItem(QDomDocument& doc, const QString& tagName) {
	QDomNodeList elems = doc.elementsByTagName(tagName);
	for (int i = 0; i < elems.count(); ++i) {
		if (elems.at(i).isElement())
			T::load(this, elems.at(i).toElement());
	}
}

void PetriNet::addActiveTransition(Transition* t) {
	mActiveTransitions.insert(t);
}

void PetriNet::removeActiveTransition(Transition* t) {
	mActiveTransitions.remove(t);
}

void PetriNet::fireRandomTransition() {
	if (!mActiveTransitions.size())
		throw NoActiveTransitionsException();
	int selectedTransition = rand() % mActiveTransitions.size();
	Q_ASSERT(selectedTransition < mActiveTransitions.size());
	// hmmm... slow?
	mActiveTransitions.values().at(selectedTransition)->fire();
}

#include "petrinet.moc"
