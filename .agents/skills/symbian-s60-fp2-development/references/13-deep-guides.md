# Deep implementation guides

> This file consolidates canonical handbook topics for progressive loading. Each section records its original corpus path and retains external source links.

---

<a id="src-14-deep-guides-active-object-state-machine-md"></a>
## ACTIVE OBJECT STATE MACHINE

- Original corpus path: `14_DEEP_GUIDES/ACTIVE_OBJECT_STATE_MACHINE.md`
- Consolidated reference: `13-deep-guides.md#src-14-deep-guides-active-object-state-machine-md`

### Active Object state machine

An Active Object is a single-outstanding-request state machine.

```text
Idle -> Issue request -> SetActive -> RunL
   \-> Cancel -> DoCancel -> Idle
```

#### Rules

- Set `iStatus` only through the asynchronous provider.
- Call `SetActive()` exactly once per issued request.
- Never leave an object active during destruction.
- `DoCancel()` must call the provider-specific cancel method.
- `RunL()` may leave; use `RunError()` when recovery is needed.
- Keep request descriptors and buffers alive until completion.

#### Multi-step operations

Represent phases explicitly:

```cpp
enum TState { EIdle, EResolving, EConnecting, EWriting, EReading };
```

Do not infer the phase only from random member values.

---

<a id="src-14-deep-guides-avkon-app-lifecycle-md"></a>
## AVKON APP LIFECYCLE

- Original corpus path: `14_DEEP_GUIDES/AVKON_APP_LIFECYCLE.md`
- Consolidated reference: `13-deep-guides.md#src-14-deep-guides-avkon-app-lifecycle-md`

### Avkon application lifecycle

```text
E32Main
  -> CAknApplication
  -> CAknDocument
  -> CAknAppUi / CAknViewAppUi
  -> CCoeControl or CAknView containers
```

#### Minimal responsibilities

- `CAknApplication::AppDllUid()`
- `CAknApplication::CreateDocumentL()`
- `CAknDocument::CreateAppUiL()`
- `CAknAppUi::ConstructL()`
- `CAknAppUi::HandleCommandL()`

#### Control stack

A control receiving keys normally belongs on the AppUi control stack.

```cpp
iView = CMyView::NewL(ClientRect());
AddToStackL(iView);

// destructor
RemoveFromStack(iView);
delete iView;
```

#### Exit

Handle both the softkey and framework exit command. Do not call `delete this`.

---

<a id="src-14-deep-guides-descriptors-complete-practice-md"></a>
## DESCRIPTORS COMPLETE PRACTICE

- Original corpus path: `14_DEEP_GUIDES/DESCRIPTORS_COMPLETE_PRACTICE.md`
- Consolidated reference: `13-deep-guides.md#src-14-deep-guides-descriptors-complete-practice-md`

### Descriptor practice for FP2 code

#### Families

- immutable interfaces: `TDesC`, `TDesC8`
- writable interfaces: `TDes`, `TDes8`
- stack buffers: `TBuf<N>`, `TBuf8<N>`
- heap buffers: `HBufC`, `HBufC8`
- pointers/views: `TPtr`, `TPtrC`, `TPtr8`, `TPtrC8`
- literals: `_L`, `_L8`, `_LIT`, `_LIT8`

#### Rules

- `Length()` is characters for 16-bit descriptors and bytes for 8-bit descriptors.
- `MaxLength()` limits writes; overflow panics.
- `Copy()` converts 8/16-bit values mechanically, not through UTF-8 decoding.
- Use conversion APIs for UTF-8.
- `HBufC::Des()` returns a writable `TPtr`.
- Do not retain a `TPtr` after the underlying `HBufC` is reallocated/destroyed.

#### Safe formatting

```cpp
TBuf<128> text;
text.Format(_L("Error %d"), err);
```

Check worst-case length before formatting untrusted data.

---

<a id="src-14-deep-guides-http-framework-fp2-md"></a>
## HTTP FRAMEWORK FP2

- Original corpus path: `14_DEEP_GUIDES/HTTP_FRAMEWORK_FP2.md`
- Consolidated reference: `13-deep-guides.md#src-14-deep-guides-http-framework-fp2-md`

