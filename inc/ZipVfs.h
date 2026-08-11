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
                          const TDesC& aArchivePath,
                          const TDesC8& aPassword);
    ~CZipVfs();

    TBool Exists(const TDesC& aRelativePath) const;

    HBufC8* ReadFileLC(const TDesC& aRelativePath,
                       TInt aMaxBytes = KDefaultMaxFileBytes) const;

    void MaterializeFileL(const TDesC& aRelativePath,
                          const TDesC& aCacheFileName,
                          TDes& aMaterializedPath,
                          TInt aMaxBytes = KDefaultMaxFileBytes) const;

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
    void ConstructL(const TDesC& aArchivePath, const TDesC8& aPassword);
    void BuildIndexL();
    void ResolveProjectRootL();
    void AddEntryL(const TDesC& aPath,
                   TUint16 aFlags,
                   TUint16 aMethod,
                   TUint32 aCrc32,
                   TUint32 aCompressedSize,
                   TUint32 aUncompressedSize,
                   TUint32 aLocalHeaderOffset,
                   TUint8 aAesStrength,
                   TUint16 aAesMethod);

    const CZipVfsEntry* FindEntry(const TDesC& aRelativePath) const;
    const CZipVfsEntry* FindFullPath(const TDesC& aFullPath) const;
    void BuildFullPathL(const TDesC& aRelativePath, TDes& aFullPath) const;
    TInt EntryDataOffsetL(const CZipVfsEntry& aEntry) const;
    void MaterializeEntryL(const CZipVfsEntry& aEntry, RFile& aOutput, TInt aMaxBytes) const;
    HBufC8* DecodeEntryLC(const CZipVfsEntry& aEntry, TInt aMaxBytes) const;

    static TUint32 HashPath(const TDesC& aPath);
    static TBool PathEquals(const TDesC& aLeft, const TDesC& aRight);
    static TBool PathEndsWith(const TDesC& aPath, const TDesC& aSuffix);
    static void NormalizePathInPlaceL(TDes& aPath);

private:
    RFs& iFs;
    RFile iArchive;
    TFileName iArchivePath;
    HBufC8* iPassword;
    HBufC* iProjectPrefix;
    RPointerArray<CZipVfsEntry> iEntries;
    RArray<TInt> iBuckets[KBucketCount];
};

#endif
