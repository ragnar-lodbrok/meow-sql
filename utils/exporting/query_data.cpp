#include "query_data.h"
#include <QTextCodec>


namespace meow {
namespace utils {
namespace exporting {

QueryData::QueryData()
{
    _encoding = defaultFileEncoding();
}

QStringList QueryData::supportedFileEncodings() const
{
    QList<QByteArray> codecs = QTextCodec::availableCodecs();

    QStringList codecNames;

    for (const QByteArray & codec : codecs) {
        QString name = QString::fromUtf8(codec);
        if (!codecNames.contains(name)) {
            codecNames.push_back(name);
        }
    }

    return codecNames;
}


} // namespace exporting
} // namespace utils
} // namespace meow
