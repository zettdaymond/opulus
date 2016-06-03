/***************************************************************************
 *   Copyright (C) 2007 by Hugo Parente Lima <hugo.pl@gmail.com>           *
 *   Copyright (C) 2007 by Adauto Trigueiro <adautofilho@gmail.com>        *
 *                 2015-2016 by Zett Daymond <zettday@gmail.com>           *
 *                 2015-2016 by Catgirl      <captainslowpoke@gmail.com>   *
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

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <QString>
#include <QCoreApplication>
#include "item.h"

/**
* Base class for all exceptions.
* @ingroup core
*/
class Exception {
public:
	/// Constructs a new Exception without a description message
	Exception() {}
	/// Constructs a new Exception with the message \p msg
	explicit Exception(const QString& msg) : mMessage(msg) {}
	/// Return the exception description message.
	const QString& message() const { return mMessage; }
private:
	QString mMessage;
};

/**
* Exception throwed when there are no active transitions in the petri net and you try to fire a random transition or start a simulation.
* @ingroup core
*/
class NoActiveTransitionsException : public Exception {
	Q_DECLARE_TR_FUNCTIONS(NoActiveTransitionsException);
public:
	NoActiveTransitionsException() : Exception(tr("There are no active transitions!")) {}
	NoActiveTransitionsException(int n) : Exception(tr("There are no active transitions after %1 transitions fired!").arg(n)) {}
};

/**
* Exception throwed when you try to add tokens to a place full loaded.
* @ingroup core
*/
class PlaceCapacityExceedException : public Exception {
	Q_DECLARE_TR_FUNCTIONS(PlaceCapacityExceedException);
public:
	explicit PlaceCapacityExceedException(int capacity) : Exception(tr("This place can have only %1 tokens").arg(capacity)) {
	}
};

class BadFileException : public Exception {
};

/**
* Exception throwed when you try to remove tokens from an empty place.
* @ingroup core
*/
class NoMoreTokensException : public Exception {
	Q_DECLARE_TR_FUNCTIONS(NoMoreTokensException);
public:
	NoMoreTokensException() : Exception(tr("There are no more tokens on this place.")) {
	}
};

/**
* Exception throwed when you try to fire a transition that is not active.
* @ingroup core
*/
class CantFireTransitionException : public Exception {
	Q_DECLARE_TR_FUNCTIONS(CantFireTransitionException);
public:
	CantFireTransitionException() : Exception(tr("This transition is not active.")) {
	}
};

/**
* Exception throwed when anyone try to run two Analysers at the same time.
* @ingroup core
*/
class ConcurrentAnalysisException : public Exception {
};

class DuplicatedIdException : public Exception {
	Q_DECLARE_TR_FUNCTIONS(DuplicatedIdException);
public:
	DuplicatedIdException(const ItemId& id) : Exception(tr("This net already has an item with id: %1").arg(id.toString())) {
	}
};

class DuplicatedNumberException : public Exception {
	Q_DECLARE_TR_FUNCTIONS(DuplicatedNumberException);
public:
	DuplicatedNumberException(int number, QString whatItem) : Exception(tr("This net already has %2 with number: %1").arg(number).arg(whatItem)) {
	}
};

class ItemNotFoundException : public Exception {
	Q_DECLARE_TR_FUNCTIONS(ItemNotFoundException);
public:
	ItemNotFoundException(const ItemId& id) : Exception(tr("Item not found, id: %1").arg(id.toString())) {
	}
};

class IOException : public Exception {
	Q_DECLARE_TR_FUNCTIONS(IOException);
public:
	IOException(int line, int col, QString msg) : Exception(tr("%1 at line %2, column %3").arg(msg).arg(line).arg(col)) {
	}
};

#endif
