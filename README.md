# cute

A (somewhat opinionated) mini package manager for C!

## Dependencies
- git
- cmake
- A C compiler (C11)

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

The installation scripts will build Cute from source and install it to your user directory.

On macOS and Linux, Cute is installed to `~/.local/bin`.

On Windows, Cute is installed to `%LOCALAPPDATA%\cute\bin`.

Both scripts will also attempt to configure your `PATH` automatically. You may need to open a new terminal for the changes to take effect.

## Usage

### Creating a project
Create a new project using `cute new`, supplying a name. You can optionally specify that the project should be a library.

```sh
cute new my-project

# Create a library instead
cute new my-library --lib
```

Alternatively, you can initialise a project in the current directory using `--init`.

```sh
mkdir my-project
cd my-project
cute new my-project --init
```

Cute will generate the following project structure:

```text
my-project/
├── .gitignore
├── CMakeLists.txt
├── Cute.ini
├── Cute.lock
├── src/
│   └── main.c
└── tests/
```

For libraries, Cute will generate a source file and a corresponding header instead of `main.c`.

### Adding dependencies

To add a dependency, use `cute add`, supplying the GitHub repository in `owner/repository` format.

```sh
cute add GitHubUser/CutePackageRepo
```

You can optionally specify a version:

```sh
cute add GitHubUser/CutePackageRepo 1.2.0
```

When a package is added, Cute will clone the repository into your project's `.libraries` directory and add it to the `[dependencies]` section of `Cute.ini`.

> Note: Cute expects packages to be Git repositories hosted on GitHub. Dependencies are cloned using Git under the hood.

Once a package has been added, it can be included in your project and linked automatically during compilation.

### Building

When you're ready, run `cute build` to build your project!

```sh
cute build
```

By default, Cute uses the Debug build configuration.

To build using the Release configuration:

```sh
cute build --release
```

Cute automatically fetches missing dependencies before configuring and building your project with CMake.

### Fetching dependencies

You can fetch your project's dependencies without building it using `cute fetch`.

```sh
cute fetch
```

This installs dependencies that are not already present in `.libraries`.

### Testing

Cute provides a testing interface through CTest.

```sh
cute test
```

This will configure the project with testing enabled, build the project, and run its tests.

You can also select individual tests by supplying their names:

```sh
cute test my-test
```

Multiple tests can be selected:

```sh
cute test test-a test-b
```

Tests are discovered from the `tests` directory, with their relative file paths (excluding the `.c` extension) used as their names.

For example:

```text
tests/
├── test-a.c
└── utilities/
    └── test-b.c
```

Can be selected using:

```sh
cute test test-a utilities/test-b
```

Like `cute build`, tests use the Debug configuration by default. You can also use the Release configuration:

```sh
cute test --release
```

> Note: The generated CMake configuration currently enables automatic test discovery for library projects. Test files should be placed in `tests/` and implemented as standalone C test executables.

## Libraries

To make your own library, create a new project using `--lib`.

```sh
cute new my-library --lib
```

This will generate a library project containing:

```text
my-library/
├── CMakeLists.txt
├── Cute.ini
├── Cute.lock
├── src/
│   ├── my-library.c
│   └── my-library.h
└── tests/
```

Libraries are built as static libraries using CMake.

To release a version of your package, make sure the version in `Cute.ini` is correct, then release a Git tag with the corresponding version.

For example, if your `Cute.ini` contains:

```ini
[project]
name = "my-library"
version = "1.2.0"
type = "library"
```

You should release the following Git tag:

```sh
git tag v1.2.0
git push origin v1.2.0
```

Other users can then add your library using:

```sh
cute add GitHubUser/my-library 1.2.0
```

> Note: Cute currently expects version tags to use the `v` prefix. Version resolution and compatibility checking are still under development.

## Example

Here is a complete example of setting up and building a `cute` project.

```sh
$ cute new hello_world
$ cd hello_world
$ ls
CMakeLists.txt  Cute.ini  Cute.lock  src  tests
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

## Commands

| Command | Description |
| --- | --- |
| `cute new <name>` | Create a new project. |
| `cute new <name> --lib` | Create a new library project. |
| `cute new <name> --init` | Initialise a project in the current directory. |
| `cute add <package> [version]` | Add a GitHub dependency. |
| `cute fetch` | Fetch missing project dependencies. |
| `cute build` | Build the current project. |
| `cute build --release` | Build using the Release configuration. |
| `cute test [tests...]` | Build and run tests, optionally selecting specific tests. |
| `cute test --release` | Build and run tests using the Release configuration. |
| `cute version` | Display the current project's version. |
| `cute --version` | Display Cute's version. |
| `cute update` | Update dependencies (not yet implemented). |
| `cute clean` | Clean the project (not yet implemented). |

## Cute.ini

`Cute.ini` is the main configuration file for a Cute project.

It contains information about the package manager version, project metadata, and project dependencies.

### Format

```ini
; cute package manager settings
[cute]
version = "v0.16.4"

; your project settings
[project]
name = "my-project"
version = "1.0.0"
type = "executable"

; your project dependencies
[dependencies]
GitHubUser/package-a = "1.5.2"
GitHubUser/package-b = ""

```

### Sections

**[cute]**

Contains settings relating to Cute itself.

| Key | Description |
| --- | --- |
| `version` | The version of Cute used to manage the project. |

**[project]**

Contains your project's metadata.

| Key | Description |
| --- | --- |
| `name` | The name of your project. |
| `version` | The current version of your project. |
| `type` | The project type, either `executable` or `library`. |

**[dependencies]**

Contains your project's dependencies.

Each key represents a GitHub repository in `owner/repository` format, and its value specifies the requested version.

An empty value indicates that no specific version has been requested.

> Note: Although version information can currently be supplied when adding a dependency, full semantic version constraint resolution is still under development.

## Cute.lock

`Cute.lock` is an automatically generated file intended to record the resolved dependencies of a project.

It is created when a new project is initialised.

> Note: Lock-file generation and dependency resolution are still under development. Do not edit this file directly.

## Building from source

You will need:

- A C compiler supporting C11
- [CMake](https://cmake.org)
- [Git](https://git-scm.com)

To build:

```sh
cmake -B build
cmake --build build --config Release
```

To install:

```sh
cmake --install build --prefix ~/.local
```
