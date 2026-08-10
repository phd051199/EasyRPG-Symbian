# Connectivity, sockets, HTTP, IAP, Bluetooth, and WLAN

> This file consolidates canonical handbook topics for progressive loading. Each section records its original corpus path and retains external source links.

---

<a id="src-05-connectivity-bluetooth-md"></a>
## Bluetooth Discovery and Sockets

- Original corpus path: `05_connectivity/bluetooth.md`
- Consolidated reference: `06-connectivity-networking.md#src-05-connectivity-bluetooth-md`

### Bluetooth Discovery and Sockets

#### Capability and dependencies

Normal local Bluetooth operations typically involve:

```text
CAPABILITY LocalServices
LIBRARY esock.lib bluetooth.lib
```

Headers may include `es_sock.h`, `bt_sock.h`, and service-discovery headers.

#### Common tasks

- Device discovery with `RHostResolver` and Bluetooth inquiry protocols.
- Service discovery through SDP APIs.
- RFCOMM socket connection.
- Listening socket for incoming RFCOMM clients.
- Physical link metrics/control through optional APIs.

#### Socket model

Bluetooth logical connections still use `RSocketServ`/`RSocket`, but address families/protocol constants and address structures are Bluetooth-specific.

#### Discovery lifecycle

Discovery is asynchronous. Repeatedly request the next result until completion, cancellation, or end-of-results. Keep result buffers alive.

#### RFCOMM

Treat RFCOMM as a stream. Apply framing and partial read/write rules like TCP.

#### Security and pairing

Pairing prompts and authentication are system managed. Do not assume a device is paired merely because inquiry found it.

#### Device variance

Stack behavior, discoverability, channel allocation, and service records vary. Test on the actual E72-class device.

---

<a id="src-05-connectivity-connections-iap-md"></a>
## Connections, Bearers, and IAP Selection

- Original corpus path: `05_connectivity/connections_iap.md`
- Consolidated reference: `06-connectivity-networking.md#src-05-connectivity-connections-iap-md`

### Connections, Bearers, and IAP Selection

#### `RConnection`

`RConnection` binds network activity to a connection/bearer and is opened from `RSocketServ`.

```cpp
#include <es_sock.h>
#include <commdbconnpref.h>
```

Typical library:

```text
esock.lib
```

#### User-prompted selection

Connection preference structures can request a dialog or specify an IAP. Exact class names and fields vary by platform/API version; verify in FP2 headers.

#### Start asynchronously

Starting a connection can be asynchronous and should be wrapped in an Active Object. Once started, sockets/HTTP can be associated with the connection according to API support.

#### Do not hardcode IAP IDs casually

IAP IDs differ per device/user configuration. Prefer user selection or application settings.

#### Connection Monitor

Advanced bearer/state monitoring may require additional APIs or plug-ins. Check capability and API availability.

#### Common errors

- `KErrNotFound`: IAP missing.
- `KErrCancel`: user canceled selection.
- Permission errors: missing capability/signing.
- Connection succeeds but DNS/HTTP fails: proxy/APN or bearer policy issue.

---

<a id="src-05-connectivity-http-client-md"></a>
## HTTP Client Framework

- Original corpus path: `05_connectivity/http_client.md`
- Consolidated reference: `06-connectivity-networking.md#src-05-connectivity-http-client-md`

### HTTP Client Framework

#### Main interfaces

Common S60/Symbian HTTP client types:

- `RHTTPSession`
- `RHTTPTransaction`
- `RHTTPHeaders`
- `MHTTPTransactionCallback`
- `MHTTPDataSupplier`

Typical includes/libraries:

```cpp
#include <http.h>
#include <mhttptransactioncallback.h>
```

```text
LIBRARY http.lib
CAPABILITY NetworkServices
```

Some projects also need connection/socket libraries depending on how the session is configured.

#### Transaction lifecycle

1. Open HTTP session.
2. Configure connection/proxy/filter properties if needed.
3. Resolve URI using the string pool.
4. Open transaction with callback.
5. Set headers and optional body supplier.
6. Submit.
7. Handle events in `MHFRunL()`.
8. Consume response body parts and release them.
9. Close transaction, then session.

#### Callback events

Handle at least:

- Response headers received.
- Response body data available.
- Response complete/succeeded.
- Failed/error events.

Always call the body supplier's release method after consuming a body part, or the pipeline can stall.

#### Redirects and filters

Behavior depends on installed filters and session configuration. Do not assume modern-browser redirect/cookie behavior.

#### Content encoding

Server compression support and filter availability vary. For maximum compatibility, request `identity` when your code does not implement gzip/deflate decoding.

