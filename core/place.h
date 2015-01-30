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

#ifndef PLACE_H
#define PLACE_H

#include "node.h"
#include <climits>
#include <QCoreApplication>

class QDomElement;

/**
* A place in the Petri Net.
* @see Marking
* @ingroup core
*/
class Place : public Node {
	Q_DECLARE_TR_FUNCTIONS(Place);
public:

	enum { INFINITY = UINT_MAX };

	/// Returns the number of tokens in this place.
	uint numTokens() const;

	/// Adds a number \p num of tokens to this place.
	void addTokens(uint num);

	/// Adds a token to this place.
	void addToken() { addTokens(1); }

	/// Sets the number of tokens for this place.
	void setNumTokens(uint num);

	/// Sets the place capacity.
	void setCapacity(uint capacity);

	/// Returns the place capacity.
	uint capacity() const { return mCapacity; }

	bool hasCapacity() const { return mCapacity != INFINITY; }

	/// Removes a token from this place.
	void removeToken() { removeTokens(1); }

	/// Removes \p num tokens from this place.
	void removeTokens(uint num);

	void save(QXmlStreamWriter& xml);
	static void load(PetriNet* pn, const QDomElement& elem);

private:
	friend class PetriNet;
	Place(PetriNet* pn, const QPointF& pos, const ItemId& id);

	uint mCapacity;

	void updateTransitionsStatus();
};

#endif
