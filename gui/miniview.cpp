/***************************************************************************
 *   Copyright (C) 2006 by Hugo Parente Lima                               *
 *   hugo.pl@gmail.com                                                     *
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

#include "miniview.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPalette>
#include <QScrollBar>
#include <QMouseEvent>
#include <QDebug>

MiniView::MiniView(QWidget* parent) : QGraphicsView(parent), mSource(0) {
	mPen.setColor(palette().highlight().color());
	mPen.setWidth(2);
	viewport()->setMouseTracking(false);
	setCursor(Qt::OpenHandCursor);

	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setInteractive(false);
	setRenderHints(QPainter::SmoothPixmapTransform);
}

QSize MiniView::sizeHint() {
	return QSize(150, 120);
}

void MiniView::setSource(QGraphicsView* source) {
	removeSource();
	if (!source || !source->scene())
		return;
	mSource = source;
	setScene(mSource->scene());
	connect(mSource, SIGNAL(destroyed()),
			this, SLOT(removeSource()));
	connect(scene(), SIGNAL(sceneRectChanged(const QRectF&)),
			this, SLOT(fitInView(const QRectF&)));
	connect(scene(), SIGNAL(destroyed()),
			this, SLOT(removeSource()));
	connect(mSource->verticalScrollBar(), SIGNAL(valueChanged(int)),
			this, SLOT(updateViewRect()));
	connect(mSource->horizontalScrollBar(), SIGNAL(valueChanged(int)),
			this, SLOT(updateViewRect()));
	fitInView(mSource->sceneRect());
}

void MiniView::removeSource() {
	if (mSource) {
		Q_ASSERT(scene());
		mSource->verticalScrollBar()->disconnect(this);
		mSource->horizontalScrollBar()->disconnect(this);
		scene()->disconnect(this);
		mSource = 0;
		setScene(0);
	}
}

void MiniView::fitInView(const QRectF& rect) {
	QGraphicsView::fitInView(rect, Qt::KeepAspectRatio);
}

void MiniView::updateViewRect() {
	viewport()->update();
}

void MiniView::paintEvent(QPaintEvent* event) {
	if (mSource) {
		QGraphicsView::paintEvent(event);
		QPainter p(viewport());
		p.setPen(mPen);
		p.drawConvexPolygon(mapFromScene(mSource->mapToScene(mSource->viewport()->rect())));
	}
}

void MiniView::resizeEvent(QResizeEvent*) {
	fitInView(sceneRect());
}

void MiniView::mousePressEvent(QMouseEvent* event) {
	if (mSource) {
		mSource->centerOn(mapToScene(event->pos()));
		setCursor(Qt::ClosedHandCursor);
	}
}

void MiniView::mouseMoveEvent(QMouseEvent* event) {
	if (mSource)
		mSource->centerOn(mapToScene(event->pos()));
}

void MiniView::mouseReleaseEvent(QMouseEvent*) {
	setCursor(Qt::OpenHandCursor);
}

//#include "miniview.moc"
