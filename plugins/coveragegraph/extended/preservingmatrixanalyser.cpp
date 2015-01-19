#include "preservingmatrixanalyser.h"
#include "matrix_util.h"

PreservingMatrixAnalyser::PreservingMatrixAnalyser(PetriNet * pn) {
    _petriNet =  pn;
}

bool PreservingMatrixAnalyser::analyse()
{
    if (_petriNet == 0) {
        return false;
    }
    _isPreserving = false;
    _isStrictlyPreserving = false;

    checkPreserving();

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

    //TODO: rewrite matrix multiply to solving linear system
    Eigen::VectorXi xW( d_mtx.cols() );
    for (int j = 0; j < d_mtx.cols(); j++) {
        xW(j) = 0;
    }

    int dim = 0;
    int max_dim = d_mtx.cols();
    int max_num = 30;
    while(true) {
        if (xW(dim) == max_num) {
            dim++;
            if (dim == max_dim)
                break;
            for (int i =0; i < dim; i++) {
                xW(i) = 0;
            }
        } else {
            xW(dim) += 1;
            //Threat
            bool flag = true;
            Eigen::VectorXi tmpResult = d_mtx * xW;
            for (int i = 0; i < tmpResult.rows(); i++) {
                if (tmpResult(i) != 0) {
                    flag = false;
                    break;
                }
            }
            dim = 0;
            if (flag == true) {
                _isPreserving = true;
                break;
            }
        }
    }

    return;
}


