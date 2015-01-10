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

#ifndef PLACEPROPERTYHANDLER_H
#define PLACEPROPERTYHANDLER_H

#include "propertyhandler.h"

class Place;

class PlacePropertyHandler : public PropertyHandler {
friend class PropertyHandler;
public:
	bool setPropertyValue(int propertyIdx, const QVariant& value);
	void updateProperties();
private:
	PlacePropertyHandler(Controller* controller, Place* place);

	Place* mPlace;
};

#endif

