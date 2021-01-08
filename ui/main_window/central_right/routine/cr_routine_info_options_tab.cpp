#include "cr_routine_info_options_tab.h"
#include "models/forms/routine_form.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {
namespace routine_info {

OptionsTab::OptionsTab(models::forms::RoutineForm *form,
                       QWidget *parent)
    : QWidget(parent)
    , _form(form)
{
    createWidgets();
    validateControls();
}

void OptionsTab::refreshData()
{
    fillDataFromForm();
    validateControls();
}

void OptionsTab::createWidgets()
{
    QGridLayout * mainGridLayout = new QGridLayout();
    int row = 0;

    // Name
    _nameLabel = new QLabel(tr("Name:"));
    mainGridLayout->addWidget(_nameLabel, row, 0);

    _nameEdit = new QLineEdit();
    _nameLabel->setBuddy(_nameEdit);
    connect(_nameEdit, &QLineEdit::textEdited,
            [=](const QString &name) {
                if (!_form->isEditingSupported()) return;
                _form->setName(name);
            });
    mainGridLayout->addWidget(_nameEdit, row, 1);

    // Definer
    _definerLabel = new QLabel(tr("Definer:"));
    mainGridLayout->addWidget(_definerLabel, row, 2);

    _definerCombobox = new QComboBox();
    _definerLabel->setBuddy(_definerCombobox);
    connect(_definerCombobox,
            &QComboBox::currentTextChanged,
            [=](const QString & definer) {
                if (!_form->isEditingSupported()) {
                    _definerCombobox->setCurrentIndex(
                        _definerCombobox->findText(_form->definer())
                    );
                    return;
                }
                _form->setDefiner(definer);
            });
    mainGridLayout->addWidget(_definerCombobox, row, 3);

    row++;

    // Comment
    _commentLabel = new QLabel(tr("Comment:"));
    mainGridLayout->addWidget(_commentLabel, row, 0);

    _commentEdit = new QLineEdit();
    _commentLabel->setBuddy(_commentEdit);
    connect(_commentEdit, &QLineEdit::textEdited,
            [=](const QString &comment) {
                if (!_form->isEditingSupported()) return;
                _form->setComment(comment);
            });
    mainGridLayout->addWidget(_commentEdit, row, 1, 1, 3);

    row++;

    // Routine type
    _routineTypeLabel = new QLabel(tr("Type:"));
    mainGridLayout->addWidget(_routineTypeLabel, row, 0);

    _routineTypeCombobox = new QComboBox();
    _routineTypeLabel->setBuddy(_routineTypeCombobox);
    connect(_routineTypeCombobox,
#if QT_VERSION >= QT_VERSION_CHECK(5, 7, 0)
            QOverload<int>::of(&QComboBox::currentIndexChanged),
#else
            static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
#endif
            [=](int index) {
                if (!_form->isEditingSupported()) {
                    // QMap is ordered so use position of value as index in cb
                    const auto & types = _form->typeNames();
                    const auto it = types.constFind(_form->type());
                    if (it == types.constEnd()) return;
                    int formValueIndex = std::distance(types.constBegin(), it);
                    _routineTypeCombobox->setCurrentIndex(formValueIndex);
                    return;
                }
                const auto & types = _form->typeNames();
                auto it = types.constBegin();
                std::advance(it, index);
                if (it != types.constEnd()) {
                    _form->setType(it.key());
                }
                validateControls();
            });
    mainGridLayout->addWidget(_routineTypeCombobox, row, 1);

    // Data access
    _dataAccessLabel = new QLabel(tr("Data access:"));
    mainGridLayout->addWidget(_dataAccessLabel, row, 2);

    _dataAccessCombobox = new QComboBox();
    _dataAccessLabel->setBuddy(_dataAccessCombobox);
    connect(_dataAccessCombobox,
            &QComboBox::currentTextChanged,
            [=](const QString & access) {
                if (!_form->isEditingSupported()) {
                    _dataAccessCombobox->setCurrentIndex(
                        _dataAccessCombobox->findText(_form->dataAccess())
                    );
                    return;
                }
                _form->setDataAccess(access);
            });
    mainGridLayout->addWidget(_dataAccessCombobox, row, 3);

    row++;

    // Return type
    _returnTypeLabel = new QLabel(tr("Returns:"));
    mainGridLayout->addWidget(_returnTypeLabel, row, 0);

    _returnTypeCombobox = new QComboBox();
    _returnTypeLabel->setBuddy(_returnTypeCombobox);
    connect(_returnTypeCombobox,
            &QComboBox::currentTextChanged,
            [=](const QString & returnType) {
                if (!_form->isEditingSupported()) {
                    _returnTypeCombobox->setCurrentIndex(
                        _returnTypeCombobox->findText(_form->returnType())
                    );
                    return;
                }
                _form->setReturnType(returnType);
            });
    mainGridLayout->addWidget(_returnTypeCombobox, row, 1);

    // SQL security
    _sqlSecurityLabel = new QLabel(tr("SQL Security:"));
    mainGridLayout->addWidget(_sqlSecurityLabel, row, 2);

    _sqlSecurityCombobox = new QComboBox();
    _sqlSecurityLabel->setBuddy(_sqlSecurityCombobox);
    connect(_sqlSecurityCombobox,
            &QComboBox::currentTextChanged,
            [=](const QString & security) {
                if (!_form->isEditingSupported()) {
                    _sqlSecurityCombobox->setCurrentIndex(
                        _sqlSecurityCombobox->findText(_form->security())
                    );
                    return;
                }
                _form->setSecurity(security);
            });
    mainGridLayout->addWidget(_sqlSecurityCombobox, row, 3);

    row++;

    // Deterministic
    _deterministicChecbox = new QCheckBox(tr("Deterministic"));
    connect(_deterministicChecbox, &QCheckBox::stateChanged,
            [=](int state){
                Q_UNUSED(state);
                if (!_form->isEditingSupported()) {
                    _deterministicChecbox->setCheckState(
                        _form->isDeterministic() ?
                        Qt::CheckState::Checked : Qt::CheckState::Unchecked);
                    return;
                }
                _form->setDeterministic(
                static_cast<Qt::CheckState>(state) == Qt::CheckState::Checked);
            });
    mainGridLayout->addWidget(_deterministicChecbox, row, 1);

    // ----------

    mainGridLayout->setColumnStretch(0, 1);
    mainGridLayout->setColumnStretch(1, 4);
    mainGridLayout->setColumnStretch(2, 1);
    mainGridLayout->setColumnStretch(3, 6);

    mainGridLayout->setHorizontalSpacing(15);

    mainGridLayout->setAlignment(Qt::AlignTop);

    setLayout(mainGridLayout);
}

void OptionsTab::fillDataFromForm()
{
    _nameEdit->blockSignals(true);
    _nameEdit->setText(_form->name());
    _nameEdit->setReadOnly(!_form->isEditingSupported());
    _nameEdit->blockSignals(false);

    _definerCombobox->blockSignals(true);
    _definerCombobox->clear();
    _definerCombobox->addItems(
        _form->allDefinerOptions(_form->isEditingSupported())
     ); // TODO: lazy load
    _definerCombobox->setCurrentIndex(
        _definerCombobox->findText(_form->definer()));
    _definerCombobox->setEditable(_form->isEditingSupported());
    _definerCombobox->blockSignals(false);

    _commentEdit->blockSignals(true);
    _commentEdit->setText(_form->comment());
    _commentEdit->setReadOnly(!_form->isEditingSupported());
    _commentEdit->blockSignals(false);

    _routineTypeCombobox->blockSignals(true);
    _routineTypeCombobox->clear();
    {
        QStringList routineTypeNames = _form->typeNames().values();
        _routineTypeCombobox->addItems(routineTypeNames);
        const auto & routineTypes = _form->typeNames();
        const auto it = routineTypes.constFind(_form->type());
        if (it != routineTypes.constEnd()) {
            int formValueIndex = std::distance(routineTypes.constBegin(), it);
            _routineTypeCombobox->setCurrentIndex(formValueIndex);
        }
    }
    _routineTypeCombobox->blockSignals(false);

    _dataAccessCombobox->blockSignals(true);
    _dataAccessCombobox->clear();
    _dataAccessCombobox->addItems(_form->dataAccessList());
    _dataAccessCombobox->setCurrentIndex(
        _dataAccessCombobox->findText(_form->dataAccess()));
    _dataAccessCombobox->blockSignals(false);

    _returnTypeCombobox->blockSignals(true);
    _returnTypeCombobox->clear();
    _returnTypeCombobox->addItems(_form->returnTypes());
    _returnTypeCombobox->setCurrentIndex(
        _returnTypeCombobox->findText(_form->returnType()));
    _returnTypeCombobox->setEditable(_form->isEditingSupported());
    _returnTypeCombobox->blockSignals(false);

    _sqlSecurityCombobox->blockSignals(true);
    _sqlSecurityCombobox->clear();
    _sqlSecurityCombobox->addItems(_form->securityOptions());
    _sqlSecurityCombobox->setCurrentIndex(
        _sqlSecurityCombobox->findText(_form->security()));
    _sqlSecurityCombobox->setEnabled(_form->supportsSecurity());
    _sqlSecurityCombobox->blockSignals(false);

    _deterministicChecbox->blockSignals(true);
    _deterministicChecbox->setCheckState(
        _form->isDeterministic() ?
        Qt::CheckState::Checked : Qt::CheckState::Unchecked);
    _deterministicChecbox->blockSignals(false);
}

void OptionsTab::validateControls()
{
    _returnTypeLabel->setEnabled(_form->supportsReturnType());
    _returnTypeCombobox->setEnabled(_form->supportsReturnType());
}

} // namespace routine_info
} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow
