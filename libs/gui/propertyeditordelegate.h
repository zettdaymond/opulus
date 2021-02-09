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

#ifndef PROPERTYEDITORDELEGATE_H
#define PROPERTYEDITORDELEGATE_H

#include <QItemDelegate>

class Property;
class PropertyEditorModel;

/**
*	Delegate used with TreeView's using PropertyEditorModel.
*	@author Some code in this class, especially the paint function
*	has taken from QtDesigner written by Trolltech ASA
*/
class PropertyEditorDelegate : public QItemDelegate {
    Q_OBJECT
public:
    explicit PropertyEditorDelegate(QObject *parent = 0);

//
// QItemDelegate Interface
//
	void paint(QPainter *painter, const QStyleOptionViewItem &option,
					const QModelIndex &index) const;

	QSize sizeHint(const QStyleOptionViewItem &option,
						const QModelIndex &index) const;

	QWidget *createEditor(QWidget *parent,
								const QStyleOptionViewItem &option,
								const QModelIndex &index) const;

	void setEditorData(QWidget *editor,
							const QModelIndex &index) const;

	void setModelData(QWidget *editor,
							QAbstractItemModel *model,
							const QModelIndex &index) const;
public slots:
	void sync();
};

#endif
