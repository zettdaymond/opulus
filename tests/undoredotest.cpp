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

#include <QTest>
#include <QUndoStack>
#include "undoredotest.h"
#include "petrinet.h"
#include "place.h"
#include "transition.h"
#include "arc.h"
#include "commands/cmdcreatearc.h"
#include "commands/cmdremoveitem.h"

void UndoRedoTest::init() {
	mPn = new PetriNet;
	mUndoStack = new QUndoStack;
}
void UndoRedoTest::cleanup() {
	delete mUndoStack;
	delete mPn;
}

void UndoRedoTest::testCreateArc() {
	Place* p = mPn->createPlace();
	Transition* t = mPn->createTransition();

	mUndoStack->push(new CmdCreateArc(mPn, p, t));

	QCOMPARE(t->inputArcs().count(), 1);
	QCOMPARE(p->outputArcs().count(), 1);
	mUndoStack->undo();
	QCOMPARE(t->inputArcs().count(), 0);
	QCOMPARE(p->outputArcs().count(), 0);
	mUndoStack->redo();
	QCOMPARE(t->inputArcs().count(), 1);
	QCOMPARE(p->outputArcs().count(), 1);
}

void UndoRedoTest::testRemoveNodeWithoutArcs() {
	QCOMPARE(mPn->itemCount(), 0);
	Place* p = mPn->createPlace();
	QCOMPARE(mPn->itemCount(), 1);
	mUndoStack->push(new CmdRemoveItem(p));
	QCOMPARE(mPn->itemCount(), 0);
	mUndoStack->undo();
	QCOMPARE(mPn->itemCount(), 1);
	mUndoStack->redo();
	QCOMPARE(mPn->itemCount(), 0);
}

void UndoRedoTest::testRemoveArc() {
	QCOMPARE(mPn->itemCount(), 0);
	Place* p = mPn->createPlace();
	Transition* t = mPn->createTransition();
	mPn->createArc(p, t);
	QCOMPARE(mPn->itemCount(), 3);

	mUndoStack->push(new CmdRemoveItem(t));
	QCOMPARE(mPn->itemCount(), 1);
	mUndoStack->undo();
	QCOMPARE(mPn->itemCount(), 3);
	mUndoStack->redo();
	QCOMPARE(mPn->itemCount(), 1);
}

QTEST_APPLESS_MAIN(UndoRedoTest)

#include "undoredotest.moc"
