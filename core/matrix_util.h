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
