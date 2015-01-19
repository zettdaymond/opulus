#ifndef PROPERTYANALYSER_H
#define PROPERTYANALYSER_H

#include <QObject>

class Transition;
class Marking;
class Node;
class MarkingNode;
class PetriNet;

class PropertyAnalyser : public QObject
{
    Q_OBJECT
public:

    PropertyAnalyser(PetriNet * pn);
    virtual ~PropertyAnalyser() {}

    bool isSafety() const;
    bool isRestricted() const;
    bool isParallel() const;
    bool isConflict() const;

    bool analyse();

private:
    bool _isSafety;
    bool _isRestricted;
    //    bool _isLive;
    bool _isParallel;
    bool _isConflict;

    void checkSafetyAndRestricted();
    void checkParallelAndConflict();

    bool IsTreeNodeBounded(MarkingNode *node);
    bool IsTreeNodeSafe(MarkingNode *node);

    QSet<Node *> getNodeFromTransition(Transition *t);

    PetriNet * _petriNet;
};

#endif
