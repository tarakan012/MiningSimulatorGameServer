#include "ServerMS/Log/Logging.h"
#include "ServerMS/Kernel.h"

using namespace Log;

int main(int argc, char **argv) {
    LogPrint(LogFlags::ALL, "Server Start\n");

    ServerMS::CKernel &kernel = ServerMS::CKernel::GetInstance();
    kernel.Initialize();

    return 0;
}