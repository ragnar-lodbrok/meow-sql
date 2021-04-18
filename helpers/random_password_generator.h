#ifndef RANDOM_PASSWORD_GENERATOR_H
#define RANDOM_PASSWORD_GENERATOR_H

#include <QString>

namespace meow {
namespace helpers {

class RandomPasswordGenerator
{
public:
    RandomPasswordGenerator();

    QString run(int length = 8) const;

    void setMinNumberCount(int min) {
        _minNumberCount = min;
    }

    void setMinSpecialCharCount(int min) {
        _minSpecialCharCount = min;
    }

private:

    int randomInt(int max) const;

    int _minNumberCount = 1;
    int _minSpecialCharCount = 1;

    const QString _specialChars = "[]<>:@/+-_!=&;!|$()*?#";
    const QString _lettersLower = "abcdefghijklmnopqrstuvwxyz";
    const QString _lettersUpper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const QString _digits = "0123456789";
};

} // namespace helpers
} // namespace meow

#endif // RANDOM_PASSWORD_GENERATOR_H
