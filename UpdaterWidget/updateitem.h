#ifndef UPDATEITEM_H
#define UPDATEITEM_H

#include <QString>

class UpdateItem
{
public:
    UpdateItem();

    QString m_size; // f.e.: 500 Kb
    QString m_package;
    QString m_uri;
};

#endif // UPDATEITEM_H
