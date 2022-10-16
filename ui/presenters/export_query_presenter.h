#ifndef MEOW_UI_PRESENTERS_EXPORT_QUERY_PRESENTER_H
#define MEOW_UI_PRESENTERS_EXPORT_QUERY_PRESENTER_H

#include <memory>
#include <QStringList>
#include "utils/exporting/query_data_exporter.h"

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

    QStringList formatNames() const;

private:
    std::unique_ptr<utils::exporting::QueryDataExporter> _exporter;
};

} // namespace presenters
} // namespace ui
} // namespace meow

#endif // MEOW_UI_PRESENTERS_EXPORT_QUERY_PRESENTER_H
