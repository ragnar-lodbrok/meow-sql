#include "table_info_widget_model.h"
#include <QObject> // tr()

namespace meow {
namespace ui {
namespace presenters {

TableInfoWidgetModel::TableInfoWidgetModel()
{

}

const QString TableInfoWidgetModel::tabTitle(TableInfoWidgetTabs tab) const
{
    switch (tab) {
    case TableInfoWidgetTabs::Basic:
        return QObject::tr("Basic");

    case TableInfoWidgetTabs::Options:
        return QObject::tr("Options");

    case TableInfoWidgetTabs::Indexes:
        return QObject::tr("Indexes");

    case TableInfoWidgetTabs::ForeignKeys:
        return QObject::tr("Foreign keys");

    default:
        return "";
    }
}

const QIcon TableInfoWidgetModel::tabIcon(TableInfoWidgetTabs tab) const
{
    switch (tab) {
    case TableInfoWidgetTabs::Basic:
        return QIcon(":/icons/table.png");

    case TableInfoWidgetTabs::Options:
        return QIcon(":/icons/wrench.png");

    case TableInfoWidgetTabs::Indexes:
        return QIcon(":/icons/lightning.png");

    case TableInfoWidgetTabs::ForeignKeys:
        return QIcon(":/icons/table_relationship.png");

    default:
        return QIcon();
    }
}

} // namespace presenters
} // namespace ui
} // namespace meow
