#ifndef BUFFER_H
#define BUFFER_H


#include <cstring>



    class CBuffer
    {
    public:
        CBuffer():m_MaxLength(1024*10),m_OriMaxLength(m_MaxLength),m_pMsg(new char[m_MaxLength]), m_CurLen(0), m_LogLvl(0), m_GOLen(0)
        {}
        CBuffer(int size):m_MaxLength(size),m_OriMaxLength(m_MaxLength),m_pMsg(new char[m_MaxLength]), m_CurLen(0), m_LogLvl(0), m_GOLen(0)
        {}
        bool Append(std::string & str)
        {
            Append(str.c_str(),str.size());
            return true;
        }
        char* GetCurData(){
        return m_pMsg + m_GOLen;
    }
        int		GetDataLength(){
        return m_CurLen;
    }
        bool Append(const char * pData, int n32Length)
        {
            if (!pData) {
                return false;
        }

//#include<iostream>
            //std::cout << "len " << n32Length;
           while(m_CurLen + n32Length > m_MaxLength)
            {
               m_MaxLength *= 2;
                m_pMsg = (char*)realloc(m_pMsg, m_MaxLength);
            }
            std::memcpy(m_pMsg + m_CurLen,pData,n32Length);
            m_CurLen+= n32Length;
            return true;
        }
            void Clear(){
        if (m_MaxLength > m_OriMaxLength){
            delete []m_pMsg;
            m_pMsg = new char[m_OriMaxLength];
            m_MaxLength = m_OriMaxLength;
        }
        if (m_CurLen > 0){
            m_CurLen = 0;
        }
        m_GOLen = 0;
    }
        int m_LogLvl;
    private:
        char * m_pMsg;
        int m_GOLen;
        const int m_OriMaxLength;
        int m_CurLen;
        int m_MaxLength;


    };




#endif /* BUFFER_H */

