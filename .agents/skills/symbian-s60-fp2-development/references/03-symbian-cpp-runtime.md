# Symbian C++ language and runtime patterns

> This file consolidates canonical handbook topics for progressive loading. Each section records its original corpus path and retains external source links.

---

<a id="src-02-language-runtime-active-objects-md"></a>
## Active Objects and Asynchronous Requests

- Original corpus path: `02_language_runtime/active_objects.md`
- Consolidated reference: `03-symbian-cpp-runtime.md#src-02-language-runtime-active-objects-md`

### Active Objects and Asynchronous Requests

#### Core idea

An Active Object wraps one outstanding asynchronous request. The Active Scheduler dispatches completion callbacks cooperatively within a thread.

#### Skeleton

```cpp
class CTimerAo : public CActive
{
public:
    static CTimerAo* NewL();
    ~CTimerAo();
    void Start(TTimeIntervalMicroSeconds32 aDelay);
private:
    CTimerAo();
    void ConstructL();
    void RunL();
    void DoCancel();
    TInt RunError(TInt aError);
private:
    RTimer iTimer;
};

CTimerAo::CTimerAo()
: CActive(CActive::EPriorityStandard)
{
    CActiveScheduler::Add(this);
}

void CTimerAo::ConstructL()
{
    User::LeaveIfError(iTimer.CreateLocal());
}

void CTimerAo::Start(TTimeIntervalMicroSeconds32 aDelay)
{
    Cancel();
    iTimer.After(iStatus, aDelay);
    SetActive();
}

void CTimerAo::RunL()
{
    User::LeaveIfError(iStatus.Int());
    // completion work
}

void CTimerAo::DoCancel()
{
    iTimer.Cancel();
}
```

#### Rules

- Set `iStatus` through the asynchronous provider, then call `SetActive()`.
- One Active Object can own only one outstanding request at a time unless it explicitly coordinates multiple statuses through other mechanisms.
- `DoCancel()` must call the provider's matching cancellation method.
- Destructor should call `Cancel()` before closing handles.
- Keep `RunL()` short; schedule more work instead of blocking the UI.
- `RunError()` can convert a leave from `RunL()` into controlled behavior.

#### Self-completion

An Active Object can queue itself:

```cpp
TRequestStatus* status = &iStatus;
User::RequestComplete(status, KErrNone);
SetActive();
```

Set the active state in the correct order for the chosen pattern and avoid double completion.

#### Periodic alternatives

- `CPeriodic`: callback-based repeating timer.
- `CTimer`: Active Object base for timer use.
- `RTimer`: raw asynchronous timer handle.

Use `CPeriodic::Start` with the exact interval types expected by the FP2 headers; later overloads may differ.

---

<a id="src-02-language-runtime-client-server-ipc-md"></a>
## Client-Server IPC

- Original corpus path: `02_language_runtime/client_server_ipc.md`
- Consolidated reference: `03-symbian-cpp-runtime.md#src-02-language-runtime-client-server-ipc-md`

### Client-Server IPC

#### Architecture

Many Symbian services use a server process/thread plus client-side `RSessionBase` handles. Custom services can use the same model.

#### Main classes

- `CServer2`: server-side request dispatcher.
- `CSession2`: per-client session.
- `RSessionBase`: client handle.
- `RMessage2`: server-side request and descriptors.

#### Client pattern

```cpp
class RMySession : public RSessionBase
{
public:
    TInt Connect();
    TInt Add(TInt a, TInt b, TInt& result);
};
```

`Connect()` usually calls `CreateSession()` with a server name and version.

#### Server pattern

- Start server.
- Install Active Scheduler.
- Signal rendezvous.
- Create sessions in `CServer2::NewSessionL()`.
- Dispatch function IDs in `CSession2::ServiceL()`.
- Complete every `RMessage2` exactly once unless deliberately retained for async completion.

#### Descriptor IPC

Use `RMessage2::ReadL()` and `WriteL()` with validated slot indexes and lengths. Never trust client-provided length or enum values.

#### Security

Use server policy objects and capability checks. The server's process boundary does not automatically validate every operation.

#### Async IPC

A client passes `TRequestStatus`; the server retains the message and completes later. Implement cancellation function IDs and handle client death.

---

<a id="src-02-language-runtime-collections-arrays-md"></a>
## Arrays and Collections

