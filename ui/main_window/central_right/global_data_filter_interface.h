#ifndef CENTRAL_RIGHT_GLOBAL_DATA_FILTER_INTERFACE_H
#define CENTRAL_RIGHT_GLOBAL_DATA_FILTER_INTERFACE_H

#include <QString>

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

class IGlobalDataFilter
{
public:
    virtual ~IGlobalDataFilter() {}

    virtual void setFilterPattern(const QString & pattern,
                                  bool regexp = false) = 0;
    virtual QString filterPattern() const = 0;
    virtual bool filterPatternIsRegexp() const { return false; }

    virtual int totalRowCount() const = 0;
    virtual int filterMatchedRowCount() const = 0;

    virtual void resetFilter() {
        setFilterPattern(QString(), false);
    }
};

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

#endif // CENTRAL_RIGHT_GLOBAL_DATA_FILTER_INTERFACE_H
