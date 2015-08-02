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

#ifndef TRANSITION_ANALYSER_H
#define TRANSITION_ANALYSER_H

#include <QObject>
#include <QSet>
class Transition;
class Marking;
class Node;
class MarkingNode;
class PetriNet;
class TransitionAnalyser : public QObject
{
    Q_OBJECT
public:
    TransitionAnalyser(PetriNet *pn);
    virtual ~TransitionAnalyser() {}

public:
    bool analyse();

    QSet<Transition *> deadTransitions() const;
    QSet<Transition *> potentialDeadTransitions() const;
    QSet<Transition *> potentialLiveTransitions() const;
    QSet<Transition *> liveTransitions() const;
    QSet<Transition *> stableTransitions() const;

    bool isNetStable();
    bool isNetLive();

private:

    QSet<Transition *>  deadTransitionInSubTree(const Marking &startMarking);
    QSet<Transition *> ptnLiveTransitionsInSubTree(const Marking &startMarking);

    void activityLevelAnalyse();
    void stabilityAnalyse();

    QSet <Transition *> _deadTransitions;
    QSet <Transition *> _potentialDeadTransitions;
    QSet <Transition *> _potentialLiveTransitions;
    QSet <Transition *> _liveTransitions;
    QSet <Transition *> _stableTransitions;

    PetriNet * _petriNet;
};

#endif
