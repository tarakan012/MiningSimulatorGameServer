#include "Logging.h"
#include "Kernel.h"

using namespace MS::Server;

int main(int argc, char **argv) {
    LogPrint(LogFlags::ALL, "Server Start\n");

    CKernel &kernel = CKernel::GetInstance();
    kernel.Initialize();

    return 0;
}

