#include "netproperty.h"
#include <QtPlugin>
#include <QDialog>
#include <QTextEdit>

#include "petrinet.h"
#include "analysisreporter.h"
#include "simulation.h"
#include "transition.h"
#include "abstractarc.h"
//#include "node.h"
class MarkingNode {
public:

    MarkingNode(MarkingNode* parent, const Marking& m) : mParent(parent), mMarking(m) {
        applyOmega();
    }

    Marking& marking() { return mMarking; }

private:
    MarkingNode* mParent;
    Marking mMarking;

    void applyOmega() {
        // apply omega if necessary
        MarkingNode* node = this;
        while(node->mParent) {
            node = node->mParent;
            if (mMarking > node->mMarking) {
                mMarking.applyOmega(node->mMarking);
                break;
            }
        }
    }
};


QString NetProperty::internalName() const
{
    return QLatin1String("netProperty");
}

QString NetProperty::name() const
{
    return tr("Net Properties");
}

bool NetProperty::setup(QWidget *parentWidget)
{
    return true;
}

void NetProperty::analyse(PetriNet *pn, AnalysisReporter *reporter)
{
    mPetriNet = pn;

    if (mPetriNet == 0) {
        reporter->fatalError(tr("Somthing wrong! Please, check your Petri Net"));
        return;
    }

    prepareForAnalysis();

    if (!mPetriNet->placeCount()) {
        reporter->fatalError(tr("There are no places in this petri net."));
        return;
    }

    BasePropertyAnalyse();

    _stableTransitions = GetStableTransitions();
    isParallelizeOrConflict();

    _isLive = true;
    foreach(Transition * t, mPetriNet->transitions()) {
        if (!_liveTransitions.contains(t)) {
            _isLive = false;
            break;
        }
    }

    mAnalysisOk = true;

}

void NetProperty::finish(QWidget *parentWidget)
{
    QDialog dlg(parentWidget);
    ui.setupUi(&dlg);
    QString out = "";

    out += "Transitions:\n";

    out +=       "\tDead :";
    foreach (Transition * t, _deadTransitions) {
        out += t->name() + ", ";
    }
    out += "\n";

    out +=       "\tPotential Dead :";
    foreach (Transition * t, _potentialDeadTransitions) {
        out += t->name() + ", ";
    }
    out += "\n";

    out +=       "\tPotential Live :";
    foreach (Transition * t, _potentialLiveTransitions) {
        out += t->name() + ", ";
    }
    out += "\n";

    out +=       "\tLive :";
    foreach (Transition * t, _liveTransitions) {
        out += t->name() + ", ";
    }
    out += "\n";

    out +=       "\tStable :";
    foreach (Transition * t, _stableTransitions) {
        out += t->name() + ", ";
    }
    out += "\n";

    out += "Petri Net :\n";
    out += "\tSafe : " + bToStr(_isSafety) + "\n";
    out += "\tBounded : " + bToStr(_isRestricted) + "\n";
    out += "\tLive :" + bToStr(_isLive)+ "\n";
    out += "\tStable :" + bToStr(_stableTransitions.contains(mPetriNet->transitions()))+ "\n";
    out += "\tParralel :" + bToStr(_isParallel)+ "\n";
    out += "\tConflicted :" + bToStr(_isConflict)+ "\n";
    ui.textEdit->setPlainText(out);
    dlg.exec();
}

bool NetProperty::IsTreeNodeBounded(MarkingNode* node)
{
    QList<const Place *> places = node->marking().printingOrder();
    foreach (const Place *place, places) {
        if (node->marking().tokensOf(place) == Marking::OMEGA)  {
            return false;
        }
    }
    return true;
}

bool NetProperty::IsTreeNodeSafe(MarkingNode* node) {
    QList<const Place *> places = node->marking().printingOrder();
    foreach (const Place *place, places) {
        if (node->marking().tokensOf(place) > 1) {
            return false;
        }

    }
    return true;
}

