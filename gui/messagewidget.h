/***************************************************************************
 *   Copyright (C) 2006-2007 by Hugo Parente Lima                          *
 *                                                                         *
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

#ifndef VIEWMESSAGEWIDGET_H
#define VIEWMESSAGEWIDGET_H

#include <QWidget>
#include <QPixmap>
// #include <kicon.h>
class QTimer;

/**
* Widget used to display message on QGraphicsView, but works on any widget.
* @author Kpdf/Amarok teams
* @ingroup gui
*/
class MessageWidget : public QWidget {
public:
	/// Type of message
	enum MsgType {
		Info, ///< Information message
		Warning, ///< Warning message
		Error ///< Error message
	};

	enum {
		Forever = -1 ///< Constant to infinite time.
	};

	/// Constructs a MessageWidget.
	explicit MessageWidget(QWidget* parent = 0);

	/// Display \p text for \p durationMs miliseconds.
	void showText(const QString& text, MsgType type = Info, int durationMs = 4000);
protected:
	void paintEvent(QPaintEvent* event);
	void mousePressEvent(QMouseEvent*);
private:
	QTimer* mTimer;
	QPixmap mPixmap;
	
	QPixmap mInfo;
	QPixmap mWarning;
	QPixmap mError;
};


#endif
