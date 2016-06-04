/***************************************************************************
 *   Copyright (C) 2006 by Hugo Parente Lima <hugo.pl@gmail.com>           *
 *                 2015-2016 by Zett Daymond <zettday@gmail.com>           *
 *                 2015-2016 by Catgirl      <captainslowpoke@gmail.com>   *
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

#ifndef DBMODELLERMINIVIEW_H
#define DBMODELLERMINIVIEW_H

#include <QGraphicsView>
#include <QPointer>

/**
*	A miniature view to a QGraphicsView.
*	@ingroup gui
*/
class MiniView : public QGraphicsView {
Q_OBJECT
public:
    explicit MiniView(QWidget* parent = 0);

	/// Set the source graphics view.
	void setSource(QGraphicsView* source);
	QSize sizeHint();
protected:
	void paintEvent(QPaintEvent* event);
	void resizeEvent(QResizeEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
private slots:
	void fitInView(const QRectF& rect);
	void updateViewRect();
	void removeSource();
private:
	QPointer<QGraphicsView> mSource;
	QPen mPen;
};

#endif
