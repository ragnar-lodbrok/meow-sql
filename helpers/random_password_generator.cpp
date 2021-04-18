#include "random_password_generator.h"
#include <random>

namespace meow {
namespace helpers {

RandomPasswordGenerator::RandomPasswordGenerator()
{

}

QString RandomPasswordGenerator::run(int length) const
{
    if (length <= 0) {
        return QString();
    }

    // Fill password with nulls, place required number of digist and special
    // chars to random free positions, then fill all non-taken with letters.

    QString password;
    password.fill(0, length);

    int generatedChars = 0;
    int i = 0;
    while (i < _minNumberCount && generatedChars < length) {
        int randomPos = randomInt(length-1);
        if (!password[randomPos].isNull()) continue;
        password[randomPos] = _digits[randomInt(_digits.length()-1)];
        ++generatedChars;
        ++i;
    }

    i = 0;
    while (i < _minSpecialCharCount && generatedChars < length) {
        int randomPos = randomInt(length-1);
        if (!password[randomPos].isNull()) continue;
        password[randomPos] = _specialChars[randomInt(_specialChars.length()-1)];
        ++generatedChars;
        ++i;
    }

    if (generatedChars == length) {
        return password;
    }

    for (i = 0; i < length; ++i) {
        if (!password[i].isNull()) continue; // taken by digit or special
        if (i % 2 == 0) {
            password[i] = _lettersLower[randomInt(_lettersLower.length()-1)];
        } else {
            password[i] = _lettersUpper[randomInt(_lettersUpper.length()-1)];
        }
    }

    return password;
}

int RandomPasswordGenerator::randomInt(int max) const
{
    std::random_device randomDevice;
    std::default_random_engine randomEngine(randomDevice());
    std::uniform_int_distribution<int> uniform_dist(0, max);

    return uniform_dist(randomEngine);
}

} // namespace helpers
} // namespace meow
