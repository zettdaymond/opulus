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

#include "transitionpropertyhandler.h"
#include "property.h"
#include "transition.h"
#include "controller.h"
#include <QDebug>

double TransitionPropertyHandler::ROTATIONS[] = {0.0, 90.0, 45.0, 135.0};

TransitionPropertyHandler::TransitionPropertyHandler(Controller* controller, Transition* t) : PropertyHandler(t, controller), mTransition(t) {
	mProperties << new StringProperty(tr("Name"), t->name());

	QStringList orientation;
	orientation << tr("Vertical") << tr("Horizontal") << tr("Diagonal 1") << tr("Diagonal 2");
	StringFromListProperty* prop = new StringFromListProperty(tr("Orientation"), orientation);

	const int N = sizeof(ROTATIONS)/sizeof(double);
	int i;
	for (i = N-1; i >= 0; --i)
		if (t->rotation() == ROTATIONS[i])
			break;
	prop->setValue(orientation[i == -1 ? ++i : i]);
	mProperties << prop;
}

bool TransitionPropertyHandler::setPropertyValue(int propertyIdx, const QVariant& value) {
	Q_ASSERT(propertyIdx < mProperties.count());

	try {
		if (propertyIdx == 0)
            //FIXME: Turn on renaming feature, when normal matrix formong algorithm will be implemented
            //mController->renameNode(mTransition, value.toString());
            ;
		else if (propertyIdx == 1) {
			StringFromListProperty* prop = static_cast<StringFromListProperty*>(mProperties[propertyIdx]);
			double rotation = ROTATIONS[prop->indexOf(value.toString())];
			mController->setItemAttribute(mTransition, &Transition::setRotation, rotation);
		}
		mProperties[propertyIdx]->setValue(value);
		return true;
	} catch (Exception& e) {
		return false;
	}
}

void TransitionPropertyHandler::updateProperties() {
	mProperties[0]->setValue(mTransition->name());
}

