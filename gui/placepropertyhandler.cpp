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

#include "placepropertyhandler.h"
#include "property.h"
#include "intproperty.h"
#include "place.h"
#include "controller.h"
// to declare tr method.
#include <QObject>

PlacePropertyHandler::PlacePropertyHandler(Controller* controller, Place* place) : PropertyHandler(place, controller), mPlace(place) {
    mProperties << new StringProperty(QObject::tr("Name"), place->name());

	int capacity = place->capacity() == Place::Infinity ? 0 : place->capacity();
	IntProperty* prop = new IntProperty(QObject::tr("Capacity"), capacity);
	prop->setRange(0, INT_MAX);
	prop->setSpecialValueText(QString::fromUtf8("\u221E"));
	mProperties << prop;

	prop = new IntProperty(QObject::tr("Tokens"), place->numTokens());
	prop->setRange(0, INT_MAX);
	mProperties << prop;
}

bool PlacePropertyHandler::setPropertyValue(int propertyIdx, const QVariant& value) {
	Q_ASSERT(propertyIdx < mProperties.count());
	Property* prop = mProperties[propertyIdx];

	try {
		switch (propertyIdx) {
            case 0:
                mController->setItemAttribute<Place, Node, const QString&>
                        (mPlace, &Place::setName, value.toString(), mPlace->name());
				break;
			case 1: {
				int ivalue = value.toUInt();
				uint realValue = !ivalue ? Place::Infinity : ivalue;
                mController->setItemAttribute(mPlace, &Place::setCapacity, realValue,mPlace->capacity());
				break;
			}
			case 2:
                mController->setItemAttribute(mPlace, &Place::setNumTokens, value.toUInt(), mPlace->numTokens());
				break;
			default:
				return false;
		}
		prop->setValue(value);
		return true;
    } catch (Exception& ) {
		return false;
	}
}

void PlacePropertyHandler::updateProperties() {
	mProperties[0]->setValue(mPlace->name());
	int capacity = mPlace->capacity() == Place::Infinity ? 0 : mPlace->capacity();
	mProperties[1]->setValue(capacity);
	mProperties[2]->setValue(mPlace->numTokens());
}

