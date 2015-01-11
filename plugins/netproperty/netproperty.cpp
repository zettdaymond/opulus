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

    if (!mPetriNet->placeCount()) {
        reporter->fatalError(tr("There are no places in this petri net."));
        return;
    }

    buidTree();

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

    out += "Petri Net is \n";
    out += "\tSafety : " + bToStr(_isSafety) + "\n";
    out += "\tRestricted : " + bToStr(_isRestricted) + "\n";

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
        const QSet<Transition*>& activeTransitions = sim.activeTransitions();

        if (activeTransitions.count()) {
            //ищем мертвые переходы.
            _deadTransitions.subtract(activeTransitions);

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

                //ищем потенциально-мертвые переходы
                //begin
                //Marking curMarking = mPetriNet->currentMarking();
                QSet<Transition *> dt = GetDeadSubTree(mPetriNet, child->marking());
                _potentialDeadTransitions.unite(dt);
                //mPetriNet->setCurrentMarking(curMarking);
                //end

                if (!markings.contains(child->marking())) {
                    newNodes.prepend(child);
                    markings << child->marking();
                } else
                    delete child;
            }


        } else {
            //scriptStream << '\"' << node->marking() << "\" [color = red]\n";
        }
    }
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

QString NetProperty::bToStr(bool b)
{
    if (b)
        return "true";
    else
        return "false";
}


#include "netproperty.moc"

Q_EXPORT_PLUGIN2(Opulus_NetProperty, NetProperty)
