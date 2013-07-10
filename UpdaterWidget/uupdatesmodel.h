#ifndef UUPDATESMODEL_H
#define UUPDATESMODEL_H

#include <QAbstractTableModel>
#include <QStringList>

class UpdateItem;

class UUpdatesModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum EColumns {
        eSTATUS = 0,
        eSIZE,
        ePACKAGE,
        eURI,
        eMAX_COLUMN
    };

    explicit UUpdatesModel(QObject *parent = 0);
    ~UUpdatesModel();

    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

    int rowCount(const QModelIndex &parent=QModelIndex()) const;
    int columnCount(const QModelIndex &parent) const;

    QVariant headerData(int section,Qt::Orientation orientation, int role=Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());

private:
    QList<UpdateItem *> m_updates;
    QStringList         m_headerData;

signals:
    
public slots:
    
};

#endif // UUPDATESMODEL_H
