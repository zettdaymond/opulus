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

#ifndef ANALYSISRUNNER_H
#define ANALYSISRUNNER_H

#include <QThread>
#include <QMutex>

class Analyser;
class AnalysisReporter;
class PetriNet;

/**
* Execute an analyser and monitor their status.
* @see Analyser
* @ingroup core
*/
class AnalysisRunner : public QThread {
Q_OBJECT
public:
	/// Constructs a new AnalyserRunner.
    AnalysisRunner(QObject* parent = nullptr);
	/**
	* Execute the analyser \p analyser on the \p petriNet.
	* @note This method is non blocking, a thread is created and the analyser is executed in another thread.
	* @see Analyser
	*/
	void executeAnalysis(Analyser* analyser, PetriNet* petriNet);
	/// Return the current analyser.
	Analyser* analyser() { return mAnalyser; }
signals:
	/// Signal emitted when the analyser changes their status message.
	void analysisStatusMessageChanged(const QString& msg);
	void analysisPercentageChanged(int value);
	/// Signal emitted when an error occurred.
	void analysisFatalErrorOccurred(const QString& msg);

protected:
	void run();
private slots:
	void enablePetriNetNotifications();
private:
	QMutex analyserLock;
	bool mAnalysing;
	Analyser* mAnalyser;
	PetriNet* mPetriNet;
	AnalysisReporter* mReporter;
};

#endif