### HTTP Framework on S60 3rd FP2

#### Core types

- `RHTTPSession`
- `RHTTPTransaction`
- `RHTTPHeaders`
- `RHTTPResponse`
- `MHTTPTransactionCallback`
- `MHTTPDataSupplier`

#### Typical flow

```cpp
void CHttpClient::ConstructL()
{
    iSession.OpenL();
}

void CHttpClient::GetL(const TDesC8& aUrl)
{
    TUriParser8 parser;
    User::LeaveIfError(parser.Parse(aUrl));

    RStringF get = iSession.StringPool().StringF(
        HTTP::EGET, RHTTPSession::GetTable());

    iTransaction = iSession.OpenTransactionL(
        parser, *this, get);

    iTransaction.SubmitL();
}
```

In `MHFRunL()`:

- inspect `THTTPEvent::EGotResponseHeaders`;
- on `EGotResponseBodyData`, obtain the body supplier;
- call `GetNextDataPart()`;
- copy/process the data before calling `ReleaseData()`;
- handle `EResponseComplete` and `ESucceeded`;
- close the transaction deterministically.

#### HTTPS

The HTTP framework accepts an `https` URI and uses the secure socket path through its
filters/transport. TLS versions and cipher support are constrained by the device stack.

#### Connection selection

For explicit IAP control, start an `RConnection` and associate HTTP connection/session
properties using APIs present in the local FP2 SDK. Do not paste S60 5th examples blindly.

#### Build

```text
LIBRARY http.lib
LIBRARY esock.lib
LIBRARY insock.lib
CAPABILITY NetworkServices
```

Exact auxiliary libraries depend on URI/filter usage.

---

<a id="src-14-deep-guides-sis-signing-and-capabilities-md"></a>
## SIS SIGNING AND CAPABILITIES

- Original corpus path: `14_DEEP_GUIDES/SIS_SIGNING_AND_CAPABILITIES.md`
- Consolidated reference: `13-deep-guides.md#src-14-deep-guides-sis-signing-and-capabilities-md`

### SIS, SISX, signing and capabilities

#### Build chain

```text
GCCE UREL -> executable/resources -> .pkg -> makesis -> unsigned .sis
          -> signsis -> signed .sisx
```

#### Package declaration

```text
#{"MyApp"},(0xE0001234),1,0,0
%{"Vendor"}
:"Vendor"
[0x101F7961], 0, 0, 0, {"S60ProductID"}
```

#### Security matching

- `UID3` and `SECUREID` normally match for an application.
- The package UID must match the application UID.
- Installed paths under `\sys\bin` are protected.
- Capabilities in the executable must be covered by the signing certificate.
- A self-signed package cannot satisfy every capability.

Always use an owned development UID range appropriate to the distribution/signing plan.

---

<a id="src-14-deep-guides-sockets-dns-tcp-udp-md"></a>
## SOCKETS DNS TCP UDP

- Original corpus path: `14_DEEP_GUIDES/SOCKETS_DNS_TCP_UDP.md`
- Consolidated reference: `13-deep-guides.md#src-14-deep-guides-sockets-dns-tcp-udp-md`

### Sockets, DNS, TCP and UDP

#### Ownership order

```text
RSocketServ -> optional RConnection -> RHostResolver / RSocket
```

Close in reverse order.

#### DNS

```cpp
User::LeaveIfError(iSocketServ.Connect());
User::LeaveIfError(iResolver.Open(
    iSocketServ, KAfInet, KProtocolInetUdp));

iResolver.GetByName(iHost, iNameEntry, iStatus);
SetActive();
```

#### TCP

1. `RSocket::Open(... KAfInet, KSockStream, KProtocolInetTcp)`.
2. Resolve host.
3. `Connect()` asynchronously.
4. `Write()`/`RecvOneOrMore()` asynchronously.
5. Cancel outstanding request before close.

#### UDP

Open with `KSockDatagram` and `KProtocolInetUdp`, then use `SendTo()`/`RecvFrom()`.

#### Common bugs

- descriptor used by async write goes out of scope;
- issuing a second request while the active object is active;
- forgetting `SetActive()`;
- closing `RSocketServ` before sockets;
- assuming Wi-Fi is available because emulator networking works.
