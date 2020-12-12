#include "routine_info_widget_model.h"

namespace meow {
namespace models {
namespace ui {

RoutineInfoWidgetModel::RoutineInfoWidgetModel()
{

}

const QString RoutineInfoWidgetModel::tabTitle(RoutineInfoWidgetTabs tab) const
{
    switch (tab) {

    case RoutineInfoWidgetTabs::Options:
        return QObject::tr("Options");

    case RoutineInfoWidgetTabs::Parameters:
        return QObject::tr("Parameters");

    default:
        return QString();
    }
}

const QIcon RoutineInfoWidgetModel::tabIcon(RoutineInfoWidgetTabs tab) const
{
    switch (tab) {
    case RoutineInfoWidgetTabs::Options:
        return QIcon(":/icons/wrench.png");

    case RoutineInfoWidgetTabs::Parameters:
        return QIcon(":/icons/go_both.png");

    default:
        return QIcon();
    }
}

} // namespace ui
} // namespace models
} // namespace meow
