#include "netproperty.h"
#include <QtPlugin>
#include <QDialog>
#include <QTextEdit>

#include "petrinet.h"
#include "analysisreporter.h"
#include "simulation.h"
#include "transition.h"

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

void NetProperty::analyse(PetriNet *pn, AnalysisReporter *reporter)
{
    mAnalysisOk = false;
    _isRestricted = true;
    _isSafety = true;


    mPetriNet = pn;

    _deadTransitions = mPetriNet->transitions();
    _liveTransitions = mPetriNet->transitions();
    //_stableTransitions = mPetriNet->transitions();

    if (!mPetriNet->placeCount()) {
        reporter->fatalError(tr("There are no places in this petri net."));
        return;
    }

    buidTree();

    _isLive = true;
    foreach(Transition * t, mPetriNet->transitions()) {
        if (!_liveTransitions.contains(t)) {
            _isLive = false;
            break;
        }
    }

    mAnalysisOk = true;

}

bool NetProperty::setup(QWidget *parentWidget)
{
    return true;
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

    ui.textEdit->setPlainText(out);
    dlg.exec();
}

void NetProperty::buidTree()
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
        //const QSet<Transition*>& activeTransitions = sim.activeTransitions();
        const QSet<Transition*> activeTransitions = sim.activeTransitions();
        if (activeTransitions.count()) {
            // создаем копию для поиска устойчивых переходов.
            QSet<Transition *> markingNotStableTransitions;
            //ищем мертвые переходы.
            _deadTransitions.subtract(activeTransitions);
            //ищем потенциально живые переходы
            _potentialLiveTransitions.unite(activeTransitions);

            Marking initialMarking = mPetriNet->currentMarking();
            foreach(Transition* t, activeTransitions) {
                mPetriNet->setCurrentMarking(initialMarking);
                sim.fireTransition(t);

                MarkingNode* child = new MarkingNode(node, mPetriNet->currentMarking());

                //проверим на безопасность и ограниченность
                //begin

                QList<const Place *> places = child->marking().printingOrder();
                foreach (const Place *place, places) {
                    if (child->marking().tokensOf(place) == Marking::OMEGA)  {
                        _isSafety = false;
                        _isRestricted = false;
                    }

                    if (child->marking().tokensOf(place) > 1) {
                        _isSafety = false;
                    }

                }
                //end


                //проверяем на устойчивость активные переходы.
                //begin
                    Marking curMarking0 = mPetriNet->currentMarking();

                    mPetriNet->setCurrentMarking(child->marking());
                    QSet<Transition *> childActiveTr = mPetriNet->activeTransitions();
                    QSet<Transition *> notActiveTrTmp = activeTransitions;
                    notActiveTrTmp.remove(t);
                    notActiveTrTmp.subtract(childActiveTr); //нарантированны неустойчивы на данной итерации t
                    markingNotStableTransitions.unite(notActiveTrTmp); // объединяем все неустойчивые на всех итерациях для t
                    //если в результате

                    mPetriNet->setCurrentMarking(curMarking0);
                //end

                //ищем потенциально-мертвые переходы
                //begin
                Marking curMarking = mPetriNet->currentMarking();
                QSet<Transition *> dt = GetDeadSubTree(mPetriNet, child->marking());
                _potentialDeadTransitions.unite(dt);
                mPetriNet->setCurrentMarking(curMarking);
                //end

                //ищем живые переходы
                //begin
                Marking curMarking2 = mPetriNet->currentMarking();
                QSet<Transition *> lt = GetPotentialLiveSubTree(mPetriNet, child->marking());
                _liveTransitions.intersect(lt);
                mPetriNet->setCurrentMarking(curMarking2);
                //end

                if (!markings.contains(child->marking())) {
                    newNodes.prepend(child);
                    markings << child->marking();
                } else
                    delete child;
            }

            //увеличиваем количество неставильных
            _stableTransitions.unite(markingNotStableTransitions);

        } else {
            //scriptStream << '\"' << node->marking() << "\" [color = red]\n";
        }
    }

    QSet<Transition *> tmp = mPetriNet->transitions();
    tmp.subtract(_stableTransitions);
    tmp.subtract(_deadTransitions);
    _stableTransitions = tmp;

    mPetriNet->setCurrentMarking(root->marking());
    _stableTransitions = GetStableTransitions();


}

QSet<Transition *> NetProperty::GetDeadSubTree(PetriNet *pn, const Marking &inM)
{
    PetriNet * net = pn;
    pn->setCurrentMarking(inM);

    MarkingNode* root = new MarkingNode(0, pn->currentMarking());

    if (!pn->activeTransitionsCount())
        return mPetriNet->transitions();


    root->marking().normalize(net);
    QLinkedList<MarkingNode*> newNodes;
    QLinkedList<MarkingNode*> allNodes;
    newNodes.append(root);
    Simulation sim(net);

    QSet<Marking> markings;
    markings << root->marking();

    QSet <Transition *> deadTr;
    deadTr = net->transitions();

    while (newNodes.count()) {
        MarkingNode* node = newNodes.takeLast();
        allNodes.append(node);
        net->setCurrentMarking(node->marking());
        const QSet<Transition*>& activeTransitions = sim.activeTransitions();

        if (activeTransitions.count()) {

            //ищем мертвые переходы.
            deadTr.subtract(activeTransitions);

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

    return deadTr;
}

QSet<Transition *> NetProperty::GetPotentialLiveSubTree(PetriNet *pn, const Marking &inM)
{
    PetriNet * net = pn;
    pn->setCurrentMarking(inM);

    MarkingNode* root = new MarkingNode(0, pn->currentMarking());

    QSet <Transition *> pLiveTr;

    if (!pn->activeTransitionsCount())
        return pLiveTr;


    root->marking().normalize(net);
    QLinkedList<MarkingNode*> newNodes;
    QLinkedList<MarkingNode*> allNodes;
    newNodes.append(root);
    Simulation sim(net);

    QSet<Marking> markings;
    markings << root->marking();

    while (newNodes.count()) {
        MarkingNode* node = newNodes.takeLast();
        allNodes.append(node);
        net->setCurrentMarking(node->marking());
        const QSet<Transition*>& activeTransitions = sim.activeTransitions();

        if (activeTransitions.count()) {

            //ищем потенциально живые переходы
             pLiveTr.unite(activeTransitions);

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
        else {
            //return pLiveTr;
        }
    }

    return pLiveTr;
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

                foreach (Transition * tj, tmp) {
                    mPetriNet->setCurrentMarking(initialMarking);
                    sim.fireTransition(tj);
                    if (mPetriNet->activeTransitions().contains(t)) {
                        isStable = true;
                    }
                    else {
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
        } else {

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


#include "netproperty.moc"

Q_EXPORT_PLUGIN2(Opulus_NetProperty, NetProperty)
