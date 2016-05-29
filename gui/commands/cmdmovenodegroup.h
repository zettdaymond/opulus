#ifndef CMDMOVENODEGROUP_H
#define CMDMOVENODEGROUP_H

#include <QUndoCommand>
#include <QPointF>
#include <QVector>
#include "item.h"

class Node;
class PetriNet;

class CmdMoveNodeGroup : public QUndoCommand
{
public:
	CmdMoveNodeGroup(QVector<Node*>& nodes, const QVector<QPointF> &poses);
    void undo();
	void redo();
private:
	QVector<QPointF> mPoses;
	QVector<QPointF> mOldPoses;
	QVector<ItemId> mIds;
	PetriNet* mPetriNet;
};

#endif
