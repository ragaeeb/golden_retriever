#include "precompiled.h"

#include "Logger.h"
#include "applicationui.hpp"

using namespace bb::cascades;

#if !defined(QT_NO_DEBUG)
namespace {

void redirectedMessageOutput(QtMsgType type, const char *msg) {
	Q_UNUSED(type);
	fprintf(stderr, "%s\n", msg);
}

}
#endif

Q_DECL_EXPORT int main(int argc, char **argv)
{
#if !defined(QT_NO_DEBUG)
	qInstallMsgHandler(redirectedMessageOutput);
#endif

    Application app(argc, argv);
    golden::ApplicationUI::create(&app);

    return Application::exec();
}
