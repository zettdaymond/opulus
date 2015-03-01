#include "reachability.h"
#include "petrinet.h"
#include "analysisreporter.h"
#include "place.h"
#include "ui_reachability.h"
#include "matrix_util.h"
#include "delegate.h"

#include <QWidget>
#include <QDialog>
#include <QTableWidget>
#include <QMessageBox>
#include <QDebug>
QString ReachabilityPlugin::internalName() const
{
    return "reachability";
}

QString ReachabilityPlugin::name() const
{
    return tr("Reachability of Marking");
}

void ReachabilityPlugin::analyse(PetriNet *pn, AnalysisReporter *reporter)
{
    mPetriNet = pn;
    if (!mPetriNet->placeCount()) {
        reporter->fatalError(tr("There are no places in this petri net."));
        return;
    }

    if (!mPetriNet->transitions().count()) {
        reporter->fatalError(tr("There are no transitions in this petri net."));
        return;
    }

    if (mPetriNet->placeCount() != _endMarking.count()) {
        qDebug() << "NetCount : " << mPetriNet->placeCount() << "\n";
        qDebug() << "End Mark: " << _endMarking.count() << "\n";
        reporter->fatalError(tr("Place count in your 'End marking' doesn't equal petri nets place count"));
        return;
    }

    calculate(reporter);
}

bool ReachabilityPlugin::setup(QWidget *parentWidget)
{
    _pWidget = parentWidget;
    _analysisPrepared = false;
    _isReachable = false;
    mAnalysisOk = false;
    //setup ui
    QDialog dlg(parentWidget);
    ui.setupUi(&dlg);

    //copy UI pointers
    //_startMtx = ui.tableWidget_StartMrk;
    _endMtx = ui.tableWidget_EndMrk;
    _spinBox = ui.spinBoxPNumber;


    _maxTransitionLaunchCheck = ui.spinBoxMaxLaunch->value();
    //BUG: value doesn't changed
    qDebug() << "Max transition launch :" << _maxTransitionLaunchCheck;

    connect(_spinBox, SIGNAL(valueChanged(int)),this,SLOT(setColumnNumber(int)));

    //Only numbers permitted
    //_startMtx->setItemDelegate(new Delegate())
    _endMtx->setItemDelegate(new Delegate());

    if ( dlg.exec() == QDialog::Accepted) {
        for(int col = 0; col < _endMtx->columnCount(); col++){
            QTableWidgetItem * item = _endMtx->item(_endMtx->rowCount() - 1 ,col);
            if (item == 0){
                QMessageBox::critical(parentWidget, tr("Error"),tr("Some of cells is empty"));
                return false;
            }
            _endMarking.push_back(item->text().toInt());
        }
        return true;
    }
    else
        return false;
}

void ReachabilityPlugin::finish(QWidget *parentWidget)
{
    if (mAnalysisOk == true) {
        QString out = "";
        out +=  tr("<h1>Analysis Results</h1>") +
                tr("<hr />");

        out += "<ul>";
        out +=  tr("<li>Marking : (");
        foreach (int i, _endMarking) {
            out += QString::number(i) + " ";
        }
        out += ")</li>";
        out +=  tr("<li>Reachable :");
        if (_isReachable == true) {
            out += tr("true </li>");
        }
        else {
            out += tr("false </li>");
        }
        out += "</ul>";
        QDialog dlg(parentWidget);
        results_ui.setupUi(&dlg);

        results_ui.textEdit->setHtml(out);
        dlg.exec();
        resetAnalyser();
    }
}

void ReachabilityPlugin::setColumnNumber(int c)
{
    // _startMtx->reset();
    _endMtx->reset();

    //  _startMtx->setColumnCount(c);
    _endMtx->setColumnCount(c);

    //setup names
    QStringList strLst;
    for (int i = 0; i < c; i++) {
        strLst += "P" + QString::number(i);
    }

    // _startMtx->setHorizontalHeaderLabels(strLst);
    _endMtx->setHorizontalHeaderLabels(strLst);
}

