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

#ifndef PROPERTYHANDLER_H
#define PROPERTYHANDLER_H

#include <QList>

class Property;
class Item;
class Controller;
class QVariant;

/**
*	Translate business logic objects in properties.
*	Create a list of properties related to an object that can be show in any view
*	using the PropertyEditorModel.
*	@ingroup gui
*/
class PropertyHandler {
public:
	explicit PropertyHandler(Item* item, Controller* controller) : mItem(item), mController(controller) {}
	/// Destructs the PropertyHandler releasing all properties.
	virtual ~PropertyHandler();

	/**
	*	Set the \p value of the property at \p propertyIdx.
	*	@returns True if the property has been set.
	*/
	virtual bool setPropertyValue(int propertyIdx, const QVariant& value) = 0;
	/// Update all properties values
	virtual void updateProperties() = 0;
	/// Return the number of properties available.
	int propertyCount() const { return mProperties.count(); }
	/// Return the property at \p propertyIdx.
	Property* property(int propertyIdx) { return mProperties.at(propertyIdx); }

	Item* item() { return mItem; }

	/**
	*	Factory to PropertyHandler's, if it can not create a PropertyHandler
	*	for \p item, a null pointer is returned.
	*/
	static PropertyHandler* create(Controller* controller, Item* item);

protected:
	/// A list of all properties
	QList<Property*> mProperties;
	Item* mItem;
	/// A pointer to the controller
	Controller* mController;

private:
	PropertyHandler(const PropertyHandler&);
	PropertyHandler& operator=(const PropertyHandler&);

};

#endif
