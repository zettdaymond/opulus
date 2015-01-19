#include "transitionanaliser.h"

#include "petrinet.h"
#include "simulation.h"
#include "transition.h"
#include "markingnode.h"

QSet<Transition *> TransitionAnalyser::deadTransitions() const
{
    return _deadTransitions;
}


QSet<Transition *> TransitionAnalyser::potentialDeadTransitions() const
{
    return _potentialDeadTransitions;
}

QSet<Transition *> TransitionAnalyser::potentialLiveTransitions() const
{
    return _potentialLiveTransitions;
}

QSet<Transition *> TransitionAnalyser::liveTransitions() const
{
    return _liveTransitions;
}

QSet<Transition *> TransitionAnalyser::stableTransitions() const
{
    return _stableTransitions;
}

bool TransitionAnalyser::isNetStable()
{
    if (_petriNet != 0)
        return _petriNet->transitions().contains(_stableTransitions);
    else
        return false;
}

bool TransitionAnalyser::isNetLive()
{
    if (_petriNet != 0)
        return _petriNet->transitions().contains(_liveTransitions);
    else {
        return false;
    }
}

QSet<Transition *> TransitionAnalyser::deadTransitionInSubTree(const Marking &startMarking)
{
    Marking saveMrk = _petriNet->currentMarking();
    _petriNet->setCurrentMarking(startMarking);

    //return empty set, if we cant start
    if (!_petriNet->activeTransitionsCount())
        return _petriNet->transitions();

    MarkingNode* root = new MarkingNode(0, _petriNet->currentMarking());
    root->marking().normalize(_petriNet);
    QLinkedList<MarkingNode*> newNodes;
    QLinkedList<MarkingNode*> allNodes;
    newNodes.append(root);
    Simulation sim(_petriNet);

    QSet<Marking> markings;
    markings << root->marking();

    QSet <Transition *> deadTransitions;
    deadTransitions = _petriNet->transitions();

    while (newNodes.count()) {
        MarkingNode* node = newNodes.takeLast();
        allNodes.append(node);
        _petriNet->setCurrentMarking(node->marking());
        const QSet<Transition*>& activeTransitions = sim.activeTransitions();

        if (activeTransitions.count()) {
            //search Dead Transitions
            deadTransitions.subtract(activeTransitions);

            Marking initialMarking = _petriNet->currentMarking();
            foreach(Transition* t, activeTransitions) {
                _petriNet->setCurrentMarking(initialMarking);
                sim.fireTransition(t);

                MarkingNode* child = new MarkingNode(node, _petriNet->currentMarking());

                if (!markings.contains(child->marking())) {
                    newNodes.prepend(child);
                    markings << child->marking();
                } else
                    delete child;
            }
        }
        //если нет больше активных то значит все последующие потенциально мертвыеё
        else {
            return _petriNet->transitions();
        }
    }
    qDeleteAll(allNodes);
    _petriNet->setCurrentMarking(saveMrk);
    return deadTransitions;
}

QSet<Transition *> TransitionAnalyser::ptnLiveTransitionsInSubTree(const Marking &startMarking)
{
    Marking saveMrk = _petriNet->currentMarking();
    _petriNet->setCurrentMarking(startMarking);
    MarkingNode* root = new MarkingNode(0, _petriNet->currentMarking());
    QSet <Transition *> ptnLiveTransition;

    if (!_petriNet->activeTransitionsCount())
        return ptnLiveTransition;

    root->marking().normalize(_petriNet);
    QLinkedList<MarkingNode*> newNodes;
    QLinkedList<MarkingNode*> allNodes;
    newNodes.append(root);
    Simulation sim(_petriNet);
    QSet<Marking> markings;
    markings << root->marking();

    while (newNodes.count()) {
        MarkingNode* node = newNodes.takeLast();
        allNodes.append(node);
        _petriNet->setCurrentMarking(node->marking());
        const QSet<Transition*>& activeTransitions = sim.activeTransitions();

        if (activeTransitions.count()) {

            //ищем потенциально живые переходы
            ptnLiveTransition.unite(activeTransitions);

            Marking initialMarking = _petriNet->currentMarking();
            foreach(Transition* t, activeTransitions) {
                _petriNet->setCurrentMarking(initialMarking);
                sim.fireTransition(t);

                MarkingNode* child = new MarkingNode(node, _petriNet->currentMarking());

                if (!markings.contains(child->marking())) {
                    newNodes.prepend(child);
                    markings << child->marking();
                } else
                    delete child;
            }
        }
        //если нет больше активных то значит ищем дальше
    }
    qDeleteAll(allNodes);
    _petriNet->setCurrentMarking(saveMrk);
    return ptnLiveTransition;
}

