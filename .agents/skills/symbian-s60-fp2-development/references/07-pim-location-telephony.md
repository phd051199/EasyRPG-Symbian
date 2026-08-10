# Contacts, calendar, messaging, location, sensors, and telephony

> This file consolidates canonical handbook topics for progressive loading. Each section records its original corpus path and retains external source links.

---

<a id="src-06-data-services-calendar-md"></a>
## Calendar Interim API

- Original corpus path: `06_data_services/calendar.md`
- Consolidated reference: `07-pim-location-telephony.md#src-06-data-services-calendar-md`

### Calendar Interim API

#### Main classes

S60 3rd-era calendar code often uses the Calendar Interim API:

- `CCalSession`
- `CCalEntryView`
- `CCalInstanceView`
- `CCalEntry`
- `CCalInstance`

Typical headers/libraries include:

```cpp
#include <calsession.h>
#include <calentryview.h>
#include <calinstanceview.h>
LIBRARY calinterimapi.lib
```

Verify exact header availability in the FP2 SDK.

#### Async view creation

Entry/instance views are commonly initialized asynchronously through observer interfaces. Keep the session alive longer than its views.

#### Entry lifecycle

- Open session/file.
- Create views.
- Create or fetch entries.
- Set time, type, summary, recurrence, alarms as supported.
- Store/update through entry view.

#### Capabilities

Reading/writing calendar data commonly requires `ReadUserData`/`WriteUserData`.

#### Time zones and recurrence

Use calendar-native time representations and verify local/UTC behavior. Recurrence rules are easy to corrupt; prefer SDK examples.

---

<a id="src-06-data-services-contacts-md"></a>
## Contacts Model API

- Original corpus path: `06_data_services/contacts.md`
- Consolidated reference: `07-pim-location-telephony.md#src-06-data-services-contacts-md`

### Contacts Model API

#### Main API

Common classes:

- `CContactDatabase`
- `CContactItem`
- `CContactCard`
- `CContactItemField`
- `CContactIdArray`

Typical dependency:

```cpp
#include <cntdb.h>
LIBRARY cntmodel.lib
```

#### Open database

```cpp
CContactDatabase* db = CContactDatabase::OpenL();
CleanupStack::PushL(db);
```

#### Read and update

Reading may return an owned contact item. Updating normally requires opening the item for edit, modifying fields, committing, then closing the contact lock correctly.

#### Field identification

Fields are identified by content types/field UIDs, not only by position. Search for phone/name/email field types.

#### Capabilities

User contacts normally require `ReadUserData` for reads and `WriteUserData` for changes.

#### Observer model

Database change notification APIs exist; use them if caching contact IDs or views.

#### Performance

Do not load every full contact when a view or ID list is sufficient. Contacts databases can be slow on device storage.

---

<a id="src-06-data-services-location-md"></a>
## Location-Based Services

- Original corpus path: `06_data_services/location.md`
- Consolidated reference: `07-pim-location-telephony.md#src-06-data-services-location-md`

### Location-Based Services

#### Main model

Location APIs typically expose:

- A position server session.
- A positioner/subsession.
- Position information structures.
- Asynchronous position updates.
- Module selection/status APIs.

Likely headers/libraries include LBS headers and `lbs.lib`, but exact names must be verified in the installed FP2 SDK or plug-in.

#### Capability

Location access requires the `Location` capability and may involve user privacy prompts/policy.

#### Async request

Position updates can take seconds or longer. Provide cancellation, timeout, and progress UI. Never block the UI thread waiting for GPS.

#### Quality and sources

Position may come from integrated GPS, assisted GPS, network methods, or external modules. Check accuracy and timestamp; do not treat every fix as fresh/high quality.

#### Emulator

Use simulation data only for logic testing. Real acquisition behavior must be tested on device.

---

<a id="src-06-data-services-messaging-md"></a>
## Messaging Framework: SMS, MMS, Email

