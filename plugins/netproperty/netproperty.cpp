#include "netproperty.h"
#include <QtPlugin>
#include <QDialog>

QString NetProperty::internalName() const
{
    return QLatin1String("netProperty");
}

QString NetProperty::name() const
{
    return tr("Net Property");
}

void NetProperty::analyse(PetriNet *pn, AnalysisReporter *reporter)
{

}

bool NetProperty::setup(QWidget *parentWidget)
{
    return true;
}

void NetProperty::finish(QWidget *parentWidget)
{
    QDialog dlg(parentWidget);
    ui.setupUi(&dlg);
    dlg.exec();
}

#include "netproperty.moc"

Q_EXPORT_PLUGIN2(Opulus_NetProperty, NetProperty)
