#ifndef UI_PRESENTERS_CR_DATA_FILTER_FORM_H
#define UI_PRESENTERS_CR_DATA_FILTER_FORM_H

#include <QObject>
#include <QString>

namespace meow {
namespace db {
    class Entity;
}

namespace ui {

namespace models {
class DataTableModel;
}

namespace presenters {

class CentralRightDataFilterForm : public QObject
{
    Q_OBJECT
public:

    CentralRightDataFilterForm(models::DataTableModel * dataTabelModel);

    void setDBEntity(db::Entity * tableOrViewEntity);

    bool hasRecentFilters() const;

    QStringList recentFilters() const;

    void setFilterEditText(const QString & text);

    QString SQLFilterText() const;
    void setSQLFilterText(const QString & text);

    void applyWhereFilter(const QString & whereFilter);

    Q_SIGNAL void SQLFilterTextChanged(const QString & text);

private:

    QString SQLByFilterText(const QString & text);

    models::DataTableModel * _dataTabelModel;
    db::Entity * _entity;
    QString _sqlFilterText;

};

} // namespace presenters
} // namespace ui
} // namespace meow

#endif // UI_PRESENTERS_CR_DATA_FILTER_FORM_H

