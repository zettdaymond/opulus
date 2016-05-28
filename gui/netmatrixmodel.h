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
//	bool insertRows(int row, int count, const QModelIndex& parent) override;
//	bool insertColumns(int column, int count, const QModelIndex& parent) override;
//	bool removeRows(int row, int count, const QModelIndex& parent) override;
//	bool removeColumns(int column, int count, const QModelIndex& parent) override;
	void resizeMatrix(int rows, int cols);
	void updateMatrixValue(int row, int col, int val);
	void updateMatrix(Eigen::MatrixXi& mtx);
//	void startUpdate();
//	void endUpdate();
	const int val(int row, int col) const;
signals:
	void mtxValueChanged(int,int,int);
public slots:
	void startUpdate();
	void endUpdate();
private:
	Eigen::MatrixXi mMtx;
	int update = 0;
};

#endif //NETMATRIXMODEL_H
