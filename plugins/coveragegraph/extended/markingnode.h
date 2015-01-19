
#include "marking.h"
class MarkingNode {
public:

    MarkingNode(MarkingNode* parent, const Marking& m);

    Marking& marking();

private:
    MarkingNode* mParent;
    Marking mMarking;

    void applyOmega();
};
