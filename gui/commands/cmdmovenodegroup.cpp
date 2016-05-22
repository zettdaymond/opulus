/****************************************************************************
 *   Copyright (C) 2008 by Hugo Parente Lima <hugo.pl@gmail.com>            *
 *   Copyright (C) 2008 by Adauto T. de Almeida Filho <adautofilho@gmail.com*
 *                                                                          *
 *   This program is free software; you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation; either version 2 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   You should have received a copy of the GNU General Public License      *
 *   along with this program; if not, write to the                          *
 *   Free Software Foundation, Inc.,                                        *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.           *
 ****************************************************************************/
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



