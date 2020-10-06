#include "view_form.h"
#include "db/entity/view_entity.h"
#include "db/connection.h"

namespace meow {
namespace models {
namespace forms {

bool isMySQL(db::Connection * connection)
{
    return connection->connectionParams()->serverType()
            == db::ServerType::MySQL;
}

ViewForm::ViewForm() : _view(nullptr)
{

}

void ViewForm::setView(meow::db::ViewEntity * view)
{
    _view = view;
}

QString ViewForm::name() const
{
    // Listening: Duuu... Du Hast
    if (!_view) return {};

    return _view->name();
}

QString ViewForm::selectStatement() const
{
    if (!_view) return {};

    return _view->structure()->selectStatement();
}

QString ViewForm::definer() const
{
    if (!_view) return {};

    return _view->structure()->definer;
}

QStringList ViewForm::allDefinerOptions() const
{
    if (!_view) return {};

    QStringList options;
    QString currentDefiner = definer();
    if (!currentDefiner.isEmpty()) {
        options << definer();
    }

    return options;
}

bool ViewForm::supportsDefiner() const
{
    if (_view) {
        return isMySQL(_view->connection());
    }
    return false;
}

QString ViewForm::security() const
{
    if (!_view) return {};
    return _view->structure()->sqlSecurity;
}

QStringList ViewForm::securityOptions() const
{
    if (!_view) return {};

    QStringList options;

    if (isMySQL(_view->connection())) {
        options << "DEFINER"; // TODO: move to connection?
        options << "INVOKER";
    }

    return options;
}

bool ViewForm::supportsSecurity() const
{
    if (_view) {
        return isMySQL(_view->connection());
    }
    return false;
}

QString ViewForm::algorithm() const
{
    if (!_view) return {};

    return _view->structure()->algorithm;
}

QStringList ViewForm::algorithmOptions() const
{
    if (!_view) return {};

    QStringList options;

    if (isMySQL(_view->connection())) {
        options << "UNDEFINED"; // TODO: move to connection?
        options << "MERGE";
        options << "TEMPTABLE";
    }

    return options;
}

bool ViewForm::supportsAlgorithm() const
{
    if (_view) {
        return isMySQL(_view->connection());
    }
    return false;
}

QString ViewForm::checkOption() const
{
    if (!_view) return {};

    if (isMySQL(_view->connection())) {
        if (_view->structure()->checkOption.isEmpty()) {
            return "None";
        }
    }

    return _view->structure()->checkOption;
}

QStringList ViewForm::checkOptions() const
{
    if (!_view) return {};

    QStringList options;

    if (isMySQL(_view->connection())) {
        options << "None"; // TODO: move to connection?
        options << "CASCADED";
        options << "LOCAL";
    }

    return options;
}

bool ViewForm::supportsCheckOptions() const
{
    if (_view) {
        return isMySQL(_view->connection());
    }
    return false;
}

} // namespace forms
} // namespace models
} // namespace meow
