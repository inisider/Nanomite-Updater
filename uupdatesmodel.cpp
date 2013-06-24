#include "uupdatesmodel.h"

#include "updateitem.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UUpdatesModel::UUpdatesModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    m_headerData    << QString::fromUtf8("Status")
                    << QString::fromUtf8("Size")
                    << QString::fromUtf8("Package")
                    << QString::fromUtf8("URI");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UUpdatesModel::~UUpdatesModel()
{
    qDeleteAll(m_updates.begin(), m_updates.end());
    m_updates.clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
QVariant UUpdatesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_updates.size())
        return QVariant();

    if (role == Qt::DisplayRole){
        if (index.column() == eSIZE) {
            return m_updates.at(index.row())->m_size;
        }
        if (index.column() == ePACKAGE) {
            return m_updates.at(index.row())->m_package;
        }
        if (index.column() == eURI) {
            return m_updates.at(index.row())->m_uri;
        }
    }

    return QVariant();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool UUpdatesModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::DisplayRole) {
        if(index.column() == eSIZE) {
            m_updates.at(index.row())->m_size = value.toString();
        }
        if(index.column() == ePACKAGE) {
            m_updates.at(index.row())->m_package = value.toString();
        }
        if(index.column() == eURI){
            m_updates.at(index.row())->m_uri = value.toString();
        }

        return true;
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int UUpdatesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return m_updates.size();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int UUpdatesModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return m_headerData.size();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
QVariant UUpdatesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
       return m_headerData.at(section);
    } else {
       return QString("%1").arg(section + 1);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Qt::ItemFlags UUpdatesModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool UUpdatesModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);

    beginInsertRows(QModelIndex(), row, row + count - 1);

    for (int i = 0; i < count; ++i) {
        m_updates.insert(row, new UpdateItem);
    }

    endInsertRows();
    return true;
}
