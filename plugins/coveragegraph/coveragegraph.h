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

#ifndef COVERAGEGRAPH_H
#define COVERAGEGRAPH_H

#include <QObject>
#include "analyser.h"
#include "ui_coveragegraph.h"

class QTextStream;
class Marking;

/**
* This plugin creates the Petri Net coverage graph and draw it using the dot util.
* @ingroup plugins
*/
class CoverageGraph : public QObject, public Analyser {
Q_OBJECT
Q_PLUGIN_METADATA(IID "opulus.sourceforge.net.Analyser")
Q_INTERFACES(Analyser)
public:
	QString internalName() const;
	QString name() const;
	void analyse(PetriNet* pn, AnalysisReporter* reporter);
	bool setup(QWidget* parentWidget);
	void finish(QWidget* parentWidget);

private slots:
	void zoomIn();
	void zoomOut();
private:
	PetriNet* mPetriNet;
	Ui::CovarageGraph ui;
	QByteArray mSvgData;

	bool mAnalysisOk;
	QString mMarkingOrder;

	void writeNode(QTextStream& out, const Marking& from, const Marking& to, const QString& arcName);
};

#endif
