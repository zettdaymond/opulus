/*
* Copyright (C) 2016 Zett Daymond <zettday@gmail.com>
*
* This software may be modified and distributed under the terms
* of the MIT license.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/

#include "graphicsviewzoom.h"

#include <QGraphicsView>
#include <QMouseEvent>
#include <QApplication>
#include <QScrollBar>
#include <QDebug>
#include <qmath.h>

GraphicsViewZoom::GraphicsViewZoom(QGraphicsView* view)
: QObject(view), mView(view)
{
	mView->viewport()->installEventFilter(this);
	mView->setMouseTracking(true);

	//update mouse position if user drag vertical or horizontal scroll bar.
	connect(mView->verticalScrollBar(), &QScrollBar::valueChanged, [=](int) {
		if (not mZooming) {
			QPointF newPoint = mView->mapFromGlobal(QCursor::pos());
			mTargetViewportPos = newPoint;
			mTargetScenePos = mView->mapToScene(newPoint.toPoint());
		}
	});

	connect(mView->horizontalScrollBar(), &QScrollBar::valueChanged, [=](int) {
		if (not mZooming) {
			QPointF newPoint = mView->mapFromGlobal(QCursor::pos());
			mTargetViewportPos = newPoint;
			mTargetScenePos = mView->mapToScene(newPoint.toPoint());
		}
	});

	mModifiers = Qt::ControlModifier;
	mZoomFactorBase = 1.0015;
}

void GraphicsViewZoom::gentleZoom(double factor) {
	mView->scale(factor, factor);
	mView->centerOn(mTargetScenePos);
	QPointF delta_viewport_pos = mTargetViewportPos - QPointF(mView->viewport()->width() / 2.0,
	mView->viewport()->height() / 2.0);
	QPointF viewport_center = mView->mapFromScene(mTargetScenePos) - delta_viewport_pos;
	mView->centerOn(mView->mapToScene(viewport_center.toPoint()));
	emit zoomed();
}

void GraphicsViewZoom::setModifiers(Qt::KeyboardModifiers modifiers) {
	mModifiers = modifiers;

}

void GraphicsViewZoom::setZoomFactorBase(double value) {
	mZoomFactorBase = value;
}

bool GraphicsViewZoom::eventFilter(QObject *object, QEvent *event) {
	if (event->type() == QEvent::MouseMove) {
		QMouseEvent* mouse_event = static_cast<QMouseEvent*>(event);
		QPointF delta = mTargetViewportPos - mouse_event->pos();
		if (qAbs(delta.x()) > 5 || qAbs(delta.y()) > 5) {
			mTargetViewportPos = mouse_event->pos();
			mTargetScenePos = mView->mapToScene(mouse_event->pos());
		}
	}
	else if (event->type() == QEvent::Wheel) {
		QWheelEvent* wheel_event = static_cast<QWheelEvent*>(event);
		if (QApplication::keyboardModifiers() == mModifiers) {
			if (wheel_event->orientation() == Qt::Vertical) {
				double angle = wheel_event->angleDelta().y();
				double factor = qPow(mZoomFactorBase, angle);
				mZooming = true;
				gentleZoom(factor);
				mZooming = false;
				return true;
			}
		}
	}
	Q_UNUSED(object)
	return false;
}
