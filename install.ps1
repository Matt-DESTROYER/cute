$ErrorActionPreference = "Stop"

$RepoUrl = "https://github.com/Matt-DESTROYER/cute.git"

$SourceDir = $null
$TempSourceDir = $null
$BuildDir = $null

function Cleanup {
    if ($null -ne $BuildDir -and (Test-Path $BuildDir)) {
        try {
            Remove-Item -Recurse -Force $BuildDir -ErrorAction SilentlyContinue
        }
        catch {
            # The build directory is temporary, so failure to clean it up
            # should not make an otherwise successful installation fail.
        }
    }

    if ($null -ne $TempSourceDir -and (Test-Path $TempSourceDir)) {
        try {
            Remove-Item -Recurse -Force $TempSourceDir -ErrorAction SilentlyContinue
        }
        catch {
            # Same reasoning as above.
        }
    }
}

try {
    # -----------------------------------------------------------------------
    # Check prerequisites
    # -----------------------------------------------------------------------

    if (-not (Get-Command git -ErrorAction SilentlyContinue)) {
        throw "git is required but was not found in PATH."
    }

    if (-not (Get-Command cmake -ErrorAction SilentlyContinue)) {
        throw "cmake is required but was not found in PATH."
    }

    # -----------------------------------------------------------------------
    # Find source
    # -----------------------------------------------------------------------

    if (Test-Path "CMakeLists.txt") {
        $SourceDir = (Get-Location).Path

        Write-Host "Using cute repository in current directory."
    }
    elseif (Test-Path "cute/CMakeLists.txt") {
        $SourceDir = (Resolve-Path "cute").Path

        Write-Host "Using cute repository in ./cute."
    }
    else {
        $TempSourceDir = Join-Path `
            $env:TEMP `
            ("cute-source-" + [Guid]::NewGuid())

        New-Item `
            -ItemType Directory `
            -Force `
            -Path $TempSourceDir | Out-Null

        Write-Host "Downloading cute..."

        git clone `
            --depth 1 `
            $RepoUrl `
            (Join-Path $TempSourceDir "cute")

        $SourceDir = Join-Path $TempSourceDir "cute"
    }

    # -----------------------------------------------------------------------
    # Build
    # -----------------------------------------------------------------------

    # Keep the build directory separate from the source tree.
    #
    # This means:
    #   - An existing checkout's build directory is untouched.
    #   - A temporary clone does not contain its build directory.
    #   - Visual Studio generators do not put build outputs underneath the
    #     temporary source directory.
    $BuildDir = Join-Path `
        $env:TEMP `
        ("cute-build-" + [Guid]::NewGuid())

    New-Item `
        -ItemType Directory `
        -Force `
        -Path $BuildDir | Out-Null

    Write-Host "Building cute..."

    cmake `
        -S $SourceDir `
        -B $BuildDir

    cmake `
        --build $BuildDir `
        --config Release

    # -----------------------------------------------------------------------
    # Install
    # -----------------------------------------------------------------------

    $InstallPrefix = Join-Path $env:LOCALAPPDATA "cute"
    $InstallBinDir = Join-Path $InstallPrefix "bin"

    Write-Host "Installing cute..."

    New-Item `
        -ItemType Directory `
        -Force `
        -Path $InstallBinDir | Out-Null

    cmake `
        --install $BuildDir `
        --config Release `
        --prefix $InstallPrefix

    # -----------------------------------------------------------------------
    # Verify installation
    # -----------------------------------------------------------------------

    $CutePath = Join-Path $InstallBinDir "cute.exe"

    if (-not (Test-Path $CutePath)) {
        throw "cute.exe was not found after installation at: $CutePath"
    }

    # -----------------------------------------------------------------------
    # Configure PATH
    # -----------------------------------------------------------------------

    $UserPath = [Environment]::GetEnvironmentVariable("Path", "User")

    if ($null -eq $UserPath) {
        $UserPath = ""
    }

    $PathEntries = @(
        $UserPath -split ';' |
        Where-Object { $_ -ne "" }
    )

    if ($PathEntries -notcontains $InstallBinDir) {
        $PathEntries += $InstallBinDir

        [Environment]::SetEnvironmentVariable(
            "Path",
            ($PathEntries -join ';'),
            "User"
        )

        Write-Host "Added $InstallBinDir to your user PATH."
    }
    else {
        Write-Host "$InstallBinDir is already in your user PATH."
    }

    # Update PATH for this PowerShell process.
    #
    # This affects the installer process itself, but cannot modify the
    # environment of the parent PowerShell that launched this script.
    $CurrentPathEntries = @(
        $env:Path -split ';' |
        Where-Object { $_ -ne "" }
    )

    if ($CurrentPathEntries -notcontains $InstallBinDir) {
        $env:Path = "$InstallBinDir;$env:Path"
    }

    # -----------------------------------------------------------------------
    # Success
    # -----------------------------------------------------------------------

    Write-Host ""
    Write-Host "cute has been installed successfully."
    Write-Host "Location: $CutePath"
    Write-Host ""
    Write-Host "Open a new terminal for the PATH change to take effect."
    Write-Host ""
    Write-Host "To use cute immediately in this PowerShell session, run:"
    Write-Host '  $env:Path = [Environment]::GetEnvironmentVariable("Path", "User")'
}
finally {
    Cleanup
}

