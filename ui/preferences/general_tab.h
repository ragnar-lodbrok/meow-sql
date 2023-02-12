#ifndef UI_PREFERENCES_GENERAL_TAB_H
#define UI_PREFERENCES_GENERAL_TAB_H

#include <QtWidgets>

namespace meow {
namespace ui {

namespace presenters {
    class PreferencesPresenter;
}

namespace preferences {

class GeneralTab : public QWidget
{
    Q_OBJECT
public:
    explicit GeneralTab(presenters::PreferencesPresenter * presenter,
                        QWidget *parent = nullptr);
private:
    void createWidgets();
    void fillDataFromPresenter();

    Q_SLOT void onLanguageComboboxIndexChanged(int index);
    Q_SLOT void onLanguagePresenterChanged();

    presenters::PreferencesPresenter * _presenter;

    QLabel * _languageLabel;
    QComboBox * _languageComboBox;
};

} // namespace preferences
} // namespace ui
} // namespace meow

#endif // UI_PREFERENCES_GENERAL_TAB_H
