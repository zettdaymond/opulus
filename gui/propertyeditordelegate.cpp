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

#include "propertyeditordelegate.h"
#include "propertyeditormodel.h"
#include "property.h"

#include <QPainter>
#include <QFrame>
#include <QKeyEvent>
#include <QApplication>
#include <QSpinBox>
#include <QLabel>

#include <QDebug>

#include <limits.h>

PropertyEditorDelegate::PropertyEditorDelegate(QObject *parent)
    : QItemDelegate(parent) {
}

void PropertyEditorDelegate::paint(QPainter* painter, const QStyleOptionViewItem& opt, const QModelIndex& index) const
{
    QStyleOptionViewItem option = opt;

    const QAbstractItemModel *model = index.model();
    Property *property = static_cast<const PropertyEditorModel*>(model)->privateData(index);

    if (property && property->isSeparator()) {
        option.palette.setColor(QPalette::Text, option.palette.color(QPalette::BrightText));
        option.font.setBold(true);
        option.state &= ~QStyle::State_Selected;
    }

    if (index.column() == 1) {
        option.state &= ~QStyle::State_Selected;
    }

    option.state &= ~QStyle::State_HasFocus;

    if (property && property->isSeparator()) {
        QBrush bg = option.palette.dark();
        painter->fillRect(option.rect, bg);
    }

    QPen savedPen = painter->pen();

    QItemDelegate::paint(painter, option, index);

    QColor color = static_cast<QRgb>(QApplication::style()->styleHint(QStyle::SH_Table_GridLineColor, &option));
    painter->setPen(QPen(color));
    if (index.column() == 1 || !(property && property->isSeparator())) {
        int right = (option.direction == Qt::LeftToRight) ? option.rect.right() : option.rect.left();
        painter->drawLine(right, option.rect.y(), right, option.rect.bottom());
    }
    painter->drawLine(option.rect.x(), option.rect.bottom(),
            option.rect.right(), option.rect.bottom());
    painter->setPen(savedPen);
}

QSize PropertyEditorDelegate::sizeHint(const QStyleOptionViewItem &opt, const QModelIndex &index) const {
    return QItemDelegate::sizeHint(opt, index) + QSize(4, 4);
}

QWidget* PropertyEditorDelegate::createEditor(QWidget *parent,
                                const QStyleOptionViewItem&,
                                const QModelIndex &index) const {
    const PropertyEditorModel* model = static_cast<const PropertyEditorModel*>(index.model());
    const Property* property = model->privateData(index);

	QWidget* editor = 0;
    if (property && property->hasEditor())
		editor = property->createEditor(parent, this, SLOT(sync()));
	return editor;
}

void PropertyEditorDelegate::sync() {
	QWidget* editor = qobject_cast<QWidget*>(sender());
	if (editor)
		emit commitData(editor);
}

void PropertyEditorDelegate::setEditorData(QWidget *editor,
                             const QModelIndex &index) const {
    const QAbstractItemModel* model = index.model();
    Property* property = static_cast<const PropertyEditorModel*>(model)->privateData(index);
    if (property && property->hasEditor())
        property->updateEditorContents(editor);
}

void PropertyEditorDelegate::setModelData(QWidget *editor,
                            QAbstractItemModel *model,
                            const QModelIndex &index) const {
	Property* property = static_cast<const PropertyEditorModel*>(model)->privateData(index);
	if (property) {
		model->setData(index, property->editorValue(editor), Qt::EditRole);
	}
}

//#include "propertyeditordelegate.moc"
