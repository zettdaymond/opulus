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

#include <QPainter>
#include "graphicarc.h"
#include "arc.h"
#include "graphicutil.h"

GraphicArc::GraphicArc(Arc* arc) : GraphicAbstractArc(arc) {
}


void GraphicArc::drawHead(QPainter* painter, const QPointF& from, const QPointF& to) {
	static const QPoint arrow[3] = {
		QPoint(0, 5),
			   QPoint(HEAD_SIZE, 0),
					  QPoint(0, -5)
	};
	QPointF origin = from - to;
	double length = DistanceFromOrign(origin);
	origin /= length;
	origin *= HEAD_SIZE;
	origin += to;

	painter->translate(origin);
	painter->rotate(AngleRelativeToHorizontal(from, to));
	painter->setBrush(mPen.color());
	painter->drawConvexPolygon(arrow, 3);
}

