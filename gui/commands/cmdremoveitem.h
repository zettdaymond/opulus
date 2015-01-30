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

#ifndef CMDREMOVEITEM_H
#define CMDREMOVEITEM_H

#include <QUndoCommand>
#include <QLinkedList>
#include <QCoreApplication>
#include "itemid.h"

class Item;
class PetriNet;

class CmdRemoveItem : public QUndoCommand {
	Q_DECLARE_TR_FUNCTIONS(CmdRemoveItem);
public:
	CmdRemoveItem(Item* item);
	~CmdRemoveItem();
	void undo();
	void redo();
private:
	ItemId mId;
	PetriNet* mPetriNet;
	QLinkedList<Item*> mItems;
};

#endif
