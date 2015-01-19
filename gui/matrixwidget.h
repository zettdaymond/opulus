#ifndef MATRIXWIDGET_H
#define MATRIXWIDGET_H

#include <QWidget>
#include <QMap>

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

signals:
	void matrix_value_changed(char which, int row, int col, int val);
	void matrices_size_changed(int rows, int cols);

public slots:
	void updateMatrices(const PetriNet* petri_net);

private slots:
	void on_height_spinbox_valueChanged(int arg1);
	void on_width_spinbox_valueChanged(int arg1);
	void d_minus_table_value_changed(int row, int col);
	void d_plus_table_value_changed(int row, int col);
	void i_textedit_changed();
	void o_textedit_changed();
    void updateIOFunctions();

private:
	QMap<int, QMap<int,int> > parseIOText(const QString& text);
	Ui::MatrixWidget *ui;
};

#endif // MATRIXWIDGET_H
