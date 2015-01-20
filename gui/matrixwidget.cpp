#include "matrixwidget.h"
#include "ui_matrixwidget.h"
#include "../core/matrix_util.h"
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
	ui->i_textedit->setFont(f);
	ui->o_textedit->setFont(f);
	connect(ui->d_minus_matrix, SIGNAL(cellChanged(int,int)),
		this, SLOT(d_minus_table_value_changed(int,int)));
	connect(ui->d_plus_matrix, SIGNAL(cellChanged(int,int)),
		this, SLOT(d_plus_table_value_changed(int,int)));

	connect(ui->io_update_net_btn, SIGNAL(pressed()), this, SLOT(io_update_button_pressed()));
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

void MatrixWidget::io_update_button_pressed()
{
	QMap<int, QMap<int,int> > m = parseIOText(ui->o_textedit->toPlainText());
	QMap<int, QMap<int,int> > mi = parseIOText(ui->i_textedit->toPlainText());
	QMap<int, QMap<int,int> >::iterator it = m.begin();
	bool dminstate = ui->d_minus_matrix->blockSignals(true);
	bool dplusstate = ui->d_plus_matrix->blockSignals(true);
	bool wspinstate = ui->width_spinbox->blockSignals(true);
	bool hspinstate = ui->height_spinbox->blockSignals(true);

	for(; it != m.end(); ++it) {
		QMap<int,int>::iterator place = it.value().begin();
		for(;place != it.value().end(); ++place) {
			emit matrix_value_changed('+', it.key(), place.key(), place.value());

		}
	}

	for(it = mi.begin(); it != mi.end(); ++it) {
		QMap<int,int>::iterator place = it.value().begin();
		for( ;place != it.value().end(); ++place) {
			emit matrix_value_changed('-', it.key(), place.key(), place.value());
		}
	}

	ui->d_minus_matrix->blockSignals(dminstate);
	ui->d_plus_matrix->blockSignals(dplusstate);
	ui->width_spinbox->blockSignals(wspinstate);
	ui->height_spinbox->blockSignals(hspinstate);
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

	updateIOFunctions();
	ui->d_minus_matrix->blockSignals(dminstate);
	ui->d_plus_matrix->blockSignals(dplusstate);
	ui->width_spinbox->blockSignals(wspinstate);
	ui->height_spinbox->blockSignals(hspinstate);

}

void MatrixWidget::updateIOFunctions()
{
	QString text;
	for(int i =0; i < ui->d_minus_matrix->rowCount(); ++i) {
		text.push_back('T');
		text.push_back(QString::number(i));
		text.push_back(" = ");
		for(int j = 0; j < ui->d_minus_matrix->columnCount(); ++j) {
			for(int k = 0; k < ui->d_minus_matrix->item(i,j)->text().toInt(); ++k) {
				text.push_back('P');
				text.push_back(QString::number(j));
				text.push_back(' ');
			}
		}
		text.push_back('\n');
	}
	QString plus_text;
	for(int i =0; i < ui->d_plus_matrix->rowCount(); ++i) {
		plus_text.push_back('T');
		plus_text.push_back(QString::number(i));
		plus_text.push_back(" = ");

		for(int j = 0; j < ui->d_plus_matrix->columnCount(); ++j) {
			for(int k = 0; k < ui->d_plus_matrix->item(i,j)->text().toInt(); ++k) {
				plus_text.push_back('P');
				plus_text.push_back(QString::number(j));
				plus_text.push_back(' ');
			}
		}
		plus_text.push_back('\n');
	}

	bool iprev = ui->i_textedit->blockSignals(true);
	bool oprev = ui->o_textedit->blockSignals(true);
	ui->i_textedit->setText(text);
	ui->o_textedit->setText(plus_text);
	ui->i_textedit->blockSignals(iprev);
	ui->o_textedit->blockSignals(oprev);
}

QMap<int, QMap<int, int> > MatrixWidget::parseIOText(const QString &text)
{
	QStringList list = text.split('\n');
	QMap<int, QMap<int,int> > res;

	foreach (const QString& line, list) {
	//	qDebug() << "parsing" << line;
		int pos = 0; // skip space
		for(int i = 0; i < line.size(); ++i) {
			if(line[i].isSpace())
				++pos;
			else break;
		}
		// must start with 't'
		if(line.size() <= pos) continue;
		if(line[pos].toLower() != 't') continue;
	//	qDebug() << "skipped" << pos << "whitespace";

		bool found_eq = false;
		QString transition_num, place_num;
		int transition = -1, place = -1;
		for(int i = pos+1; i < line.size(); ++i) {
			//qDebug() << "@@" << line[i];

			if(!line[i].isDigit() && line[i].toLower() != 'p' && line[i] != '=' && line[i] != ' ')
				break;


			if(line[i].isDigit() && !found_eq) {
				transition_num.push_back(line[i]);
				continue;
			}
			if(line[i] == '=') {
				//qDebug() << "found =";
				if(found_eq) break; // '=' must appear only once
				if(transition_num.isEmpty()) break; // not found transition number
				std::reverse(transition_num.begin(), transition_num.end());
				transition = transition_num.toInt();
				//qDebug() << "found transition" << transition;
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
					//qDebug() << "place num" << line[i];
				}
				if(line[i].toLower() =='p' || i == line.size()-1) {
					// have previous place number
					//qDebug() << "found p";
					if(!place_num.isEmpty()) {
						std::reverse(place_num.begin(), place_num.end());
						place = place_num.toInt();
						place_num.clear();
						//qDebug() << "found place" << place;
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
