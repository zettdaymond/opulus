#include "invertibilityanalyser.h"
#include "petrinet.h"
#include "markingnode.h"
#include "simulation.h"
bool InvertibilityAnalyser::analyse()
{
    if (_petriNet == 0) {
        return false;
    }

    Marking saveMrk = _petriNet->currentMarking();
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
        const QSet<Transition*>& activeTransitions = sim.activeTransitions();

        if (activeTransitions.count()) {
            Marking initialMarking = _petriNet->currentMarking();
            foreach(Transition* t, activeTransitions) {
                _petriNet->setCurrentMarking(initialMarking);
                sim.fireTransition(t);

                MarkingNode* child = new MarkingNode(node, _petriNet->currentMarking());

                if (isStartReachableFromMarking(_petriNet->currentMarking()) == false) {
                    _isInvertibility = false;
                    return true; //analysis complete successful
                }

                if (!markings.contains(child->marking())) {
                    newNodes.prepend(child);
                    markings << child->marking();
                } else
                    delete child;
            }
        }
    }
    qDeleteAll(allNodes);
    _petriNet->setCurrentMarking(saveMrk);
    _isInvertibility = true;
    return true; //analysis complete successful
}

bool InvertibilityAnalyser::isStartReachableFromMarking(Marking marking)
{
    if (_petriNet->currentMarking() == startMarking) {\
        return true;
    }
    Marking saveMrk = _petriNet->currentMarking();
    _petriNet->setCurrentMarking(marking);
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
        const QSet<Transition*>& activeTransitions = sim.activeTransitions();

        if (activeTransitions.count()) {
            Marking initialMarking = _petriNet->currentMarking();
            foreach(Transition* t, activeTransitions) {
                _petriNet->setCurrentMarking(initialMarking);
                sim.fireTransition(t);

                MarkingNode* child = new MarkingNode(node, _petriNet->currentMarking());

                if(_petriNet->currentMarking() == startMarking) {
                    qDeleteAll(allNodes);
                    return true;
                }

                if (!markings.contains(child->marking())) {
                    newNodes.prepend(child);
                    markings << child->marking();
                } else
                    delete child;
            }
        }
    }
    qDeleteAll(allNodes);
    _petriNet->setCurrentMarking(saveMrk);
    return false;
}

InvertibilityAnalyser::InvertibilityAnalyser(PetriNet *pn) {
    _petriNet = pn;
    startMarking = _petriNet->currentMarking();
}
bool InvertibilityAnalyser::isInvertibility() const
{
    return _isInvertibility;
}
