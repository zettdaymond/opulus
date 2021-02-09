/***************************************************************************
 *   Copyright (C) 2007 by Hugo Parente Lima <hugo.pl@gmail.com>           *
 *                 2015-2016 by Zett Daymond <zettday@gmail.com>           *
 *                 2015-2016 by Catgirl      <captainslowpoke@gmail.com>   *
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

#ifndef ITEM_H
#define ITEM_H

#include <QPointF>
#include <QString>
#include <list>
#include "itemid.h"

class PetriNet;
class QXmlStreamWriter;

/**
* Base class for all items in the petri net, including not petrinet related items
* like regions, labels, etc.
* @ingroup core
*/
class Item {
	friend class PetriNet;
public:
	/// Constructs a new Item at position (0,0).
    Item(PetriNet* pn, const ItemId& id) : mPetriNet(pn), mId(id) {}
    virtual ~Item() = default;
	/// Returns the item's petri net
	PetriNet* petriNet() const { return mPetriNet; }
	/// Syntax sugar to a dynamic cast
	template <typename T>
	T* isA() {
		return dynamic_cast<T*>(this);
	}
	/// Returns the item id.
	ItemId id() const { return mId; }

	virtual void save(QXmlStreamWriter& out) = 0;

protected:
	/// Notify the petri net about changes.
	void notifyModification();
	virtual void beforeAdd() = 0;
	/**
	* Called before remove the item from the petri net
	* @return A list of all items removed due to dependence on this item.
	* @note This item is \b NOT included in the return value.
	*/
    virtual std::list<Item*> beforeDelete() = 0;
private:
	PetriNet* mPetriNet;
	ItemId mId;

};

#endif
