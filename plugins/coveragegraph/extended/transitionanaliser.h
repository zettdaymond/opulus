#ifndef TRANSITION_ANALYSER_H
#define TRANSITION_ANALYSER_H

#include <QObject>
#include <QSet>
class Transition;
class Marking;
class Node;
class MarkingNode;
class PetriNet;
class TransitionAnalyser : public QObject
{
    Q_OBJECT
public:
    TransitionAnalyser(PetriNet *pn);
    virtual ~TransitionAnalyser() {}

public:
    bool analyse();

    QSet<Transition *> deadTransitions() const;
    QSet<Transition *> potentialDeadTransitions() const;
    QSet<Transition *> potentialLiveTransitions() const;
    QSet<Transition *> liveTransitions() const;
    QSet<Transition *> stableTransitions() const;

    bool isNetStable();
    bool isNetLive();

private:

    QSet<Transition *>  deadTransitionInSubTree(const Marking &startMarking);
    QSet<Transition *> ptnLiveTransitionsInSubTree(const Marking &startMarking);

    void activityLevelAnalyse();
    void stabilityAnalyse();

    QSet <Transition *> _deadTransitions;
    QSet <Transition *> _potentialDeadTransitions;
    QSet <Transition *> _potentialLiveTransitions;
    QSet <Transition *> _liveTransitions;
    QSet <Transition *> _stableTransitions;

    PetriNet * _petriNet;
};

#endif
