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
