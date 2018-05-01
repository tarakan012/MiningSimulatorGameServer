#ifndef MININGSIMULATORGAMESERVER_ALLERRORCODE_H
#define MININGSIMULATORGAMESERVER_ALLERRORCODE_H

enum ePreDefineErrorCode {
    eNormal = 0
};

enum eAllErrorCode {
    eEC_Begin = 0,
    eEC_InvalidCompId,
    eEC_NotEnoughEnergy,
    eEC_NotEnoughGold,
    eEC_InvalidItemId,
    eEC_End
};

#endif //MININGSIMULATORGAMESERVER_ALLERRORCODE_H