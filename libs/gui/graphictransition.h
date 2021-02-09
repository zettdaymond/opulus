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

#ifndef GRAPHICTRANSITION_H
#define GRAPHICTRANSITION_H

#include "graphicnode.h"

class Transition;

/**
* Transition graphic representation.
* @ingroup gui
*/
class GraphicTransition : public GraphicNode {
public:
	explicit GraphicTransition(Transition* transition);

	enum { WIDTH = 10, HEIGHT= 30 };

	enum {
		Type = UserType+2
	};

	int type() const { return Type; }
	Transition* transition() const { return mTransition; }
	QRectF boundingRect() const;

	QPointF calcDockPoint(const QPointF& from) const;

protected:
	void doPaint(QPainter* painter);

private:
	Transition* mTransition;

	void updateArcs();
};

#endif
