#include "ServerMS/Log/Logging.h"
#include "ServerMS/Kernel.h"

using namespace Log;

int main(int argc, char **argv) {
    LogPrint(LogFlags::ALL, "Server Start\n");

    auto kernel = boost::make_shared<ServerMS::CKernel>();
    kernel->Initialize();
    kernel->MainLoop();
    return 0;
}