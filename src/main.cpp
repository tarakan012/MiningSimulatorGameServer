#include "Kernel.h"
#include "util.h"

using namespace std;

int main(int argc, char **argv) {
    LogPrint(LogFlags::ALL, "Server Start\n");

    MS::Server::CKernel &kernel = MS::Server::CKernel::GetInstance();
    kernel.Initialize();

    return 0;
}

