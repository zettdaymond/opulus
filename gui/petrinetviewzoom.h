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
