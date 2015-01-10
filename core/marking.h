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

#ifndef MARKING_H
#define MARKING_H

#include <QHash>
#include <QList>

class Place;
class PetriNet;
class QTextStream;

/**
* This class represents a Petri Net marking.
* It's only a container to store the tokens for all places in the Petri Net.
* @note You can pass this class by value, it's implicity shared.
* @ingroup core
*/
class Marking {
public:
	static const uint OMEGA;

	/// Add \p tokens to the \p place.
	void addTokens(const Place* place, uint tokens);
	/// Set the number of tokens for the place \p place.
	void setNumTokens(const Place* place, uint tokens);
	/// Remove \p tokens from the \p place.
	void removeTokens(const Place* place, uint tokens);
	/// Returns the number of token of \p place.
	uint tokensOf(const Place* place) const;
	/// Remove a place from this marking.
	void removePlace(const Place* place);

	/**
	* Fill the marking internal hash.
	* For peformace reasons if you never put a token in a place, the Marking class will
	* never know about this forgotten place, so when you print the marking you will notice
	* that some places is missing. If you want a nice print from this marking, just call
	* fillMarking() and all places with zero tokens will be added to the marking internal
	* hash, so it'll be printed correctly.
	*/
	void normalize(const PetriNet* pn);
	/// Returns a list of places in the order that they are printed when this marking goes to a text stream.
	const QList<const Place*> printingOrder() const { return mPrintOrder; }
	/// Returns true if other is equal to this marking; otherwise returns false.
	bool operator==(const Marking& other) const;
	/// Returns true if other is not equal to this marking; otherwise returns false.
	bool operator!=(const Marking& other) const { return !operator==(other); }
	bool operator>(const Marking& other) const;
	void applyOmega(const Marking& other);
	/// Friend function to print this marking to a QTextStream.
	friend QTextStream& operator<<(QTextStream& stream, const Marking& m);

	friend uint qHash(const Marking& m);
private:
	QHash<const Place*, uint> mMarking;
	QList<const Place*> mPrintOrder;
};

#endif
