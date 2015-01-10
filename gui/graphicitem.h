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

#ifndef GRAPHICITEM_H
#define GRAPHICITEM_H

#include <QGraphicsItem>
#include <QPen>
#include <QBrush>
class Item;

/**
* @ingroup gui
*/
class GraphicItem : public QGraphicsItem {
public:
	GraphicItem(QGraphicsItem* parent = 0);
	virtual ~GraphicItem() {}
	virtual Item* item() const = 0;
	virtual void itemChanged() = 0;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);

	const QPen& currentPen() const { return mPen; }
	const QBrush& currentBrush() const { return mBrush; }
protected:
	virtual void doPaint(QPainter* painter) = 0;

	/// Pen used to paint this item.
	QPen mPen;
	/// Brush used to paint this item.
	QBrush mBrush;
};

#endif
