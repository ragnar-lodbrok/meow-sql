#ifndef MODELS_VIEWFORM_H
#define MODELS_VIEWFORM_H

#include <QObject>
#include <QString>
#include <memory>
#include "db/entity/view_entity.h"

namespace meow {
namespace models {
namespace forms {

class ViewForm : public QObject
{
    Q_OBJECT
public:
    ViewForm(QObject * parent = nullptr);

    void setView(const meow::db::ViewEntityPtr & view);

    meow::db::ViewEntityPtr sourceView() const {
        return _sourceView;
    }

    QString name() const;
    void setName(const QString & name);

    QString selectStatement() const;
    void setSelectStatement(const QString & select);

    QString definer() const;
    QStringList allDefinerOptions(bool loadFromDB = false) const;
    bool supportsDefiner() const;
    void setDefiner(const QString & definer);

    QString security() const;
    QStringList securityOptions() const;
    bool supportsSecurity() const;
    void setSecurity(const QString & security);

    QString algorithm() const;
    QStringList algorithmOptions() const;
    bool supportsAlgorithm() const;
    void setAlgorithm(const QString & algorithm);

    QString checkOption() const;
    QStringList checkOptions() const;
    bool supportsCheckOptions() const;
    void setCheckOption(const QString & opt);

    void save();

    bool hasUnsavedChanges() const { return _hasUnsavedChanges; }
    void setHasUnsavedChanges(bool modified);
    Q_SIGNAL void unsavedChanged(bool hasUnsavedChanges);

    bool isEditingSupported() const;

private:

    void setDefaultValuesForNewView();

    meow::db::ViewEntityPtr _view; // copy of source view to edit
    meow::db::ViewEntityPtr _sourceView;

    bool _hasUnsavedChanges;
};

} // namespace forms
} // namespace models
} // namespace meow

#endif // MODELS_VIEWFORM_H
