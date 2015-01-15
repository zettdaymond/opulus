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

#include "cmdremoveitem.h"
#include "petrinet.h"
#include "transition.h"
#include "place.h"
#include "item.h"
#include <QDebug>

CmdRemoveItem::CmdRemoveItem(Item* item) : mId(item->id()), mPetriNet(item->petriNet()), mItemNumber(-1) {
	setText(tr("Remove item"));
}

CmdRemoveItem::~CmdRemoveItem() {
	qDeleteAll(mItems);
}

void CmdRemoveItem::undo() {
	while (mItems.count()) {
		Item* i = mItems.takeLast();
		fixPlaceTransitionNumeration(mItemNumber, i, mPetriNet);
		mPetriNet->addItem(i);
	}
	mItems.clear();
}

void CmdRemoveItem::redo() {
	Item* item = mPetriNet->item(mId);
	mItemNumber = fixPlaceTransitionNumeration(item, mPetriNet);
	mItems = mPetriNet->removeItem(mId);
}

int CmdRemoveItem::fixPlaceTransitionNumeration(Item *to_delete, PetriNet *from)
{
	int ret = -1;
	if (to_delete->isA<Transition>() || to_delete->isA<Place>()) {
		Node *t = static_cast<Node*>(to_delete);
		QString name = t->name();
		if(name.startsWith('T') || name.startsWith('P')) {
			bool ok;
			int tnum = name.remove(0,1).toInt(&ok);
			if(ok) {
				ret = tnum;
				QVector<Node*> nodes;

				if(to_delete->isA<Transition>()) {
					foreach (Transition* t, from->transitions()) {
						nodes.push_back(t);
					}
				} else {
					foreach (Place* p, from->places()) {
						nodes.push_back(p);
					}
				}

				foreach (Node* n, nodes) {
					QString tmp = n->name();
					QChar l = tmp[0];
					int num; bool ok;
					if((num = tmp.remove(0,1).toInt(&ok)) > ret && ok) {
						n->setName(l + QString::number(num-1));
						qDebug() << "fixed name for" << n->name();
					}
				}
			}
		}

	}
	return ret;
}


void CmdRemoveItem::fixPlaceTransitionNumeration(int restore_num, Item *restored, PetriNet *net)
{
	if(restore_num < 0)
		return;

	if(restored->isA<Transition>()) {
		foreach (Transition* tr, net->transitions()) {
			QString tmp = tr->name();
			int num; bool ok;
			if((num = tmp.remove(0,1).toInt(&ok)) >= restore_num && ok) {
				tr->setName("T" + QString::number(num+1));
				return;
			}
		}
	}
	if (restored->isA<Place>()) {
		foreach (Place* p, net->places()) {
			QString tmp = p->name();
			int num; bool ok;
			if((num = tmp.remove(0,1).toInt(&ok)) >= restore_num && ok) {
				p->setName("P" + QString::number(num+1));
				return;
			}
		}
	}
}
