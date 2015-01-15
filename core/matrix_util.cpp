#include "matrix_util.h"
#include "petrinet.h"
#include "abstractarc.h"
#include "transition.h"
#include <stdexcept>
#include <QDebug>

using namespace Eigen;

MatrixXi d_minus_matrix(const PetriNet *petrinet)
{
	if(!petrinet) return MatrixXi::Zero(0, 0);

	const QSet<Transition*> transitions = petrinet->transitions();
	const QSet<Place*> places = petrinet->places();
	const int trans_cnt = transitions.size();
	const int places_cnt = places.size();

	MatrixXi d_minus = MatrixXi::Zero(trans_cnt, places_cnt);

	foreach (Transition* t, transitions) {
		bool ok = false;
		int t_index = -1;
		if(t->name().startsWith('T')) {
			QString name = t->name();
			t_index = name.remove(0,1).toInt(&ok);
			//qDebug() << "found t index" << t_index;
		}
		if(!ok) throw std::runtime_error("Invalid transition name");

		foreach (AbstractArc* arc, t->inputArcs()) {
			bool ok = false;
			int p_index;
			if(arc->from()->name().startsWith('P')) {

				QString name = arc->from()->name();
				p_index = name.remove(0,1).toInt(&ok);
				//qDebug() << "found p index" << p_index;
			}
			if(!ok) throw std::runtime_error("Invalid place name");

			++d_minus(t_index, p_index);

		}
	}
	return d_minus;
}


MatrixXi d_plus_matrix(const PetriNet *petrinet)
{
	if(!petrinet) return MatrixXi::Zero(0,0);

	const QSet<Transition*> transitions = petrinet->transitions();
	const QSet<Place*> places = petrinet->places();
	const int trans_cnt = transitions.size(), places_cnt = places.size();

	MatrixXi d_plus = MatrixXi::Zero(trans_cnt, places_cnt);

	foreach (Transition* t, transitions) {
		bool ok = false;
		int t_index = -1;
		if(t->name().startsWith('T')) {
			QString name = t->name();
			t_index = name.remove(0,1).toInt(&ok);
			//qDebug() << "found t index" << t_index;
		}
		if(!ok) throw std::runtime_error("Invalid transition name");

		foreach (AbstractArc* arc, t->outputArcs()) {
			bool ok = false;
			int p_index;
			if(arc->to()->name().startsWith('P')) {

				QString name = arc->to()->name();
				p_index = name.remove(0,1).toInt(&ok);
				//qDebug() << "found p index" << p_index;
			}
			if(!ok) throw std::runtime_error("Invalid place name");

			++d_plus(t_index, p_index);

		}
	}
	return d_plus;
}

MatrixXi d_matrix(const PetriNet *petrinet)
{
	return d_plus_matrix(petrinet) - d_minus_matrix(petrinet);
}
