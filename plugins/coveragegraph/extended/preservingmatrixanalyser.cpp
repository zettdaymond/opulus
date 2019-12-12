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

#include "preservingmatrixanalyser.h"
#include "matrix_util.h"
using namespace Eigen;
PreservingMatrixAnalyser::PreservingMatrixAnalyser(PetriNet * pn) {
    _petriNet =  pn;
}

bool PreservingMatrixAnalyser::analyse()
{
    if (_petriNet == nullptr) {
        return false;
    }
    _isPreserving = false;
    _isStrictlyPreserving = false;

    checkPreserving();
    return true;
}
bool PreservingMatrixAnalyser::isPreserving() const
{
    return _isPreserving;
}

bool PreservingMatrixAnalyser::isStrictlyPreserving() const
{
    return _isStrictlyPreserving;
}

void PreservingMatrixAnalyser::checkPreserving()
{
    Eigen::MatrixXi d_mtx = d_matrix(_petriNet);


    //проверим на строгое сохранение
    Eigen::VectorXi W( d_mtx.cols() );

    for (int i = 0; i < d_mtx.cols(); i++) {
        W(i) = 1;
    }

    _isStrictlyPreserving = true;

    Eigen::MatrixXi result = d_mtx * W;
    for (int i = 0; i < result.rows(); i++) {
        if (result(i) != 0) {
            _isStrictlyPreserving = false;
            break;
        }
    }
    if (_isStrictlyPreserving == true) {
        _isPreserving = true;
        return;
    }

    //DW = 0 -> W = kernel(D) ->
    // solution vector = kernel * x, where x
    //is vector of free variables

    //WARNING: method cast() might bring some problems
    //WARNING: check this, if solution isn't real;
    FullPivLU<MatrixXf> lu(d_mtx.cast<float>());
    MatrixXf kernel = lu.kernel();

    _isPreserving = true;
    for (int inequality = 0; inequality < kernel.rows(); inequality++) {
        bool thereIsPositive = false;
        for (int coeff = 0; coeff < kernel.cols(); coeff++) {
            if ( kernel(inequality,coeff) > 0 ){
                //if there is at least one positive cofficient
                //in inequation, we can anyway choose such values of the arguments,
                //that make inequality is solvable.
                thereIsPositive = true;
                break;
            }
            else if (kernel(inequality,coeff) == 0) {
                continue;
            }
        }
        if (thereIsPositive == false) {
            _isPreserving = false;
            return;
        }
    }
    return;
}


