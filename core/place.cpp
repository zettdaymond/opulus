/***************************************************************************
 *   Copyright (C) 2007 by Hugo Parente Lima <hugo.pl@gmail.com>           *
 *   Copyright (C) 2007 by Adauto Trigueiro <adautofilho@gmail.com>        *
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

#include "place.h"
#include "arc.h"
#include "transition.h"
#include "exceptions.h"
#include "petrinet.h"

#include <QXmlStreamWriter>
#include <QDomElement>

Place::Place(PetriNet* pn, const QPointF& pos, const ItemId& id, const QString &name) : Node(pn, pos, id, name), mCapacity(INFINITY) {
}

uint Place::numTokens() const {
	return petriNet()->currentMarking().tokensOf(this);
}

void Place::addTokens(uint num) {
	uint tokens = numTokens();
	if (tokens == Marking::OMEGA)
		return;
	setNumTokens(tokens+num);
}

void Place::setNumTokens(uint num) {
	if (num > mCapacity)
		throw PlaceCapacityExceedException(mCapacity);
	else {
		petriNet()->currentMarking().setNumTokens(this, num);
		updateTransitionsStatus();
		notifyModification();
	}
}

void Place::setCapacity(uint capacity) {
	uint tokens = numTokens();
	if (capacity < tokens)
		throw Exception(tr("You need to remove %1 tokens before set the capacity to %2").arg(tokens-capacity).arg(capacity));
	mCapacity = capacity;
	updateTransitionsStatus();
	notifyModification();
}

void Place::removeTokens(uint num) {
	uint tokens = numTokens();
	if (tokens < num)
		throw NoMoreTokensException();
	if (tokens != Marking::OMEGA)
		setNumTokens(tokens - num);
	notifyModification();
}

void Place::save(QXmlStreamWriter& out){
	out.writeEmptyElement("place");
	Node::save(out);
	out.writeAttribute("tokens", QString::number(numTokens()));
	if (mCapacity != Place::INFINITY)
		out.writeAttribute("capacity", QString::number(mCapacity));
}

void Place::load(PetriNet* pn, const QDomElement& elem) {
	Place* place = pn->createPlace(QPointF(), elem.attribute("id"));
	Node::load(place, elem);
	place->setNumTokens(elem.attribute("tokens", "0").toUInt());
	place->setCapacity(elem.attribute("capacity", QString::number(Place::INFINITY)).toUInt());
}

void Place::updateTransitionsStatus() {
	foreach(AbstractArc* arc, inputArcs()) {
		Transition* t = static_cast<Transition*>(arc->from());
		t->updateStatus();
	}
	foreach(AbstractArc* arc, outputArcs()) {
		Transition* t = static_cast<Transition*>(arc->to());
		t->updateStatus();
	}
}
