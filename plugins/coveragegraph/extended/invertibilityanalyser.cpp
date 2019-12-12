/*
* Copyright (C) 2015-2016 Zett Daymond <zettday@gmail.com> and catgirl <captainslowpoke@gmail.com>
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

#include "invertibilityanalyser.h"
#include "petrinet.h"
#include "markingnode.h"
#include "simulation.h"
bool InvertibilityAnalyser::analyse()
{
    if (_petriNet == nullptr) {
        return false;
    }

    Marking saveMrk = _petriNet->currentMarking();
    MarkingNode* root = new MarkingNode(nullptr, _petriNet->currentMarking());

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
                    qDeleteAll(allNodes);
                    _petriNet->setCurrentMarking(saveMrk);
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
    if (_petriNet->currentMarking() == startMarking) {
        return true;
    }
	if (_petriNet->activeTransitionsCount() == 0 ) {
		return false;
	}
    Marking saveMrk = _petriNet->currentMarking();
    _petriNet->setCurrentMarking(marking);
    MarkingNode* root = new MarkingNode(nullptr, _petriNet->currentMarking());

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
    if (_petriNet != nullptr)
        startMarking = _petriNet->currentMarking();
}
bool InvertibilityAnalyser::isInvertibility() const
{
    return _isInvertibility;
}
