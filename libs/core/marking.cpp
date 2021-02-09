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

#include "marking.h"
#include "place.h"
#include "petrinet.h"
#include <QTextStream>

const uint Marking::OMEGA = UINT_MAX;

void Marking::addTokens(const Place* place, uint tokens) {
	mMarking[place] += tokens;
}

void Marking::setNumTokens(const Place* place, uint tokens) {
	mMarking[place] = tokens;
}

void Marking::removeTokens(const Place* place, uint tokens) {
	mMarking[place] -= tokens;
}

uint Marking::tokensOf(const Place* place) const {
	return mMarking[place];
}

void Marking::removePlace(const Place* place) {
	mMarking.remove(place);
}

bool Marking::operator==(const Marking& other) const {
	return mMarking == other.mMarking;
}

bool Marking::operator>(const Marking& other) const {
	foreach(const Place* p, mMarking.keys()) {
		if (mMarking[p] < other.mMarking[p])
			return false;
	}
	return true;
}

void Marking::applyOmega(const Marking& other) {
	foreach(const Place* p, mMarking.keys()) {
		if (!p->hasCapacity() && mMarking[p] > other.mMarking[p])
			mMarking[p] = OMEGA;
	}
}

QTextStream& operator<<(QTextStream& stream, const Marking& m) {
	stream << '(';
	const QString omega = QString::fromUtf8("\u03C9");
	int lastPlace = m.mMarking.count()-1;
	int i = 0;

	if (m.mPrintOrder.count() == m.mMarking.count()) {
		foreach(const Place* p, m.mPrintOrder) {
			uint value = m.mMarking[p];
			if (value == Marking::OMEGA)
				stream << omega;
			else
				stream << value;
			if (i != lastPlace)
				stream << ", ";
			++i;
		}
	} else {
		foreach(uint value, m.mMarking.values()) {
			if (value == Marking::OMEGA)
				stream << omega;
			else
				stream << value;
			if (i != lastPlace)
				stream << ", ";
			++i;
		}
	}
	stream << ')';
	return stream;
}

class PlacePtrComparator {
public:
	bool operator()(const Place* p1, const Place* p2) {
		return p1->name() < p2->name();
	}
};

void Marking::normalize(const PetriNet* pn) {
	mPrintOrder.clear();
	mMarking.clear();
	foreach (const Place* p, pn->places()) {
		setNumTokens(p, p->numTokens());
		mPrintOrder << p;
    }
    std::sort(mPrintOrder.begin(), mPrintOrder.end(), PlacePtrComparator());
}

uint qHash(const Marking& m) {
	// an ugly hash function... only use the first 32 places to calc the hash.
	uint hash = 0;
	uint i = 0;
	foreach(uint value, m.mMarking.values()) {
		hash += value << i;
		++i;
		if (i > sizeof(uint)*8)
			break;
	}
	return hash;
}
