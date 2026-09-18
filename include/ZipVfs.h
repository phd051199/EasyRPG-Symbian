#ifndef EASYRPG_ZIP_VFS_H
#define EASYRPG_ZIP_VFS_H

#include <e32base.h>
#include <f32file.h>

struct TZipVfsEntry
{
    TUint32 iHash;
    TUint32 iFileSize;
    TUint32 iLocalHeaderOffset;
    TInt iCentralOffset;
    mutable TInt iDataOffset;
};

class CZipVfs : public CBase
{
  public:
    enum
    {
        KDefaultMaxFileBytes = 8 * 1024 * 1024
    };

    static CZipVfs *OpenL(RFs &aFs,
                          const TDesC &aArchivePath);
    ~CZipVfs();

    TBool Exists(const TDesC &aRelativePath) const;

    HBufC8 *ReadFileLC(const TDesC &aRelativePath,
                       TInt aMaxBytes = KDefaultMaxFileBytes) const;
    void OpenStoredFileL(const TDesC &aRelativePath,
                         TInt &aDataOffset,
                         TInt &aFileSize) const;
    void ReadStoredFileL(TInt aDataOffset,
                         TInt aFileSize,
                         TInt aPosition,
                         TDes8 &aBuffer) const;

    TInt EntryCount() const;
    void EntryRelativePathL(TInt aIndex, TDes &aPath) const;

  private:
    enum
    {
        KBucketCount = 2048,
        KMaxCentralDirectoryBytes = 2 * 1024 * 1024,
        KMaxArchivePathChars = KMaxFileName
    };

    CZipVfs(RFs &aFs);
    void ConstructL(const TDesC &aArchivePath);
    void BuildIndexL();
    void ResolveProjectRootL();
    void PruneToProjectRootL();
    void AddEntryL(const TDesC &aPath,
                   TUint32 aFileSize,
                   TUint32 aLocalHeaderOffset,
                   TInt aCentralOffset);
    void DecodeEntryPathL(const TZipVfsEntry &aEntry, TDes &aPath) const;

    const TZipVfsEntry *FindEntry(const TDesC &aRelativePath) const;
    const TZipVfsEntry *FindFullPath(const TDesC &aFullPath) const;
    void BuildFullPathL(const TDesC &aRelativePath, TDes &aFullPath) const;
    TInt EntryDataOffsetL(const TZipVfsEntry &aEntry) const;

    static TUint32 HashPath(const TDesC &aPath);
    static TBool PathEquals(const TDesC &aLeft, const TDesC &aRight);
    static TBool PathEndsWith(const TDesC &aPath, const TDesC &aSuffix);
    static void NormalizePathInPlaceL(TDes &aPath);

  private:
    RFs &iFs;
    RFile iArchive;
    HBufC *iProjectPrefix;
    HBufC8 *iCentralDirectory;
    TInt iArchiveSize;
    RArray<TZipVfsEntry> iEntries;
    RArray<TInt> iBuckets[KBucketCount];
};

#endif
