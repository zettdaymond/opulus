#include "markingnode.h"



MarkingNode::MarkingNode(MarkingNode *parent, const Marking &m) : mParent(parent), mMarking(m) {
    applyOmega();
}

Marking &MarkingNode::marking() { return mMarking; }

void MarkingNode::applyOmega() {
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
