#ifndef MATRIX_UTIL_H
#define MATRIX_UTIL_H

#include <eigen3/Eigen/Dense>
class PetriNet;

Eigen::MatrixXi d_minus_matrix(const PetriNet* const petrinet);
Eigen::MatrixXi d_plus_matrix(const PetriNet* const petrinet);
Eigen::MatrixXi d_matrix(PetriNet* petrinet);

#endif
