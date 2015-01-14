#ifndef NETPROPERTY_H
#define NETPROPERTY_H


#include <QObject>

#include "analyser.h"
#include "ui_netproperty.h"

class Transition;
class Marking;
class Node;
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
private:
    PetriNet* mPetriNet;
    Ui::Dialog ui;
    bool mAnalysisOk;

    bool _isSafety;
    bool _isRestricted;
    bool _isLive;
    void buidTree();
    QSet <Transition *> GetDeadSubTree(PetriNet *  pn, const Marking &inM);
    QSet <Transition *> GetPotentialLiveSubTree(PetriNet *  pn, const Marking &inM);
    QSet <Transition *> GetStableTransitions();
    QString bToStr(bool b);
    QSet <Transition *> _deadTransitions;
    QSet <Transition *> _potentialDeadTransitions;
    QSet <Transition *> _potentialLiveTransitions;
    QSet <Transition *> _liveTransitions;
    QSet <Transition *> _stableTransitions;
    bool _isParallel;
    bool _isConflict;
    void isParallelize();

    QSet<Node *> getNodeFromTransition(Transition *t);


};

#endif
