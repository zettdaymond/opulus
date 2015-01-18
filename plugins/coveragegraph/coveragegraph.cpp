/***************************************************************************
 *   Copyright (C) 2007 by Hugo Parente Lima <hugo.pl@gmail.com>           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 ***************************************************************************/

#include "coveragegraph.h"
#include "marking.h"
#include "place.h"
#include "transition.h"
#include "petrinet.h"
#include "simulation.h"
#include "exceptions.h"
#include "analysisreporter.h"
#include <QtPlugin>
#include <QMessageBox>
#include <QTextStream>
#include <QLinkedList>
#include <QProcess>
#include <QStringList>
#include <QImage>
#include <QDialog>
#include <QGraphicsScene>
#include <QTemporaryFile>
#include <QGraphicsSvgItem>
#include <QSvgRenderer>
#include <QSet>
#include <QDebug>
#include <QDir>

class MarkingNode {
public:

	MarkingNode(MarkingNode* parent, const Marking& m) : mParent(parent), mMarking(m) {
		applyOmega();
	}

	Marking& marking() { return mMarking; }

private:
	MarkingNode* mParent;
	Marking mMarking;

	void applyOmega() {
		// apply omega if necessary
		MarkingNode* node = this;
		while(node->mParent) {
			node = node->mParent;
			if (mMarking > node->mMarking) {
				mMarking.applyOmega(node->mMarking);
				break;
			}
		}
	}
};

QString CoverageGraph::internalName() const {
	return QLatin1String("coverageGraph");
}

QString CoverageGraph::name() const {
	return tr("Coverage Graph");
}

void CoverageGraph::analyse(PetriNet* pn, AnalysisReporter* reporter) {
	mAnalysisOk = false;
	mPetriNet = pn;

	if (!mPetriNet->placeCount()) {
		reporter->fatalError(tr("There are no places in this petri net."));
		return;
	}

	reporter->setStatusMessage(tr("Creating graph..."));

	QTemporaryFile tempFile;
	tempFile.open();
	QTextStream scriptStream(&tempFile);
	scriptStream.setCodec("UTF-8");
	scriptStream << "digraph G {\n";
	scriptStream << "node [shape=box, style=rounded, fontsize=12, height=0.2]\n";
	scriptStream << "edge [fontsize=12]\n";

	MarkingNode* root = new MarkingNode(0, mPetriNet->currentMarking());
	root->marking().normalize(mPetriNet);
	QLinkedList<MarkingNode*> newNodes;
	QLinkedList<MarkingNode*> allNodes;
	newNodes.append(root);
	Simulation sim(mPetriNet);

	QSet<Marking> markings;
	markings << root->marking();
	scriptStream << '\"' << root->marking() << "\" [color = green]\n";

	while (newNodes.count()) {
		MarkingNode* node = newNodes.takeLast();
		allNodes.append(node);
		mPetriNet->setCurrentMarking(node->marking());
		const QSet<Transition*>& activeTransitions = sim.activeTransitions();

		if (activeTransitions.count()) {
			Marking initialMarking = mPetriNet->currentMarking();
			foreach(Transition* t, activeTransitions) {
				mPetriNet->setCurrentMarking(initialMarking);
				sim.fireTransition(t);

				MarkingNode* child = new MarkingNode(node, mPetriNet->currentMarking());
				writeNode(scriptStream, node->marking(), child->marking(), t->name());

				if (!markings.contains(child->marking())) {
					newNodes.prepend(child);
					markings << child->marking();
				} else
					delete child;
			}
		} else {
			scriptStream << '\"' << node->marking() << "\" [color = red]\n";
		}
	}
	scriptStream << '}';
	scriptStream.flush();
	reporter->setStatusMessage(tr("Creating graph image with %1 nodes...").arg(allNodes.count()));
	reporter->setPercenage(40);

	mMarkingOrder.clear();
	foreach (const Place* p, root->marking().printingOrder()) {
		mMarkingOrder += p->name();
		mMarkingOrder += ", ";
	}
	mMarkingOrder.remove(mMarkingOrder.length()-2, 2);

	qDeleteAll(allNodes);

	QProcess dot;
	QString exec = QDir::toNativeSeparators(qApp->applicationDirPath()+"/graphviz/bin/dot.exe");
	QStringList args;
	args << "-Tsvg" << tempFile.fileName();
	qDebug() << exec;
	dot.start(exec, args);

	// wait...
	dot.waitForFinished(-1);
	reporter->setStatusMessage(tr("Loading graph image..."));
	reporter->setPercenage(90);

	mSvgData = dot.readAllStandardOutput();
	reporter->setPercenage(100);
	mAnalysisOk = true;
}

void CoverageGraph::writeNode(QTextStream& out, const Marking& from, const Marking& to, const QString& arcName) {
	QString newArcName(arcName);
	newArcName.replace("\"", "\\\"");
	out << '\"' << from << "\" -> \"" << to
	    << "\" [label = \"" << newArcName << "\"]\n";
}

bool CoverageGraph::setup(QWidget* parentWidget) {
	QProcess dot;
	QString exec = QDir::toNativeSeparators(qApp->applicationDirPath()+"/graphviz/bin/dot.exe");
	dot.start(exec, QStringList() << "--version");
	if (!dot.waitForFinished()) {
		QMessageBox::critical(parentWidget, tr("Coverage graph plugin"), tr("<html>You need graphviz installed on your computer in order to use this plugin. If graphiviz is already installed, make sure that it is in your system path.\nYou can get graphviz at: <a href=\"http://www.graphviz.org\">http://www.graphviz.org</a></html>"));
		return false;
	}
	return true;
}

void CoverageGraph::finish(QWidget* parentWidget) {
	if (!mAnalysisOk)
		return;
	QSvgRenderer svgRenderer(mSvgData);

	QDialog dlg(parentWidget);
	ui.setupUi(&dlg);

	QGraphicsScene* scene = new QGraphicsScene(&dlg);

	QGraphicsSvgItem* item = new QGraphicsSvgItem;
	item->setSharedRenderer(&svgRenderer);
	scene->addItem(item);

	ui.view->setScene(scene);
	ui.markingOrder->setText(mMarkingOrder);

	connect(ui.zoomIn, SIGNAL(clicked(bool)), this, SLOT(zoomIn()));
	connect(ui.zoomOut, SIGNAL(clicked(bool)), this, SLOT(zoomOut()));
	dlg.exec();
}

void CoverageGraph::zoomIn() {
	ui.view->scale(1.1, 1.1);
}

void CoverageGraph::zoomOut() {
	ui.view->scale(0.9, 0.9);
}

//#include "coveragegraph.moc"

//Q_EXPORT_PLUGIN2(Opulus_CoverageGraph, CoverageGraph)