void NetProperty::BasePropertyAnalyse()
{

    MarkingNode* root = new MarkingNode(0, mPetriNet->currentMarking());
    root->marking().normalize(mPetriNet);
    QLinkedList<MarkingNode*> newNodes;
    QLinkedList<MarkingNode*> allNodes;
    newNodes.append(root);
    Simulation sim(mPetriNet);

    QSet<Marking> markings;
    markings << root->marking();

    while (newNodes.count()) {
        MarkingNode* node = newNodes.takeLast();
        allNodes.append(node);
        mPetriNet->setCurrentMarking(node->marking());

        const QSet<Transition*> activeTransitions = sim.activeTransitions();
        if (activeTransitions.count()) {

            //ищем мертвые переходы.
            _deadTransitions.subtract(activeTransitions);
            //ищем потенциально живые переходы
            _potentialLiveTransitions.unite(activeTransitions);

            Marking initialMarking = mPetriNet->currentMarking();
            foreach(Transition* t, activeTransitions) {
                mPetriNet->setCurrentMarking(initialMarking);
                sim.fireTransition(t);

                MarkingNode* child = new MarkingNode(node, mPetriNet->currentMarking());

                if (_isRestricted == true) {
                    _isRestricted = IsTreeNodeBounded(child);
                }
                if (_isSafety == true) {
                    _isSafety = IsTreeNodeSafe(child);
                }

                //Переход будет потенциально мертв, если начиная с некоторой ноды
                //он будет мертв на всем под-дереве вывода (которое начинается с этой ноды).
                QSet<Transition *> dt = GetDeadTransitionSubTree(child->marking());
                _potentialDeadTransitions.unite(dt);

                //Переход будет жив, если начиная с некоторой ноды он будет потенциально жив
                //на всем под-дереве вывода (которое начинается с этой ноды).
                //QSet<Transition *> lt = GetPtnLiveTransitionsSubTree(child->marking());
                QSet<Transition *> lt = GetPtnLiveTransitionsSubTree(child->marking());
                _liveTransitions.intersect(lt);

                if (!markings.contains(child->marking())) {
                    newNodes.prepend(child);
                    markings << child->marking();
                } else
                    delete child;
            }
        }
    }
}

QSet<Transition *> NetProperty::GetDeadTransitionSubTree(const Marking &startMarking)
{
    Simulation sim(mPetriNet);
    mPetriNet->setCurrentMarking(startMarking);

    //return empty set, if we cant start
    if (!mPetriNet->activeTransitionsCount())
        return mPetriNet->transitions();

    MarkingNode* root = new MarkingNode(0, mPetriNet->currentMarking());
    root->marking().normalize(mPetriNet);
    QLinkedList<MarkingNode*> newNodes;
    QLinkedList<MarkingNode*> allNodes;
    newNodes.append(root);

    QSet<Marking> markings;
    markings << root->marking();

    QSet <Transition *> deadTransitions;
    deadTransitions = mPetriNet->transitions();

    while (newNodes.count()) {
        MarkingNode* node = newNodes.takeLast();
        allNodes.append(node);
        mPetriNet->setCurrentMarking(node->marking());
        const QSet<Transition*>& activeTransitions = sim.activeTransitions();

        if (activeTransitions.count()) {
            //search Dead Transitions
            deadTransitions.subtract(activeTransitions);

            Marking initialMarking = mPetriNet->currentMarking();
            foreach(Transition* t, activeTransitions) {
                mPetriNet->setCurrentMarking(initialMarking);
                sim.fireTransition(t);

                MarkingNode* child = new MarkingNode(node, mPetriNet->currentMarking());

                if (!markings.contains(child->marking())) {
                    newNodes.prepend(child);
                    markings << child->marking();
                } else
                    delete child;
            }
        }
        //если нет больше активных то значит все последующие потенциально мертвыеё
        else {
            return mPetriNet->transitions();
        }
    }
    qDeleteAll(allNodes);
    return deadTransitions;
}

