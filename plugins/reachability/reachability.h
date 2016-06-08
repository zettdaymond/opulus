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

#ifndef REACHABILITY_H
#define REACHABILITY_H

#include <QObject>
#include <QSpinBox>
#include <QVector>

//#include <matrix_util.h>
#include "analyser.h"

#include "ui_reachability.h"
#include "ui_results.h"

#ifdef BUILD_STATIC_PLUGINS
#	define QT_STATICPLUGIN
#endif

class QTableWidget;
class Marking;
//using namespace Eigen;
class ReachabilityPlugin : public QObject, public Analyser
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "opulus.sourceforge.net.Analyser")
    Q_INTERFACES(Analyser)
public:
	ReachabilityPlugin();
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
