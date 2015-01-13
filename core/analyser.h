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

#ifndef ANALYSER_H
#define ANALYSER_H

#include <QString>
//TODO
//#include <QWidget>
class PetriNet;
class AnalysisReporter;
class QWidget;

/**
* Interface for analyser plugins.
*
* An analysis occur in three steps:
* <ol>
*	<li>Analyser::setup() is called, is it returns false the analysis is aborted. Here you can show a setup dialog for your analysis algorithm.</li>
*	<li>Analyser::analyse() is called <b>in another thread</b>.</li>
*	<li>Analyser::finish() is called after the analysis thread death from the current application thread. Here you must show the analysis results.</li>
* </ol>
* @ingroup core
*/
class Analyser {
public:
	/**
	* Plugin internal name.
	* @note This string CAN NOT be translated.
	*/
	virtual QString internalName() const = 0;
	/// The plugin name, can be translated.
	virtual QString name() const = 0;
	/// This method is called (in another thread) when an analysis is requested.
	virtual void analyse(PetriNet* pn, AnalysisReporter* reporter) = 0;
	/// This method is called before the analysis start.
	virtual bool setup(QWidget* parentWidget) = 0;
	/// This method is called when an analysis is finished.
	virtual void finish(QWidget* parentWidget) = 0;
};

Q_DECLARE_INTERFACE(Analyser, "opulus.sourceforge.net.Analyser/1.0")

#endif
