# Platform security, capabilities, SIS/SISX, signing, and deployment

> This file consolidates canonical handbook topics for progressive loading. Each section records its original corpus path and retains external source links.

---

<a id="src-08-security-packaging-capabilities-matrix-md"></a>
## Capability Matrix

- Original corpus path: `08_security_packaging/capabilities_matrix.md`
- Consolidated reference: `09-security-packaging-deployment.md#src-08-security-packaging-capabilities-matrix-md`

### Capability Matrix

#### Practical matrix

| Capability | Typical purpose | Notes |
|---|---|---|
| `NetworkServices` | normal network client/server use | sockets, HTTP, send operations |
| `LocalServices` | local connectivity | Bluetooth/infrared/local links |
| `ReadUserData` | read contacts, calendar, messages | personal data |
| `WriteUserData` | modify contacts, calendar, messages | personal data changes |
| `Location` | position data | privacy-sensitive |
| `UserEnvironment` | microphone/camera or nearby environment APIs | verify per method |
| `ReadDeviceData` | protected device/system information | stronger signing requirements |
| `WriteDeviceData` | protected system settings/data | stronger signing requirements |
| `NetworkControl` | privileged network control | not needed for ordinary sockets |
| `PowerMgmt` | power/process control | privileged |
| `ProtServ` | protected server naming/operations | privileged |
| `SwEvent` | synthetic/system events | restricted |
| `AllFiles` | broad filesystem access | highly restricted |
| `TCB` | trusted computing base | most restricted |

#### Self-sign expectation

Historically, only a subset of user capabilities can be self-signed. Exact signing services and certificate policies have changed or disappeared over time; for retro development, developer certificates or modified test devices are common.

#### Agent behavior

Never suggest `CAPABILITY ALL -TCB` as a casual fix. Determine the method-level need and explain signing implications.

---

<a id="src-08-security-packaging-device-deployment-md"></a>
## Device Deployment and Runtime Dependencies

- Original corpus path: `08_security_packaging/device_deployment.md`
- Consolidated reference: `09-security-packaging-deployment.md#src-08-security-packaging-device-deployment-md`

### Device Deployment and Runtime Dependencies

#### Dependency classes

A SIS may depend on:

- Base S60 platform version.
- Open C/C++ runtime.
- PIPS/STLport components.
- API plug-in runtime DLLs.
- Device-specific libraries.
- Codec/controller availability.

#### SDK presence is not device presence

An SDK plug-in can provide headers and import libraries without guaranteeing every target firmware ships the corresponding DLL. Confirm the device ROM or package the redistributable runtime when licensing/docs permit.

#### Installation order

Install runtimes/dependencies before the application, or create an embedded package according to SIS packaging rules.

#### Binary diagnosis

A launch that immediately fails may be a missing DLL/import problem. Use device logs/debugger and dependency tools available in the SDK ecosystem.

#### Versioning

Increment package version for upgrades. Changing UID creates a separate application and private data area.

---

<a id="src-08-security-packaging-platform-security-md"></a>
## Platform Security Model

- Original corpus path: `08_security_packaging/platform_security.md`
- Consolidated reference: `09-security-packaging-deployment.md#src-08-security-packaging-platform-security-md`

### Platform Security Model

#### Core concepts

- **Capabilities** grant access to protected operations.
- **Secure ID (SID)** identifies a process/application.
- **Vendor ID (VID)** identifies a vendor under signing schemes.
- **Data caging** protects private directories and system areas.

#### Principle of least privilege

Declare only capabilities required by code. Extra capabilities can make signing harder and do not improve functionality.

#### Capability failure

Symptoms include:

- Installation rejected.
- API returns permission denied.
- Process panics when accessing protected service.
- Works in emulator but fails on phone.

#### Private data

An app can access its own `\private\<SID>\` area. Access to another SID's private directory is restricted.

#### Signing

The certificate must grant all capabilities declared by the executable. Self-signing covers only a limited set of user-grantable capabilities.

#### API policy

A class itself does not have one universal capability. Individual methods can have different policies. Always read the method-level requirement.

---

<a id="src-08-security-packaging-sis-pkg-signing-md"></a>
## SIS/SISX Packaging and Signing

- Original corpus path: `08_security_packaging/sis_pkg_signing.md`
- Consolidated reference: `09-security-packaging-deployment.md#src-08-security-packaging-sis-pkg-signing-md`

### SIS/SISX Packaging and Signing

#### Build first

```bat
abld build gcce urel
```

#### Package file

```text
#{"MyApp"},(0xE0001234),1,0,0

%{"Duy Pham"}
:"Duy Pham"

[0x101F7961], 0, 0, 0, {"S60ProductID"}

"$(EPOCROOT)Epoc32\release\gcce\urel\MyApp.exe" - "!:\sys\bin\MyApp.exe"
"$(EPOCROOT)Epoc32\data\z\resource\apps\MyApp.rsc" - "!:\resource\apps\MyApp.rsc"
"$(EPOCROOT)Epoc32\data\z\private\10003a3f\import\apps\MyApp_reg.rsc" - "!:\private\10003a3f\import\apps\MyApp_reg.rsc"
```

Use actual generated paths from your SDK. Some SDK versions stage registration resources under slightly different subpaths.

#### Create SIS

```bat
makesis MyApp.pkg MyApp_unsigned.sis
```

#### Sign

```bat
signsis -s MyApp_unsigned.sis MyApp.sisx cert.cer key.key
```

With key password:

```bat
signsis -s MyApp_unsigned.sis MyApp.sisx cert.cer key.key PASSWORD
```

#### Installation failures

Check:

- Certificate validity and device date.
- UID conflict.
- Declared capabilities versus certificate grants.
- Platform dependency line.
- Missing file in `.pkg`.
- Existing newer version/downgrade policy.
- Binary built for the wrong target.

---

<a id="src-08-security-packaging-uid-secureid-vendorid-md"></a>
## UID, Secure ID, and Vendor ID

- Original corpus path: `08_security_packaging/uid_secureid_vendorid.md`
- Consolidated reference: `09-security-packaging-deployment.md#src-08-security-packaging-uid-secureid-vendorid-md`

### UID, Secure ID, and Vendor ID

#### Application identity

A normal application executable commonly uses:

```text
UID       0x100039CE 0xE0001234
SECUREID  0xE0001234
VENDORID  0
```

- UID2 `0x100039CE` identifies the application executable type.
- UID3 uniquely identifies the application.
- `SECUREID` usually matches UID3.
- `VENDORID 0` is common for unsigned/self-signed development.

#### Consistency locations

The same application UID must match across:

- `.mmp` UID3 and `SECUREID`.
- Application class `AppDllUid()`.
- Registration resource `UID3`.
- `.pkg` package UID.
- Private directory expectations.

#### Test UID ranges

Use a valid development/test UID range appropriate to your workflow. Do not copy another application's UID; it causes installation conflicts and private-data identity collisions.

#### Changing UID

Clean generated resources and rebuild everything after a UID change. Stale registration resources can make the launcher show old/missing entries.
