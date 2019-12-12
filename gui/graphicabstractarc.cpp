 /***************************************************************************
 *   Copyright (C) 2007 by Hugo Parente Lima <hugo.pl@gmail.com>           *
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

#include "graphicabstractarc.h"
#include "graphicplace.h"
#include "graphictransition.h"
#include "abstractarc.h"
#include "place.h"
#include "petrinetscene.h"
#include "graphicutil.h"
#include "opulusmath.h"
#include <QPainter>
#include <QPainterPath>
#include <QMenu>
#include <QDebug>
#include <QGraphicsSceneContextMenuEvent>
#include <QLineF>
#include <QPainterPathStroker>

GraphicAbstractArc::GraphicAbstractArc(AbstractArc* arc) : mArc(arc), mMovingPoint(nullptr) {
	setFlags(ItemIsSelectable);
	setZValue(0);
}

Item* GraphicAbstractArc::item() const {
	return mArc;
}

void GraphicAbstractArc::itemChanged() {
	prepareGeometryChange();

	const IntermediatePointList& pointList = mArc->intermediatePoints();

	PetriNetScene* pnScene = static_cast<PetriNetScene*>(scene());

	mFrom = pnScene->graphicItem(mArc->from())->pos();
	mTo = pnScene->graphicItem(mArc->to())->pos();

	QPointF lastPointBeforeDock;
	if (pointList.count())
		lastPointBeforeDock = *pointList.last();
	else
		lastPointBeforeDock = mFrom;
	adjustDestinationPoint(mArc->to(), lastPointBeforeDock, mTo);

	mPath = QPainterPath();
	mPath.moveTo(mFrom);
	createCurvedArcPath();

	QPainterPathStroker stroker;
	stroker.setWidth(10);
	mShape = stroker.createStroke(mPath);
	mBoundingRect = mShape.boundingRect();

	update();
}

void GraphicAbstractArc::createCurvedArcPath() {
	const IntermediatePointList& pointList = mArc->intermediatePoints();
	if (pointList.count() > 0) {
		IntermediatePointList::const_iterator it = pointList.begin();
		IntermediatePointList::const_iterator end = pointList.end();

		QPointF c1, c2 = mFrom; // control points
		QPointF oldC2;
		QPointF currentPos, nextPos, lastPos = mFrom;
		for (; it != end; ++it) {
			currentPos = **it;
			oldC2 = c2;
			nextPos = it == (end-1) ? mTo : **(it+1);
			calcControlPoints(lastPos, currentPos, nextPos, c1, c2);
			mPath.cubicTo(oldC2, c1, currentPos);
			lastPos = currentPos;
		}
		QPointF lastVector = mTo;
		lastVector.rx() += 10.0;
		oldC2 = c2;
		calcControlPoints(lastPos, mTo, lastVector, c1, c2);
		mPath.cubicTo(oldC2, c1, mTo);
	} else { // no segments... just a line
		mPath.lineTo(mTo);
	}
}

void GraphicAbstractArc::calcControlPoints(const QPointF& a, const QPointF& b, const QPointF& c, QPointF& c1, QPointF& c2) {
	QLineF ac(a, c);
	QLineF ab = QLineF(a, b);
	double angle = ab.angle(ac);
	double cateto = ab.length() * std::cos(angle*M_PI/180.0);
	// d = ponto na linha ac que forma uma reta bd perpendicular a reta ac
	QPointF d = ac.pointAt(cateto/ac.length());
	QPointF dbVector = b - d;
	QLineF paralela(ac);
	paralela.translate(dbVector);

	const double CONTROL_DIST = 15.0;
	// acha control points
	paralela.translate(b - paralela.p1());
	double dist = Distance(b, c);
	double controlTranslation = dist < CONTROL_DIST ? dist : CONTROL_DIST;
	paralela.setLength(controlTranslation);
	c2 = paralela.p2();

	dist = Distance(a, b);
	controlTranslation = dist < CONTROL_DIST ? dist : CONTROL_DIST;
	paralela.setLength(-controlTranslation);
	c1 = paralela.p2();
}

QRectF GraphicAbstractArc::boundingRect() const {
	return mBoundingRect;
}

void GraphicAbstractArc::doPaint(QPainter* painter) {
	if (mPath.isEmpty())
		return;
	painter->setBrush(QBrush());
	painter->drawPath(mPath);
	if (mArc->weight() > 1) {
		QPointF point = mPath.pointAtPercent(0.5);
		QString weight = QString::number(mArc->weight());
		QRectF rect = painter->boundingRect(QRectF(), weight);
		rect.moveCenter(point);

		painter->setPen(Qt::white);
		painter->setBrush(Qt::white);
		painter->drawRect(rect);
		painter->setPen(currentPen());
		painter->setBrush(currentBrush());
		painter->drawText(rect, weight);
	}

	QPointF lastPointBeforeDock;
	if (mArc->intermediatePoints().size())
		lastPointBeforeDock = *mArc->intermediatePoints().last();
	else
		lastPointBeforeDock = mFrom;
	drawHead(painter, lastPointBeforeDock, mTo);
}

void GraphicAbstractArc::adjustDestinationPoint(Node* dest, const QPointF& from, QPointF& to) {
	to -= from;
	double length = DistanceFromOrign(to);
	if (length < GraphicPlace::Radious+HEAD_SIZE) {
		to += from;
	} else if (dest->isA<Place>()) {
		to /= length;
		to *= length - GraphicPlace::Radious;
		to += from;
	} else {
		PetriNetScene* sc = static_cast<PetriNetScene*>(scene());
		GraphicTransition* gt = static_cast<GraphicTransition*>(sc->graphicItem(dest));
		to = gt->calcDockPoint(from);
	}
}

void GraphicAbstractArc::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
	QPointF* point = getIntermediatePoint(event->scenePos());
	if (point) {
		QMenu menu;
		QAction* removeControlPointAction = menu.addAction(QObject::tr("Remove control point"));
		QAction* selectedAction = menu.exec(event->screenPos());

		if (selectedAction == removeControlPointAction) {
			mArc->removeIntermediatePoint(point);
			itemChanged();
		}
	}
}

int GraphicAbstractArc::getArcSegmentIndex(const QPointF& point) const {
	static QRectF rect(0, 0, 10, 10);
	rect.moveCenter(point);

	int segment = -1;
	// This is slow!!! and I'm doing in the lazzy way..
	// using QPainterPath to calc intersections.
	QPointF lastPoint = mFrom;

	foreach (const QPointF* p, mArc->intermediatePoints()) {
		segment++;
		const QPointF& curPoint = *p;

		QPainterPath path;
		path.moveTo(lastPoint);
		path.lineTo(curPoint);
		if (path.intersects(rect))
			return segment;
		lastPoint = curPoint;
	}
	// test the last segment
	QPainterPath path;
	path.moveTo(lastPoint);
	path.lineTo(mTo);
	return path.intersects(rect) ? ++segment : -1;
}

void GraphicAbstractArc::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
	if (!mMovingPoint)
		mMovingPoint = getIntermediatePoint(event->scenePos());

	if (mMovingPoint) {
		*mMovingPoint = event->scenePos();
	} else {
		int segmentIndex = getArcSegmentIndex(event->scenePos());
		mMovingPoint = static_cast<PetriNetScene*>(scene())->addIntermediatePoint(mArc, event->scenePos(), segmentIndex);
	}
	itemChanged();
}

void GraphicAbstractArc::mouseReleaseEvent(QGraphicsSceneMouseEvent* ) {
    mMovingPoint = nullptr;
}

QPointF* GraphicAbstractArc::getIntermediatePoint(const QPointF& ref) const {
	foreach(QPointF* point, mArc->intermediatePoints()) {
		if (Distance(ref, *point) < 20.0)
			return point;
	}
    return nullptr;
}