- Original corpus path: `06_data_services/messaging.md`
- Consolidated reference: `07-pim-location-telephony.md#src-06-data-services-messaging-md`

### Messaging Framework: SMS, MMS, Email

#### Architecture

Symbian messaging uses the Message Server and MTM (Message Type Module) architecture.

Core concepts:

- `CMsvSession`: connection to Message Server.
- `CMsvEntry`: access to message entries/folders.
- `CMsvOperation`: asynchronous operation.
- Client MTMs for SMS, MMS, email, etc.

Typical core include/library:

```cpp
#include <msvapi.h>
LIBRARY msgs.lib
```

Additional MTM headers/libraries depend on message type and installed SDK components.

#### Session

Open one session per thread, synchronously or asynchronously, with an observer for server events.

#### Entry tree

Messages and folders are entries identified by IDs. Select an entry context, inspect metadata, then use the proper MTM to create/edit/send.

#### Asynchronous operations

Send/copy/move operations often return `CMsvOperation*`. Own it, observe its `TRequestStatus`, inspect progress, then destroy it.

#### Capabilities

Reading/writing user messages commonly involves `ReadUserData` and/or `WriteUserData`; sending can involve `NetworkServices`. Exact requirements depend on operation and MTM.

#### Complexity warning

Do not build SMS sending by guessing only `CMsvSession`. Use an FP2 SMS example because service IDs, MTM UIDs, settings, and entry construction must be correct.

---

<a id="src-06-data-services-sensors-md"></a>
## Sensor Framework on S60 3rd FP2

- Original corpus path: `06_data_services/sensors.md`
- Consolidated reference: `07-pim-location-telephony.md#src-06-data-services-sensors-md`

### Sensor Framework on S60 3rd FP2

#### Availability

Sensor APIs on S60 3rd FP2 commonly require the Sensor API plug-in and compatible device firmware. The base SDK alone may not contain the needed headers/libraries.

Potential classes include:

- Sensor server channel finder.
- Sensor channel.
- Data and property listener interfaces.

Headers/libraries often use `sensrv*` naming, such as `sensrvchannel.h` and `sensrvclient.lib`, but confirm the exact plug-in version.

#### Workflow

1. Find channels matching sensor type/location/vendor criteria.
2. Open a channel.
3. Configure properties if allowed.
4. Start listening for data.
5. Process callbacks quickly.
6. Stop listening and close.

#### Capability

Capability depends on sensor and device policy. Do not assume `None`.

#### Robustness

Handle no channel found, sensor busy, unsupported property, data loss, and orientation differences.

---

<a id="src-06-data-services-telephony-md"></a>
## Telephony APIs

- Original corpus path: `06_data_services/telephony.md`
- Consolidated reference: `07-pim-location-telephony.md#src-06-data-services-telephony-md`

### Telephony APIs

#### High-level API

`CTelephony` provides a simplified asynchronous interface for selected phone/network/call information.

Common dependency:

```cpp
#include <etel3rdparty.h>
LIBRARY etel3rdparty.lib
```

#### Operations

Depending on platform support:

- Network and registration information.
- Subscriber/phone information.
- Call status and dial/hangup operations.
- Notifications of state changes.

#### Asynchronous design

Requests use `TRequestStatus` and cancellation IDs. Wrap each outstanding operation in a dedicated Active Object or a carefully designed state machine.

#### Capabilities

Telephony capability requirements vary significantly by operation. Possible capabilities include `ReadDeviceData`, `NetworkServices`, `ReadUserData`, or stronger grants for sensitive control. Read the exact FP2 method documentation.

#### Low-level ETel

Lower-level APIs (`RTelServer`, phone/line/call subsessions) are more complex and may be restricted. Do not choose them unless the high-level API is insufficient.

#### Device and SIM variance

Emulator support is limited. Test with real SIM/network conditions and handle unsupported requests.
