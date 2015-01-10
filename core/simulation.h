/***************************************************************************
 *   Copyright (C) 2007 by Hugo Parente Lima <hugo.pl@gmail.com>           *
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

#ifndef SIMULATION_H
#define SIMULATION_H

#include "marking.h"

class PetriNet;
class Transition;

/**
* Helper class to simulate a petri net, it just restore the initial marking on destructor.
* @ingroup core
*/
class Simulation {
public:
	/**
	* Start a new simulation.
	* @throws NoActiveTransitionsException
	*/
	explicit Simulation(PetriNet* petriNet);
	/// Finish the simulation and restore the original marking.
	~Simulation();

	/**
	* Fire a random transition.
	* @throws NoActiveTransitionsException
	*/
	void fireRandomTransition();
	/**
	* Fire n random transitions.
	* @throws NoActiveTransitionsException
	*/
	void fireNRandomTransitions(int n);
	/**
	* Fire the ransition @p t.
	* This is equivalent to: @code t->fire(); @endcode
	*/
	void fireTransition(Transition* t);
	/// Returns a collection with all active transitions.
	const QSet<Transition*>& activeTransitions() const;
private:
	PetriNet* mPetriNet;
	Marking mInitialMarking;
};
#endif
