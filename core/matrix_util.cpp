/*
* Copyright (C) 2015 Zett Daymond zettday@gmail.com and catgirl captainslowpoke@gmail.com
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

#include "matrix_util.h"
#include "petrinet.h"
#include "abstractarc.h"
#include "transition.h"
#include <stdexcept>
#include <QDebug>

using namespace Eigen;

MatrixXi d_minus_matrix(const  PetriNet *petrinet)
{
	if(!petrinet) return MatrixXi::Zero(0, 0);

	const QSet<Transition*> transitions = petrinet->transitions();
	const QSet<Place*> places = petrinet->places();
	const int trans_cnt = transitions.size();
	const int places_cnt = places.size();

	MatrixXi d_minus = MatrixXi::Zero(trans_cnt, places_cnt);

	foreach (Transition* t, transitions) {
		Q_ASSERT(t->number() >= 0);
		foreach (AbstractArc* arc, t->inputArcs()) {
			Q_ASSERT(arc->from()->number() >= 0);
			d_minus(t->number(), arc->from()->number()) += arc->weight();
		}
	}
	return d_minus;
}


MatrixXi d_plus_matrix(const PetriNet * petrinet)
{
	if(!petrinet) return MatrixXi::Zero(0,0);

	const QSet<Transition*> transitions = petrinet->transitions();
	const QSet<Place*> places = petrinet->places();
	const int trans_cnt = transitions.size(), places_cnt = places.size();

	MatrixXi d_plus = MatrixXi::Zero(trans_cnt, places_cnt);

	foreach (Transition* t, transitions) {
		Q_ASSERT(t->number() >= 0);
		foreach (AbstractArc* arc, t->outputArcs()) {
			Q_ASSERT(arc->to()->number() >= 0);
			d_plus(t->number(), arc->to()->number()) += arc->weight();
		}
	}
	return d_plus;
}

MatrixXi d_matrix(const PetriNet *petrinet)
{
	return d_plus_matrix(petrinet) - d_minus_matrix(petrinet);
}