QSet<Transition *> NetProperty::GetPtnLiveTransitionsSubTree(const Marking &startMarking)
{
    Simulation sim(mPetriNet);
    mPetriNet->setCurrentMarking(startMarking);

    MarkingNode* root = new MarkingNode(0, mPetriNet->currentMarking());

    QSet <Transition *> ptnLiveTransition;

    if (!mPetriNet->activeTransitionsCount())
        return ptnLiveTransition;

    root->marking().normalize(mPetriNet);
    QLinkedList<MarkingNode*> newNodes;
    QLinkedList<MarkingNode*> allNodes;
    newNodes.append(root);

    QSet<Marking> markings;
    markings << root->marking();

    while (newNodes.count()) {
        MarkingNode* node = newNodes.takeLast();
        allNodes.append(node);
        mPetriNet->setCurrentMarking(node->marking());
        const QSet<Transition*>& activeTransitions = sim.activeTransitions();

        if (activeTransitions.count()) {

            //ищем потенциально живые переходы
            ptnLiveTransition.unite(activeTransitions);

            Marking initialMarking = mPetriNet->currentMarking();
            foreach(Transition* t, activeTransitions) {
                mPetriNet->setCurrentMarking(initialMarking);
                sim.fireTransition(t);

                MarkingNode* child = new MarkingNode(node, mPetriNet->currentMarking());

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
    return ptnLiveTransition;
}

QSet<Transition *> NetProperty::GetStableTransitions()
{
    QSet <Transition *> _stable;
    MarkingNode* root = new MarkingNode(0, mPetriNet->currentMarking());
    root->marking().normalize(mPetriNet);
    QLinkedList<MarkingNode*> newNodes;
    QLinkedList<MarkingNode*> allNodes;
    newNodes.append(root);
    Simulation sim(mPetriNet);

    QSet<Marking> markings;
    markings << root->marking();

    while (newNodes.count()) {
        MarkingNode* node = newNodes.takeLast();
        allNodes.append(node);
        mPetriNet->setCurrentMarking(node->marking());
        const QSet<Transition*> activeTransitions = sim.activeTransitions();

        if (activeTransitions.count()) {

            QSet<Transition*> currMarkingStable;
            QSet<Transition*> currMarkNotStable;
            Marking initialMarking = mPetriNet->currentMarking();
            foreach(Transition* t, activeTransitions) {
                mPetriNet->setCurrentMarking(initialMarking);
                sim.fireTransition(t);

                MarkingNode* child = new MarkingNode(node, mPetriNet->currentMarking());

                //--------
                Marking tmpMarking = mPetriNet->currentMarking();

                QSet<Transition*> tmp = activeTransitions;
                tmp.remove(t);
                bool isStable = false;

                //Запуск любого активного перехода не лишает возможности запуска
                //перехода t
                foreach (Transition * tj, tmp) {
                    mPetriNet->setCurrentMarking(initialMarking);
                    sim.fireTransition(tj);
                    if (mPetriNet->activeTransitions().contains(t)) {
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

                mPetriNet->setCurrentMarking(tmpMarking);

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
    mPetriNet->setCurrentMarking(root->marking());

    return _stable;
}

QString NetProperty::bToStr(bool b)
{
    if (b)
        return "true";
    else
        return "false";
}

void NetProperty::isParallelizeOrConflict()
{
    Simulation sim(mPetriNet);

    MarkingNode* root = new MarkingNode(0, mPetriNet->currentMarking());
    root->marking().normalize(mPetriNet);
    QLinkedList<MarkingNode*> newNodes;
    QLinkedList<MarkingNode*> allNodes;
    newNodes.append(root);

    QSet<Marking> markings;
    markings << root->marking();

    while (newNodes.count()) {
        MarkingNode* node = newNodes.takeLast();
        allNodes.append(node);
        mPetriNet->setCurrentMarking(node->marking());
        const QSet<Transition*> activeTransitions = sim.activeTransitions();

        if (activeTransitions.count()) {
            Marking initialMarking = mPetriNet->currentMarking();
            foreach(Transition* t, activeTransitions) {
                mPetriNet->setCurrentMarking(initialMarking);
                sim.fireTransition(t);

                MarkingNode* child = new MarkingNode(node, mPetriNet->currentMarking());

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
}

QSet<Node *> NetProperty::getNodeFromTransition(Transition *t)
{
    QSet <Node *> output;
    const ArcCollection arcs = t->inputArcs();
    foreach (const AbstractArc * arc, arcs.values()) {
        output.insert(arc->from());
    }
    return output;
}

void NetProperty::prepareForAnalysis()
{
    //initialization of variables
    mAnalysisOk = false;
    _isRestricted = true;
    _isSafety = true;
    _isParallel = false;
    _isConflict = false;

    _deadTransitions = mPetriNet->transitions();
    _liveTransitions = mPetriNet->transitions();
}
