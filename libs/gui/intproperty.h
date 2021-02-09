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

#ifndef INTPROPERTY_H
#define INTPROPERTY_H

#include "property.h"

/// Int property
class IntProperty : public Property {
public:
	IntProperty(const QString& name, int value);

	QString name() const {
		return mName;
	}

	QVariant value() const {
		return mValue;
	}

	void setValue(const QVariant& value) {
		mValue = value.toInt();
	}
	
	QString toString() const;
	QVariant fromString(const QString& value) const;

	bool hasEditor() const {
		return true;
	}

	QWidget* createEditor(QWidget *parent, const QObject* target, const char* slot) const;
	void updateEditorContents(QWidget* editor) const;
	QVariant editorValue(QWidget* editor) const;

	void setRange(int min, int max) {
		mMin = min;
		mMax = max;
	}

	void setSpecialValueText(const QString& text) {
		mSpecialValueText = text;
	}
private:
	int mMin, mMax;
	QString mSpecialValueText;

	QString mName;
	int mValue;
};

#endif
