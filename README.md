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
> Not yet supported, but in future you may be able to specify a URL and it will happily clone any valid Git repository (using Git under the hood).

Once a package is added, it's ready to use.

When you're ready, run `cute build` to build your project!

## Libraries
To make your own library, follow the steps above and make sure your library is hosted in a GitHub repository you have access to.
Then anyone with access to your GitHub repository can simply `cute add` it to their package.
To release a version of your package, make sure the version in `Cute.ini` is correct, then simply release a tag with said version.

> Note: versions must start with 'v' and be valid SemVer 2.0.0 versions, otherwise `cute` will ignore them ().

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
$ cute add Matt-DESTROYER/file-io
$ cute build
$ ./build/hello_world
$ cat hello.txt
Hello, world!
```

Refer to the `examples` folder to see more examples of cute projects.

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
 - [CMake](http://cmake.org)

To build:
```sh
cd src
cmake -B build
cmake --build build --config Release
```

