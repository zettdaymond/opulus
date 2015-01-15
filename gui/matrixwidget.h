#ifndef MATRIXWIDGET_H
#define MATRIXWIDGET_H

#include <QWidget>

namespace Ui {
class MatrixWidget;
}

class PetriNet;

class MatrixWidget : public QWidget
{
	Q_OBJECT

public:
	explicit MatrixWidget(QWidget *parent = 0);
	~MatrixWidget();

signals:
	void d_minus_matrix_value_changed(int row, int col, int val);
	void d_plus_matrix_value_changed(int row, int col, int val);
	void matrices_size_changed(int rows, int cols);

public slots:
	void updateMatrices(const PetriNet* petri_net);

private slots:
	void on_height_spinbox_valueChanged(int arg1);
	void on_width_spinbox_valueChanged(int arg1);
	void d_minus_table_value_changed(int row, int col);
	void d_plus_table_value_changed(int row, int col);

private:
	Ui::MatrixWidget *ui;
};

#endif // MATRIXWIDGET_H
