#ifndef MEOW_UI_PRESENTERS_EXPORT_QUERY_PRESENTER_H
#define MEOW_UI_PRESENTERS_EXPORT_QUERY_PRESENTER_H

#include <memory>
#include <QStringList>
#include "utils/exporting/query_data.h"

namespace meow {
namespace ui {
namespace presenters {

class ExportQueryPresenter
{
public:
    ExportQueryPresenter();

    void setModeClipboard();
    bool isModeClipboard() const;

    void setModeFile();
    bool isModeFile() const;

    QString fileEncoding() const;
    void setFileEncoding(const QString & encoding);
    QStringList supportedFileEncodings() const;

private:
    std::unique_ptr<utils::exporting::QueryData> _exporter;
};

} // namespace presenters
} // namespace ui
} // namespace meow

#endif // MEOW_UI_PRESENTERS_EXPORT_QUERY_PRESENTER_H
