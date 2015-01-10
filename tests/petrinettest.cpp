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

#include <QTest>
#include <QSignalSpy>
#include "petrinettest.h"
#include "petrinet.h"
#include "simulation.h"
#include "place.h"
#include "arc.h"
#include "transition.h"
#include "marking.h"
#include "exceptions.h"

void PetriNetTest::init() {
	qRegisterMetaType<Item*>("Item*");
	mPn = new PetriNet;
}
void PetriNetTest::cleanup() {
	delete mPn;
}

void PetriNetTest::testNoActiveTransitions() {
	mPn->createArc(mPn->createPlace(), mPn->createTransition());
	try {
		Simulation s(mPn);
		s.fireRandomTransition();
		QFAIL("expected NoActiveTransitionsException");
	} catch (NoActiveTransitionsException& e) {
	}
}

void PetriNetTest::testActiveTransitions() try {
	Place* place1 = mPn->createPlace();
	Place* place2 = mPn->createPlace();
	Transition* t = mPn->createTransition();
	mPn->createArc(place1, t);
	mPn->createArc(t, place2);
	place1->addTokens(5);
	Simulation sim(mPn);
	sim.fireRandomTransition();
	QCOMPARE(place1->numTokens(), 4u);
	QCOMPARE(place2->numTokens(), 1u);
	sim.fireRandomTransition();
	QCOMPARE(place1->numTokens(), 3u);
	QCOMPARE(place2->numTokens(), 2u);
	sim.fireRandomTransition();
	QCOMPARE(place1->numTokens(), 2u);
	QCOMPARE(place2->numTokens(), 3u);
	sim.fireRandomTransition();
	QCOMPARE(place1->numTokens(), 1u);
	QCOMPARE(place2->numTokens(), 4u);
	sim.fireRandomTransition();
	QCOMPARE(place1->numTokens(), 0u);
	QCOMPARE(place2->numTokens(), 5u);
	try {
		Simulation sim(mPn);
		sim.fireRandomTransition();
		QFAIL("expected NoActiveTransitionsException");
	} catch (NoActiveTransitionsException& e) {
	}
	QCOMPARE(place1->numTokens(), 0u);
	QCOMPARE(place2->numTokens(), 5u);
} catch (Exception& e) {
	QFAIL(qPrintable(e.message()));
}



void PetriNetTest::testArc() {
	Place* place1 = mPn->createPlace();
	Place* place2 = mPn->createPlace();
	Transition* t = mPn->createTransition();
	mPn->createArc(place1, t);
	mPn->createArc(t, place2);
	QCOMPARE(t->numInputArcs(), 1);
	QCOMPARE(t->numOutputArcs(), 1);
	QCOMPARE(place1->numInputArcs(), 0);
	QCOMPARE(place1->numOutputArcs(), 1);
	QCOMPARE(place2->numInputArcs(), 1);
	QCOMPARE(place2->numOutputArcs(), 0);

}

void PetriNetTest::testArcWithWeight() {
	Place* place1 = mPn->createPlace();
	Place* place2 = mPn->createPlace();
	place1->setNumTokens(20);
	Transition* t = mPn->createTransition();
	Arc* arc = mPn->createArc(place1, t);
	arc->setWeight(5);
	mPn->createArc(t, place2);
	Simulation sim(mPn);
	sim.fireTransition(t);
	QCOMPARE(place1->numTokens(), 15u);
	sim.fireTransition(t);
	QCOMPARE(place1->numTokens(), 10u);
	sim.fireTransition(t);
	QCOMPARE(place1->numTokens(), 5u);
	sim.fireTransition(t);
	QCOMPARE(place1->numTokens(), 0u);
	try {
		sim.fireTransition(t);
		QFAIL("An exception expected!");
	} catch (Exception& e) {
	}
}

void PetriNetTest::testUpdateOnMove() {
	Place* place1 = mPn->createPlace();
	Transition* t = mPn->createTransition();
	mPn->createArc(place1, t);
	QSignalSpy spy(mPn, SIGNAL(itemModified(Item*)));
	place1->setPos(QPointF(55, 47));
	QCOMPARE(spy.count(), 1);
}

void PetriNetTest::testSourceTransition() {
	Transition* t = mPn->createTransition();
	Place* place = mPn->createPlace();
	mPn->createArc(t, place);
	Simulation sim(mPn);
	QCOMPARE(place->numTokens(), 0u);
	sim.fireRandomTransition();
	QCOMPARE(place->numTokens(), 1u);
	sim.fireRandomTransition();
	QCOMPARE(place->numTokens(), 2u);
	sim.fireRandomTransition();
	QCOMPARE(place->numTokens(), 3u);
}

void PetriNetTest::testWhirlPoolTransition() {
	Transition* t = mPn->createTransition();
	Place* place = mPn->createPlace();
	place->addTokens(2);
	mPn->createArc(place, t);

	Simulation sim(mPn);
	sim.fireRandomTransition();
	QCOMPARE(place->numTokens(), 1u);
	sim.fireRandomTransition();
	QCOMPARE(place->numTokens(), 0u);
}

