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

#ifndef GRAPHICABSTRACTARC_H
#define GRAPHICABSTRACTARC_H

#include "graphicitem.h"

class Node;
class AbstractArc;
class IntermediatePoint;

/**
* Graphic representation of an arc.
* @ingroup gui
*/
class GraphicAbstractArc : public GraphicItem {
public:

	enum { HEAD_SIZE = 10 };

	virtual ~GraphicAbstractArc() {}
	explicit GraphicAbstractArc(AbstractArc* arc);
	Item* item() const;

	void changeIntermediatePoint(int pointIdx, const QPointF& value);

	void itemChanged();
	QRectF boundingRect() const;
	QPainterPath shape () const { return mShape; }
protected:
	void doPaint(QPainter* painter);
	void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

	// draw the arc arrow
	virtual void drawHead(QPainter* painter, const QPointF& from, const QPointF& to) = 0;
	
private:
	AbstractArc* mArc;
	QPainterPath mPath;
	QPainterPath mShape;
	QRectF mBoundingRect;
	QPointF mTo, mFrom;
	QPointF* mMovingPoint;


	/**
	*	Adjust the path end point to the border of the node (place or transition).
	*/
	void adjustDestinationPoint(Node* dest, const QPointF& from, QPointF& to);

	void calcControlPoints(const QPointF& a, const QPointF& b, const QPointF& c, QPointF& c1, QPointF& c2);

	void createCurvedArcPath();
	int getArcSegmentIndex(const QPointF& point) const;
	QPointF* getIntermediatePoint(const QPointF& ref) const;
};

#endif
