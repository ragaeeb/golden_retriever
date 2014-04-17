#ifndef ALARM_H_
#define ALARM_H_

#include <QVariantList>

#include <bb/multimedia/MediaPlayer>
#include <bb/device/VibrationController>

namespace golden {

using namespace bb::device;
using namespace bb::multimedia;

class Alarm : public QObject
{
    Q_OBJECT

    MediaPlayer m_player;
    VibrationController m_vibrator;
    unsigned int m_duration;
    float m_currentVol;

private slots:
    void timeout();

Q_SIGNALS:
    void commandProcessed(int command, QString const& replyBody, QVariantList const& attachments=QVariantList());

public:
    Alarm(QStringList const& tokens, QObject* parent=NULL);
    virtual ~Alarm();

    void play();
};

} /* namespace golden */

#endif /* ALARM_H_ */
