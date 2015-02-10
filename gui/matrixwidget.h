#ifndef MATRIXWIDGET_H
#define MATRIXWIDGET_H

#include <QWidget>
#include <QMap>

class PetriNet;
namespace Ui {
class MatrixWidget;
}

enum class MatrixType {
	dMinusMatrix = 1, dPlusMatrix = 2
};


class MatrixWidget : public QWidget
{
	Q_OBJECT

public:
	explicit MatrixWidget(QWidget *parent = 0);
	~MatrixWidget();

signals:
	void matrixValueChanged(MatrixType which, int row, int col, int val);
	void matrixSizeChanged(int rows, int cols);

public slots:
	void updateMatrices(const PetriNet* petri_net);

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
