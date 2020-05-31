#include "text.h"

static const int MAX_STR_SCAN_LEN = 1024 * 1024;

#define SET_LINE_BREAKS(NEW_VAL) \
    if (result != (NEW_VAL)) {\
        if (result != LineBreaks::None) {\
            return LineBreaks::Mixed;\
        } else {\
            result = (NEW_VAL);\
        }\
    }

namespace meow {
namespace helpers {

LineBreaks scanLineBreaks(const QString & str)
{
    if (str.length() == 0) {
        return LineBreaks::None;
    }

    int size = std::min(str.length(), MAX_STR_SCAN_LEN);

    LineBreaks result = LineBreaks::None;

    for (int i=0; i < size; ++i) {
        const QChar c = str.at(i);
        if (c == QChar::CarriageReturn) { // \r
            if (i < size-1 && str.at(i+1) == QChar::LineFeed) { // \r\n
                SET_LINE_BREAKS(LineBreaks::Windows);
                ++i;
            } else {
                SET_LINE_BREAKS(LineBreaks::OldMac);
            }
        } else if (c == QChar::LineFeed) { // \n
            SET_LINE_BREAKS(LineBreaks::Unix);
        } else if (c == QChar::LineSeparator) { // \u2028
            SET_LINE_BREAKS(LineBreaks::Unicode);
        }
    }

    return result;
}

QString replaceLineBreaks(QString & str,
                          const LineBreaks oldBreaks,
                          const LineBreaks newBreaks)
{
    if (newBreaks == oldBreaks) {
        return str;
    }

    QString beforeStr = strForLineBreaks(oldBreaks);
    if (beforeStr.isEmpty()) {
        return str;
    }

    QString afterStr = strForLineBreaks(newBreaks);
    if (afterStr.isEmpty()) {
        return str;
    }


    // if we have 1 char line ending -- try to optimize
    if (beforeStr.length() == 1) {
        if (afterStr.length() == 1) { // 1 -> 1
            return str.replace(
                        beforeStr.at(0),
                        afterStr.at(0));
        } else {
            return str.replace( // 1 -> 2
                        beforeStr.at(0),
                        afterStr);
        }
    } else { // 2 chars
        if (afterStr.length() == 1) { // 2 -> 1
            return str.replace(
                        beforeStr,
                        afterStr.at(0));
        } else {
            return str.replace( // 2 -> 2
                        beforeStr,
                        afterStr);
        }
    }
}

QString strForLineBreaks(const LineBreaks breaks)
{
    switch (breaks) {

    case LineBreaks::Windows:
        return QString("\r\n");

    case LineBreaks::Unix:
        return QString("\n");

    case LineBreaks::OldMac:
        return QString("\r");

    case LineBreaks::Unicode:
        return QString(QChar::LineSeparator);


    default:
        return {};
    }
}

int countLineBreaks(const QString & str, const LineBreaks breaks)
{
    QString lineBreaksStr = strForLineBreaks(breaks);
    if (lineBreaksStr.isEmpty()) {
        return 0;
    } else if (lineBreaksStr.length() == 1) {
        return str.count(lineBreaksStr.at(0));
    } else {
        return str.count(lineBreaksStr);
    }
}

bool hasLineBreaks(const QString & str)
{
    return str.contains(QChar::LineFeed) // \n
            || str.contains(QChar::CarriageReturn) // \r
            || str.contains(QChar::LineSeparator); // \u2028
}

} // namespace helpers
} // namespace meow