void ReachabilityPlugin::setupMatrixNames()
{
    if (_endMtx == 0)
        return;
    //_startMtx->setColumnCount(mPetriNet->placeCount());
    _endMtx->setColumnCount(mPetriNet->placeCount());

    //setup names
    QStringList strLst;
    mPetriNet->currentMarking().normalize(mPetriNet);
    foreach(Place * p, mPetriNet->places()){
        strLst += p->name();
    }
    //_startMtx->setHorizontalHeaderLabels(strLst);
    _endMtx->setHorizontalHeaderLabels(strLst);

    //setup start values;
    int i = 0;
    foreach(Place * p, mPetriNet->places()){
        QTableWidgetItem * newItem = new QTableWidgetItem(QString::number( p->numTokens() ));
        //_startMtx->setItem(_startMtx->rowCount() - 1, i, newItem);
        i++;
    }
}

void ReachabilityPlugin::calculate(AnalysisReporter *reporter)
{
    reporter->setStatusMessage(tr("Prepare..."));
    //Recieve D matrix
    Eigen::MatrixXi d_mtx = d_matrix(mPetriNet);

    qDebug() << "D row : " << d_mtx.rows();
    qDebug() << "D col : " << d_mtx.cols();
    if (d_mtx.rows() == 0 || d_mtx.cols() == 0) {
        reporter->fatalError(tr("Somthing has wrohg... Restart programm and check your petri net"));
    }

    //Recieve u(initial) marking vector
    Marking initMark = mPetriNet->currentMarking();
    initMark.normalize(mPetriNet);
    qDebug() << "after normalize : \n";
    qDebug() << "Order : ";

    Eigen::RowVectorXi start_m(mPetriNet->placeCount());

    qDebug() << "Start: ";
    int prt = 0;
    foreach (const Place * place, initMark.printingOrder()) {
        start_m(prt) = place->numTokens(); //P0 P1 P2 etc...
        qDebug() << place->numTokens();
        prt++;
    }

    Eigen::RowVectorXi end_m(mPetriNet->placeCount());

    qDebug() << "End: ";
    for (prt = 0; prt < mPetriNet->placeCount(); prt++ ) {
        end_m(prt) = _endMarking[prt];
        qDebug() << end_m(prt);
    }

    //solve equation:
    // end = start + x*D
    Eigen::RowVectorXi search_m = end_m - start_m;
    qDebug() << "end_m row : " << end_m.rows();
    qDebug() << "end_m col : " << end_m.cols();

    Eigen::RowVectorXi x( d_mtx.rows() );
    qDebug() << "x row : " << x.rows();
    qDebug() << "x col : " << x.cols();
    for (int j = 0; j < d_mtx.rows(); j++) {
        x(j) = 0;
    }

    reporter->setStatusMessage(tr("Calculating..."));
    reporter->setPercenage(0);

    //TODO: come up with a new algorithm and rewrite code;
    int dim = 0;
    int max_dim = d_mtx.rows();
    int max_num = _maxTransitionLaunchCheck;
    while(true) {
        if (x(dim) == max_num) {
            dim++;
            if (dim == max_dim)
                break;
            for (int i =0; i < dim; i++) {
                x(i) = 0;
            }
            if(dim == max_dim-1) {
                reporter->setPercenage((int)( x(dim) * 100 / max_num) );
            }
        } else {
            x(dim) += 1;
            //Threat
            bool flag = true;
            Eigen::RowVectorXi tmpResult = x * d_mtx;
            for (int i = 0; i < tmpResult.cols(); i++) {
                if (tmpResult(i) != search_m(i)) {
                    flag = false;
                    break;
                }
            }
            dim = 0;
            if (flag == true) {
                _isReachable = true;
                break;
            }
        }
    }
    reporter->setPercenage(100);
    qDebug() << "Reachable : " << _isReachable;
    mAnalysisOk = true;
}

void ReachabilityPlugin::resetAnalyser()
{
    mAnalysisOk = false;
    _analysisPrepared = false;
    _isReachable = false;
    _startMarking.clear();
    _endMarking.clear();
}


