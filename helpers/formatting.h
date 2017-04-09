#ifndef FORMATTING_H
#define FORMATTING_H

#include <QString>

namespace meow {
namespace helpers {

typedef unsigned long long byteSize;

QString formatByteSize(byteSize bytes, int decimals = 1);

} // namespace helpers
} // namespace meow

#endif // FORMATTING_H
