#include "reachability.h"
#include "petrinet.h"
#include "analysisreporter.h"
#include "place.h"

#include <QWidget>
#include <QDialog>
#include <QTableWidget>
#include "ui_reachability.h"

QString ReachabilityPlugin::internalName() const
{
    return "reachability";
}

QString ReachabilityPlugin::name() const
{
    return "Reachability of Marking";
}

void ReachabilityPlugin::analyse(PetriNet *pn, AnalysisReporter *reporter)
{
    mPetriNet = pn;
    if (!mPetriNet->placeCount()) {
        reporter->fatalError(tr("There are no places in this petri net."));
        return;
    }
    //because plugin interface doesn't support widget pre-preparing,
    //we need prepare widget in method finish();
    if (_analysisPrepared == false) {
        return;
    }
}

bool ReachabilityPlugin::setup(QWidget *parentWidget)
{
    _pWidget = parentWidget;
    _analysisPrepared = false;



    return true;
}

void ReachabilityPlugin::finish(QWidget *parentWidget)
{
    if (_analysisPrepared == false) {
        QDialog dlg(parentWidget);
        ui.setupUi(&dlg);

        //setup ui
        //copy UI pointers
        _startMtx = ui.tableWidget_3;
        _endMtx = ui.tableWidget_2;

        setupMatrixNames();
        dlg.exec();

        //TODO:  handling Ok. cancel button
        //TODO: if OK -> create new analyse thread -> call analise
    }
}

void ReachabilityPlugin::setupMatrixNames()
{
    if (_startMtx && _endMtx == 0)
        return;
    _startMtx->setColumnCount(mPetriNet->placeCount());
    _endMtx->setColumnCount(mPetriNet->placeCount());

    //setup names
    QStringList strLst;
    mPetriNet->currentMarking().normalize(mPetriNet);
    foreach(Place * p, mPetriNet->places()){
        strLst += p->name();
    }
    _startMtx->setHorizontalHeaderLabels(strLst);
    _endMtx->setHorizontalHeaderLabels(strLst);

    //setup start values;
    int i = 0;
    foreach(Place * p, mPetriNet->places()){
        QTableWidgetItem * newItem = new QTableWidgetItem(QString::number( p->numTokens() ));
        _startMtx->setItem(_startMtx->rowCount() - 1, i, newItem);
        i++;
    }

    //TODO: Setup RegEx on input (only integer)
    //TODO: Add Places sort;
}


