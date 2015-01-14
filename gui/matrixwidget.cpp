#include "matrixwidget.h"
#include "ui_matrixwidget.h"
#include <QDebug>

MatrixWidget::MatrixWidget(QWidget *_parent) :
	QWidget(_parent),
	ui(new Ui::MatrixWidget)
{
	ui->setupUi(this);
	connect(ui->d_minus_matrix, SIGNAL(cellChanged(int,int)), this, SLOT(on_d_matrices_cellChanged(int,int)));
	connect(ui->d_plus_matrix, SIGNAL(cellChanged(int,int)), this, SLOT(on_d_matrices_cellChanged(int,int)));
}

MatrixWidget::~MatrixWidget()
{
	delete ui;
}

void MatrixWidget::on_height_spinbox_valueChanged(int arg1)
{
	// TODO: update petri net
	ui->d_minus_matrix->setRowCount(arg1);
	ui->d_plus_matrix->setRowCount(arg1);
}

void MatrixWidget::on_width_spinbox_valueChanged(int arg1)
{
	// TODO: update petri net
	ui->d_minus_matrix->setColumnCount(arg1);
	ui->d_plus_matrix->setColumnCount(arg1);
}

void MatrixWidget::on_d_matrices_cellChanged(int row, int column)
{
	Q_UNUSED(row);
	Q_UNUSED(column);
	qDebug() << "cell changed";
	// TODO: update petri net

}
