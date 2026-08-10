# Avkon application architecture and UI

> This file consolidates canonical handbook topics for progressive loading. Each section records its original corpus path and retains external source links.

---

<a id="src-03-app-ui-application-architecture-md"></a>
## Avkon Application Architecture

- Original corpus path: `03_app_ui/application_architecture.md`
- Consolidated reference: `04-avkon-application-ui.md#src-03-app-ui-application-architecture-md`

### Avkon Application Architecture

#### Standard classes

| Role | Typical base |
|---|---|
| Application | `CAknApplication` |
| Document | `CAknDocument` |
| App UI | `CAknAppUi` |
| View/control | `CCoeControl` or Avkon view classes |

#### Startup chain

1. `E32Main()` calls `EikStart::RunApplication()`.
2. Framework calls `NewApplication()`.
3. Application creates the document.
4. Document creates the AppUi.
5. AppUi constructs controls/views and handles commands.

#### Minimal entry

```cpp
LOCAL_C CApaApplication* NewApplication()
{
    return new CMyApplication;
}

GLDEF_C TInt E32Main()
{
    return EikStart::RunApplication(NewApplication);
}
```

#### AppUi construction

```cpp
void CMyAppUi::ConstructL()
{
    BaseConstructL(EAknEnableSkin);
    iView = CMyView::NewL(ClientRect());
    AddToStackL(iView);
}
```

#### Destruction order

Remove controls from the control stack before deleting them:

```cpp
if (iView)
{
    RemoveFromStack(iView);
    delete iView;
    iView = NULL;
}
```

#### Commands

`HandleCommandL(TInt aCommand)` receives softkey/menu/framework commands. Handle `EAknSoftkeyExit` and `EEikCmdExit` explicitly.

#### View framework

Larger apps may use `CAknViewAppUi` and `CAknView` with view UIDs and activation/deactivation. Use one architecture consistently; do not mix simple-control and view-framework lifecycles casually.

---

<a id="src-03-app-ui-application-services-browser-md"></a>
## Application Services, Document Handling, and Browser Launch

- Original corpus path: `03_app_ui/application_services_browser.md`
- Consolidated reference: `04-avkon-application-ui.md#src-03-app-ui-application-services-browser-md`

### Application Services, Document Handling, and Browser Launch

#### Launching another application

Symbian application architecture provides process/document-launch mechanisms. Use documented application services instead of starting random executables by path.

#### Open a document/URL

S60 offers document-handler and browser-launch patterns. Depending on SDK, classes may include `CDocumentHandler` and browser UIDs/command-line parameters.

Typical use cases:

- Open a file in its registered handler.
- Send a file through available services.
- Launch browser with an HTTP/HTTPS URL.

#### Risks

- Browser UID and command format can differ by S60 version.
- A handler may be absent.
- The target application may reject unsupported MIME types.
- Passing untrusted URI/filename data requires validation.

#### Preferred approach

Find the FP2 SDK example for document handling or browser launch and use its constants. Avoid copying a Symbian^3 browser UID from a forum post.

---

<a id="src-03-app-ui-avkon-ui-controls-md"></a>
## Avkon UI Controls and Dialogs

- Original corpus path: `03_app_ui/avkon_ui_controls.md`
- Consolidated reference: `04-avkon-application-ui.md#src-03-app-ui-avkon-ui-controls-md`

### Avkon UI Controls and Dialogs

#### Control basics

A custom `CCoeControl` typically:

1. Calls `CreateWindowL()` or becomes a compound control.
2. Sets its rectangle.
3. Calls `ActivateL()`.
4. Implements `Draw()`.
5. Implements `SizeChanged()`.

#### Common UI classes

- `CAknMessageQueryDialog`
- `CAknInformationNote`
- `CAknErrorNote`
- `CAknTextQueryDialog`
- `CAknListQueryDialog`
- `CAknSingleStyleListBox`
- `CEikEdwin`
- `CAknTitlePane`
- `CEikStatusPane`

Exact listbox model formatting uses tab-separated text and optional icon indexes. Consult the FP2 example for the chosen list style.

#### Dialog lifecycle

Many dialogs use `PrepareLC()` then `RunLD()`, where `RunLD()` destroys the dialog:

