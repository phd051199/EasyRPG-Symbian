#ifndef EASYRPG_ZIP_VFS_H
#define EASYRPG_ZIP_VFS_H

#include <e32base.h>
#include <f32file.h>

class CZipVfsEntry;

class CZipVfs : public CBase
{
public:
    enum
    {
        KDefaultMaxFileBytes = 8 * 1024 * 1024
    };

    static CZipVfs* OpenL(RFs& aFs,
                          const TDesC& aArchivePath);
    ~CZipVfs();

    TBool Exists(const TDesC& aRelativePath) const;

    HBufC8* ReadFileLC(const TDesC& aRelativePath,
                       TInt aMaxBytes = KDefaultMaxFileBytes) const;
    void OpenStoredFileL(const TDesC& aRelativePath,
                         TInt& aDataOffset,
                         TInt& aFileSize) const;
    void ReadStoredFileL(TInt aDataOffset,
                         TInt aFileSize,
                         TInt aPosition,
                         TDes8& aBuffer) const;

    const TDesC& ArchivePath() const;
    const TDesC& ProjectPrefix() const;
    TInt EntryCount() const;
    void EntryRelativePathL(TInt aIndex, TDes& aPath) const;

private:
    enum
    {
        KBucketCount = 512,
        KMaxCentralDirectoryBytes = 2 * 1024 * 1024,
        KMaxArchivePathChars = KMaxFileName
    };

    CZipVfs(RFs& aFs);
    void ConstructL(const TDesC& aArchivePath);
    void BuildIndexL();
    void ResolveProjectRootL();
    void AddEntryL(const TDesC& aPath,
                   TUint32 aFileSize,
                   TUint32 aLocalHeaderOffset);

    const CZipVfsEntry* FindEntry(const TDesC& aRelativePath) const;
    const CZipVfsEntry* FindFullPath(const TDesC& aFullPath) const;
    void BuildFullPathL(const TDesC& aRelativePath, TDes& aFullPath) const;
    TInt EntryDataOffsetL(const CZipVfsEntry& aEntry) const;

    static TUint32 HashPath(const TDesC& aPath);
    static TBool PathEquals(const TDesC& aLeft, const TDesC& aRight);
    static TBool PathEndsWith(const TDesC& aPath, const TDesC& aSuffix);
    static void NormalizePathInPlaceL(TDes& aPath);

private:
    RFs& iFs;
    RFile iArchive;
    TFileName iArchivePath;
    HBufC* iProjectPrefix;
    RPointerArray<CZipVfsEntry> iEntries;
    RArray<TInt> iBuckets[KBucketCount];
};

#endif
