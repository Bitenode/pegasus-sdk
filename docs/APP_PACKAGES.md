# Pegasus App Packages (`.pap`)

Ship a userspace app (built with this SDK) as an installable **manifest package**:
a folder named `Something.pap/` with a `manifest` plus payload files. There is
no zip/tar unpacker in v1 — packages live expanded on the PegasusFS data disk
(for example under `Downloads/`).

Built-in kernel apps (Terminal, Notepad, Settings, Browser, …) are not managed
by the installer.

## Build the ELF with this SDK

```sh
export PATH="/path/to/pegasus-sdk/bin:$PATH"
pegasus-cc -o myapp.elf myapp.c -lgui   # GUI apps need -lgui
```

GUI apps must be launched with the **GUI spawn** profile on the guest (see
[GUI_SDK.md](GUI_SDK.md)). After packaging + install, Start menu / Desktop
shortcuts use that path automatically for `kind=gui`.

## Package layout

```
MyApp.pap/
├── manifest
├── bin/myapp              # ELF (same relative path as binary=)
└── apps/myapp/icon.png    # optional icon
```

## Manifest format

Line-oriented `key=value` (no JSON):

```
id=myapp
name=My App
version=1.0
binary=bin/myapp
kind=gui
start_menu=1
files=bin/myapp,apps/myapp/icon.png
icon=apps/myapp/icon.png
```

| Field | Meaning |
|-------|---------|
| `id` | Stable uninstall key; `[a-z0-9_-]+` |
| `name` | Display name (Settings / Start menu / Desktop) |
| `version` | Free-form version string |
| `binary` | Launch path of the main ELF (relative, no `..`) |
| `kind` | `gui` → GUI spawn; `cli` → CLI spawn |
| `start_menu` | `1` pin after install; `0` skip |
| `files` | Comma-separated relative paths **owned** by the package |
| `icon` | Optional PNG/ICO (or other supported image) for Start / Desktop / taskbar. Omit for a procedural letter-tile default. |

**Install** copies each `files` entry from `pkg/<rel>` → `/<rel>` on the data
disk, then appends a row to `System/installed-apps.txt`. If `icon=` is set and
not already listed in `files=`, that path is auto-copied the same way.

**Uninstall** deletes exactly the paths in that registry row’s `files` list
(never a whole tree like `bin/`), then drops the row. Put the icon path in
`files=` if it should be removed on uninstall.

When `icon=` is omitted or the file cannot be decoded, the shell draws a
colored square with the first letter of the app name.

`binary=` is the run path. It may also appear in `files=` (copied on install /
deleted on uninstall), or it may refer to a pre-seeded system ELF.

## Registry

Persistent file: `System/installed-apps.txt`

```
id|name|version|binary|kind|start_menu|files|icon
```

Older rows without the trailing `|icon` field still load.

## Install / uninstall on the guest

**Settings → Apps**

1. Copy your `.pap` folder onto the data disk (e.g. `Downloads/MyApp.pap`).
2. Open Settings → **Apps**, enter the package path, click **Install**.
3. Select a row and click **Uninstall** twice to confirm.

**Terminal** (kernel builtins):

```
install Downloads/MyApp.pap
apps
uninstall myapp
```

With `start_menu=1`, the app is pinned in the Start menu (above Log Out /
Shut Down). Right-click a Start menu app → **Add to desktop** creates a
`Desktop/<Name>.app` shortcut.

## Demo package

Pegasus seeds `Downloads/sample-gui.pap` (uses the built-in `bin/gui_hello`
ELF; no custom `icon=` — letter-tile default). Install it from Settings → Apps
to try the flow end-to-end.

## Constraints

- PegasusFS has **64 inodes** and **64-character** path names — keep packages small.
- Install requires a mounted persistent PegasusFS disk (not RAM-only).
- `id` must be unique; reinstall after `uninstall`.
- Path entries reject `..` and absolute escapes.
- ELF magic is checked on `binary` after install.

## Shell icons (no userspace API)

Start menu, desktop shortcuts, and taskbar window buttons show app icons.
There is no syscall yet to set a window icon at runtime — supply `icon=` in
the package (or rely on the default letter tile). Taskbar icons resolve from
the process name / installed registry entry when possible.
