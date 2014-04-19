#ifndef INTERPRETERHELPER_H_
#define INTERPRETERHELPER_H_

#define equals(b) command.compare(b, Qt::CaseInsensitive) == 0

namespace golden {

class InterpreterHelper
{
public:
    static QString fetchBatteryInfo(QStringList const& tokens);
    static QString processProfile(QStringList const& tokens);
    static QString processVolume(QStringList const& tokens);
};

} /* namespace golden */

#endif /* INTERPRETERHELPER_H_ */
