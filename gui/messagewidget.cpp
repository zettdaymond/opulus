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

#include "messagewidget.h"
#include <QPaintEvent>
#include <QPainter>
#include <QBitmap>
#include <QPalette>
#include <QTimer>
//#include <kiconloader.h>

MessageWidget::MessageWidget(QWidget* parent)
	: QWidget(parent), mTimer(0), mInfo(":/dialog-information"), mWarning(":/dialog-warning"), mError(":/dialog-error") {
	move(10, 10);
	hide();
}

void MessageWidget::showText(const QString& text, MessageWidget::MsgType type, int durationMs) {
	hide();

	QPixmap icon;
	switch(type) {
		case Info:
			icon = mInfo;
			break;
		case Error:
			icon = mError;
			break;
		case Warning:
			icon = mWarning;
			break;
	}


	QRect textRect = fontMetrics().boundingRect( text );
	textRect.adjust(0, 0, 2, 2);
	// when the icon code
	const int horizMargin = 5;
	int height = qMax(textRect.height(), icon.height());
	int width = textRect.width() + icon.width()+horizMargin;
	QRect geometry(0, 0, width+10, height+8);



	// create transparent mask
	QBitmap mask(geometry.width()+1, geometry.height()+1);
	QPainter maskPainter(&mask);
	mask.fill( Qt::white );
	maskPainter.setBackgroundMode(Qt::OpaqueMode);
	maskPainter.setBrush(Qt::black);
	maskPainter.drawRoundRect( geometry, 1600/geometry.width(), 1600/geometry.height() );
	setMask(mask);

	mPixmap = QPixmap(geometry.width()+1, geometry.height()+1);
	QPainter p(&mPixmap);
	p.setBrush(palette().window());
	p.drawRoundRect( geometry, 1600/geometry.width(), 1600/geometry.height() );
    int yText = geometry.height() - height / 2;
	p.drawText((horizMargin*2)+icon.width(), yText, text);
	p.drawPixmap(horizMargin, (geometry.height()-icon.height())/2, icon);
	resize(geometry.width()+1, geometry.height()+1);

	show();

	// timer
	if (!mTimer) {
		mTimer = new QTimer(this);
		mTimer->setSingleShot(true);
		connect(mTimer, SIGNAL(timeout()), this, SLOT(hide()));
	}
	if (durationMs > 0)
		mTimer->start(durationMs);
}

void MessageWidget::paintEvent(QPaintEvent* e) {
	QPainter p( this );
	p.drawPixmap( e->rect().topLeft(), mPixmap, e->rect() );

}

void MessageWidget::mousePressEvent( QMouseEvent* ) {
    if ( mTimer )
        mTimer->stop();
    hide();
}