- Original corpus path: `02_language_runtime/collections_arrays.md`
- Consolidated reference: `03-symbian-cpp-runtime.md#src-02-language-runtime-collections-arrays-md`

### Arrays and Collections

#### Common containers

| Type | Contents | Cleanup |
|---|---|---|
| `RArray<T>` | value objects | `Close()` |
| `RPointerArray<T>` | pointers | `Close()` only or `ResetAndDestroy()` if owning |
| `CArrayFixFlat<T>` | fixed-size element array | delete object |
| `CArrayPtrFlat<T>` | pointer array | ownership depends on use |

#### `RArray`

```cpp
RArray<TInt> values;
CleanupClosePushL(values);
User::LeaveIfError(values.Append(10));
User::LeaveIfError(values.Append(20));
CleanupStack::PopAndDestroy(&values);
```

#### Owning pointer array

```cpp
RPointerArray<CItem> items;
CleanupResetAndDestroyPushL(items);
items.AppendL(CItem::NewL());
CleanupStack::PopAndDestroy(&items);
```

If `Append()` fails, ownership remains with the caller. Prefer a sequence that protects the new object:

```cpp
CItem* item = CItem::NewLC();
items.AppendL(item);
CleanupStack::Pop(item); // array owns it now
```

#### Sorting and finding

Use `TLinearOrder<T>` and `TIdentityRelation<T>` where supported. Verify comparator signatures in FP2 headers.

#### Descriptor arrays

`CDesCArrayFlat` and related classes are useful for UI list models. Their ownership and formatting conventions differ from `RPointerArray<HBufC>`; choose based on the consumer API.

---

<a id="src-02-language-runtime-descriptors-md"></a>
## Descriptors: Text and Binary Buffers

- Original corpus path: `02_language_runtime/descriptors.md`
- Consolidated reference: `03-symbian-cpp-runtime.md#src-02-language-runtime-descriptors-md`

### Descriptors: Text and Binary Buffers

#### Why descriptors exist

Descriptors carry a pointer, length, and maximum-length contract. They avoid NUL-terminated ambiguity and support bounds checking.

#### Main families

| Type | Storage | Mutable | Typical use |
|---|---|---|---|
| `TBuf<N>` | inline/stack | yes | short fixed-capacity text |
| `TBuf8<N>` | inline/stack | yes | bytes/UTF-8/protocol data |
| `HBufC` | heap | via `Des()` | variable text |
| `HBufC8` | heap | via `Des()` | variable binary data |
| `TPtr` / `TPtr8` | non-owning view | yes | writable view |
| `TPtrC` / `TPtrC8` | non-owning view | no | read-only slice |
| `TDes` / `TDes8` | abstract writable | yes | function parameter |
| `TDesC` / `TDesC8` | abstract read-only | no | function parameter |
| `RBuf` / `RBuf8` | resizable handle buffer | yes | dynamic buffer; call `Close()` |

#### Literals

```cpp
_LIT(KName, "MIDPlay");
_LIT8(KHeader, "Content-Type");
```

Use `_L()` only for immediate expressions; use `_LIT` for reusable constants.

#### Copy and append

```cpp
TBuf<64> text;
text.Copy(_L("Hello"));
text.Append(_L(" Symbian"));
text.AppendNum(93);
```

Every append must fit `MaxLength()` or a descriptor panic can occur.

#### Heap descriptor

```cpp
HBufC* value = HBufC::NewLC(128);
TPtr ptr(value->Des());
ptr.Copy(_L("owned heap text"));
CleanupStack::PopAndDestroy(value);
```

#### Allocate exact copy

```cpp
HBufC* copy = source.AllocL();
CleanupStack::PushL(copy);
```

#### 16-bit and 8-bit conversion

Do not blindly `Copy()` arbitrary Unicode into an 8-bit descriptor. For protocol text, use an explicit character converter when encoding matters. Simple ASCII-only HTTP tokens are safe to copy carefully; user text is not.

#### Parsing

Useful classes and methods:

- `TLex`, `TLex8` for lexical parsing.
- `Locate`, `Find`, `Mid`, `Left`, `Right` for slices.
- `Compare`, `CompareF` for comparison.
- `Format`, `AppendFormat` for formatting; validate buffer capacity.

#### Function signature design

```cpp
void SetNameL(const TDesC& aName);   // read-only input
void ReadData(TDes8& aTarget);       // caller-owned writable output
const TDesC& Name() const;           // borrowed reference
HBufC* NameAllocL() const;           // ownership transferred to caller
```

