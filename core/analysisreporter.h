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

#ifndef ANALYSISREPORTER_H
#define ANALYSISREPORTER_H

#include <QObject>

class QWidget;

/**
* Provides an interface to the analysis plugins report about their status
* and return the analysis result.
* @ingroup core
*/
class AnalysisReporter : public QObject {
Q_OBJECT
public:
	AnalysisReporter(QObject* parent = 0);
	/// Set the current status of the plugin, a typical "What I am doing?" message.
	void setStatusMessage(const QString& msg);
	/// Set the amount of job done, a value from 0 to 100
	void setPercenage(int value);
	void fatalError(const QString& error);
signals:
	void statusMessageChanged(const QString& msg);
	void percentageChanged(int value);
	void fatalErrorOccurred(const QString& msg);
};

#endif
