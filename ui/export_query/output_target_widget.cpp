#include "output_target_widget.h"

namespace meow {
namespace ui {
namespace export_query {

OutputTargetWidget::OutputTargetWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(100, 120);

    createWidgets();
}

void OutputTargetWidget::createWidgets()
{
    QVBoxLayout * mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(mainLayout);

    _groupBox = new QGroupBox(tr("Output target"));
    _groupBox->setAlignment(Qt::AlignLeft);
    mainLayout->addWidget(_groupBox);

    QVBoxLayout * mainGroupLayout = new QVBoxLayout();

    _clipboardRadioButton = new QRadioButton(tr("Copy to clipboard"));
    _fileRadioButton = new QRadioButton(tr("File"));

    mainGroupLayout->addWidget(_clipboardRadioButton, 0, Qt::AlignTop);

    QGridLayout * gridLayout = new QGridLayout;

    gridLayout->addWidget(_fileRadioButton, 0, 0);

    QHBoxLayout * filenameLayout = new QHBoxLayout();

    _filenameEdit = new QLineEdit;
    _filenameSelectionButton = new QPushButton(
                QIcon(":/icons/folder_explore.png"),
                tr(""));
    /*connect(_filenameSelectionButton,
            &QAbstractButton::clicked,
            this,
            &OutputTargetWidget::onFilenameSelectionButtonClicked
    );*/
    _filenameSelectionButton->setMinimumWidth(30);

    filenameLayout->addWidget(_filenameEdit, 20);
    filenameLayout->addWidget(_filenameSelectionButton, 1);

    gridLayout->addLayout(filenameLayout, 0, 1);

    _encodingLabel = new QLabel(tr("Encoding:"));
    gridLayout->addWidget(_encodingLabel, 1, 0);

    _encodingCombobox = new QComboBox();
    gridLayout->addWidget(_encodingCombobox, 1, 1);

    gridLayout->setColumnStretch(0, 1);
    gridLayout->setColumnStretch(1, 4);
    gridLayout->setAlignment(Qt::AlignTop);

    mainGroupLayout->addLayout(gridLayout, 0);

    _groupBox->setLayout(mainGroupLayout);
}

} // namespace export_query
} // namespace ui
} // namespace meow