```cpp
CAknMessageQueryDialog* dlg = new (ELeave) CAknMessageQueryDialog;
dlg->PrepareLC(R_MY_QUERY);
dlg->RunLD();
```

Do not delete `dlg` afterward.

#### Notes

```cpp
CAknInformationNote* note = new (ELeave) CAknInformationNote;
note->ExecuteLD(_L("Saved"));
```

#### Status pane

Retrieve panes through the AppUi status pane and cast using the expected pane UID. Layout and availability vary; test on the target skin/device.

#### Keypad-first design

S60 3rd FP2 is primarily keypad-driven:

- Softkeys and Options menu are central.
- Focus visibility matters.
- Avoid touch assumptions.
- Support directional navigation and numeric shortcuts where appropriate.

---

<a id="src-03-app-ui-background-tasks-md"></a>
## Background Execution and UI Lifecycle

- Original corpus path: `03_app_ui/background_tasks.md`
- Consolidated reference: `04-avkon-application-ui.md#src-03-app-ui-background-tasks-md`

### Background Execution and UI Lifecycle

#### Hiding versus terminating

On S60, pressing the Home/Menu key normally sends the application to the background; the process may continue. Exit softkeys/commands terminate the application.

#### Background work

Use Active Objects for timers, sockets, HTTP, playback callbacks, and other asynchronous work. Avoid busy loops.

#### System pressure

The OS can reclaim resources or terminate applications under memory pressure. Persist essential state.

#### Autostart

Autostart mechanisms are platform-security and packaging sensitive. They may require recognizers, startup list integration, or protected capabilities depending on approach. Verify against FP2-specific documentation and signing policy.

#### UI updates

When background work completes, update controls on the UI thread and verify the view still exists.

#### Audio

Background playback is possible when using audio APIs correctly, but audio focus/policy and interruptions are device controlled. Handle playback callbacks and errors.

---

<a id="src-03-app-ui-graphics-window-server-md"></a>
## Graphics, Controls, and the Window Server

- Original corpus path: `03_app_ui/graphics_window_server.md`
- Consolidated reference: `04-avkon-application-ui.md#src-03-app-ui-graphics-window-server-md`

### Graphics, Controls, and the Window Server

#### Drawing model

Controls draw through a graphics context such as `CWindowGc`. `Draw(const TRect&) const` should render the current state without long-running work.

```cpp
void CMyControl::Draw(const TRect& aRect) const
{
    CWindowGc& gc = SystemGc();
    gc.Clear(aRect);
    gc.DrawRect(Rect());
}
```

#### Fonts

Use fonts obtained from the UI environment or screen device. Call `UseFont()` and `DiscardFont()` around drawing.

#### Invalidation

Call `DrawNow()` for immediate redraw or invalidate through the window/control mechanisms appropriate to the UI. Avoid continuous redraw loops; use timers/animation frameworks.

#### Bitmaps

- `CFbsBitmap`: bitmap handle.
- `CFbsBitmapDevice`: draw device for a bitmap.
- `CFbsBitGc`: bitmap graphics context.
- `AknIconUtils`: scalable icon utilities when available.

#### Double buffering

For complex custom graphics, draw to an off-screen bitmap then blit. Measure memory: a full-color buffer on a constrained device is not free.

#### Screen changes

Handle `SizeChanged()` and resource/layout changes. Do not hardcode E72 dimensions when code may run on another FP2 device.

#### Threading

Window-server objects are thread-affine in normal UI usage. Keep drawing and control mutation on the UI thread.

---

<a id="src-03-app-ui-input-keys-commands-md"></a>
## Keys, Focus, Commands, and Menus

- Original corpus path: `03_app_ui/input_keys_commands.md`
- Consolidated reference: `04-avkon-application-ui.md#src-03-app-ui-input-keys-commands-md`

### Keys, Focus, Commands, and Menus

#### Key events

Controls can implement:

```cpp
TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType);
```

Return `EKeyWasConsumed` only when the control handled the event.

#### Scan codes versus character codes

- `iScanCode` identifies physical keys.
- `iCode` carries translated character/key codes.
- Event type distinguishes key down/up/repeat behavior.

#### Command IDs

Define application commands in `.hrh` outside ranges used by framework commands.

```cpp
enum TMyCommands
{
    EMyRefresh = 0x6001,
    EMyAbout
};
```

