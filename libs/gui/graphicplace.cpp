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

#include "graphicplace.h"
#include "place.h"
#include <QPainter>

GraphicPlace::GraphicPlace(Place* place) : GraphicNode(place), mPlace(place) {
	setZValue(10.0);
}

QRectF GraphicPlace::boundingRect() const {
	return QRectF(-Radious, -Radious, Radious*2, Radious*2);
}

void GraphicPlace::doPaint(QPainter* painter) {
	painter->drawEllipse(boundingRect());
	drawTokens(painter);
}

void GraphicPlace::drawTokens(QPainter* painter) {
	static const QPoint oneToken[] = {QPoint(0,0)};
	static const QPoint twoTokens[] = {QPoint(-6, 0), QPoint(6, 0)};
	static const QPoint threeTokens[] = {QPoint(-12, 0), QPoint(0, 0), QPoint(12, 0)};
	static const QPoint fourTokens[] = {QPoint(-6, -6), QPoint(6, -6), QPoint(-6, 6), QPoint(6, 6)};
	static const QPoint fiveTokens[] = {QPoint(-8, -8), QPoint(8, -8), QPoint(-8, 8), QPoint(8, 8), QPoint()};

	static const QPoint* tokenPositions[] = {oneToken, twoTokens, threeTokens, fourTokens, fiveTokens};
	const uint NUM_BEAUTIFULL_TOKENS = sizeof(tokenPositions)/sizeof(QPoint*);

	uint numTokens = mPlace->numTokens();

	if (!numTokens) {
		return;
	} if (numTokens > NUM_BEAUTIFULL_TOKENS) {
		painter->drawText(boundingRect(), Qt::AlignCenter, QString::number(numTokens));
	} else {
		painter->setBrush(painter->pen().color());
		painter->translate(0, 0);
		const QPoint* posList = tokenPositions[numTokens-1];
		for (uint i = 0; i < numTokens; ++i) {
			painter->drawEllipse(posList[i].x()-4, posList[i].y()-4, 8, 8);
		}
	}
}

