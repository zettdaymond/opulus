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
#include "matrixwidget.h"
#include "ui_matrixwidget.h"
#include "matrix_util.h"
#include <QDebug>
#include <QFont>
#include <algorithm>

MatrixWidget::MatrixWidget(QWidget *_parent) :
	QWidget(_parent),
	ui(new Ui::MatrixWidget)
{
	ui->setupUi(this);
	QFont f("unexistent");
	f.setPixelSize(14);
	f.setStyleHint(QFont::Monospace);
	ui->IFunctionTextedit->setFont(f);
	ui->OFunctionTextedit->setFont(f);

	ui->dMinusTable->setModel(&mMinus);
	ui->dPlusTable->setModel(&mPlus);

	connect(ui->IOUpdateNetBtn, SIGNAL(pressed()), this, SLOT(IOUpdateNetPressed()));
	connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(IOUpdateText()));
	connect(ui->rowsSpinbox, SIGNAL(valueChanged(int)), this, SLOT(rowsSpinboxChanged(int)));
	connect(ui->colsSpinbox, SIGNAL(valueChanged(int)), this, SLOT(colsSpinboxChanged(int)));

	connect(&mMinus, &NetMatrixModel::mtxValueChanged, this, &MatrixWidget::dMinusTableChanged);
	connect(&mPlus, &NetMatrixModel::mtxValueChanged, this, &MatrixWidget::dPlusTableChanged);
}

MatrixWidget::~MatrixWidget()
{
	delete ui;
}

void MatrixWidget::rowsSpinboxChanged(int val)
{
	if(val < 0)
		return;
	emit matrixSizeChanged(val, mMinus.columnCount());
}

void MatrixWidget::colsSpinboxChanged(int val)
{
	if(val < 0)
		return;
	emit matrixSizeChanged(mMinus.rowCount(), val);
}

void MatrixWidget::dMinusTableChanged(int row, int col, int val)
{
	emit matrixValueChanged(MatrixType::dMinusMatrix,row,col,val);
}

void MatrixWidget::dPlusTableChanged(int row, int col, int val)
{
	emit matrixValueChanged(MatrixType::dPlusMatrix,row,col,val);
}

void MatrixWidget::IOUpdateNetPressed()
{
	QMap<int, QMap<int,int> > m = parseIOText(ui->OFunctionTextedit->toPlainText());
	QMap<int, QMap<int,int> > mi = parseIOText(ui->IFunctionTextedit->toPlainText());
	QMap<int, QMap<int,int> >::iterator it = m.begin();

	bool wspinstate = ui->rowsSpinbox->blockSignals(true);
	bool hspinstate = ui->colsSpinbox->blockSignals(true);

//	const int rows = m.size();
//	const int cols = m.size() > 0 ? m.begin().value().size() : 0;
//	std::max_element(m.keys().begin(), m.keys().end());
//	Q_ASSERT((rows == mPlus.rowCount()) && (cols == mPlus.columnCount()));

//	Eigen::MatrixXi z = Eigen::MatrixXi::Zero(mMinus.rowCount(), mMinus.columnCount());
//	mMinus.updateMatrix(z);
//	mPlus.updateMatrix(z);

	bool dminstate = ui->dMinusTable->blockSignals(true);
	bool dplusstate = ui->dPlusTable->blockSignals(true);

	Eigen::MatrixXi dPlus = Eigen::MatrixXi::Zero(0,0);
	Eigen::MatrixXi dMinus = Eigen::MatrixXi::Zero(0,0);

	for(; it != m.end(); ++it) {
		if (it.key() >= dPlus.rows()) {
			dPlus.conservativeResizeLike(Eigen::MatrixXi::Zero(it.key() + 1, dPlus.cols() ));
		}

		QMap<int,int>::iterator place = it.value().begin();
		for(;place != it.value().end(); ++place) {
			if (place.key() >= dPlus.cols()) {
				dPlus.conservativeResizeLike(Eigen::MatrixXi::Zero(dPlus.rows(), place.key() + 1 ));
			}
			dPlus(it.key(), place.key()) = place.value();
		}
	}

	for(it = mi.begin(); it != mi.end(); ++it) {
		if (it.key() >= dMinus.rows()) {
			dMinus.conservativeResizeLike(Eigen::MatrixXi::Zero(it.key() + 1, dMinus.cols() ));
		}

		QMap<int,int>::iterator place = it.value().begin();
		for( ;place != it.value().end(); ++place) {
			if (place.key() >= dMinus.cols()) {
				dMinus.conservativeResizeLike(Eigen::MatrixXi::Zero(dMinus.rows(), place.key() + 1 ));
			}
			dMinus(it.key(), place.key()) = place.value();
		}
	}
	auto rows = std::max(dMinus.rows(), dPlus.rows());
	auto cols = std::max(dMinus.cols(), dPlus.cols());

	dMinus.conservativeResizeLike(Eigen::MatrixXi::Zero(rows,cols));
	dPlus.conservativeResizeLike(Eigen::MatrixXi::Zero(rows,cols));

	emit IOFunctionsUpdated(dMinus, dPlus);

	ui->dMinusTable->blockSignals(dminstate);
	ui->dPlusTable->blockSignals(dplusstate);
	ui->rowsSpinbox->blockSignals(wspinstate);
	ui->colsSpinbox->blockSignals(hspinstate);
}

