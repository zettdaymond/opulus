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
#include "placetest.h"
#include "petrinet.h"
#include "place.h"
#include "exceptions.h"

void PlaceTest::init() {
	mPn = new PetriNet;
}
void PlaceTest::cleanup() {
	delete mPn;
}

void PlaceTest::testNegativeTokens() {
	Place* place = mPn->createPlace();
	place->addTokens(2);
	place->removeToken();
	place->removeToken();
	try {
		place->removeToken();
		QFAIL("NoMoreTokensException expected");
	} catch (NoMoreTokensException& e) {
	}
}

QTEST_APPLESS_MAIN(PlaceTest)

#include "placetest.moc"
