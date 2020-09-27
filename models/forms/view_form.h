#ifndef MODELS_VIEWFORM_H
#define MODELS_VIEWFORM_H

#include <QString>

namespace meow {

namespace db {
    class ViewEntity;
}

namespace models {
namespace forms {

class ViewForm
{
public:
    ViewForm();

    void setView(meow::db::ViewEntity * view);

    QString name() const;

    QString selectStatement() const;

    QString definer() const;
    QStringList allDefinerOptions() const;
    bool supportsDefiner() const;

    QString security() const;
    QStringList securityOptions() const;
    bool supportsSecurity() const;

    QString algorithm() const;
    QStringList algorithmOptions() const;
    bool supportsAlgorithm() const;

    QString checkOption() const;
    QStringList checkOptions() const;
    bool supportsCheckOptions() const;

    bool isEditingSupported() const {
        return false;
    }

private:
    meow::db::ViewEntity * _view;
};

} // namespace forms
} // namespace models
} // namespace meow

#endif // MODELS_VIEWFORM_H
