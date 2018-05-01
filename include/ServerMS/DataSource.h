#ifndef DATASOURCE_H
#define DATASOURCE_H

#include "ServerMS/DB/DBManager.h"

namespace ServerMS {

    class CDataSource {
    public:

        void Start();

        DB::CDBConnector *GetDBConnector();

    private:
        DB::CDBConnector *m_pDBConnector;
    };

}

#endif /* DATASOURCE_H */