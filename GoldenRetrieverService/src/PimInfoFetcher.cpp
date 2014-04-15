#include "precompiled.h"

#include "PimInfoFetcher.h"
#include "Command.h"
#include "IOUtils.h"
#include "Logger.h"
#include "MessageImporter.h"
#include "PimUtil.h"

namespace {

using namespace bb::pim::contacts;

QString textualize(ContactAttribute const& c) {
	return QString("%1: %2").arg( c.attributeDisplayLabel() ).arg( c.value() );
}

}

namespace golden {

using namespace bb::pim::calendar;
using namespace bb::pim::contacts;
using namespace canadainc;

PimInfoFetcher::PimInfoFetcher(QStringList const& tokens, int command) : m_tokens(tokens), m_command(command)
{
}


void PimInfoFetcher::run()
{
	switch (m_command)
	{
		case Command::UnreadSMS:
			fetchUnreadMessages();
			break;
		case Command::Calendar:
			processCalendarSearch();
			break;
		case Command::Contact:
			processContactSearch();
			break;
		default:
			break;
	}
}


void PimInfoFetcher::processContactSearch()
{
	QVariantList attachments;

	QString replyBody = tr("Contact name not specified. Please try something like:\ncontact Yunis");

	if ( !m_tokens.isEmpty() )
	{
		QString target = m_tokens.join(" "); // since the query can have multiple tokens

		ContactService service;
		ContactSearchFilters options;
		options.setSearchValue(target);

		QList<Contact> contacts = service.searchContacts(options);

		if ( !contacts.isEmpty() )
		{
			replyBody = tr( "%n contacts matched.", "", contacts.size() );

			for (int i = contacts.size()-1; i >= 0; i--)
			{
				Contact contact = service.contactDetails( contacts[i].id() );
				QString fileName = QString("%1.txt").arg( contact.displayName() );
				QString currentBody;

				QList<ContactAttribute> attribs = contact.phoneNumbers();
				LOGGER("=== Phone numbers found" << attribs.size() );

				for (int j = attribs.size()-1; j >= 0; j--) {
					currentBody += textualize(attribs[j]) + "\n";
				}

				attribs = contact.emails();
				LOGGER("=== Emails found" << attribs.size() );

				for (int j = attribs.size()-1; j >= 0; j--) {
					currentBody += textualize(attribs[j]) + "\n";
				}

				QString path = QString("%1/%2.txt").arg(IOUtils::directory_temp).arg( contact.id() );
				IOUtils::writeTextFile(path, currentBody);

				attachments << QVariant::fromValue( Attachment("text/plain", fileName, QUrl::fromLocalFile(path) ) );
			}
		} else {
			replyBody = tr("No contacts matched your search query.");
		}
	}

	emit commandProcessed(Command::Contact, replyBody, attachments);
}


void PimInfoFetcher::processCalendarSearch()
{
	QVariantList attachments;

	QString replyBody = tr("Event name not specified. Please try something like:\ncalendar Doctor");

	if ( !m_tokens.isEmpty() )
	{
		QString target = m_tokens.join(" "); // since the query can have multiple tokens with spaces

		CalendarService calendarService;

		EventSearchParameters params;
		QDateTime now = QDateTime::currentDateTime();
		params.setStart(now);
		params.setPrefix(target);
		params.setEnd( now.addYears(1) );
		params.setDetails(DetailLevel::Monthly);

		QList<CalendarEvent> evs = calendarService.events(params);

		if ( !evs.isEmpty() )
		{
			replyBody = tr( "%n events matched.", "", evs.size() );

			for (int i = evs.size()-1; i >= 0; i--)
			{
				CalendarEvent event = calendarService.event( evs[i].accountId(), evs[i].id() );
				QString fileName = QString("%1.txt").arg( event.subject() );
				QString currentBody = QString("From: %1\nTo: %2").arg( event.startTime().toString("MMM d/yy, hh:mm:ss") ).arg( event.endTime().toString("MMM d/yy, hh:mm:ss") );

				QString eventBody = event.body();

				if ( !eventBody.isEmpty() ) {
					currentBody += "\n\n"+eventBody;
				}

				QString path = QString("%1/%2.txt").arg(IOUtils::directory_temp).arg( event.id() );
				IOUtils::writeTextFile(path, currentBody);

				attachments << QVariant::fromValue( Attachment("text/plain", fileName, QUrl::fromLocalFile(path) ) );
			}
		} else {
			replyBody = tr("No events matched your search query.");
		}
	}

	emit commandProcessed(Command::Calendar, replyBody, attachments);
}


void PimInfoFetcher::fetchUnreadMessages()
{
	QVariantList attachments;

	MessageImporter importer(ACCOUNT_KEY_SMS);
	importer.setUnreadOnly();

	QVariantList result = importer.getResult();
	int n = result.size();

	for (int i = 0; i < n; i++)
	{
		QVariantMap current = result[i].toMap();
		QDateTime t = current["time"].toDateTime();
		QString body = QString("%1: %2").arg( t.toString("MMM d/yy, hh:mm:ss") ).arg( current["text"].toString() );
		QString sender = current["sender"].toString();
		QString address = current["senderAddress"].toString();
		qint64 messageId = current["id"].toLongLong();

		if (sender != address) {
			sender = QString("%1 (%2).txt").arg(sender).arg(address);
		} else {
			sender = QString("%1.txt").arg(sender);
		}

		QString path = QString("%1/%2.txt").arg(IOUtils::directory_temp).arg(messageId);
		bool written = IOUtils::writeTextFile(path, body);

		if (written) {
			LOGGER("ATTEMPTING TO ADD ATTACHMENT" << sender << body);
			attachments << QVariant::fromValue( Attachment("text/plain", sender, QUrl::fromLocalFile(path) ) );
		}
	}

	QString replyBody = n == 0 ? tr("No unread messages.") : tr("%n unread conversations", "", n);

	emit commandProcessed(Command::UnreadSMS, replyBody, attachments);
}

} /* namespace golden */
