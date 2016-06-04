/*
* Copyright (C) 2016 Zett Daymond <zettday@gmail.com>
*
* This software may be modified and distributed under the terms
* of the MIT license.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/

#include "cmdcreatearcwithweight.h"
#include "petrinet.h"
#include "arc.h"
#include "place.h"
#include "transition.h"

CmdCreateArcWithWeight::CmdCreateArcWithWeight(PetriNet* petriNet, Place* from, Transition* to, uint weight) {
    init(petriNet, from, to, weight);
	mInvertArgs = false;
}

CmdCreateArcWithWeight::CmdCreateArcWithWeight(PetriNet* petriNet, Transition* from, Place* to, uint weight) {
    init(petriNet, to, from, weight);
	mInvertArgs = true;
}

void CmdCreateArcWithWeight::init(PetriNet* petriNet, Place* p, Transition* t, uint weight) {
	mPetriNet = petriNet;
	mPlace = p->id();
	mTransition = t->id();
    mWeight = weight;
    setText(tr("Create arc with weight %1").arg(weight));
}

void CmdCreateArcWithWeight::undo() {
	QLinkedList<Item*> items = mPetriNet->removeItem(mArc);
	Q_ASSERT(items.count() == 1);
	qDeleteAll(items);
}

void CmdCreateArcWithWeight::redo() {
	Transition* t = dynamic_cast<Transition*>(mPetriNet->item(mTransition));
	Place* p = dynamic_cast<Place*>(mPetriNet->item(mPlace));
	if (!p || !t) {
		qWarning("this should never happen! " __FILE__);
	} else {
		Arc* a =  mInvertArgs ?	mPetriNet->createArc(t, p, mArc) :  mPetriNet->createArc(p, t, mArc);
		mArc = a->id();
        a->setWeight(mWeight);
	}
}

