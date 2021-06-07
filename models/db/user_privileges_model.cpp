#include "user_privileges_model.h"
#include "models/forms/user_management_form.h"
#include "db/entity/session_entity.h"
#include "db/exception.h"
#include <QDebug>
#include <QColor>
#include <QGuiApplication>
#include <QPalette>
#include <QIcon>

namespace meow {
namespace models {
namespace db {

static inline bool isLightTheme()
{
    QColor textColor = QGuiApplication::palette().color(QPalette::WindowText);

    // very stupid way to distinguish dark and light themes
    return (textColor.redF() + textColor.greenF() + textColor.blueF()) < 0.5*3;
}

class UserPrivilegesModel::TreeItem
{
public:
    // item data
    meow::db::UserPrivilegePtr privilege = nullptr;
    QString privilegeName;
    bool isScopeLevel = false;
    // item data

    TreeItem * parent = nullptr;
    QVector<TreeItem *> children;

    ~TreeItem() {
        qDeleteAll(children);
    }

    int row() const {
        if (parent) {
            return parent->children.indexOf(const_cast<TreeItem*>(this));
        }
        return 0;
    }
};

UserPrivilegesModel::UserPrivilegesModel(
        models::forms::UserManagementForm * form)
    : QAbstractItemModel(nullptr)
    , _form(form)
    , _rootItem(new UserPrivilegesModel::TreeItem())
{

}

UserPrivilegesModel::~UserPrivilegesModel()
{
    delete _rootItem;
}

void UserPrivilegesModel::setUser(const meow::db::UserPtr & user)
{
    removeData();
    _user = user;
    insertData();
}

Qt::ItemFlags UserPrivilegesModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags flags = QAbstractItemModel::flags(index);

    flags |= Qt::ItemIsUserCheckable;

    return flags;
}

QModelIndex UserPrivilegesModel::index(int row,
                                       int column,
                                       const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    TreeItem * parentItem;

    if (!parent.isValid()) {
        parentItem = _rootItem;
    } else {
        parentItem = static_cast<TreeItem*>(parent.internalPointer());
    }

    TreeItem * childItem = parentItem->children.value(row, nullptr);
    if (childItem) {
        return createIndex(row, column, childItem);
    }

    return QModelIndex();
}

QModelIndex UserPrivilegesModel::parent(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return QModelIndex();
    }

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parent;

    if (parentItem == _rootItem) {
        return QModelIndex();
    }

    return createIndex(parentItem->row(), 0, parentItem);
}

int UserPrivilegesModel::rowCount(const QModelIndex &parent) const
{
    TreeItem * parentItem;
    if (parent.column() > 0) {
        return 0;
    }

    if (!parent.isValid()) {
        parentItem = _rootItem;
    } else {
        parentItem = static_cast<TreeItem*>(parent.internalPointer());
    }

    if (!parentItem) return 0;

    return parentItem->children.count();
}

int UserPrivilegesModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant UserPrivilegesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    TreeItem * item = static_cast<TreeItem *>(index.internalPointer());

    const meow::db::UserPrivilegePtr & privilege = item->privilege;

    if (role == Qt::DisplayRole) {
        if (item->isScopeLevel) {
            return labelForPrivilege(privilege);
        } else {
            return item->privilegeName;
        }
    } else if (role == Qt::CheckStateRole) {
        if (item->isScopeLevel) {
            int allGranted
                    = _form->userManager()->allPrivilegesGranted(privilege);
            switch (allGranted) {
                case  1: return static_cast<int>(Qt::Checked);
                case -1: return static_cast<int>(Qt::Unchecked);
                default: return static_cast<int>(Qt::PartiallyChecked);
            }
        } else {
            bool granted = privilege->hasPrivilege(item->privilegeName);
            return static_cast<int>(granted ? Qt::Checked : Qt::Unchecked);
        }
    } else if (role == Qt::ForegroundRole) {

        if (item->isScopeLevel == false) {
            meow::db::PrivilegeType type
                    = _form->userManager()->typeOfPrivilege(item->privilegeName);
            switch (type) {
            case meow::db::PrivilegeType::Read:
                return isLightTheme() ? QColor(0,128,0) : QColor(115,213,115);
            case meow::db::PrivilegeType::Write:
                return isLightTheme() ? QColor(128,0,50) : QColor(201,115,115);
            case meow::db::PrivilegeType::Admin:
                return isLightTheme() ? QColor(0,0,128) : QColor(133, 151, 255);
            default:
                return QVariant();
            }
        }
    } else if (role == Qt::DecorationRole) {
        if (item->isScopeLevel) {
            return iconForPrivilege(privilege);
        }
    }

