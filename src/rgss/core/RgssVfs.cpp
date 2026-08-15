#include "RgssVfs.h"
#include "RgssHost.h"

#include <cstdlib>
#include <cstring>

namespace {

RgssadArchive* gArchive = 0;
const unsigned char* gArchiveData = 0; /* malloc'd, owned here */
long gArchiveSize = 0;

void Unmount()
{
    RgssadFree(gArchive);
    gArchive = 0;
    free(const_cast<unsigned char*>(gArchiveData));
    gArchiveData = 0;
    gArchiveSize = 0;
}

} // namespace

extern "C" int RgssVfsMountRgssad(void)
{
    Unmount();

    static const char* kCandidates[] =
        { "Game.rgssad", "Game.rgss2a", "Game.rgss3a" };

    const RgssHostVtable* host = RgssHost();
    for (int i = 0; i < 3; ++i)
    {
        if (!host->FileExists(host->ctx, kCandidates[i]))
            continue;

        unsigned char* data = 0;
        const long size = host->FileRead(host->ctx, kCandidates[i], &data);
        if (size <= 0 || !data)
            continue;

        RgssadArchive* archive = 0;
        if (RgssadParse(data, size, &archive) != 0)
        {
            free(data);
            continue;
        }
        gArchive = archive;
        gArchiveData = data;
        gArchiveSize = size;
        return 0;
    }
    return -1;
}

extern "C" void RgssVfsUnmount(void)
{
    Unmount();
}

extern "C" int RgssVfsExists(const char* aPath)
{
    if (gArchive && RgssadFind(gArchive, aPath, 0) >= 0)
        return 1;

    const RgssHostVtable* host = RgssHost();
    return host->FileExists(host->ctx, aPath);
}

extern "C" long RgssVfsRead(const char* aPath, unsigned char** aOut)
{
    *aOut = 0;
    if (gArchive)
    {
        RgssadEntry entry;
        if (RgssadFind(gArchive, aPath, &entry) >= 0)
        {
            unsigned char* data = 0;
            const long size = RgssadReadEntry(gArchiveData, &entry, &data);
            if (size >= 0)
            {
                *aOut = data;
                return size;
            }
        }
    }

    const RgssHostVtable* host = RgssHost();
    unsigned char* data = 0;
    const long size = host->FileRead(host->ctx, aPath, &data);
    if (size >= 0)
        *aOut = data;
    return size;
}
