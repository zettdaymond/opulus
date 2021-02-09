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

#include "graphicutil.h"
#include "opulusmath.h"
#include <QPointF>

double AngleRelativeToHorizontal(const QPointF& from, const QPointF& to) {
	double x = to.x() - from.x();
	double y = to.y() - from.y();
	double r = std::sqrt(std::pow(x,2)+std::pow(y,2));
	double teta = std::asin(y/r)*180.0/M_PI;
	if (x < 0)
		teta = 180-teta;
	else if(y < 0)
		teta = 360+teta;
	return teta;
}
