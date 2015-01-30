#ifndef NETPROPERTY_H
#define NETPROPERTY_H


#include <QObject>

#include "analyser.h"
#include "ui_netproperty.h"

class Transition;
class Marking;
class Node;
class MarkingNode;
/**
* This plugin analyzes Petri Net properties .
* @ingroup plugins
*/
class NetProperty : public QObject, public Analyser
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "opulus.sourceforge.net.Analyser")
    Q_INTERFACES(Analyser)
public:
    QString internalName() const;
    QString name() const;
    void analyse(PetriNet *pn, AnalysisReporter *reporter);
    bool setup(QWidget *parentWidget);
    void finish(QWidget *parentWidget);
   // void checkTreeNodeForBounded(MarkingNode* node);
private:
    PetriNet* mPetriNet;
    Ui::Dialog ui;
    bool mAnalysisOk;

    bool _isSafety;
    bool _isRestricted;
    bool _isLive;
    void BasePropertyAnalyse();
    QSet<Transition *>  GetDeadTransitionSubTree(const Marking &startMarking);
    QSet<Transition *> GetPtnLiveTransitionsSubTree(const Marking &startMarking);
    QSet <Transition *> GetStableTransitions();
    QString bToStr(bool b);
    QSet <Transition *> _deadTransitions;
    QSet <Transition *> _potentialDeadTransitions;
    QSet <Transition *> _potentialLiveTransitions;
    QSet <Transition *> _liveTransitions;
    QSet <Transition *> _stableTransitions;
    QHash <Transition *, uint> _transitionLevels;
    bool _isParallel;
    bool _isConflict;
    bool _isPreserving;
    bool _isStrictlyPreserving;
    static constexpr float eqSolverE = 0.000001;
    void isParallelizeOrConflict();
    void isPreserving();
    void resetAnalyser();

    QSet<Node *> getNodeFromTransition(Transition *t);
    void prepareForAnalysis();


    bool IsTreeNodeBounded(MarkingNode *node);
    bool IsTreeNodeSafe(MarkingNode *node);

    QSet <Transition *> zeroActivityTransitions();
    QSet <Transition *> firstActivityTransitions();
    QSet <Transition *> secondActivityLevelTransitions();
    QSet <Transition *> thirdActivityLevelTransitions();
    QSet <Transition *> fourthActivityLevelTransitions();


};

#endif
