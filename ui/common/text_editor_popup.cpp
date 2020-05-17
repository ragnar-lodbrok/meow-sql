#include "text_editor_popup.h"

// https://doc.qt.io/qt-5/qtwidgets-mainwindows-application-example.html

namespace meow {
namespace ui {

TextEditorPopup::TextEditorPopup()
    : QDialog(nullptr, Qt::WindowCloseButtonHint)
{
    setMinimumSize(500, 250);

    createActions();
    createWidgets();
}

void TextEditorPopup::createWidgets()
{
    QVBoxLayout * mainLayout = new QVBoxLayout();
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    _textEdit = new QPlainTextEdit();
    // TODO: monospace font
    // TODO: line numbers
    mainLayout->addWidget(_textEdit);

    _statusBar = new QStatusBar();
    _statusBar->setContentsMargins(0, 0, 0, 0);

    _toolbar = new QToolBar();
    _toolbar->setFloatable(false);
#ifdef Q_OS_MAC
    _dataToolBar->setStyle(QStyleFactory::create("windows"));
#endif

    _toolbar->addAction(_wordWrapAction);

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
    _textStat->setText("%s characters (max: %s), %s lines");


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
    _applyAction = new QAction(QIcon(":/icons/tick.png"),
                                tr("Apply"), this);
    _applyAction->setToolTip(tr("Apply changes"));

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
}

void TextEditorPopup::onLineBreaksAction()
{
    QAction * action = static_cast<QAction *>(sender());
    updateLineBreaksButtonInfo(action);
}

} // namespace ui
} // namespace meow
