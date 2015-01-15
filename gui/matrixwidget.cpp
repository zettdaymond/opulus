#include "matrixwidget.h"
#include "ui_matrixwidget.h"
#include "matrix_util.h"
#include <QDebug>

MatrixWidget::MatrixWidget(QWidget *_parent) :
	QWidget(_parent),
	ui(new Ui::MatrixWidget)
{
	ui->setupUi(this);
	connect(ui->d_minus_matrix, SIGNAL(cellChanged(int,int)),
		this, SLOT(d_minus_table_value_changed(int,int)));
	connect(ui->d_plus_matrix, SIGNAL(cellChanged(int,int)),
		this, SLOT(d_plus_table_value_changed(int,int)));
}

MatrixWidget::~MatrixWidget()
{
	delete ui;
}

void MatrixWidget::on_height_spinbox_valueChanged(int arg1)
{
	if(arg1 < 0)
		return;
	ui->d_minus_matrix->setRowCount(arg1);
	ui->d_plus_matrix->setRowCount(arg1);
	emit matrices_size_changed(ui->d_minus_matrix->rowCount(), ui->d_minus_matrix->columnCount());
}

void MatrixWidget::on_width_spinbox_valueChanged(int arg1)
{
	if(arg1 < 0)
		return;
	ui->d_minus_matrix->setColumnCount(arg1);
	ui->d_plus_matrix->setColumnCount(arg1);
	emit matrices_size_changed(ui->d_minus_matrix->rowCount(), ui->d_minus_matrix->columnCount());
}

void MatrixWidget::d_minus_table_value_changed(int row, int col)
{
	if(!ui->d_minus_matrix->item(row,col))
		return;

	bool ok;
	int val = ui->d_minus_matrix->item(row,col)->text().toInt(&ok);

	if(ok && val >= 0)
		emit matrix_value_changed('-',row,col,val);
}

void MatrixWidget::d_plus_table_value_changed(int row, int col)
{
	if(!ui->d_plus_matrix->item(row,col))
		return;

	bool ok;
	int val = ui->d_plus_matrix->item(row,col)->text().toInt(&ok);

	if(ok && val >= 0)
		emit matrix_value_changed('+',row, col, val);
}

void MatrixWidget::updateMatrices(const PetriNet *petri_net)
{
	if(!petri_net)
		return;

	Eigen::MatrixXi d_min = d_minus_matrix(petri_net);
	Eigen::MatrixXi d_plu = d_plus_matrix(petri_net);

	bool dminstate = ui->d_minus_matrix->blockSignals(true);
	bool dplusstate = ui->d_plus_matrix->blockSignals(true);
	bool wspinstate = ui->width_spinbox->blockSignals(true);
	bool hspinstate = ui->height_spinbox->blockSignals(true);

	ui->width_spinbox->setValue(d_min.cols());
	ui->height_spinbox->setValue(d_min.rows());

	ui->d_minus_matrix->setRowCount(d_min.rows());
	ui->d_plus_matrix->setRowCount(d_plu.rows());
	ui->d_minus_matrix->setColumnCount(d_min.cols());
	ui->d_plus_matrix->setColumnCount(d_plu.cols());

	for(int i = 0; i < d_min.rows(); ++i) {
		if(!ui->d_minus_matrix->verticalHeaderItem(i))
			ui->d_minus_matrix->setVerticalHeaderItem(i, new QTableWidgetItem(QString("T")+QString::number(i)));

		for(int j = 0; j < d_min.cols(); ++j) {

			if(ui->d_minus_matrix->item(i,j))
				ui->d_minus_matrix->item(i,j)->setText(QString::number(d_min(i,j)));
			else ui->d_minus_matrix->setItem(i,j, new QTableWidgetItem(QString::number(d_min(i,j))));
		}
	}
	for(int i = 0; i < d_plu.rows(); ++i) {
		if(!ui->d_plus_matrix->verticalHeaderItem(i))
			ui->d_plus_matrix->setVerticalHeaderItem(i, new QTableWidgetItem(QString("T")+QString::number(i)));

		for(int j = 0; j < d_plu.cols(); ++j) {
			if(ui->d_plus_matrix->item(i,j))
				ui->d_plus_matrix->item(i,j)->setText(QString::number(d_plu(i,j)));
			else ui->d_plus_matrix->setItem(i,j, new QTableWidgetItem(QString::number(d_plu(i,j))));
		}
	}

	for(int i = 0; i < d_min.cols(); ++i) {
		if(!ui->d_minus_matrix->horizontalHeaderItem(i))
			ui->d_minus_matrix->setHorizontalHeaderItem(i, new QTableWidgetItem(QString("P")+QString::number(i)));

		if(!ui->d_plus_matrix->horizontalHeaderItem(i))
			ui->d_plus_matrix->setHorizontalHeaderItem(i, new QTableWidgetItem(QString("P")+QString::number(i)));
	}

	ui->d_minus_matrix->blockSignals(dminstate);
	ui->d_plus_matrix->blockSignals(dplusstate);
	ui->width_spinbox->blockSignals(wspinstate);
	ui->height_spinbox->blockSignals(hspinstate);

}
