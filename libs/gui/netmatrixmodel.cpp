/*
* Copyright (C) 2016 Zett Daymond <zettday@gmail.com>
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

#include <netmatrixmodel.h>
#include <QDebug>
#include <QTime>

NetMatrixModel::NetMatrixModel(QObject* parent) :
	QAbstractTableModel(parent),
	mMtx(0,0)
{}

int NetMatrixModel::rowCount(const QModelIndex&) const
{
	return mMtx.rows();
}

int NetMatrixModel::columnCount(const QModelIndex&) const
{
	return mMtx.cols();
}

QVariant NetMatrixModel::data(const QModelIndex& index, int role) const
{
	if (role == Qt::TextAlignmentRole ) {
		return Qt::AlignCenter;
	}
	if (role == Qt::DisplayRole) {
		Q_ASSERT(index.row() < mMtx.rows() && index.column() < mMtx.cols());
		return QString::number( mMtx(index.row(), index.column()) );
	}
	return QVariant();
}

bool NetMatrixModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
	if (role == Qt::EditRole) {
		mMtx(index.row(),index.column()) = value.toInt();
		emit mtxValueChanged( index.row(), index.column(), value.toInt() );
	}
	return true;
}

Qt::ItemFlags NetMatrixModel::flags(const QModelIndex& index) const
{
	return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
}

QVariant NetMatrixModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole) {
		if (orientation == Qt::Horizontal) {
			return QString("P") + QString::number(section);
		} else if (orientation == Qt::Vertical) {
			return QString("T") + QString::number(section);
		}
	}
	return QVariant();
}

void NetMatrixModel::updateMatrix(Eigen::MatrixXi& mtx)
{
	mMtx = mtx;
	emit layoutChanged();
}

int NetMatrixModel::val(int row, int col)
{
	Q_ASSERT(row < mMtx.rows() && col < mMtx.cols());
	return mMtx(row,col);
}


