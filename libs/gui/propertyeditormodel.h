/****************************************************************************
**
** Copyright (C) 1992-2006 Trolltech ASA. All rights reserved.
** Copyright (C) 2006 Hugo Parente Lima <hugo.pl@gmail.com>
**
** This file is part of the Qt Designer of the Qt Toolkit.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.trolltech.com/products/qt/opensource.html
**
** If you are unsure which license is appropriate for your use, please
** review the following information:
** http://www.trolltech.com/products/qt/licensing.html or contact the
** sales department at sales@trolltech.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef PROPERTYMODEL_H
#define PROPERTYMODEL_H

#include <QAbstractTableModel>

class Controller;
class Item;
class Property;
class PropertyHandler;
class Place;
class Arc;
class Transition;

class PropertyEditorModel: public QAbstractTableModel {
	Q_OBJECT
public:
	explicit PropertyEditorModel(Controller* controller);
	~PropertyEditorModel();

	inline Property* privateData(const QModelIndex &index) const {
		return static_cast<Property*>(index.internalPointer());
	}

public slots:
	void setModelSource(Item* item);
	void itemModified(Item* item);

//
// QAbstractItemModel interface
//
	QModelIndex index(int row, int column, const QModelIndex &parent) const;

	int rowCount(const QModelIndex &parent) const;
	int columnCount(const QModelIndex &parent) const;

	QVariant data(const QModelIndex &index, int role) const;
	bool setData(const QModelIndex &index, const QVariant &value, int role);

	Qt::ItemFlags flags(const QModelIndex &index) const;
	QModelIndex buddy(const QModelIndex &index) const;

	QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
	PropertyHandler* mPropHandler;
	Controller* mController;

	// Return true if the property is editable
	bool isEditable(const QModelIndex &index) const;
};

#endif
