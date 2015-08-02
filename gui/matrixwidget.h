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
#ifndef MATRIXWIDGET_H
#define MATRIXWIDGET_H

#include <QWidget>
#include <QMap>
#include "matrix_util.h"

class PetriNet;
namespace Ui {
class MatrixWidget;
}

class MatrixWidget : public QWidget
{
	Q_OBJECT

public:
	explicit MatrixWidget(QWidget *parent = 0);
	~MatrixWidget();
    void retranslate();

signals:
	void matrixValueChanged(MatrixType which, int row, int col, int val);
	void matrixSizeChanged(int rows, int cols);

public slots:
    void updateMatrices(PetriNetMatrices matrices);

private slots:
	void rowsSpinboxChanged(int val);
	void colsSpinboxChanged(int val);
	void dMinusTableChanged(int row, int col);
	void dPlusTableChanged(int row, int col);
	void IOUpdateNetPressed();
	void IOUpdateText();

private:
	QMap<int,QMap<int,int>> parseIOText(const QString& text);
	Ui::MatrixWidget *ui;
};

#endif // MATRIXWIDGET_H
