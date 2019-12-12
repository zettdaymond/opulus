/*
* Copyright (C) 2015 Zett Daymond zettday@gmail.com and catgirl captainslowpoke@gmail.com
*
* This software may be modified and distributed under the terms
* of the MIT license.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/

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
    if (_petriNet != nullptr)
        return _stableTransitions.contains(_petriNet->transitions());
    else
        return false;
}

bool TransitionAnalyser::isNetLive()
{
    if (_petriNet != nullptr) {
        foreach(Transition * t, _petriNet->transitions()) {
            if (!_liveTransitions.contains(t)) {
                return false;
            }
        }
        return true;
    }
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

    MarkingNode* root = new MarkingNode(nullptr, _petriNet->currentMarking());
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
    MarkingNode* root = new MarkingNode(nullptr, _petriNet->currentMarking());
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

    MarkingNode* root = new MarkingNode(nullptr, _petriNet->currentMarking());
    root->marking().normalize(_petriNet);
    QLinkedList<MarkingNode*> newNodes;
    QLinkedList<MarkingNode*> allNodes;
    newNodes.append(root);
    Simulation sim(_petriNet);

    QSet<Marking> markings;
    markings << root->marking();

    const auto rootMarking = root->marking();

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
    _petriNet->setCurrentMarking(rootMarking);
}

void TransitionAnalyser::stabilityAnalyse()
{
    QSet <Transition *> _stable;
    MarkingNode* root = new MarkingNode(nullptr, _petriNet->currentMarking());
    root->marking().normalize(_petriNet);
    QLinkedList<MarkingNode*> newNodes;
    QLinkedList<MarkingNode*> allNodes;
    newNodes.append(root);
    Simulation sim(_petriNet);

    QSet<Marking> markings;
    markings << root->marking();

    const auto rootMarking = root->marking();

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
                bool isStable = true;

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
    _petriNet->setCurrentMarking(rootMarking);
    qDeleteAll(allNodes);

    _stableTransitions = _stable;
}



TransitionAnalyser::TransitionAnalyser(PetriNet* pn) {
    _petriNet = pn;
}

bool TransitionAnalyser::analyse()
{
    if (_petriNet == nullptr) {
        return false;
    }
    _deadTransitions = _petriNet->transitions();
    _liveTransitions = _petriNet->transitions();

    activityLevelAnalyse();
    stabilityAnalyse();
    return true;
}
