//
// Created by cucaracha on 28.04.18.
//

#ifndef MININGSIMULATORGAMESERVER_COMPUTERCOMPONENT_H
#define MININGSIMULATORGAMESERVER_COMPUTERCOMPONENT_H

#include "PreDefine.h"
#include "all_const.h"

struct SComputerComponent : public SItemRecord {
    SComputerComponent(const SItemRecord &itemrecord)
            : SItemRecord(itemrecord) {

    }
};

struct SCCMatherboard : public SComputerComponent {
    SCCMatherboard(const SItemRecord &itemrecord)
            : SComputerComponent(itemrecord) {

    }
};

struct SCCCpu : public SComputerComponent {
    SCCCpu(const SItemRecord &itemrecord)
            : SComputerComponent(itemrecord) {

    }
};

struct SCCGpu : public SComputerComponent {
    SCCGpu(const SItemRecord &itemrecord)
            : SComputerComponent(itemrecord) {

    }
};

#endif //MININGSIMULATORGAMESERVER_COMPUTERCOMPONENT_H
