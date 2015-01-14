#ifndef REACHABILITY_H
#define REACHABILITY_H

#include <QObject>
#include "analyser.h"

#include "ui_reachability.h"
class QTableWidget;
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


private:
    PetriNet* mPetriNet;
    Ui::Dialog ui;
    bool mAnalysisOk;
    bool _analysisPrepared;

    QWidget *_pWidget;
    QTableWidget *_startMtx;
    QTableWidget *_endMtx;

    void setupMatrixNames();

};


#endif
