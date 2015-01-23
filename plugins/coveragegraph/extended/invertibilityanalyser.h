#ifndef INVERTIBILITY_ANALYSER_H
#define INVERTIBILITY_ANALYSER_H

#include <QObject>

#include "marking.h"
class PetriNet;
class InvertibilityAnalyser : public QObject
{
    Q_OBJECT
public:
    InvertibilityAnalyser(PetriNet * pn);
    virtual ~InvertibilityAnalyser() {}
    
    bool isInvertibility() const;
    
    bool analyse();
private:
    bool isStartReachableFromMarking(Marking marking);
    bool _isInvertibility;
    PetriNet *_petriNet;
    Marking startMarking;
};
#endif
