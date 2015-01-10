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

#include "graphictransition.h"
#include "graphicutil.h"
#include "transition.h"
#include "arc.h"
#include "petrinetscene.h"
#include <QPainter>
#include <QDebug>

GraphicTransition::GraphicTransition(Transition* transition) : GraphicNode(transition), mTransition(transition) {
	setZValue(5.0);
}

QRectF GraphicTransition::boundingRect() const {
	return QRectF(-HEIGHT/2, -HEIGHT/2, HEIGHT, HEIGHT);
}

void GraphicTransition::doPaint(QPainter* painter) {
	if (!isSelected()) {
		painter->setBrush(Qt::white);
		if (mTransition->canFire())
			painter->setBrush(Qt::green);
	}
	if (mTransition->rotation() != 0.0)
		painter->rotate(mTransition->rotation());
	painter->drawRect(QRect(-WIDTH/2, -HEIGHT/2, WIDTH, HEIGHT));
}

QPointF GraphicTransition::calcDockPoint(const QPointF& from) const {
	double angle = AngleRelativeToHorizontal(from, pos());
	angle -= mTransition->rotation();
	QPointF adjust;
	if (angle < 75 || angle > 285) // left
		adjust.rx() -= WIDTH/2;
	else if (angle < 105) // top
		adjust.ry() -= HEIGHT/2;
	else if (angle < 255) // right
		adjust.rx() += WIDTH/2;
	else // bottom
		adjust.ry() += HEIGHT/2;

	if (mTransition->rotation() != 0.0) {
		QMatrix matrix;
		matrix.rotate(mTransition->rotation());
		adjust = matrix.map(adjust);
	}
	return pos()+adjust;
}

