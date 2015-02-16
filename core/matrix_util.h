#ifndef MATRIX_UTIL_H
#define MATRIX_UTIL_H

#include <eigen3/Eigen/Dense>
class PetriNet;

Eigen::MatrixXi d_minus_matrix(const PetriNet *petrinet);
Eigen::MatrixXi d_plus_matrix(const PetriNet *petrinet);
Eigen::MatrixXi d_matrix(const PetriNet *petrinet);

enum class MatrixType {
	dMinusMatrix = 1, dPlusMatrix = 2, dMatrix = 3
};

struct PetriNetMatrices {
	PetriNetMatrices(
		Eigen::MatrixXi d_minus,
		Eigen::MatrixXi d_plus,
		Eigen::MatrixXi d_matrix) :
			dMinus(d_minus),
			dPlus(d_plus),
			dMatrix(d_matrix) { }

	Eigen::MatrixXi dMinus;
	Eigen::MatrixXi dPlus;
	Eigen::MatrixXi dMatrix;
};

#endif
