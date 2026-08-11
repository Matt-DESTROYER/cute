# cute
A (somewhat opinionated) mini package manager for C!

## Dependencies
 - libcurl (planned to remove at some point, and just have a small TLS library/pure sockets)
 - git

## Planned functionality
 - Initialise a C program (similar to `cargo new`)
 - 'Install' libraries (similar to `cargo add`, will be limited as to where they can be installed from)
     - Header libraries: just added as a header in `.includes/`
     - Linked libraries: will be added as a project in `.libraries/`
 - To track project dependencies and versions, `Cute.ini` will be used
     - `Cute.ini` can be modified directly or can be modified using `cute add <package> <?version>`
     - Dependencies of your dependencies will be listed in `Cute.lock` and stored in `.includes/` and `.libraries/`

## Cute.ini
Format:
```ini
; cute package manager settings
[cute]
version = "0"

; your project settings
[project]
name = "my-project"
version = "1.0.0"

; your project dependencies
[dependencies]
package-a = "1.5.2"
package-b = "2.1"

; source URL overrides
[overide-source]
package-a = "https://somedomain.ext/path/to/repo.git"
```

## Building from source
You will need...
 - A C compiler
 - CMake

To build:
```sh
cd src
cmake -B build
cmake --build build --config Release
```

