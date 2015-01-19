#ifndef REACHABILITY_H
#define REACHABILITY_H

#include <QObject>
#include <QSpinBox>
#include <QVector>

//#include <matrix_util.h>
#include "analyser.h"

#include "ui_reachability.h"
#include "ui_results.h"

class QTableWidget;
class Marking;
//using namespace Eigen;
class ReachabilityPlugin : public QObject, public Analyser
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "opulus.sourceforge.net.Analyser")
    Q_INTERFACES(Analyser)
public:
    ReachabilityPlugin() {}
    virtual ~ReachabilityPlugin() {}

    // Analyser interface
public:
    QString internalName() const;
    QString name() const;
    void analyse(PetriNet *pn, AnalysisReporter *reporter);
    bool setup(QWidget *parentWidget);
    void finish(QWidget *parentWidget);

public slots:

    void setColumnNumber(int c);
private:
    PetriNet* mPetriNet;
    Ui::Reachability ui;
    Ui::Results results_ui;
    bool mAnalysisOk;
    bool _analysisPrepared;
    bool _isReachable;
    QWidget *_pWidget;
    QTableWidget *_startMtx;
    QTableWidget *_endMtx;
    QSpinBox *_spinBox;

    QVector<int> _startMarking;
    QVector<int> _endMarking;

    void setupMatrixNames();

    void calculate(AnalysisReporter *reporter);
    void resetAnalyser();
    int _maxTransitionLaunchCheck;
};


#endif
