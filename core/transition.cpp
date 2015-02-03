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

#include "transition.h"
#include "arc.h"
#include "place.h"
#include "marking.h"
#include "petrinet.h"
#include "exceptions.h"

#include <QXmlStreamWriter>
#include <QDebug>

Transition::Transition(PetriNet* pn, const QPointF& pos, const ItemId& id, const QString &name) : Node(pn, pos, id, name), mRotation(0.0), mCanFire(true) {
	petriNet()->addActiveTransition(this);
}

bool Transition::canFire() {
	return mCanFire;
}

void Transition::fire() {
	if (!canFire())
		throw CantFireTransitionException();
	foreach(AbstractArc* arc, inputArcs()) {
		arc->fire();
	}
	foreach(AbstractArc* arc, outputArcs()) {
		arc->fire();
	}
	if (!inputArcs().count())
		updateStatus();
}

void Transition::setRotation(double rotation) {
	mRotation = rotation;
	notifyModification();
}

void Transition::save(QXmlStreamWriter& out){
	out.writeEmptyElement("transition");
	Node::save(out);
	out.writeAttribute("rotation", QString::number(rotation()));
}


void Transition::updateStatus() {
	bool oldValue = mCanFire;

	mCanFire = false;
	// pre conditions
	foreach(AbstractArc* arc, inputArcs()) {
		if (!arc->canFireTransition()) {
			mCanFire = false;
			goto FINISHED;
		}	
	}
	
	// post conditions
	foreach(AbstractArc* arc, outputArcs()) {
		if (!arc->canFireTransition()) {
			mCanFire = false;
			goto FINISHED;
		}	
	}
	// if it pass all tests... the transition can fire.
	mCanFire = true;

	FINISHED:
	if (oldValue != mCanFire) {
		if (mCanFire)
			petriNet()->addActiveTransition(this);
		else
			petriNet()->removeActiveTransition(this);
		notifyModification();
	}
}

void Transition::load(PetriNet* pn, const QDomElement& elem) {
	Transition* transition = new Transition(pn, QPointF(), elem.attribute("id"), QString());
	Node::load(transition, elem);
	transition->setRotation(elem.attribute("rotation").toDouble());
	pn->addItem(transition);
}

