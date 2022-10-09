#ifndef MEOW_UTILS_EXPORTING_QUERY_DATA_H
#define MEOW_UTILS_EXPORTING_QUERY_DATA_H

#include <QStringList>

namespace meow {
namespace utils {
namespace exporting {

class QueryData
{
public:
    QueryData();

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

private:
    Mode _mode = Mode::Clipboard;
    QString _encoding;
};

} // namespace exporting
} // namespace utils
} // namespace meow

#endif // MEOW_UTILS_EXPORTING_QUERY_DATA_H
