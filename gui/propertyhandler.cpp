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

#include "propertyhandler.h"
#include "placepropertyhandler.h"
#include "transitionpropertyhandler.h"
#include "arcpropertyhandler.h"
#include "place.h"
#include "transition.h"
#include "arc.h"
#include "property.h"

PropertyHandler::~PropertyHandler() {
	qDeleteAll(mProperties);
}

PropertyHandler* PropertyHandler::create(Controller* controller, Item* item) {
	if (item->isA<Place>())
		return new PlacePropertyHandler(controller, static_cast<Place*>(item));
	if (item->isA<Transition>())
		return new TransitionPropertyHandler(controller, static_cast<Transition*>(item));
	if (item->isA<Arc>())
		return new ArcPropertyHandler(controller, static_cast<Arc*>(item));
	qWarning("PropHandler not found!");
	return 0;
}
