#ifndef HELPERS_TEXT_H
#define HELPERS_TEXT_H

#include <QString>

namespace meow {
namespace helpers {

enum class LineBreaks {
    None,
    Windows, // \r\n
    Unix,    // \n
    OldMac,  // \r (macOS X uses Unix \n, but pre-X use \r)
    Unicode,
    Mixed
};

LineBreaks scanLineBreaks(const QString & str);

QString replaceLineBreaks(QString & str,
                          const LineBreaks oldBreaks,
                          const LineBreaks newBreaks);

QString strForLineBreaks(const LineBreaks breaks);

int countLineBreaks(const QString & str, const LineBreaks breaks);

bool hasLineBreaks(const QString & str);

} // namespace helpers
} // namespace meow

#endif // HELPERS_TEXT_H
