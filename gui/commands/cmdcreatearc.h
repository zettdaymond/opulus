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

#ifndef CMDCREATEARC_H
#define CMDCREATEARC_H

#include <QUndoCommand>
#include <QCoreApplication>
#include <QLinkedList>
#include "itemid.h"

class PetriNet;
class Item;
class Arc;
class Place;
class Transition;

class CmdCreateArc : public QUndoCommand {
	Q_DECLARE_TR_FUNCTIONS(CmdCreateArc);
public:
	CmdCreateArc(PetriNet* petriNet, Place* from, Transition* to);
	CmdCreateArc(PetriNet* petriNet, Transition* from, Place* to);
	virtual ~CmdCreateArc();

	void undo();
	void redo();
private:
	PetriNet* mPetriNet;
	ItemId mArc;
	ItemId mPlace;
	ItemId mTransition;
	bool mInvertArgs;
	QLinkedList<Item*> mItems;

	void init(PetriNet* petriNet, Place* p, Transition* t);
};

#endif
