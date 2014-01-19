#include "precompiled.h"

#include "service.hpp"
#include "Logger.h"

using namespace bb;
using namespace golden;

namespace {

FILE* f = NULL;

void redirectedMessageOutput(QtMsgType type, const char *msg)
{
	Q_UNUSED(type);
	fprintf(f, "%s\n", msg);
}

}

Q_DECL_EXPORT int main(int argc, char **argv)
{
	f = fopen("/accounts/1000/shared/misc/golden.txt", "w");
	qInstallMsgHandler(redirectedMessageOutput);

	LOGGER("Started");

	Application app(argc, argv);
	new Service(&app);

	LOGGER("Executing event loop");

	return Application::exec();
}
