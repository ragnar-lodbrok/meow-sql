#ifndef MEOW_SETTINGS_TABLE_FILTERS_STORAGE_H
#define MEOW_SETTINGS_TABLE_FILTERS_STORAGE_H

#include <QStringList>

namespace meow {
namespace settings {

class TableFiltersStorage
{
public:

    TableFiltersStorage();

    void setTable(
            const QString & sessionName,
            const QString & databaseName,
            const QString & tableName);

    QStringList load();
    void save(const QStringList & filters);

    static int maxCount() { return 20; }
private:
    QString _sessionName;
    QString _databaseName;
    QString _tableName;

};

} // namespace meow
} // namespace settings

#endif // MEOW_SETTINGS_TABLE_FILTERS_STORAGE_H
