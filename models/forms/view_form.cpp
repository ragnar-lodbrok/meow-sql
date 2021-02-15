#include "view_form.h"
#include "db/connection.h"
#include "app/app.h"
#include <QDebug>

namespace meow {
namespace models {
namespace forms {

namespace  {

bool isMySQL(db::Connection * connection)
{
    return connection->connectionParams()->serverType()
            == db::ServerType::MySQL;
}

}


ViewForm::ViewForm(QObject *parent)
    : QObject(parent)
    , _view(nullptr)
    , _sourceView(nullptr)
    , _hasUnsavedChanges(false)
{

}

void ViewForm::setView(meow::db::ViewEntity * view)
{
    // TODO: copy only when we start editing

    Q_ASSERT(view != nullptr);

    if (view->isNew()) {
        _sourceView = nullptr;
        _view.reset(view); // take ownership
        setDefaultValuesForNewView();
    } else {
        _sourceView = view; // just hold a ref to view for update
        _view.reset(_sourceView->deepCopy()); // and edit copy
    }

    setHasUnsavedChanges(false);
}

QString ViewForm::name() const
{
    // Listening: Duuu... Du Hast
    if (!_view) return {};

    return _view->name();
}

void ViewForm::setName(const QString & name)
{
    // Listening: Edict - Genus Ordinis Dei
    if (_view) {
        _view->setName(name);
        setHasUnsavedChanges(true);
    }
}

QString ViewForm::selectStatement() const
{
    if (!_view) return {};

    return _view->structure()->selectStatement();
}

void ViewForm::setSelectStatement(const QString & select)
{
    if (_view) {
        _view->structure()->setSelectStatement(select);
        setHasUnsavedChanges(true);
    }
}

QString ViewForm::definer() const
{
    if (!_view) return {};

    return _view->structure()->definer;
}

QStringList ViewForm::allDefinerOptions(bool loadFromDB) const
{
    if (!_view) return {};

    if (loadFromDB) {
        return _view->connection()->userManager()->allUserNames();
    }

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

void ViewForm::setDefiner(const QString & definer)
{
    if (_view) {
        _view->structure()->definer = definer;
        setHasUnsavedChanges(true);
    }
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

void ViewForm::setSecurity(const QString & security)
{
    if (_view) {
        _view->structure()->sqlSecurity = security;
        setHasUnsavedChanges(true);
    }
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

void ViewForm::setAlgorithm(const QString & algorithm)
{
    if (_view) {
        _view->structure()->algorithm = algorithm;
        setHasUnsavedChanges(true);
    }
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

void ViewForm::setCheckOption(const QString & opt)
{
    if (_view) {
        _view->structure()->checkOption = opt;
        setHasUnsavedChanges(true);
    }
}

void ViewForm::save()
{
    if (_view->isNew()) { // insert
        // try to transfer ownership - take back on error
        // TODO: why not just use shared_ptr ?
        meow::db::ViewEntity * view = _view.release();
        try {
            bool inserted = meow::app()->dbConnectionsManager()
                    ->activeSession()->insertEntityToDB(view);
            if (!inserted) {
                _view.reset(view);
            }
        } catch (meow::db::Exception & exc) {
            _view.reset(view);
            throw;
        }

    } else { // update
        meow::app()->dbConnectionsManager()->activeSession()->editEntityInDB(
            _sourceView, _view.get());
    }

    setHasUnsavedChanges(false);
}

void ViewForm::setHasUnsavedChanges(bool modified)
{
    if (_hasUnsavedChanges != modified) {
        _hasUnsavedChanges = modified;
        emit unsavedChanged(_hasUnsavedChanges);
    }
}

bool ViewForm::isEditingSupported() const
{
    if (_view == nullptr) return false;
    return _view->connection()->features()->supportsEditingViewsStructure();
}

void ViewForm::setDefaultValuesForNewView()
{
    _view->structure()->setSelectStatement("SELECT");

    if (supportsDefiner()) {
        _view->structure()->definer
                = _view->connection()->userManager()->currentUserName();
    }

    if (supportsAlgorithm()) {
        _view->structure()->algorithm = "UNDEFINED";
    }

    if (supportsSecurity()) {
        _view->structure()->sqlSecurity = "DEFINER";
    }
}

} // namespace forms
} // namespace models
} // namespace meow