---

<a id="src-02-language-runtime-ecom-plugins-md"></a>
## ECom Plug-in Framework

- Original corpus path: `02_language_runtime/ecom_plugins.md`
- Consolidated reference: `03-symbian-cpp-runtime.md#src-02-language-runtime-ecom-plugins-md`

### ECom Plug-in Framework

#### Purpose

ECom is Symbian's plug-in discovery and instantiation framework. A client depends on an interface, while implementations are registered through resource metadata and loaded by implementation UID.

#### Main concepts

- Interface class exported to clients.
- Implementation class in a DLL.
- Interface UID and implementation UID.
- ECom registration resource.
- `REComSession` for creation and destruction notification.

Typical include/library:

```cpp
#include <ecom/ecom.h>
#include <ecom/implementationinformation.h>
LIBRARY ecom.lib
```

#### Creation pattern

An interface `NewL()` commonly calls `REComSession::CreateImplementationL()` and stores an instance key at a known offset. The interface destructor notifies ECom using `DestroyedImplementation()`.

#### Registration

Implementation proxy tables export constructors from the plug-in DLL. Resource files declare interface/implementation UIDs and optional opaque data.

#### Lifecycle risks

- Forgetting `DestroyedImplementation()`.
- Mismatched UIDs between resource, proxy table, and client.
- Calling `REComSession::FinalClose()` while live implementations remain.
- Stale ECom cache after installing/changing plug-ins; reboot or cache refresh may be required.

#### Agent rule

Copy the skeleton from an FP2 ECom example. The offset/key pattern is subtle and a near-correct implementation can compile yet leak or fail at runtime.

---

<a id="src-02-language-runtime-leaves-traps-cleanup-md"></a>
## Leaves, TRAP, and Cleanup Stack

- Original corpus path: `02_language_runtime/leaves_traps_cleanup.md`
- Consolidated reference: `03-symbian-cpp-runtime.md#src-02-language-runtime-leaves-traps-cleanup-md`

### Leaves, TRAP, and Cleanup Stack

#### Leave model

A leave unwinds to the nearest `TRAP` boundary without normal C++ stack unwinding guarantees for all object types used by classic Symbian code. The Cleanup Stack protects resources that would otherwise leak.

#### Leave sources

```cpp
User::Leave(KErrArgument);
User::LeaveIfError(iFs.Connect());
HBufC* p = HBufC::NewL(128);
```

#### TRAP

```cpp
TRAPD(err, DoWorkL());
if (err != KErrNone)
{
    HandleError(err);
}
```

`TRAP_IGNORE` discards errors; use it only when failure is genuinely irrelevant.

#### Heap object cleanup

```cpp
CWidget* widget = CWidget::NewLC();
widget->LoadL();
CleanupStack::PopAndDestroy(widget);
```

#### `R` handle cleanup

```cpp
RFs fs;
User::LeaveIfError(fs.Connect());
CleanupClosePushL(fs);

RFile file;
User::LeaveIfError(file.Open(fs, path, EFileRead));
CleanupClosePushL(file);

CleanupStack::PopAndDestroy(&file);
CleanupStack::PopAndDestroy(&fs);
```

#### Custom cleanup item

Use `TCleanupItem` when a resource needs a custom release function.

#### Trap boundaries

Good boundaries include:

- Framework callbacks that cannot leave.
- Thread entry points.
- Observer methods where the contract is non-leaving.
- A top-level command handler with user-visible error reporting.

Do not wrap every line in `TRAP`; that hides ownership design errors and makes control flow soup.

#### Common failure

```cpp
HBufC* p = HBufC::NewL(100);
RiskyCallL();
delete p;
```

If `RiskyCallL()` leaves, `p` leaks. Push it or transfer ownership before the leaving call.

---

<a id="src-02-language-runtime-symbian-cpp-conventions-md"></a>
## Symbian C++ Types, Naming, and Ownership

- Original corpus path: `02_language_runtime/symbian_cpp_conventions.md`
- Consolidated reference: `03-symbian-cpp-runtime.md#src-02-language-runtime-symbian-cpp-conventions-md`

### Symbian C++ Types, Naming, and Ownership

#### Prefix conventions

