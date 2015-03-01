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

#include "analyserstatusdlg.h"
#include <QTimer>
#include <QPixmap>

AnalyserStatusDlg::AnalyserStatusDlg(QWidget* parent) : QDialog(parent, Qt::Dialog), mCurrentMovieFrame(0) {
	ui.setupUi(this);
	setModal(true);
	// load movie
	for (int i = 0; i < NUM_FRAMES; ++i) {
		QString imgName = ":/anim/wait"+QString::number(i+1);
		mMovie[i] = QPixmap(imgName);
	}

	mTimer = new QTimer(this);
	connect(mTimer, SIGNAL(timeout()), this, SLOT(updateMovie()));
	ui.movie->setPixmap(mMovie[0]);
}

void AnalyserStatusDlg::updateMovie() {
	mCurrentMovieFrame = (mCurrentMovieFrame+1) % NUM_FRAMES;
	ui.movie->setPixmap(mMovie[mCurrentMovieFrame]);
}

void AnalyserStatusDlg::setStatusMessage(const QString& status) {
	ui.progressBar->setFormat(status);
	if (!isVisible())
		show();
}

void AnalyserStatusDlg::setPercenage(int value) {
	ui.progressBar->setValue(value);
}

// void AnalyserStatusDlg::reset() {
// 	hide();
// 	ui.progressBar->setValue(0);
// }

void AnalyserStatusDlg::hideEvent(QHideEvent* ev) {
	mTimer->stop();
}

void AnalyserStatusDlg::showEvent(QShowEvent* ev) {
	mTimer->start(200);
}

//#include "analyserstatusdlg.moc"
