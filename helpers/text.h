#ifndef HELPERS_TEXT_H
#define HELPERS_TEXT_H

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


} // namespace helpers
} // namespace meow

#endif // HELPERS_TEXT_H
