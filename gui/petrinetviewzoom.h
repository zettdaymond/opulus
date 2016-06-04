/*
* Copyright (C) 2016 Zett Daymond <zettday@gmail.com>
*
* This software may be modified and distributed under the terms
* of the MIT license.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/

/**
 * Based on this talk : http://stackoverflow.com/questions/19113532/qgraphicsview-zooming-in-and-out-under-mouse-position-using-mouse-wheel
 **/

#ifndef PETRINETVIEWZOOM_H
#define PETRINETVIEWZOOM_H

#include <QObject>
#include <QPointF>

class QGraphicsView;
class QScrollBar;

class PetriNetViewZoom : public QObject {
  Q_OBJECT
public:
  PetriNetViewZoom(QGraphicsView* view);
  void gentleZoom(double factor);
  void setModifiers(Qt::KeyboardModifiers modifiers);
  void setZoomFactorBase(double value);

signals:
  void zoomed();

private:
  QGraphicsView* mView;
  Qt::KeyboardModifiers mModifiers;

  bool mZooming = false;
  double mZoomFactorBase;

  QPointF mTargetScenePos;
  QPointF mTargetViewportPos;

  bool eventFilter(QObject* object, QEvent* event);
  template <typename T>
  T* isA(QObject* who) {
	  return dynamic_cast<T*>(who);
  }
};

#endif