void MatrixWidget::updateMatrices(PetriNetMatrices matrices)
{

	Eigen::MatrixXi d_min = matrices.dMinus;
	Eigen::MatrixXi d_plu = matrices.dPlus;
	Q_ASSERT((d_min.rows() == d_plu.rows()) && (d_min.cols() == d_plu.cols()));

	bool dminstate = ui->dMinusTable->blockSignals(true);
	bool dplusstate = ui->dPlusTable->blockSignals(true);
	bool rspinstate = ui->rowsSpinbox->blockSignals(true);
	bool cspinstate = ui->colsSpinbox->blockSignals(true);

	ui->colsSpinbox->setValue(d_min.cols());
	ui->rowsSpinbox->setValue(d_min.rows());

	mMinus.updateMatrix(d_min);
	mPlus.updateMatrix(d_plu);

	if(ui->tabWidget->currentWidget() == ui->tab_io) {
			IOUpdateText();
	}

	ui->dMinusTable->blockSignals(dminstate);
	ui->dPlusTable->blockSignals(dplusstate);
	ui->colsSpinbox->blockSignals(cspinstate);
	ui->rowsSpinbox->blockSignals(rspinstate);

}

void MatrixWidget::retranslate()
{
    ui->retranslateUi(this);
}

void MatrixWidget::IOUpdateText()
{
	const int rows = mMinus.rowCount();
	const int cols = mMinus.columnCount();
	Q_ASSERT((rows == mPlus.rowCount()) && (cols == mPlus.columnCount()));

	QString minus_text;
	QString plus_text;
	for(int i =0; i < rows; ++i) {
		minus_text.push_back('T');
		plus_text.push_back('T');
		minus_text.push_back(QString::number(i));
		plus_text.push_back(QString::number(i));
		minus_text.push_back(" = ");
		plus_text.push_back(" = ");

		for(int j = 0; j < cols; ++j) {
//			Q_ASSERT(mMinus.val(i,j) && mPlus.val(i,j));

			for(int k = 0; k < mMinus.val(i,j); ++k) {
				minus_text.push_back('P');
				minus_text.push_back(QString::number(j));
				minus_text.push_back(' ');
			}
			for(int k = 0; k < mPlus.val(i,j); ++k) {
				plus_text.push_back('P');
				plus_text.push_back(QString::number(j));
				plus_text.push_back(' ');
			}

		}
		minus_text.push_back('\n');
		plus_text.push_back('\n');
	}

	bool iprev = ui->IFunctionTextedit->blockSignals(true);
	bool oprev = ui->OFunctionTextedit->blockSignals(true);

	ui->IFunctionTextedit->setText(minus_text);
	ui->OFunctionTextedit->setText(plus_text);

	ui->IFunctionTextedit->blockSignals(iprev);
	ui->OFunctionTextedit->blockSignals(oprev);
}

void MatrixWidget::startUpdateMatrixViewTransaction()
{
	mMinus.startUpdateTransaction();
	mPlus.startUpdateTransaction();
}

void MatrixWidget::stopUpdateMatrixViewTransaction()
{
	mMinus.stopUpdateTransaction();
	mPlus.stopUpdateTransaction();
}

QMap<int,QMap<int,int>> MatrixWidget::parseIOText(const QString &text)
{
	QStringList list = text.split('\n');
	QMap<int, QMap<int,int> > res;

	foreach (const QString& line, list) {
		int pos = 0; // skip space
		for(int i = 0; i < line.size(); ++i) {
			if(line[i].isSpace())
				++pos;
			else break;
		}
		// must start with 't'
		if(line.size() <= pos) continue;
		if(line[pos].toLower() != 't') continue;

		bool found_eq = false;
		QString transition_num, place_num;
		int transition = -1, place = -1;
		for(int i = pos+1; i < line.size(); ++i) {
			qDebug() << "@@" << line[i];

			if(!line[i].isDigit() && line[i].toLower() != 'p' && line[i] != '=' && line[i] != ' ')
				break;


			if(line[i].isDigit() && !found_eq) {
				transition_num.push_back(line[i]);
				continue;
			}
			if(line[i] == '=') {
				qDebug() << "found =";
				if(found_eq) break; // '=' must appear only once
				if(transition_num.isEmpty()) break; // not found transition number
//				std::reverse(transition_num.begin(), transition_num.end());
				transition = transition_num.toInt();
				qDebug() << "found transition" << transition;
				transition_num.clear();
				if(res.find(transition) != res.end())
					break; // transition must appear only once
				else res.insert(transition, QMap<int,int>());
				found_eq = true;
				continue;
			}

			if(found_eq && transition >= 0) {
				if(line[i].isDigit()) {
					place_num.push_back(line[i]);
					qDebug() << "place num" << line[i];
				}
				if(line[i].toLower() =='p' || i == line.size()-1) {
					// have previous place number
					qDebug() << "found p";
					if(!place_num.isEmpty()) {
//						std::reverse(place_num.begin(), place_num.end());
						place = place_num.toInt();
						place_num.clear();
						qDebug() << "found place" << place;
						if(res[transition].find(place) != res[transition].end()) {
							++res[transition][place];
						} else res[transition].insert(place,1);
					}
				}
			}
		}
	}
	return res;
}