    return QVariant();
}

bool UserPrivilegesModel::setData(const QModelIndex &index,
                         const QVariant &value,
                         int role)
{
    if (!index.isValid() || role != Qt::CheckStateRole) {
        return false;
    }

    TreeItem * item = static_cast<TreeItem *>(index.internalPointer());

    if (item->isScopeLevel) {

        _form->userManager()->setPrivilegeGranted(
                    item->privilege, (value == Qt::Checked));

        emit dataChanged(index, index);
        int childenCount = rowCount(index);
        if (childenCount) {
            // update all children
            emit dataChanged(this->index(0,0,index),
                             this->index(childenCount-1,0,index));
        }

    } else {
        if (value == Qt::Checked) {
            item->privilege->grantPrivilege(item->privilegeName);
        } else {
            item->privilege->invokePrivilege(item->privilegeName);
        }
        QModelIndex parentIndex = parent(index);
        emit dataChanged(index, index);
        // parent may change as a result:
        emit dataChanged(parentIndex, parentIndex);
    }

    _form->setHasUnsavedChanges(true);

    return true;
}

bool UserPrivilegesModel::appendPrivilegeObject(
                       const meow::db::Entity::Type type,
                       const QString & databaseName,
                       const QString & entityName,
                       const QString & fieldName)
{
    Q_ASSERT(_user != nullptr);
    if (!_user) {
        return false;
    }

    using PrivScope = meow::db::UserPrivilege::Scope;
    using Type = meow::db::Entity::Type;

    std::shared_ptr<meow::db::UserPrivilege> privilege = nullptr;

    if (type == Type::Session || type == Type::Database) {
        privilege = std::make_shared<meow::db::UserPrivilege>(
                        PrivScope::DatabaseLevel,
                        databaseName);

    } else if (type == Type::Table || type == Type::View) {
        privilege = std::make_shared<meow::db::UserPrivilege>(
                        PrivScope::TableLevel,
                        databaseName,
                        entityName,
                        QString(),
                        type == Type::View);

    } else if (type == Type::Field) {
        privilege = std::make_shared<meow::db::UserPrivilege>(
                        PrivScope::TableColumnLevel,
                        databaseName,
                        entityName,
                        fieldName);

    } else if (type == Type::Procedure) {
        privilege = std::make_shared<meow::db::UserPrivilege>(
                        PrivScope::ProcedureLevel,
                        databaseName,
                        entityName);

    } else if (type == Type::Function) {
        privilege = std::make_shared<meow::db::UserPrivilege>(
                        PrivScope::FunctionLevel,
                        databaseName,
                        entityName);
    }

    if (privilege == nullptr) {
        return false;
    }

    if (_user->privilegeById(privilege->id()) != nullptr) {
        throw meow::db::Exception("Selected object is already accessible.");
    }

    // add minimum privileges:
    if (type == Type::Function || type == Type::Procedure) {
        privilege->grantPrivilege("EXECUTE");
    } else {
        privilege->grantPrivilege("SELECT");
    }

    int oldRowCount = _user->privileges().count();

    beginInsertRows(QModelIndex(), oldRowCount, oldRowCount+1);
    _user->privileges().append(privilege);
    appendPrivilegeToRootItem(privilege);
    endInsertRows();

    return true;
}