#### TLS

HTTPS support depends on device TLS stack, certificates, cipher suites, and system date. Modern servers may reject old TLS clients. A proxy/compatibility gateway may be required for 2026-era endpoints.

#### Exact API verification

The HTTP framework is stable conceptually, but string-table constants, properties, filters, and body-supplier details should be copied from the FP2 SDK examples rather than from later Symbian docs.

---

<a id="src-05-connectivity-sockets-networking-md"></a>
## Socket Networking with RSocketServ and RSocket

- Original corpus path: `05_connectivity/sockets_networking.md`
- Consolidated reference: `06-connectivity-networking.md#src-05-connectivity-sockets-networking-md`

### Socket Networking with RSocketServ and RSocket

#### Requirements

```cpp
#include <es_sock.h>
#include <in_sock.h>
```

Typical libraries/capability:

```text
LIBRARY esock.lib insock.lib
CAPABILITY NetworkServices
```

`NetworkControl` is needed only for privileged control operations, not normal client connections.

#### Lifecycle

1. `RSocketServ::Connect()`.
2. Optionally open/start `RConnection`.
3. Open `RSocket` with address family/type/protocol.
4. Resolve host using `RHostResolver`.
5. Connect asynchronously.
6. Read/write asynchronously.
7. Cancel outstanding requests.
8. Close socket, resolver, connection, socket server.

#### Open TCP socket

```cpp
User::LeaveIfError(iSocketServ.Connect());
User::LeaveIfError(iSocket.Open(iSocketServ, KAfInet, KSockStream, KProtocolInetTcp));
```

#### Async connect

```cpp
iSocket.Connect(address, iStatus);
SetActive();
```

#### Framing

TCP is a byte stream. One `Write()` is not guaranteed to equal one `Recv()` at the peer. Define message framing: length prefix, delimiter, or protocol parser.

#### Read behavior

Handle partial reads, EOF, cancellation, and transient errors. Keep the receive descriptor alive until completion.

#### DNS

`RHostResolver` queries names asynchronously. Results may return IPv4 structures; inspect exact FP2 examples.

#### Cleanup

Destroy Active Objects before the handles they use. `DoCancel()` must call the matching socket cancellation method.

---

<a id="src-05-connectivity-usb-serial-md"></a>
## USB, Serial, and Communications Server

- Original corpus path: `05_connectivity/usb_serial.md`
- Consolidated reference: `06-connectivity-networking.md#src-05-connectivity-usb-serial-md`

### USB, Serial, and Communications Server

#### Serial communications

Classic serial APIs use the Communications Server:

- `RCommServ`
- `RComm`
- port configuration structures

Typical dependencies include communications headers and `c32.lib`; exact module/port names depend on device drivers.

#### Lifecycle

1. Connect to comm server.
2. Load communication module if required.
3. Open port.
4. Configure baud/parity/handshake.
5. Read/write asynchronously.
6. Cancel and close.

#### USB

USB client APIs and modes are more device/platform-specific. A phone may expose PC Suite, mass storage, modem, or vendor modes without offering a general application USB endpoint.

#### App TRK versus application protocol

App TRK uses its own debugging transport. Do not assume the same port can be opened by your application simultaneously.

#### Recommendation

Search installed headers/examples for `RCommServ`, `RComm`, `usb`, and device-specific extension docs. Mark implementation as plug-in/device dependent.

---

<a id="src-05-connectivity-wlan-connection-monitor-md"></a>
## WLAN and Connection Monitoring

- Original corpus path: `05_connectivity/wlan_connection_monitor.md`
- Consolidated reference: `06-connectivity-networking.md#src-05-connectivity-wlan-connection-monitor-md`

### WLAN and Connection Monitoring

#### Availability

WLAN management and connection-monitor APIs can be base, API plug-in, Eseries, or device-specific depending on operation.

Potential tasks:

- Read active bearer and connection state.
- Observe connection creation/deletion.
- Scan visible WLAN networks.
- Read signal strength/SSID.
- Request or manage an IAP.

#### Capability

Passive information can still require `NetworkServices` or `ReadDeviceData`; control operations may require `NetworkControl`. Check each method.

#### Architecture

Connection Monitor APIs commonly use a client session and asynchronous notifications/attribute requests. WLAN scanning may use a dedicated WLAN management API.

#### Agent rule

Do not infer classes from later `RConnectionMonitor` or Qt Mobility examples. Search the FP2 SDK/API Plug-in Pack for:

```text
ConnectionMonitor
WlanMgmt
WlanScanInfo
ConnMon
```

Then report whether the required header exists before generating code.
