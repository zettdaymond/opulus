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
	// TODO: petri net integration

private slots:
	void on_height_spinbox_valueChanged(int arg1);

	void on_width_spinbox_valueChanged(int arg1);

	void on_d_matrices_cellChanged(int row, int column);

	void updateMatrices(const PetriNet* petri_net);

private:
	Ui::MatrixWidget *ui;
};

#endif // MATRIXWIDGET_H
