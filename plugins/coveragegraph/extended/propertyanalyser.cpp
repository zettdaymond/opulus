#include "propertyanalyser.h"

#include "petrinet.h"
#include "simulation.h"
#include "transition.h"
#include "markingnode.h"
#include "abstractarc.h"

PropertyAnalyser::PropertyAnalyser(PetriNet *pn) {
    _petriNet = pn;
}
bool PropertyAnalyser::isSafety() const
{
    return _isSafety;
}


bool PropertyAnalyser::isRestricted() const
{
    return _isRestricted;
}

bool PropertyAnalyser::isParallel() const
{
    return _isParallel;
}

bool PropertyAnalyser::isConflict() const
{
    return _isConflict;
}

bool PropertyAnalyser::analyse()
{
    if (_petriNet == 0) {
        _isSafety = false;
        _isRestricted = false;
        _isParallel = false;
        _isConflict = false;
        return false;
    }
    _isSafety = true;
    _isRestricted = true;
    _isParallel = false;
    _isConflict = false;
    checkSafetyAndRestricted();
    checkParallelAndConflict();
    return true;
}

void PropertyAnalyser::checkSafetyAndRestricted()
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
            Marking initialMarking = _petriNet->currentMarking();
            foreach(Transition* t, activeTransitions) {
                _petriNet->setCurrentMarking(initialMarking);
                sim.fireTransition(t);

                MarkingNode* child = new MarkingNode(node, _petriNet->currentMarking());

                if (_isRestricted == true) {
                    _isRestricted = IsTreeNodeBounded(child);
                }
                if (_isSafety == true) {
                    _isSafety = IsTreeNodeSafe(child);
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
    _petriNet->setCurrentMarking(root->marking());
}

void PropertyAnalyser::checkParallelAndConflict()
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
            Marking initialMarking = _petriNet->currentMarking();
            foreach(Transition* t, activeTransitions) {
                _petriNet->setCurrentMarking(initialMarking);
                sim.fireTransition(t);

                MarkingNode* child = new MarkingNode(node, _petriNet->currentMarking());

                //----
                QSet<Transition*> tmp = activeTransitions;
                tmp.remove(t); //все остальные

                QSet <Node *> t_inputNodes = getNodeFromTransition(t);
                foreach(Transition * tj, tmp) {
                    QSet <Node *> tj_inputNodes = getNodeFromTransition(tj);

                    //если два активных перехода имеют одинаковую входную ноду
                    if (tj_inputNodes.intersect(t_inputNodes).count() > 0) {
                        _isConflict = true;
                    }
                    else {
                        _isParallel = true;
                    }
                }
                //----
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

bool PropertyAnalyser::IsTreeNodeBounded(MarkingNode *node)
{
    QList<const Place *> places = node->marking().printingOrder();
    foreach (const Place *place, places) {
        if (node->marking().tokensOf(place) == Marking::OMEGA)  {
            return false;
        }
    }
    return true;
}

bool PropertyAnalyser::IsTreeNodeSafe(MarkingNode *node)
{
    QList<const Place *> places = node->marking().printingOrder();
    foreach (const Place *place, places) {
        if (node->marking().tokensOf(place) > 1) {
            return false;
        }

    }
    return true;
}

QSet<Node *> PropertyAnalyser::getNodeFromTransition(Transition *t)
{
    QSet <Node *> output;
    const ArcCollection arcs = t->inputArcs();
    foreach (const AbstractArc * arc, arcs.values()) {
        output.insert(arc->from());
    }
    return output;
}






