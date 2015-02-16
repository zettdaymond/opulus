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

#ifndef ARC_H
#define ARC_H

#include "abstractarc.h"

class PetriNet;
class Node;
class Place;
class Transition;
class QDomElement;

/**
* An petri net normal arc.
* @ingroup core
*/
class Arc : public AbstractArc {
friend class PetriNet;
public:
	bool canFireTransition() const;
	void fire();
	void setWeight(uint weight) override;

	void save(QXmlStreamWriter& out);
	static void load(PetriNet* pn, const QDomElement& elem);
protected:

protected:
	Arc(PetriNet* pn, Place* from, Transition* to, const ItemId& id);
	Arc(PetriNet* pn, Transition* from, Place* to, const ItemId& id);
private:

	void init();
};

#endif
