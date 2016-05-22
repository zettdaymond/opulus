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

#include "cmdremoveitemgroup.h"
#include "petrinet.h"
#include "transition.h"
#include "place.h"
#include "item.h"
#include <QDebug>

CmdRemoveItemGroup::CmdRemoveItemGroup(QVector<Item*>& items)
{
	for (auto item : items) {
		Q_ASSERT(item);
		mIds.push_back(item->id());
	}
	Q_ASSERT(items.size() > 0);

	mPetriNet = items[0]->petriNet();
	Q_ASSERT(mPetriNet);

	setText(tr("Remove item"));
}

CmdRemoveItemGroup::~CmdRemoveItemGroup() {
	qDeleteAll(mItems);
}

void CmdRemoveItemGroup::undo() {
	while (mItems.count()) {
		Item* i = mItems.takeLast();
		mPetriNet->addItem(i);
	}
	mItems.clear();
}

void CmdRemoveItemGroup::redo() {
	//FIXME: does we need to call destructor?
	mItems.clear();
	for (auto& id : mIds) {
		mItems += mPetriNet->removeItem(id);
	}
}
