# cute
A (somewhat opinionated) mini package manager for C!

## Dependencies
 - git
 - cmake

## Installation
Clone the repo and run the relevant install script.

On Windows:
```pwsh
.\install.ps1
```

On macOS and Linux:
```sh
./install.sh
```

## Usage
Create a new package using `cute new` supply a name, and optionally specify that the package is a library.
```sh
cute new my-package
# OR
cute new my-library --lib
```

Then to add any package you wish, use `cute add`.
```sh
cute add GitHubUser/CutePackageRepo
```

> Note: cute expects packages to be Git repositories.
> By default it will look for the package on GitHub.
> Not yet supported, but you can specify a URL and it will happily clone any valid Git repository (using Git under the hood).

Once a package is added, it's ready to use.

When you're ready, run `cute build` to build your project!

## Example

Here is a complete example of setting up and building a `cute` project.

```sh
$ cute new hello_world
$ cd hello_world
$ ls
CMakeLists.txt  Cute.ini  Cute.lock  src
$ ls src
main.c
$ cat src/main.c
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
        printf("Hello, world!\n");

        return EXIT_SUCCESS;
}
$ nano src/main.c
$ cat src/main.c
#include <stdlib.h>
#include <string.h>
#include <file-io.h>

const char* hello_world = "Hello, world!\n";

int main(int argc, char* argv[]) {
        file_t file = file_open("hello.txt", WRITE);
        if (file)
                file_write(file, hello_world, strlen(hello_world));
        file_close(file);

        return EXIT_SUCCESS;
}
cute add Matt-DESTROYER/file-io
cute build
./build/hello_world
cat hello.txt
Hello, world!
```

Refer to the `examples` folder to see more examples of cute projects.

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
version = "0.2"

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