void UserPrivilegesModel::addPrivilegeScope(
        const meow::db::UserPrivilegePtr & privilege)
{
    Q_ASSERT(_user != nullptr);
    if (!_user) {
        return;
    }
    _user->privileges().append(privilege);
}

void UserPrivilegesModel::reinitItems()
{
    delete _rootItem;
    _rootItem = new UserPrivilegesModel::TreeItem();
    if (!_user) {
        return;
    }

    for (const meow::db::UserPrivilegePtr & privilege : _user->privileges()) {
        appendPrivilegeToRootItem(privilege);
    }
}

void UserPrivilegesModel::appendPrivilegeToRootItem(
        const meow::db::UserPrivilegePtr & privilege)
{
    TreeItem * scopePrivItem = new TreeItem();
    scopePrivItem->privilege = privilege;
    scopePrivItem->parent = _rootItem;
    scopePrivItem->isScopeLevel = true;
    _rootItem->children.append(scopePrivItem);

    const QStringList & allScopePrivs
            = _form->userManager()->supportedPrivilegesForScope(
                privilege->scope());

    for (const QString & privName : allScopePrivs) {
        TreeItem * privItem = new TreeItem();
        privItem->privilege = privilege;
        privItem->parent = scopePrivItem;
        privItem->privilegeName = privName;
        scopePrivItem->children.append(privItem);
    }
}

void UserPrivilegesModel::removeData()
{
    if (rowCount()) {
        beginRemoveRows(QModelIndex(), 0, rowCount()-1);
        delete _rootItem;
        _rootItem = nullptr;
        endRemoveRows();
    }
}
void UserPrivilegesModel::insertData()
{
    if (!_user) return;

    int newRowCount = _user->privileges().count();

    if (newRowCount > 0) {
        beginInsertRows(QModelIndex(), 0, newRowCount-1);
        reinitItems();
        endInsertRows();
    }
}

QString UserPrivilegesModel::labelForPrivilege(
        const meow::db::UserPrivilegePtr & privilege) const
{
    using Scope = meow::db::UserPrivilege::Scope;
    switch (privilege->scope()) {

    case Scope::Global:
        return tr("Global privileges");

    case Scope::DatabaseLevel:
        return tr("Database") + ": " + privilege->databaseName();

    case Scope::TableLevel:
    case Scope::FunctionLevel:
    case Scope::ProcedureLevel: {

        QString entityType;
        if (privilege->scope() == Scope::TableLevel) {
            entityType = privilege->isView() ? tr("View") : tr("Table");
        } else if (privilege->scope() == Scope::FunctionLevel) {
            entityType = tr("Function");
        } else if (privilege->scope() == Scope::ProcedureLevel) {
            entityType = tr("Procedure");
        } else {
            Q_ASSERT(false);
        }

        return entityType + ": "
                + privilege->databaseName() + "." + privilege->entityName();
    }

    case Scope::TableColumnLevel:
        return tr("Column") + ": "
                + privilege->databaseName() + "."
                + privilege->entityName() + "."
                + privilege->fieldName();

    default:
        Q_ASSERT(false);
        return "Unknown Scope";
    }
}

QVariant UserPrivilegesModel::iconForPrivilege(
        const meow::db::UserPrivilegePtr & privilege) const
{
    using Scope = meow::db::UserPrivilege::Scope;
    switch (privilege->scope()) {

    case Scope::Global:
        return _form->session()->icon();
    case Scope::DatabaseLevel:
        return QIcon(":/icons/database.png");
    case Scope::TableLevel:
        return privilege->isView() ? QIcon(":/icons/view.png")
                                   : QIcon(":/icons/table.png");
    case Scope::FunctionLevel:
        return QIcon(":/icons/stored_function.png");
    case Scope::ProcedureLevel:
        return QIcon(":/icons/stored_procedure.png");
    case Scope::TableColumnLevel:
        return QIcon(":/icons/bullet_white.png");

    default:
        Q_ASSERT(false);
        return {};
    }
}

} // namespace db
} // namespace models
} // namespace meow
