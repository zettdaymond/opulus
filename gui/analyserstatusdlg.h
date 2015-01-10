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

#ifndef ANALYSERSTATUSDLG_H
#define ANALYSERSTATUSDLG_H

#include "ui_analyserstatus.h"
#include "analysisreporter.h"
#include <QPixmap>

class QTimer;

class AnalyserStatusDlg : public QDialog {
Q_OBJECT
public:
	AnalyserStatusDlg(QWidget* parent);
public slots:
	void setStatusMessage(const QString& status);
	void setPercenage(int value);
protected:
	void hideEvent(QHideEvent* ev);
	void showEvent(QShowEvent* ev);

private slots:
	void updateMovie();
private:
	Ui::AnalyserStatus ui;
	QTimer* mTimer;

	enum { NUM_FRAMES = 10 };
	QPixmap mMovie[NUM_FRAMES];
	int mCurrentMovieFrame;
};

#endif
