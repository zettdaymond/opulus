/***************************************************************************
 *   Copyright (C) 2007 by Hugo Parente Lima <hugo.pl@gmail.com>           *
 *   Copyright (C) 2007 by Adauto Trigueiro <adautofilho@gmail.com>        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 ***************************************************************************/

#ifndef TRANSITION_H
#define TRANSITION_H

#include "node.h"

/**
* A petri net transition.
* @see PetriNet#createTransition
* @ingroup core
*/
class Transition : public Node {
	friend class PetriNet;
public:
	/**
	* Return true if this transition can be fired.
	* @throw CantFireTransitionException
	*/
	bool canFire();
	/// Triggers this transition.
	void fire();
	/// Sets the transition rotation in degrees. This info is used by the GUI to draw the transition.
	void setRotation(double rotation);
	/// Returns the transition rotation in degrees.
	double rotation() const { return mRotation; }
	/// Updates the transition status (can/can't fire).
	void updateStatus();
	void save(QXmlStreamWriter& out);
	static void load(PetriNet* pn, const QDomElement& elem);

private:
	Transition(PetriNet* pn, const QPointF& pos, const ItemId& id);

	double mRotation;
	bool mCanFire;
};

#endif
