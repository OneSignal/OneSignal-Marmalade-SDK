/*
C++ Shim implementation of the s3eOneSignal extension's.

Not intended for modifications

These functions are called by C# to access native interface and from C++ to
access managed interface.
*/
/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */
#include "s3eOneSignal_shim.h"

using namespace s3eOneSignalExtension;


IManagedS3Es3eOneSignalAPI^ s3eOneSignalShim::s_ManagedInterface = nullptr;

s3eOneSignalShim::s3eOneSignalShim()
{
}

s3eOneSignalShim::~s3eOneSignalShim()
{
    Terminate();
}

bool s3eOneSignalShim::Init(IManagedS3Es3eOneSignalAPI^ managedAPI)
{
    // It's an error to do this twice.
    if (s_ManagedInterface)
        return false;

    s_ManagedInterface = managedAPI;
    return true;
}

bool s3eOneSignalShim::Terminate()
{
    // It's an error to do this twice.
    if (!s_ManagedInterface)
        return false;

    s_ManagedInterface = nullptr;
    return true;
}

IManagedS3Es3eOneSignalAPI^ s3eOneSignalShim::GetInterface()
{
    return s_ManagedInterface;
}