| Prefix | Meaning | Ownership pattern |
|---|---|---|
| `T` | value type | stack/member; usually no destructor responsibility |
| `C` | heap class, often `CBase` | owned pointer, delete when done |
| `R` | handle to server/resource | call `Close()` |
| `M` | mixin/interface | not owned unless documented |
| `E` | enumeration | value |
| `K` | constant | static/read-only |

#### Suffix conventions

- `L`: may leave.
- `LC`: may leave and returns/preserves an item on the Cleanup Stack.
- `D`: historically may destroy the object as part of the call; read docs carefully.

#### Ownership language

Common documentation phrases:

- **takes ownership**: caller must not destroy after transfer.
- **does not take ownership**: caller keeps object alive.
- **returns a pointer to internal data**: do not delete; lifetime tied to owner.
- **adopts**: callee becomes owner.

#### `CBase`

Memory allocated with `new (ELeave)` for a `CBase`-derived object is zero-filled. Do not generalize that guarantee to arbitrary stack objects or non-`CBase` allocations.

#### Member initialization

Prefer explicit initialization even when zero-fill exists. It documents intent and avoids surprises when class hierarchy changes.

#### Error model

Symbian APIs use:

- Return codes (`TInt`, `KErrNone`, negative system errors).
- Leaves for exceptional failure.
- Panics for programming errors/invariant violations.
- Asynchronous completion via `TRequestStatus`.

A panic is not a recoverable runtime error. Fix the bug.

---

<a id="src-02-language-runtime-threads-processes-tls-md"></a>
## Threads, Processes, Mutexes, and TLS

- Original corpus path: `02_language_runtime/threads_processes_tls.md`
- Consolidated reference: `03-symbian-cpp-runtime.md#src-02-language-runtime-threads-processes-tls-md`

### Threads, Processes, Mutexes, and TLS

#### Processes

`RProcess` creates or controls a process. Executables run in separate protected processes under platform security.

```cpp
RProcess process;
User::LeaveIfError(process.Create(_L("Other.exe"), _L("args")));
CleanupClosePushL(process);
process.Resume();
CleanupStack::PopAndDestroy(&process);
```

Use rendezvous when the child must report initialization success.

#### Threads

`RThread` creates or controls a thread. A new thread needs:

- An entry function.
- Stack/heap sizing.
- An installed Active Scheduler if it uses Active Objects.
- A top-level `TRAP` boundary and cleanup.

#### Synchronization

Common handles:

- `RMutex`
- `RSemaphore`
- `RChunk`
- `RMsgQueue<T>` where available

All are `R` handles: close them.

#### Thread-local storage

`Dll::Tls()` / `Dll::SetTls()` are classic TLS mechanisms. Avoid global mutable state across threads.

#### UI rule

Avkon controls and window-server interactions normally belong to the UI thread. Use worker threads for CPU or blocking operations, then marshal results back safely.

#### Panic and exit information

`RThread::ExitType()`, `ExitReason()`, and `ExitCategory()` help inspect a terminated thread/process.

---

<a id="src-02-language-runtime-two-phase-construction-md"></a>
## Two-Phase Construction

- Original corpus path: `02_language_runtime/two_phase_construction.md`
- Consolidated reference: `03-symbian-cpp-runtime.md#src-02-language-runtime-two-phase-construction-md`

### Two-Phase Construction

#### Pattern

```cpp
class CWorker : public CBase
{
public:
    static CWorker* NewL();
    static CWorker* NewLC();
    ~CWorker();
private:
    CWorker();
    void ConstructL();
};

CWorker* CWorker::NewLC()
{
    CWorker* self = new (ELeave) CWorker;
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
}

CWorker* CWorker::NewL()
{
    CWorker* self = NewLC();
    CleanupStack::Pop(self);
    return self;
}
```

#### Why

The C++ constructor itself should not leave after partially acquiring resources. `ConstructL()` may leave while the object is protected by the Cleanup Stack.

#### Destructor rule

The destructor must tolerate partial construction. Initialize handles/pointers to safe states and close/delete only what may have been acquired.

#### Ownership transfer

```cpp
CWorker* worker = CWorker::NewLC();
iWorker = worker;
CleanupStack::Pop(worker); // ownership now held by this object
```

#### Anti-patterns

- Calling leaving functions from a normal C++ constructor.
- Forgetting to pop after ownership transfer.
- Popping and destroying when a container adopted the object.
- Creating a `NewLC()` function that does not leave the result on the stack.
