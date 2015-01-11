#ifndef NETPROPERTY_H
#define NETPROPERTY_H

#include <QObject>
#include "analyser.h"
#include "ui_netproperty.h"

/**
* This plugin analyzes Petri Net properties .
* @ingroup plugins
*/
class NetProperty : public QObject, public Analyser
{
    Q_OBJECT
    Q_INTERFACES(Analyser)
public:
    QString internalName() const;
    QString name() const;
    void analyse(PetriNet *pn, AnalysisReporter *reporter);
    bool setup(QWidget *parentWidget);
    void finish(QWidget *parentWidget);
private:
    PetriNet* mPetriNet;
    Ui::Dialog ui;
};

#endif
