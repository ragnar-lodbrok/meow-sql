#ifndef MODELS_TABLE_INFO_FORM_H
#define MODELS_TABLE_INFO_FORM_H

#include <QObject>
#include <QString>

namespace meow {

namespace db {
    class TableEntity;
}

namespace models {
namespace forms {

class TableInfoForm : public QObject
{
    Q_OBJECT
public:
    explicit TableInfoForm(QObject *parent = nullptr);

    void setTable(db::TableEntity * table);

    const QString tableName() const;
    const QString tableComment() const;

private:
    db::TableEntity * _table;
};

} // namespace forms
} // namespace models
} // namespace meow

#endif // MODELS_TABLE_INFO_FORM_H
