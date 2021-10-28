#ifndef MEOW_SETTINGS_QUERIES_STORAGE_H
#define MEOW_SETTINGS_QUERIES_STORAGE_H

#include <QStringList>
#include <vector>

namespace meow {
namespace db {
class UserQuery;
}
namespace settings {

class Core;
class QueriesStorage
{
public:
    void save(const std::vector<db::UserQuery *> & queries);
    std::vector<db::UserQuery *> load();
private:
    QueriesStorage();
    friend class Core;

    bool backup(db::UserQuery * query);
    QString backupPath() const;
    QString backupFileName(const QString & queryId) const;
    QStringList loadQueryIds();
};

} // namespace meow
} // namespace settings

#endif // MEOW_SETTINGS_QUERIES_STORAGE_H
