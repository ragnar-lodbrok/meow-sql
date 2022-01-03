#include "text_editor_popup.h"

// https://doc.qt.io/qt-5/qtwidgets-mainwindows-application-example.html

namespace meow {
namespace ui {

TextEditorPopup::TextEditorPopup()
    : QDialog(nullptr, Qt::WindowCloseButtonHint)
{
    setMinimumSize(500, 250);
    setTitleText("");

    createActions();
    createWidgets();

    connect(&_form, &presenters::TextEditorPopupForm::wordWrapChanged,
        [=](bool wrap){
            _textEdit->setWordWrapMode(
                wrap ? QTextOption::WrapAnywhere : QTextOption::NoWrap
            );
        });

    connect(&_form, &presenters::TextEditorPopupForm::lineBreaksChanged,
        [=](helpers::LineBreaks lineBreaks, bool detected){
            QAction * action = _lineBreaksActions.value(lineBreaks,
                                                        nullptr);
            if (action) {
                if (detected) {
                    const QString detectedStr = " (" + tr("detected") + ')';
                    if (!action->text().endsWith(detectedStr)) {
                        action->setText(
                            action->text() + detectedStr
                        );
                    }
                }
                updateLineBreaksButtonInfo(action);
            }
        });


    _form.setTextEdit(_textEdit);

    onTextChanged();
}

void TextEditorPopup::createWidgets()
{
    QVBoxLayout * mainLayout = new QVBoxLayout();
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    _textEdit = new ui::common::TextEditor();
    _textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    mainLayout->addWidget(_textEdit);

    connect(_textEdit, &QPlainTextEdit::textChanged,
            this, &TextEditorPopup::onTextChanged);

    _statusBar = new QStatusBar();
    _statusBar->setContentsMargins(0, 0, 0, 0);

    _toolbar = new QToolBar();
    _toolbar->setFloatable(false);
#ifdef Q_OS_MAC
    _dataToolBar->setStyle(QStyleFactory::create("windows"));
#endif

    _toolbar->addAction(_wordWrapAction);
    connect(_wordWrapAction, &QAction::toggled,
            this, &TextEditorPopup::onWordWrapToggled);

    _lineBreaksButton = new QToolButton();
    updateLineBreaksButtonInfo(defaultLineBreakAction());

    _toolbar->addWidget(_lineBreaksButton);
    _lineBreaksButton->setPopupMode(QToolButton::InstantPopup);

    auto it = _lineBreaksActions.constBegin();
    while (it != _lineBreaksActions.constEnd()) {
        QAction * action = it.value();
        _lineBreaksButton->addAction(action);
        connect(action, &QAction::triggered,
                this, &TextEditorPopup::onLineBreaksAction);
        ++it;
    }

    _toolbar->addSeparator();

    _toolbar->addAction(_cancelAction);
    QToolButton * cancelActionButton = static_cast<QToolButton *>(
        _toolbar->widgetForAction(_cancelAction)
    );
    cancelActionButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    _toolbar->addAction(_applyAction);
    QToolButton * applyActionButton = static_cast<QToolButton *>(
        _toolbar->widgetForAction(_applyAction)
    );
    applyActionButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    _textStat = new QLabel();


    QWidget * spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    _statusBar->addPermanentWidget(_toolbar);
    _statusBar->addPermanentWidget(spacer, 1);
    _statusBar->addPermanentWidget(_textStat);

    mainLayout->addWidget(_statusBar);

    setLayout(mainLayout);
}

void TextEditorPopup::createActions()
{
    _wordWrapAction = new QAction(QIcon(":/icons/wordwrap.png"),
                                  tr("Wrap long lines"), this);

    _wordWrapAction->setCheckable(true);

    _lineBreaksActions[helpers::LineBreaks::Windows] = new QAction(
                QIcon(":/icons/os_windows.png"),
                tr("Windows linebreaks [\\r\\n]"), this);

    _lineBreaksActions[helpers::LineBreaks::Unix] = new QAction(
                QIcon(":/icons/os_unix.png"),
                tr("Unix linebreaks [\\n]"), this);

    _lineBreaksActions[helpers::LineBreaks::OldMac] = new QAction(
                QIcon(":/icons/os_mac.png"),
                tr("Old macOS linebreaks [\\r]"), this);

    _lineBreaksActions[helpers::LineBreaks::Unicode] = new QAction(
                QIcon(":/icons/page_white_text.png"),
                tr("Unicode linebreaks"), this);

    _lineBreaksActions[helpers::LineBreaks::Mixed] = new QAction(
                QIcon(":/icons/go_both.png"),
                tr("Mixed linebreaks"), this);

    _cancelAction = new QAction(QIcon(":/icons/cross.png"),
                                tr("Cancel"), this);
    connect(_cancelAction, &QAction::triggered,
            this, &QDialog::reject);

    _applyAction = new QAction(QIcon(":/icons/tick.png"),
                                tr("Apply"), this);
    _applyAction->setShortcuts(QKeySequence::Save);
    _applyAction->setToolTip(tr("Apply changes"));
    connect(_applyAction, &QAction::triggered,
            this, &QDialog::accept);

}

void TextEditorPopup::setText(const QString & text)
{
    _form.setText(text);
    _textEdit->setPlainText(text);
}

void TextEditorPopup::setTitleText(const QString & text)
{
    QString caption = tr("Text editor");
    if (!text.isEmpty()) {
        caption = text + " - " + caption;
    }
    setWindowTitle(caption);
}

QString TextEditorPopup::text() const
{
    return _form.textWithCurLineBreaks();
}

QAction * TextEditorPopup::defaultLineBreakAction() const
{
    // TODO: take form settings/OS
    return _lineBreaksActions[helpers::LineBreaks::Windows];
}

void TextEditorPopup::updateLineBreaksButtonInfo(QAction * action)
{
    _lineBreaksButton->setIcon(action->icon());
    _lineBreaksButton->setText(action->text());
    _lineBreaksButton->setToolTip(action->toolTip());

    // make selected action bold
    auto it = _lineBreaksActions.constBegin();
    while (it != _lineBreaksActions.constEnd()) {
        QAction * actionIt = it.value();

        QFont font = actionIt->font();
        font.setBold(actionIt == action);
        actionIt->setFont(font);

        ++it;
    }
}

QString TextEditorPopup::textStats() const
{
    QString stats = tr("%1 characters").arg(_form.charCount());
    if (_form.maxLength()) {
        stats += " " + tr("(max: %1)").arg(_form.maxLength());
    }
    stats += ", ";
    stats += tr("%1 lines").arg(_form.lineNumber());

    return stats;
}

void TextEditorPopup::onLineBreaksAction()
{
    QAction * action = static_cast<QAction *>(sender());

    helpers::LineBreaks lineBreaks = _lineBreaksActions.key(action);
    _form.setLineBreaks(lineBreaks);
}

void TextEditorPopup::onWordWrapToggled(bool checked)
{
    _form.setWordWrap(checked);
}

void TextEditorPopup::onTextChanged()
{
    // TODO: do it by timer
    _textStat->setText(textStats());
}

} // namespace ui
} // namespace meow
