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

#include "arc.h"
#include "place.h"
#include "transition.h"
#include "petrinet.h"
#include "exceptions.h"

#include <QXmlStreamWriter>

Arc::Arc(PetriNet* pn, Place* from, Transition* to, const ItemId& id) : AbstractArc(pn, from, to, id) {
	to->updateStatus();
}

Arc::Arc(PetriNet* pn, Transition* from, Place* to, const ItemId& id) : AbstractArc(pn, from, to, id) {
	from->updateStatus();
}

bool Arc::canFireTransition() const {
	if (mDirection == At_ToTransition)
		return static_cast<Place*>(from())->numTokens() >= weight();
	else {
		Place* place = static_cast<Place*>(to());
		if (place->hasCapacity()) {
			uint numTokens = place->numTokens();
			if (numTokens != Marking::OMEGA && numTokens + weight() > place->capacity())
				return false;
		}
	}
	return true;	
}

void Arc::fire() {
	if (mDirection == At_ToTransition)
		static_cast<Place*>(from())->removeTokens(weight());
	else
		static_cast<Place*>(to())->addTokens(weight());
}

void Arc::setWeight(uint weight) {
	AbstractArc::setWeight(weight);
	petriNet()->notifyAboutArcWeightChanged(this);
}

void Arc::save(QXmlStreamWriter& out) {
	out.writeStartElement("arc");
	AbstractArc::save(out);
	out.writeEndElement();
}

void Arc::load(PetriNet* pn, const QDomElement& elem) {
	QString id = elem.attribute("id");

	QString sourceId = elem.attribute("source");
	QString targetId = elem.attribute("target");
	Item* source = pn->item(sourceId);
	Item* target = pn->item(targetId);

	Arc* arc;
	if (source->isA<Place>() && target->isA<Transition>()) {
		arc = pn->createArc(static_cast<Place*>(source), static_cast<Transition*>(target), id);
	} else if (source->isA<Transition>() && target->isA<Place>()) {
		arc = pn->createArc(static_cast<Transition*>(source), static_cast<Place*>(target), id);
	} else
		throw BadFileException();

	AbstractArc::load(arc, elem);
}

