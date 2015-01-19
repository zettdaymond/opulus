#include "delegate.h"

#include <QIntValidator>
#include <QLineEdit>
#include <limits>
QWidget *Delegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QLineEdit *lineEdit = new QLineEdit(parent);
    // Set validator
    int imax = std::numeric_limits<int>::max();
    QIntValidator *validator = new QIntValidator(0,imax,lineEdit);
    lineEdit->setValidator(validator);
    return lineEdit;
}
