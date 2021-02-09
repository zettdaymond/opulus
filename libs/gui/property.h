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

#ifndef DBMODELLERPROPERTY_H
#define DBMODELLERPROPERTY_H

#include <list>
#include <QVariant>
#include <QStringList>

class QWidget;
class QComboBox;

/**
*	An interface to all Properties.
*	Properties are used in the PropertyEditorModel, they store a value and know how
*	to create the correct editor to edit itself.
*	Properties are created by PropertyHandler's, the PropertyEditorModel use the
*	PropertyHandler interface to know what properties it will show.
*	@ingroup gui
*/
class Property {
public:
	friend class PropertyOwner;

	Property() : mEnabled(true) {}
	/// Destrucor
	virtual ~Property() {}
	/// Return true if his property is a separator (i.e. not a really property =/)
	virtual bool isSeparator() const { return false; }
	/// Return the property name
	virtual QString name() const = 0;
	/// Return the property value
	virtual QVariant value() const = 0;
	/// Set the property value
	virtual void setValue(const QVariant& value) = 0;
	/// Return true is the propery is enabled
	bool isEnabled() const { return mEnabled; }
	/// Enable/disable the property in GUI's
	void setEnabled(bool value) { mEnabled = value; }
	/// Return the string representation of the property value
	virtual QString toString() const = 0;
	/// Return a QVariant from the string representation of the property value
	virtual QVariant fromString(const QString& value) const = 0;

	/// Return the property decoration
	virtual QVariant decoration() const { return QVariant(); }
	/// Return true if the property have a editor
	virtual bool hasEditor() const = 0;
	/// Create a editor for this property
	virtual QWidget* createEditor(QWidget *parent, const QObject* target, const char* slot) const = 0;
	/// Update the editor contents with the property value
	virtual void updateEditorContents(QWidget* editor) const = 0;
	/// Extract the property value from a editor
	virtual QVariant editorValue(QWidget* editor) const = 0;

private:
	bool mEnabled;
};

/**
*	A factory for a bolean editor used in boolean properties.
*	@see GenericProperty
*	@ingroup gui
*/
class BoolEditorFactory {
public:
	typedef bool ValueType;
	static void updateEditorContents(QWidget* widget, ValueType value);
	static QWidget* createEditor(QWidget* parent, const QObject* target, const char* slot);
	static QVariant editorValue(QWidget* editor);
	static QString toString(ValueType value);
	static QVariant fromString(const QString& value) { return value == "true" ? true : false; }
};

/**
*	A factory for a bolean editor used in boolean properties.
*	@see GenericProperty
*	@ingroup gui
*/
class StringEditorFactory {
public:
	typedef QString ValueType;
	static void updateEditorContents(QWidget* widget, ValueType value);
	static QWidget* createEditor(QWidget* parent, const QObject* target, const char* slot);
	static QVariant editorValue(QWidget* editor);
	static QString toString(ValueType value) { return value; }
	static QVariant fromString(const QString& value) { return value; }
};

/**
*	A generic property, use the template params to create a useful property.
*	You need pass a Factory as template param, a factory is just a class with some static
*	static methods and a typedef to specify the property value type, see StringEditorFactory or
*	BoolEditorFactory as example.
*	@param EditorImpl The editor implementation, it also define the property value type.
*	@see BoolEditorFactory
*	@see StringEditorFactory
*	@ingroup gui
*/
template <typename EditorImpl>
class GenericProperty : public Property {
public:
    GenericProperty(const QString &name, const typename EditorImpl::ValueType& value)
        : mValue(value), mName(name) {}

    QVariant decoration() const { return QVariant(); }
    QString name() const { return mName; }
    QVariant value() const { return qVariantFromValue(mValue); }

	QString toString() const { return EditorImpl::toString(mValue); }
	QVariant fromString(const QString& value) const { return EditorImpl::fromString(value); }

    bool hasEditor() const { return true; }

	void updateEditorContents(QWidget* widget) const {
		EditorImpl::updateEditorContents(widget, mValue);
	}

    QWidget* createEditor(QWidget* parent, const QObject* target, const char* slot) const {
		return EditorImpl::createEditor(parent, target, slot);
	}

    QVariant editorValue(QWidget* editor) const {
		return EditorImpl::editorValue(editor);
	}

    void setValue(const QVariant& value) {
		mValue = value.value<typename EditorImpl::ValueType>();
	}

    typename EditorImpl::ValueType mValue;
    QString mName;
};

/**
*	Not really a property, just a separator.
*	@ingroup gui
*/
class SeparatorProperty: public Property {
public:
	SeparatorProperty(const QString &name) : mName(name) {}

	QString name() const { return mName; }
	QVariant value() const { return QVariant(); }

	QString toString() const { return QString(); }
	QVariant fromString(const QString&) const { return QVariant(); }
	bool isSeparator() const { return true; }
	bool hasEditor() const { return false; }
	QWidget* createEditor(QWidget*, const QObject*, const char*) const { return 0; }
	void updateEditorContents(QWidget*) const {}
	void updateValue(QWidget*) {}
    QVariant editorValue(QWidget*) const { return QVariant(); }
	void setValue(const QVariant&) {}
private:
	QString mName;
};

/// Boolean property
typedef GenericProperty<BoolEditorFactory> BoolProperty;
/// String property
typedef GenericProperty<StringEditorFactory> StringProperty;

/**
*	A property to store a string from a string list.
*	@ingroup gui
*/
class StringFromListProperty : public Property {
public:
	StringFromListProperty(const QString& name) : mName(name), mCurrent(0) {}
	StringFromListProperty(const QString& name, const QStringList& list, int value = 0) : mName(name), mList(list), mCurrent(value) {}
	void setStringList(const QStringList& list, int defaultValue = 0) {
		mList = list;
		mCurrent = defaultValue;
	}

	QString name() const { return mName; }
	void setValue(const QVariant& value);
	QVariant value() const;
	int indexOf(const QString& value) const { return mList.indexOf(value); }
	QString toString() const;
	QVariant fromString(const QString& value) const { return value; }

	bool hasEditor() const { return true; }
	QWidget* createEditor(QWidget* parent, const QObject* target, const char* slot) const;
	void updateEditorContents(QWidget* editor) const;
	void updateValue(QWidget* editor);
    QVariant editorValue(QWidget* editor) const;
private:
	QString mName;
	QStringList mList;
	int mCurrent;
};


#endif
