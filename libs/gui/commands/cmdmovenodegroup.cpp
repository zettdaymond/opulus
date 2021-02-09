/*
* Copyright (C) 2016 Zett Daymond <zettday@gmail.com>
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

#include "cmdmovenodegroup.h"
#include "node.h"
#include "petrinet.h"

CmdMoveNodeGroup::CmdMoveNodeGroup(QVector<Node*> &nodes, const QVector<QPointF> &poses) :
	mPoses(poses),
	mOldPoses(poses.size())
{
	for(const auto node : nodes) {
		Q_ASSERT(node);
		mIds.push_back(node->id());
	}

	Q_ASSERT(nodes[0]);
	mPetriNet = nodes[0]->petriNet();

	Q_ASSERT(mIds.size() == mPoses.size());
}

void CmdMoveNodeGroup::undo() {
	for(auto i{0}; i < mIds.size(); i++) {
		static_cast<Node*>(mPetriNet->item(mIds[i]))->setPos(mOldPoses[i]);
	}
}

void CmdMoveNodeGroup::redo() {
	Q_ASSERT(mOldPoses.size() == mPoses.size() and mPoses.size() == mIds.size());
	for(auto i{0}; i < mIds.size(); i++) {
		Node* node = static_cast<Node*>(mPetriNet->item(mIds[i]));
		mOldPoses[i] = node->pos();
		node->setPos(mPoses[i]);
	}
}



