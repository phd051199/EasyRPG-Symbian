# Common class guide index

> This file consolidates canonical handbook topics for progressive loading. Each section records its original corpus path and retains external source links.

---

<a id="src-12-class-guides-000-class-matrix-md"></a>
## 000 CLASS MATRIX

- Original corpus path: `12_CLASS_GUIDES/000_CLASS_MATRIX.md`
- Consolidated reference: `40-class-guide-index.md#src-12-class-guides-000-class-matrix-md`

### Common class matrix

| # | Class | Header | Library | Capability | Confidence |
|---:|---|---|---|---|---|
| 1 | [`CBase`](41-class-guides-001-025.md#src-12-class-guides-001-cbase-md) | `e32base.h` | `euser.lib` | `None` | verified-common |
| 2 | [`CleanupStack`](41-class-guides-001-025.md#src-12-class-guides-002-cleanupstack-md) | `e32base.h` | `euser.lib` | `None` | verified-common |
| 3 | [`CActive`](41-class-guides-001-025.md#src-12-class-guides-003-cactive-md) | `e32base.h` | `euser.lib` | `None` | verified-common |
| 4 | [`CActiveScheduler`](41-class-guides-001-025.md#src-12-class-guides-004-cactivescheduler-md) | `e32base.h` | `euser.lib` | `None` | verified-common |
| 5 | [`CTimer`](41-class-guides-001-025.md#src-12-class-guides-005-ctimer-md) | `e32base.h` | `euser.lib` | `None` | verified-common |
| 6 | [`RTimer`](41-class-guides-001-025.md#src-12-class-guides-006-rtimer-md) | `e32std.h` | `euser.lib` | `None` | verified-common |
| 7 | [`RProcess`](41-class-guides-001-025.md#src-12-class-guides-007-rprocess-md) | `e32std.h` | `euser.lib` | `operation-specific` | verified-common |
| 8 | [`RThread`](41-class-guides-001-025.md#src-12-class-guides-008-rthread-md) | `e32std.h` | `euser.lib` | `operation-specific` | verified-common |
| 9 | [`RProperty`](41-class-guides-001-025.md#src-12-class-guides-009-rproperty-md) | `e32property.h` | `euser.lib` | `policy-specific` | verified-common |
| 10 | [`RFs`](41-class-guides-001-025.md#src-12-class-guides-010-rfs-md) | `f32file.h` | `efsrv.lib` | `path-specific` | verified-common |
| 11 | [`RFile`](41-class-guides-001-025.md#src-12-class-guides-011-rfile-md) | `f32file.h` | `efsrv.lib` | `path-specific` | verified-common |
| 12 | [`CFileMan`](41-class-guides-001-025.md#src-12-class-guides-012-cfileman-md) | `f32file.h` | `efsrv.lib` | `path-specific` | verified-common |
| 13 | [`TParse`](41-class-guides-001-025.md#src-12-class-guides-013-tparse-md) | `f32file.h` | `efsrv.lib` | `None` | verified-common |
| 14 | [`RReadStream`](41-class-guides-001-025.md#src-12-class-guides-014-rreadstream-md) | `s32strm.h` | `estor.lib` | `source-specific` | verified-common |
| 15 | [`RWriteStream`](41-class-guides-001-025.md#src-12-class-guides-015-rwritestream-md) | `s32strm.h` | `estor.lib` | `sink-specific` | verified-common |
| 16 | [`RFileReadStream`](41-class-guides-001-025.md#src-12-class-guides-016-rfilereadstream-md) | `s32file.h` | `estor.lib, efsrv.lib` | `path-specific` | verified-common |
| 17 | [`RFileWriteStream`](41-class-guides-001-025.md#src-12-class-guides-017-rfilewritestream-md) | `s32file.h` | `estor.lib, efsrv.lib` | `path-specific` | verified-common |
| 18 | [`RDbNamedDatabase`](41-class-guides-001-025.md#src-12-class-guides-018-rdbnameddatabase-md) | `d32dbms.h` | `edbms.lib` | `path-specific` | verified-common |
| 19 | [`RDbView`](41-class-guides-001-025.md#src-12-class-guides-019-rdbview-md) | `d32dbms.h` | `edbms.lib` | `database-specific` | verified-common |
| 20 | [`CRepository`](41-class-guides-001-025.md#src-12-class-guides-020-crepository-md) | `centralrepository.h` | `centralrepository.lib` | `key-policy-specific` | verified-common |
| 21 | [`CCoeControl`](41-class-guides-001-025.md#src-12-class-guides-021-ccoecontrol-md) | `coecntrl.h` | `cone.lib` | `None` | verified-common |
| 22 | [`CAknApplication`](41-class-guides-001-025.md#src-12-class-guides-022-caknapplication-md) | `aknapp.h` | `avkon.lib, apparc.lib` | `None` | verified-quickref |
| 23 | [`CAknDocument`](41-class-guides-001-025.md#src-12-class-guides-023-cakndocument-md) | `akndoc.h` | `avkon.lib` | `None` | verified-quickref |
| 24 | [`CAknAppUi`](41-class-guides-001-025.md#src-12-class-guides-024-caknappui-md) | `aknappui.h` | `avkon.lib, eikcore.lib` | `None` | verified-quickref |
| 25 | [`CAknView`](41-class-guides-001-025.md#src-12-class-guides-025-caknview-md) | `aknview.h` | `avkon.lib` | `None` | verified-quickref |
| 26 | [`CAknViewAppUi`](42-class-guides-026-050.md#src-12-class-guides-026-caknviewappui-md) | `aknviewappui.h` | `avkon.lib` | `None` | verified-quickref |
| 27 | [`CEikMenuPane`](42-class-guides-026-050.md#src-12-class-guides-027-ceikmenupane-md) | `eikmenup.h` | `local-sdk-check` | `None` | verified-header |
| 28 | [`CAknMessageQueryDialog`](42-class-guides-026-050.md#src-12-class-guides-028-caknmessagequerydialog-md) | `aknmessagequerydialog.h` | `avkon.lib` | `None` | verified-quickref |
| 29 | [`CAknQueryDialog`](42-class-guides-026-050.md#src-12-class-guides-029-caknquerydialog-md) | `aknquerydialog.h` | `avkon.lib` | `None` | verified-quickref |
| 30 | [`CAknSingleStyleListBox`](42-class-guides-026-050.md#src-12-class-guides-030-caknsinglestylelistbox-md) | `aknlists.h` | `avkon.lib` | `None` | verified-quickref |
| 31 | [`CAknSettingItemList`](42-class-guides-026-050.md#src-12-class-guides-031-caknsettingitemlist-md) | `aknsettingitemlist.h` | `avkon.lib` | `None` | verified-quickref |
| 32 | [`RWsSession`](42-class-guides-026-050.md#src-12-class-guides-032-rwssession-md) | `w32std.h` | `ws32.lib` | `None` | verified-common |
| 33 | [`RWindowGroup`](42-class-guides-026-050.md#src-12-class-guides-033-rwindowgroup-md) | `w32std.h` | `ws32.lib` | `None` | verified-common |
| 34 | [`CFbsBitmap`](42-class-guides-026-050.md#src-12-class-guides-034-cfbsbitmap-md) | `fbs.h` | `fbscli.lib` | `None` | verified-common |
| 35 | [`AknIconUtils`](42-class-guides-026-050.md#src-12-class-guides-035-akniconutils-md) | `akniconutils.h` | `avkon.lib` | `None` | verified-quickref |
| 36 | [`RSocketServ`](42-class-guides-026-050.md#src-12-class-guides-036-rsocketserv-md) | `es_sock.h` | `esock.lib` | `NetworkServices for network use` | verified-common |
| 37 | [`RSocket`](42-class-guides-026-050.md#src-12-class-guides-037-rsocket-md) | `es_sock.h` | `esock.lib` | `NetworkServices` | verified-common |
| 38 | [`RConnection`](42-class-guides-026-050.md#src-12-class-guides-038-rconnection-md) | `es_sock.h` | `esock.lib` | `NetworkServices` | verified-common |
| 39 | [`RHostResolver`](42-class-guides-026-050.md#src-12-class-guides-039-rhostresolver-md) | `in_sock.h` | `esock.lib, insock.lib` | `NetworkServices` | verified-common |
| 40 | [`TInetAddr`](42-class-guides-026-050.md#src-12-class-guides-040-tinetaddr-md) | `in_sock.h` | `insock.lib` | `None` | verified-common |
| 41 | [`RHTTPSession`](42-class-guides-026-050.md#src-12-class-guides-041-rhttpsession-md) | `http.h` | `http.lib` | `NetworkServices` | verified-common |
| 42 | [`RHTTPTransaction`](42-class-guides-026-050.md#src-12-class-guides-042-rhttptransaction-md) | `http.h` | `http.lib` | `NetworkServices` | verified-common |
| 43 | [`MHTTPTransactionCallback`](42-class-guides-026-050.md#src-12-class-guides-043-mhttptransactioncallback-md) | `http.h` | `http.lib` | `NetworkServices` | verified-common |
| 44 | [`CUri8`](42-class-guides-026-050.md#src-12-class-guides-044-curi8-md) | `uri8.h` | `inetprotutil.lib` | `None` | verified-common |
| 45 | [`CMdaAudioPlayerUtility`](42-class-guides-026-050.md#src-12-class-guides-045-cmdaaudioplayerutility-md) | `MdaAudioSamplePlayer.h` | `mediaclientaudio.lib` | `UserEnvironment/device-policy` | verified-common |
| 46 | [`CMdaAudioRecorderUtility`](42-class-guides-026-050.md#src-12-class-guides-046-cmdaaudiorecorderutility-md) | `MdaAudioSampleEditor.h` | `mediaclientaudio.lib` | `UserEnvironment/device-policy` | verified-common |
| 47 | [`CVideoPlayerUtility`](42-class-guides-026-050.md#src-12-class-guides-047-cvideoplayerutility-md) | `videoplayer.h` | `mediaclientvideo.lib` | `UserEnvironment/device-policy` | verified-common |
| 48 | [`CCamera`](42-class-guides-026-050.md#src-12-class-guides-048-ccamera-md) | `ecam.h` | `ecam.lib` | `UserEnvironment/device-policy` | verified-common |
| 49 | [`CImageDecoder`](42-class-guides-026-050.md#src-12-class-guides-049-cimagedecoder-md) | `imageconversion.h` | `imageconversion.lib` | `path-specific` | verified-common |
| 50 | [`CImageEncoder`](42-class-guides-026-050.md#src-12-class-guides-050-cimageencoder-md) | `imageconversion.h` | `imageconversion.lib` | `path-specific` | verified-common |
| 51 | [`CMsvSession`](43-class-guides-051-075.md#src-12-class-guides-051-cmsvsession-md) | `msvapi.h` | `msgs.lib` | `ReadUserData, WriteUserData for message data` | verified-common |
| 52 | [`CMsvEntry`](43-class-guides-051-075.md#src-12-class-guides-052-cmsventry-md) | `msvapi.h` | `msgs.lib` | `ReadUserData, WriteUserData` | verified-common |
| 53 | [`CContactDatabase`](43-class-guides-051-075.md#src-12-class-guides-053-ccontactdatabase-md) | `cntdb.h` | `cntmodel.lib` | `ReadUserData, WriteUserData` | verified-common |
| 54 | [`CContactItem`](43-class-guides-051-075.md#src-12-class-guides-054-ccontactitem-md) | `cntitem.h` | `cntmodel.lib` | `ReadUserData, WriteUserData` | verified-common |
| 55 | [`CCalSession`](43-class-guides-051-075.md#src-12-class-guides-055-ccalsession-md) | `calsession.h` | `calinterimapi.lib` | `ReadUserData, WriteUserData` | verified-common |
| 56 | [`CCalEntry`](43-class-guides-051-075.md#src-12-class-guides-056-ccalentry-md) | `calentry.h` | `calinterimapi.lib` | `ReadUserData, WriteUserData` | verified-common |
| 57 | [`CTelephony`](43-class-guides-051-075.md#src-12-class-guides-057-ctelephony-md) | `etel3rdparty.h` | `etel3rdparty.lib` | `operation-specific phone capabilities` | verified-common |
| 58 | [`RPositionServer`](43-class-guides-051-075.md#src-12-class-guides-058-rpositionserver-md) | `lbs.h` | `lbs.lib` | `Location` | verified-quickref |
| 59 | [`RPositioner`](43-class-guides-051-075.md#src-12-class-guides-059-rpositioner-md) | `lbs.h` | `lbs.lib` | `Location` | verified-quickref |
| 60 | [`CPosLandmarkDatabase`](43-class-guides-051-075.md#src-12-class-guides-060-cposlandmarkdatabase-md) | `epos_cposlandmarkdatabase.h` | `lbslandmarks.lib` | `Location / data-policy` | verified-quickref |
| 61 | [`REComSession`](43-class-guides-051-075.md#src-12-class-guides-061-recomsession-md) | `ecom/ecom.h` | `ecom.lib` | `None` | verified-common |
| 62 | [`FeatureManager`](43-class-guides-051-075.md#src-12-class-guides-062-featuremanager-md) | `featmgr.h` | `featmgr.lib` | `None` | verified-common |
| 63 | [`CDocumentHandler`](43-class-guides-051-075.md#src-12-class-guides-063-cdocumenthandler-md) | `documenthandler.h` | `commonui.lib` | `operation-specific` | verified-quickref |
| 64 | [`CHWRMLight`](43-class-guides-051-075.md#src-12-class-guides-064-chwrmlight-md) | `hwrmlight.h` | `hwrmlightclient.lib` | `device-policy` | verified-quickref |
| 65 | [`CHWRMVibra`](43-class-guides-051-075.md#src-12-class-guides-065-chwrmvibra-md) | `hwrmvibra.h` | `hwrmvibraclient.lib` | `device-policy` | verified-quickref |
| 66 | [`RConnectionMonitor`](43-class-guides-051-075.md#src-12-class-guides-066-rconnectionmonitor-md) | `rconnmon.h` | `connmon.lib` | `NetworkServices` | verified-quickref |
| 67 | [`CBluetoothSynchronousLink`](43-class-guides-051-075.md#src-12-class-guides-067-cbluetoothsynchronouslink-md) | `bt_sock.h` | `bluetooth.lib` | `LocalServices` | verified-class-page |
| 68 | [`RBTPhysicalLinkAdapter`](43-class-guides-051-075.md#src-12-class-guides-068-rbtphysicallinkadapter-md) | `bt_sock.h` | `bluetooth.lib` | `LocalServices` | verified-common |
| 69 | [`CSendUi`](43-class-guides-051-075.md#src-12-class-guides-069-csendui-md) | `sendui.h` | `sendui.lib` | `ReadUserData depending attachments` | verified-quickref |
| 70 | [`CMessageData`](43-class-guides-051-075.md#src-12-class-guides-070-cmessagedata-md) | `cmessagedata.h` | `sendui.lib` | `ReadUserData depending attachments` | verified-quickref |
| 71 | [`PathInfo`](43-class-guides-051-075.md#src-12-class-guides-071-pathinfo-md) | `pathinfo.h` | `platformenv.lib` | `None` | verified-quickref |
| 72 | [`SysUtil`](43-class-guides-051-075.md#src-12-class-guides-072-sysutil-md) | `sysutil.h` | `sysutil.lib` | `None` | verified-quickref |
| 73 | [`CTextResolver`](43-class-guides-051-075.md#src-12-class-guides-073-ctextresolver-md) | `textresolver.h` | `commonengine.lib` | `None` | verified-quickref |
| 74 | [`StringLoader`](43-class-guides-051-075.md#src-12-class-guides-074-stringloader-md) | `stringloader.h` | `commonengine.lib` | `None` | verified-quickref |
| 75 | [`CVersionInfo`](43-class-guides-051-075.md#src-12-class-guides-075-cversioninfo-md) | `versioninfo.h` | `platformenv.lib` | `None` | verified-quickref |
