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

#include "property.h"
#include "exceptions.h"

#include <QLineEdit>
#include <QListView>
#include <QComboBox>
#include <QValidator>
#include <QPainter>
#include <QDateTimeEdit>
#include <QApplication>
#include <QLabel>
#include <QList>

// -------------------------------------------------------------------------

QWidget* BoolEditorFactory::createEditor(QWidget* parent, const QObject* target, const char* slot) {
    QComboBox *combo = new QComboBox(parent);
    combo->view()->setTextElideMode(Qt::ElideLeft);
    combo->addItems(QStringList() << "false" << "true");
	QObject::connect(combo, SIGNAL(activated(int)), target, slot);
    return combo;
}

void BoolEditorFactory::updateEditorContents(QWidget* editor, ValueType value) {
    if (QComboBox *combo = qobject_cast<QComboBox*>(editor)) {
        combo->setCurrentIndex(value ? 1 : 0);
    }
}

QVariant BoolEditorFactory::editorValue(QWidget* editor) {
    if (QComboBox *combo = qobject_cast<QComboBox*>(editor)) {
        return combo->currentIndex() ? true : false;
    }
	return false;
}

QString BoolEditorFactory::toString(ValueType value) {
	return value ? QLatin1String("true") : QLatin1String("false");
}

// -------------------------------------------------------------------------

QWidget* StringEditorFactory::createEditor(QWidget* parent, const QObject* target, const char* slot) {
    QLineEdit *lineEdit = new QLineEdit(parent);
	QObject::connect(lineEdit, SIGNAL(editingFinished()), target, slot);
    return lineEdit;
}

void StringEditorFactory::updateEditorContents(QWidget *editor, ValueType value) {
    if (QLineEdit *lineEdit = qobject_cast<QLineEdit*>(editor)) {
        if (lineEdit->text() != value)
            lineEdit->setText(value);
    }
}

QVariant StringEditorFactory::editorValue(QWidget* editor) {
    if (QLineEdit *lineEdit = qobject_cast<QLineEdit*>(editor)) {
        return lineEdit->text();
    }
	return QString();
}

// -------------------------------------------------------------------------

QVariant StringFromListProperty::value() const {
	Q_ASSERT(mCurrent < mList.count());
	return mList.at(mCurrent);
}

void StringFromListProperty::setValue(const QVariant& value) {
	if (!mList.count())
		return;
	int newValue = mList.indexOf(value.toString());
	if (newValue != -1) {
		mCurrent = newValue;
	}
}

QString StringFromListProperty::toString() const {
	Q_ASSERT(mCurrent < mList.count());
	return mList.at(mCurrent);
}

QWidget* StringFromListProperty::createEditor(QWidget* parent, const QObject* target, const char* slot) const {
    QComboBox* combo = new QComboBox(parent);
	combo->addItems(mList);
	QObject::connect(combo, SIGNAL(activated(int)), target, slot);
    return combo;
}

void StringFromListProperty::updateEditorContents(QWidget* editor) const {
    if (QComboBox *combo = qobject_cast<QComboBox*>(editor))
        combo->setCurrentIndex(mCurrent);
}

QVariant StringFromListProperty::editorValue(QWidget* editor) const {
	QComboBox* combo = qobject_cast<QComboBox*>(editor);
	Q_ASSERT(combo);
	return mList.at(combo->currentIndex());
}

