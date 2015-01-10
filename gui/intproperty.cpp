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

#include "intproperty.h"
#include <QSpinBox>

IntProperty::IntProperty(const QString& name, int value) : mName(name), mValue(value) {
	mMin = INT_MIN;
	mMax = INT_MAX;
}

QString IntProperty::toString() const {
	if (!mSpecialValueText.isNull() && mValue == mMin)
		return mSpecialValueText;
	else
		return QString::number(mValue);
}

QVariant IntProperty::fromString(const QString& value) const {
	return (value == mSpecialValueText) ? mMin : value.toInt();
}


QWidget* IntProperty::createEditor(QWidget *parent, const QObject* target, const char* slot) const {
	QSpinBox* spinBox = new QSpinBox(parent);
	spinBox->setWrapping(true);
	spinBox->setRange(mMin, mMax);
	if (!mSpecialValueText.isNull())
		spinBox->setSpecialValueText(mSpecialValueText);
	QObject::connect(spinBox, SIGNAL(editingFinished()), target, slot);
	return spinBox;
}

void IntProperty::updateEditorContents(QWidget* editor) const {
	QSpinBox* spinBox = qobject_cast<QSpinBox*>(editor);
	Q_ASSERT(spinBox);
	spinBox->setValue(mValue);
	spinBox->selectAll();
}

QVariant IntProperty::editorValue(QWidget* editor) const {
	QSpinBox* spinBox = qobject_cast<QSpinBox*>(editor);
	Q_ASSERT(spinBox);
	return spinBox->value();
}

