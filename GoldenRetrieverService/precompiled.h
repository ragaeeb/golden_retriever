#include <bb/Application>

#include <bb/data/XmlDataAccess>

#include <bb/device/BatteryInfo>
#include <bb/device/Led>

#include <bb/pim/account/AccountService>

#include <bb/pim/calendar/CalendarEvent>
#include <bb/pim/calendar/CalendarService>
#include <bb/pim/calendar/EventSearchParameters>

#include <bb/pim/contacts/ContactService>

#include <bb/pim/message/MessageBuilder>
#include <bb/pim/message/MessageFilter>
#include <bb/pim/message/MessageService>

#include <bb/platform/Notification>
#include <bb/platform/NotificationGlobalSettings>

#include <bps/audiomixer.h>

#include <QtLocationSubset/QGeoServiceProvider>
#include <QtLocationSubset/QGeoSearchManager>
#include <QtLocationSubset/QGeoSearchReply>

#include <QtConcurrentRun>
#include <QTimer>