void PetriNetTest::testRemoveArc() {
	QSignalSpy spy(mPn, SIGNAL(itemRemoved(Item*)));

	Transition* t1 = mPn->createTransition();
	Place* place = mPn->createPlace();
	Transition* t2 = mPn->createTransition();
	Arc* a1 = mPn->createArc(place, t1);
	Arc* a2 = mPn->createArc(place, t2);
	Arc* a3 = mPn->createArc(t2, place);
	Arc* a4 = mPn->createArc(t1, place);

	QCOMPARE(place->numInputArcs(), 2);
	QCOMPARE(place->numOutputArcs(), 2);
	QCOMPARE(t1->numInputArcs(), 1);
	QCOMPARE(t1->numOutputArcs(), 1);
	QCOMPARE(t2->numInputArcs(), 1);
	QCOMPARE(t2->numOutputArcs(), 1);

	mPn->removeItem(a1);
	QCOMPARE(spy.count(), 1);
	QCOMPARE(place->numInputArcs(), 2);
	QCOMPARE(place->numOutputArcs(), 1);
	QCOMPARE(t1->numInputArcs(), 0);
	QCOMPARE(t1->numOutputArcs(), 1);
	QCOMPARE(t2->numInputArcs(), 1);
	QCOMPARE(t2->numOutputArcs(), 1);

	mPn->removeItem(a2);
	QCOMPARE(spy.count(), 2);
	QCOMPARE(place->numInputArcs(), 2);
	QCOMPARE(place->numOutputArcs(), 0);
	QCOMPARE(t1->numInputArcs(), 0);
	QCOMPARE(t1->numOutputArcs(), 1);
	QCOMPARE(t2->numInputArcs(), 0);
	QCOMPARE(t2->numOutputArcs(), 1);

	mPn->removeItem(a3);
	QCOMPARE(spy.count(), 3);
	QCOMPARE(place->numInputArcs(), 1);
	QCOMPARE(place->numOutputArcs(), 0);
	QCOMPARE(t1->numInputArcs(), 0);
	QCOMPARE(t1->numOutputArcs(), 1);
	QCOMPARE(t2->numInputArcs(), 0);
	QCOMPARE(t2->numOutputArcs(), 0);

	mPn->removeItem(a4);
	QCOMPARE(spy.count(), 4);
	QCOMPARE(place->numInputArcs(), 0);
	QCOMPARE(place->numOutputArcs(), 0);
	QCOMPARE(t1->numInputArcs(), 0);
	QCOMPARE(t1->numOutputArcs(), 0);
	QCOMPARE(t2->numInputArcs(), 0);
	QCOMPARE(t2->numOutputArcs(), 0);
}

void PetriNetTest::testRemoveNode() {
	QSignalSpy spy(mPn, SIGNAL(itemRemoved(Item*)));

	Transition* t1 = mPn->createTransition();
	Place* place = mPn->createPlace();
	Transition* t2 = mPn->createTransition();
	Arc* a1 = mPn->createArc(place, t1);
	Arc* a2 = mPn->createArc(place, t2);
	Arc* a3 = mPn->createArc(t2, place);
	Arc* a4 = mPn->createArc(t1, place);

	QLinkedList<Item*> removeItems = mPn->removeItem(place);
	QCOMPARE(spy.count(), 5);
 	QCOMPARE(removeItems.count(), 5);
	QVERIFY(!mPn->contains(a1));
	QVERIFY(!mPn->contains(a2));
	QVERIFY(!mPn->contains(a3));
	QVERIFY(!mPn->contains(a4));
	qDeleteAll(removeItems);
}

void PetriNetTest::testCapacity() {
	Place* p = mPn->createPlace();
	Transition* t = mPn->createTransition();
	Arc* a = mPn->createArc(t, p);

	p->setCapacity(4);
	a->setWeight(2);

	Simulation sim(mPn);

	QCOMPARE(sim.activeTransitions().count(), 1);
	sim.fireTransition(t);
	QCOMPARE(p->numTokens(), 2u);

	QCOMPARE(sim.activeTransitions().count(), 1);
	sim.fireTransition(t);
	QCOMPARE(p->numTokens(), 4u);

	QCOMPARE(sim.activeTransitions().count(), 0);
}

void PetriNetTest::testOmega() {
	Place* p = mPn->createPlace();
	Transition* t = mPn->createTransition();
	Arc* arc = mPn->createArc(p, t);
	p->setNumTokens(Marking::OMEGA);

	{
		Simulation sim(mPn);
		sim.fireTransition(t);
		QCOMPARE(p->numTokens(), Marking::OMEGA);
	}

	QLinkedList<Item*> removeItems = mPn->removeItem(arc);

	arc = mPn->createArc(t, p);
	{
		Simulation sim(mPn);
		sim.fireTransition(t);
		QCOMPARE(p->numTokens(), Marking::OMEGA);
	}

	qDeleteAll(removeItems);
}

QTEST_APPLESS_MAIN(PetriNetTest)

#include "petrinettest.moc"