#### Dynamic menus

Override menu initialization callbacks when items must be dimmed or changed based on state. Use the exact AppUi callback signature available in FP2.

#### Focus

Compound controls should implement component count/accessors and propagate focus. Keypad UI bugs often come from invisible or misplaced focus, not from input not arriving.

#### Long-running commands

A command handler may start an Active Object, then return. Do not perform blocking network or filesystem scans directly in the UI callback.

---

<a id="src-03-app-ui-localization-md"></a>
## Localization and String Resources

- Original corpus path: `03_app_ui/localization.md`
- Consolidated reference: `04-avkon-application-ui.md#src-03-app-ui-localization-md`

### Localization and String Resources

#### Keep UI text in resources

Hardcoded literals are acceptable for debugging, not for a real localized application.

#### Language resources

S60 projects commonly build language-specific resource files. `LANG` in `.mmp`, localized `.rss` files, and package language declarations must agree.

#### Package language

A `.pkg` can declare supported languages and localized names. Exact syntax depends on packaging needs; use SDK examples matching the target.

#### Unicode

Symbian descriptors are normally 16-bit for UI text. External files and network data require explicit encoding conversion.

#### Layout

Translations can be longer than English. Avoid fixed pixel widths and verify list/dialog behavior.

#### Agent rule

When generating code, place visible strings in resource files unless the task is a minimal diagnostic sample.

---

<a id="src-03-app-ui-resources-rss-rsg-md"></a>
## Resource Files: RSS, RH, RSG, Registration

- Original corpus path: `03_app_ui/resources_rss_rsg.md`
- Consolidated reference: `04-avkon-application-ui.md#src-03-app-ui-resources-rss-rsg-md`

### Resource Files: RSS, RH, RSG, Registration

#### Roles

- `.rss`: resource definitions.
- `.rh`: resource structures/enums provided by SDK or project.
- `.hrh`: project command IDs and enums.
- `.rsg`: generated resource IDs included by C++.
- `_reg.rss`: application registration resource.

#### Main application resource

```text
RESOURCE EIK_APP_INFO
{
    menubar = r_my_menubar;
    cba = R_AVKON_SOFTKEYS_OPTIONS_EXIT;
}
```

#### Menu

```text
RESOURCE MENU_PANE r_my_menu
{
    items =
    {
        MENU_ITEM { command = EMyAbout; txt = "About"; },
        MENU_ITEM { command = EAknCmdExit; txt = "Exit"; }
    };
}
```

#### Registration resource

```text
UID2 KUidAppRegistrationResourceFile
UID3 0xE0001234

RESOURCE APP_REGISTRATION_INFO
{
    app_file = "MyApp";
    localisable_resource_file = "\\resource\\apps\\MyApp";
    localisable_resource_id = R_MYAPP_LOCALISABLE_APP_INFO;
}
```

#### Resource compiler troubleshooting

`Cannot convert "Identifier" to a number` usually means:

- The symbol is not defined by included `.rh/.hrh` files.
- The symbol belongs to another platform edition.
- Include order or generated `.rsg` is wrong.
- A UI Designer-generated resource expects a plug-in.

Do not replace the symbol with a random integer. Identify the expected enum/resource definition.

---

<a id="src-03-app-ui-view-framework-md"></a>
## Avkon View Framework

- Original corpus path: `03_app_ui/view_framework.md`
- Consolidated reference: `04-avkon-application-ui.md#src-03-app-ui-view-framework-md`

### Avkon View Framework

#### When to use

Use `CAknViewAppUi`/`CAknView` for applications with multiple navigable screens and view activation through UIDs.

#### Core lifecycle

- AppUi registers views.
- Each view has a view UID.
- `DoActivateL()` creates/configures the visible container and adds it to the control stack.
- `DoDeactivate()` removes and destroys/hides the container.
- Navigation uses view activation APIs and optional custom messages.

#### Ownership

The AppUi normally owns registered views. A view often owns its current container. Remove the container from the control stack before deletion.

#### Activation messages

Activation can carry a custom message UID and descriptor payload. Copy payload data if it must outlive the callback.

#### Pitfalls

- Adding a container twice to the control stack.
- Not removing it during deactivation.
- Updating a deactivated/destroyed container from an async callback.
- Duplicate view UIDs.
