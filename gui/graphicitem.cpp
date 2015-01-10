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

#include "graphicitem.h"
#include <QPainter>
#include <QApplication>
#include <QPalette>

GraphicItem::GraphicItem(QGraphicsItem* parent) : QGraphicsItem(parent), mPen(Qt::black), mBrush(Qt::white) {
}

void GraphicItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
	if (isSelected()) {
		QColor color(QApplication::palette().highlight().color());
		mPen.setColor(color);
		// manual alpha compositing with white
		color.setRedF(color.redF()*0.5 + 0.5);
		color.setBlueF(color.blueF()*0.5 + 0.5);
		color.setGreenF(color.greenF()*0.5 + 0.5);
		mBrush.setColor(color);
	} else {
		mPen.setColor(QColor(1,1,1));
		mBrush.setColor(Qt::white);
	}
	painter->setPen(mPen);
	painter->setBrush(mBrush);
	doPaint(painter);
}
