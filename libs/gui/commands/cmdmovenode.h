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
#ifndef CMDMOVENODE_H
#define CMDMOVENODE_H

#include <QUndoCommand>
#include <QPointF>
#include "item.h"

class Node;
class PetriNet;

/**
	@author Adauto Filho <adautofilho@gmail.com>
*/
class CmdMoveNode : public QUndoCommand
{
public:
    CmdMoveNode(Node* node, const QPointF& pos);
    void undo();
	void redo();
private:
	QPointF mOldPos;
	QPointF mPos;
	ItemId mId;
	PetriNet* mPetriNet;
};

#endif