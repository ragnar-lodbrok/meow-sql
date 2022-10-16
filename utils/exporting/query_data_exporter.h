#ifndef MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORTER_H
#define MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORTER_H

#include <QStringList>
#include "query_data_export_formats/format_interface.h"

namespace meow {
namespace utils {
namespace exporting {

class QueryDataExporter
{
public:
    QueryDataExporter();

    enum class Mode {
        Clipboard,
        File
    };

    void setMode(Mode mode) {
        _mode = mode;
    }

    Mode mode() const {
        return _mode;
    }

    QStringList supportedFileEncodings() const;
    QString fileEncoding() const {
        return _encoding;
    }
    QString defaultFileEncoding() const {
        return "UTF-8";
    }
    void setFileEncoding(const QString & encoding) {
        _encoding = encoding;
    }

    QStringList formatNames() const;

private:
    Mode _mode = Mode::Clipboard;
    QString _encoding;

    std::vector<QueryDataExportFormatPtr> _formats;
};

} // namespace exporting
} // namespace utils
} // namespace meow

#endif // MEOW_UTILS_EXPORTING_QUERY_DATA_EXPORTER_H
