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

#include "propertyeditormodel.h"
#include "property.h"
#include "controller.h"
#include "exceptions.h"
#include "propertyhandler.h"

#include "place.h"
#include <QDebug>

PropertyEditorModel::PropertyEditorModel(Controller* controller)
    : QAbstractTableModel(controller), mPropHandler(0), mController(controller) {
}

PropertyEditorModel::~PropertyEditorModel() {
	delete mPropHandler;
}

void PropertyEditorModel::setModelSource(Item* item) {
	if (!item) {
		delete mPropHandler;
		mPropHandler = 0;
		reset();
	} else {
		delete mPropHandler;
		mPropHandler = 0;
		mPropHandler = PropertyHandler::create(mController, item);
		reset();
	}
}

QModelIndex PropertyEditorModel::index(int row, int column, const QModelIndex &parent) const {
	if (!mPropHandler || parent.isValid() || row >= mPropHandler->propertyCount())
		return QModelIndex();
	return createIndex(row, column, mPropHandler->property(row));
}

int PropertyEditorModel::rowCount(const QModelIndex &parent) const {
	return (parent.isValid() || !mPropHandler) ? 0  : mPropHandler->propertyCount();
}

int PropertyEditorModel::columnCount(const QModelIndex &parent) const {
	return parent.isValid() ? 0 : 2;
}

bool PropertyEditorModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (Property *property = privateData(index)) {
		if (role == Qt::EditRole && value != property->value()) {
			if (mPropHandler->setPropertyValue(index.row(), value)) {
				emit dataChanged(createIndex(index.row(), 0, index.internalPointer()),
						createIndex(index.row(), 1, index.internalPointer()));
				return true;
			}
		}
    }
    return false;
}

QVariant PropertyEditorModel::data(const QModelIndex &index, int role) const {
	Property* o = privateData(index);
	if (!o)
		return QVariant();

	switch (index.column()) {  // ### cleanup
		case 0:
			switch (role) {
				case Qt::EditRole:
				case Qt::DisplayRole:
					return o->name().isEmpty()
						? QLatin1String("<noname>")
						: o->name();
				default:
					break;
			}
			break;

		case 1: {
			switch (role) {
				case Qt::EditRole:
					return o->value();
				case Qt::DisplayRole:
					return o->toString();
				case Qt::DecorationRole:
					return o->decoration();
				default:
					break;
			}
			break;
		}
		default:
			break;
	}
	return QVariant();
}

bool PropertyEditorModel::isEditable(const QModelIndex &index) const {
    return index.column() == 1 && privateData(index)->hasEditor();
}

QModelIndex PropertyEditorModel::buddy(const QModelIndex &index) const {
    if (index.column() == 0)
        return createIndex(index.row(), 1, index.internalPointer());
    return index;
}

QVariant PropertyEditorModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal) {
        if (role != Qt::DisplayRole)
            return QVariant();

		switch (section) {
			case 0:
				return tr("Property");
			case 1:
				return tr("Value");
			default:
				return QString();
		}
    }

    return QAbstractItemModel::headerData(section, orientation, role);
}

Qt::ItemFlags PropertyEditorModel::flags(const QModelIndex &index) const {
    Q_ASSERT(index.isValid());

    Qt::ItemFlags foo = Qt::ItemIsSelectable;

	if (isEditable(index))
		foo |= Qt::ItemIsEditable;
	if (privateData(index)->isEnabled())
		foo |= Qt::ItemIsEnabled;

    return foo;
}

void PropertyEditorModel::itemModified(Item* item) {
	if (mPropHandler && mPropHandler->item() == item) {
		mPropHandler->updateProperties();
		reset();
	}
}

#include "propertyeditormodel.moc"
