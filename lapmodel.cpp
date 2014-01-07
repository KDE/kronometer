#include "lapmodel.h"

#include <QtDebug>

LapModel::LapModel(QObject* parent): QAbstractTableModel(parent) {}

int LapModel::columnCount(const QModelIndex& parent) const
{
	qDebug() << "columnCount()";
	
	return 2;
}

int LapModel::rowCount(const QModelIndex& parent) const
{
	qDebug() << "rowCount()";

	return lapList.size();
}

QVariant LapModel::data(const QModelIndex& index, int role) const
{
	if (role == Qt::DisplayRole)
	{
		qDebug() << "data()";

		return QString("test");
	}
	
	return QVariant::Invalid;
}


QVariant LapModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole)
	{
		qDebug() << "headerData()";
		
		if (orientation == Qt::Horizontal)
		{
			if (section == 0)
				return QString("lap #");
			
			else if (section == 1)
				return QString("lap time");
		}
	}
	
	return QVariant::Invalid;
}

