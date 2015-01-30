#ifndef PRESERVING_MATRIX_ANALYSER_H
#define PRESERVING_MATRIX_ANALYSER_H

#include <QObject>

class Transition;
class Marking;
class Node;
class MarkingNode;
class PetriNet;

class PreservingMatrixAnalyser : public QObject
{
    Q_OBJECT
public:
    PreservingMatrixAnalyser(PetriNet * pn);
    virtual ~PreservingMatrixAnalyser() {}

    bool analyse();
    bool isPreserving() const;
    bool isStrictlyPreserving() const;

private:
    void checkPreserving();

    bool _isPreserving;
    bool _isStrictlyPreserving;
    static constexpr float eqSolverE = 0.000001;

    PetriNet * _petriNet;
};

#endif
