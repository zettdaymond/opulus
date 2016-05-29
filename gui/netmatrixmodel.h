#ifndef NETMATRIXMODEL_H
#define NETMATRIXMODEL_H

#include <QAbstractTableModel>
#include <eigen3/Eigen/Dense>

class NetMatrixModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	explicit NetMatrixModel(QObject *parent = 0);
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	bool setData(const QModelIndex& index, const QVariant& value, int role) override;
	Qt::ItemFlags flags(const QModelIndex& index) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
	void updateMatrix(Eigen::MatrixXi& mtx);
	int val(int row, int col);
signals:
	void mtxValueChanged(int,int,int);
private:
	Eigen::MatrixXi mMtx;
};

#endif //NETMATRIXMODEL_H
