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

#include "cmdcreatearc.h"
#include "petrinet.h"
#include "arc.h"
#include "place.h"
#include "transition.h"

CmdCreateArc::CmdCreateArc(PetriNet* petriNet, Place* from, Transition* to) {
	init(petriNet, from, to);
	mInvertArgs = false;
}

CmdCreateArc::CmdCreateArc(PetriNet* petriNet, Transition* from, Place* to) {
	init(petriNet, to, from);
	mInvertArgs = true;
}

void CmdCreateArc::init(PetriNet* petriNet, Place* p, Transition* t) {
	mPetriNet = petriNet;
	mPlace = p->id();
	mTransition = t->id();
	setText(tr("Create arc"));
}

void CmdCreateArc::undo() {
	QLinkedList<Item*> items = mPetriNet->removeItem(mArc);
	Q_ASSERT(items.count() == 1);
	qDeleteAll(items);
}

void CmdCreateArc::redo() {
	Transition* t = dynamic_cast<Transition*>(mPetriNet->item(mTransition));
	Place* p = dynamic_cast<Place*>(mPetriNet->item(mPlace));
	if (!p || !t) {
		qWarning("this should never happen! " __FILE__);
	} else {
		Arc* a =  mInvertArgs ?	mPetriNet->createArc(t, p, mArc) :  mPetriNet->createArc(p, t, mArc);
		mArc = a->id();
	}
}

