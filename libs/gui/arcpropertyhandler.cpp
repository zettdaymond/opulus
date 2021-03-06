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

#include "arcpropertyhandler.h"
#include "intproperty.h"
#include "abstractarc.h"
#include "controller.h"

#include <QObject>

ArcPropertyHandler::ArcPropertyHandler(Controller* controller, AbstractArc* arc) : PropertyHandler(arc, controller), mArc(arc) {

	IntProperty* prop = new IntProperty(QObject::tr("Weight"), arc->weight());
	prop->setRange(1, INT_MAX);
	mProperties << prop;
}

bool ArcPropertyHandler::setPropertyValue(int propertyIdx, const QVariant& value) {
	Q_ASSERT(propertyIdx < mProperties.count());
	Property* prop = mProperties[propertyIdx];

	try {
		if (propertyIdx == 0) {
            mController->setItemAttribute(mArc, &AbstractArc::setWeight, value.toUInt(), mArc->weight());
			prop->setValue(value);
			return true;
		}
    } catch (Exception& ) {
	}
	return false;
}

void ArcPropertyHandler::updateProperties() {
	mProperties[0]->setValue(mArc->weight());
}