void TransitionAnalyser::activityLevelAnalyse()
{

    MarkingNode* root = new MarkingNode(0, _petriNet->currentMarking());
    root->marking().normalize(_petriNet);
    QLinkedList<MarkingNode*> newNodes;
    QLinkedList<MarkingNode*> allNodes;
    newNodes.append(root);
    Simulation sim(_petriNet);

    QSet<Marking> markings;
    markings << root->marking();

    while (newNodes.count()) {
        MarkingNode* node = newNodes.takeLast();
        allNodes.append(node);
        _petriNet->setCurrentMarking(node->marking());

        const QSet<Transition*> activeTransitions = sim.activeTransitions();
        if (activeTransitions.count()) {

            //ищем мертвые переходы.
            _deadTransitions.subtract(activeTransitions);
            //ищем потенциально живые переходы
            _potentialLiveTransitions.unite(activeTransitions);

            Marking initialMarking = _petriNet->currentMarking();
            foreach(Transition* t, activeTransitions) {
                _petriNet->setCurrentMarking(initialMarking);
                sim.fireTransition(t);

                MarkingNode* child = new MarkingNode(node, _petriNet->currentMarking());

                //Переход будет потенциально мертв, если начиная с некоторой ноды
                //он будет мертв на всем под-дереве вывода (которое начинается с этой ноды).
                QSet<Transition *> dt = deadTransitionInSubTree(child->marking());
                // QSet<Transition *> dt = GetDeadTransitionSubTree(initialMarking);
                //FIXME: Алгоритм должен работать с начала дерева (initialMarking), но он раюотает только с child.matking()
                _potentialDeadTransitions.unite(dt);

                //Переход будет жив, если начиная с некоторой ноды он будет потенциально жив
                //на всем под-дереве вывода (которое начинается с этой ноды).
                QSet<Transition *> lt = ptnLiveTransitionsInSubTree(child->marking());
                //QSet<Transition *> lt = GetPtnLiveTransitionsSubTree(initialMarking);
                //FIXME: Алгоритм должен работать с начала дерева (initialMarking), но он раюотает только с child.matking()
                //
                _liveTransitions.intersect(lt);

                if (!markings.contains(child->marking())) {
                    newNodes.prepend(child);
                    markings << child->marking();
                } else
                    delete child;
            }
        }
    }
    qDeleteAll(allNodes);
    _petriNet->setCurrentMarking(root->marking());
}

void TransitionAnalyser::stabilityAnalyse()
{
    QSet <Transition *> _stable;
    MarkingNode* root = new MarkingNode(0, _petriNet->currentMarking());
    root->marking().normalize(_petriNet);
    QLinkedList<MarkingNode*> newNodes;
    QLinkedList<MarkingNode*> allNodes;
    newNodes.append(root);
    Simulation sim(_petriNet);

    QSet<Marking> markings;
    markings << root->marking();

    while (newNodes.count()) {
        MarkingNode* node = newNodes.takeLast();
        allNodes.append(node);
        _petriNet->setCurrentMarking(node->marking());
        const QSet<Transition*> activeTransitions = sim.activeTransitions();

        if (activeTransitions.count()) {

            QSet<Transition*> currMarkingStable;
            QSet<Transition*> currMarkNotStable;
            Marking initialMarking = _petriNet->currentMarking();
            foreach(Transition* t, activeTransitions) {
                _petriNet->setCurrentMarking(initialMarking);
                sim.fireTransition(t);

                MarkingNode* child = new MarkingNode(node, _petriNet->currentMarking());

                //--------
                Marking tmpMarking = _petriNet->currentMarking();

                QSet<Transition*> tmp = activeTransitions;
                tmp.remove(t);
                bool isStable = false;

                //Запуск любого активного перехода не лишает возможности запуска
                //перехода t
                foreach (Transition * tj, tmp) {
                    _petriNet->setCurrentMarking(initialMarking);
                    sim.fireTransition(tj);
                    if (_petriNet->activeTransitions().contains(t)) {
                        isStable = true;
                    }
                    else {
                        isStable = false;
                        break;
                    }
                }
                if (isStable) {
                    currMarkingStable.insert(t);
                }
                else {
                    currMarkNotStable.insert(t);
                }

                _petriNet->setCurrentMarking(tmpMarking);

                //----

                if (!markings.contains(child->marking())) {
                    newNodes.prepend(child);
                    markings << child->marking();
                } else
                    delete child;
            }
            //---
            _stable.unite(currMarkingStable);
            _stable.subtract(currMarkNotStable);
            //---
        }
    }
    qDeleteAll(allNodes);
    _petriNet->setCurrentMarking(root->marking());

    _stableTransitions = _stable;
}



TransitionAnalyser::TransitionAnalyser(PetriNet* pn) {
    _petriNet = pn;
}

bool TransitionAnalyser::analyse()
{
    if (_petriNet == 0) {
        return false;
    }
    _deadTransitions = _petriNet->transitions();
    _liveTransitions = _petriNet->transitions();

    activityLevelAnalyse();
    stabilityAnalyse();
    return true;
}
