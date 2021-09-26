#ifndef UDTDEMO_SOCKETBASE_H
#define UDTDEMO_SOCKETBASE_H

#include <sys/types.h>

class CSocketBase
{
protected:
    int mSock;
    unsigned short mLocalPort; //bind port
public:
    CSocketBase()
    {
        mSock = -1;
        mLocalPort = 0;
    }
    virtual ~CSocketBase()
    {
        //
    }

    virtual void close()=0;

    virtual int getSock()
    {
        return mSock;
    }

    virtual u_short getLocalPort()
    {
        return mLocalPort;
    }
};


#endif //UDTDEMO_SOCKETBASE_H
