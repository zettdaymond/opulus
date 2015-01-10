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

#include "analysisrunner.h"
#include "exceptions.h"
#include "analyser.h"
#include "analysisreporter.h"
#include "petrinet.h"
#include <QMutexLocker>

AnalysisRunner::AnalysisRunner(QObject* parent) : QThread(parent), mAnalysing(false), mAnalyser(0), mPetriNet(0), mReporter(0) {
	connect(this, SIGNAL(finished()), this, SLOT(enablePetriNetNotifications()));

	mReporter = new AnalysisReporter(this);
	connect(mReporter, SIGNAL(statusMessageChanged(const QString&)),
			this, SIGNAL(analysisStatusMessageChanged(const QString&)));
	connect(mReporter, SIGNAL(percentageChanged(int)),
			this, SIGNAL(analysisPercentageChanged(int)));
	connect(mReporter, SIGNAL(fatalErrorOccurred(const QString&)),
			this, SIGNAL(analysisFatalErrorOccurred(const QString&)));
}

void AnalysisRunner::enablePetriNetNotifications() {
	mPetriNet->setModifyNotificationEnabled(true);
}

void AnalysisRunner::executeAnalysis(Analyser* analyser, PetriNet* petriNet) {
	QMutexLocker locker(&analyserLock);
	if (mAnalysing)
		throw ConcurrentAnalysisException();
	mAnalysing = true;
	mAnalyser = analyser;
	mPetriNet = petriNet;
	mPetriNet->setModifyNotificationEnabled(false);
	start();
}

void AnalysisRunner::run() {
	Q_ASSERT(mAnalyser);
	Q_ASSERT(mPetriNet);
	try {
		mAnalyser->analyse(mPetriNet, mReporter);
	} catch (Exception& e) {
		mReporter->fatalError(e.message());
	}
	analyserLock.lock();
	mAnalysing = false;
	analyserLock.unlock();
}

#include "analysisrunner.moc"
