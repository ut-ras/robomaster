# Robomaster
UT RoboMaster team - Main Branch
=======
[![pipeline status](https://gitlab.com/aruw/controls/taproot-template-project/badges/develop/pipeline.svg)](https://gitlab.com/aruw/controls/taproot-template-project/-/commits/develop)

## Resources

- **The [Taproot wiki](https://gitlab.com/aruw/controls/taproot/-/wikis/home). It has lots of content and we strongly recommend you browse through it to get a sense of
  what's there.**
- [aruw-edu](https://gitlab.com/aruw/controls/aruw-edu): a hands-on tutorial for building robot code with Taproot
- [aruw-mcb](https://gitlab.com/aruw/controls/aruw-mcb), ARUW's full robot code project available for reference
- The [generated API documentation for Taproot](https://aruw.gitlab.io/controls/taproot/)
- The [modm website](https://modm.io/) and associated documentation

## New user guide

### Setting up a development environment

If you want the easiest setup experience and **_do not_ require deploying code to hardware**,
consider developing within the provided [Docker container](https://gitlab.com/aruw/controls/taproot/-/wikis/Docker-Container-Setup).

Otherwise, follow the guide appropriate for your operating system.
- Linux
  - Debian: https://gitlab.com/aruw/controls/taproot/-/wikis/Debian-Linux-Setup
  - Fedora: https://gitlab.com/aruw/controls/taproot/-/wikis/Fedora-Linux-Setup
  - Other: follow one of the above guides, substituting your distribution's package names in place
    of Debian or Fedora packages.
- macOS: https://gitlab.com/aruw/controls/taproot/-/wikis/macOS-Setup
- Windows: https://gitlab.com/aruw/controls/taproot/-/wikis/Windows-Setup

Finally, install `pipenv` and set up the build tools:

```
pip3 install pipenv
cd ut-robomaster/
pipenv install
```

### Getting started with this repo

_Make sure you have followed the above setup instructions._

Run the following to clone this repository:

```
git clone --recursive https://github.com/ut-ras/robomaster.git
```

If you use the Docker container, or have already cloned the repository yourself, you should instead
run:

```
git submodule update --init --recursive
```

Now, `cd` into the project directory, activate the virtualenv, and run some builds:

```
cd robomaster/ut-robomaster
pipenv shell

# Build for hardware
scons build

# Run automated tests
scons run-tests
```

### Returning to the development environment

**You will need to run `pipenv shell` from this directory _every time_ you open a new terminal,
before using `scons` or `lbuild`.**

## Workflow guide

### Getting around VSCode

Microsoft provides a [helpful
website](https://code.visualstudio.com/docs/getstarted/tips-and-tricks) with a number of shortcuts
for getting around VSCode. There are many shortcuts that make programming faster.

### Building code and programming the RoboMaster Development Board

_If you would like to use the terminal instead, see the section "Building and running via the
terminal" below._

1. Make sure you have VSCode opened in the folder `taproot-template-project` (**not
   `template-project`**)
2. Connect an ST-Link to the RoboMaster Development Board and your computer.
3. In VSCode, open the Command Palette (<kbd>Ctrl</kbd>+<kbd>shift</kbd>+<kbd>P</kbd>)
4. Find `Tasks: Run Task`. You should see the options below. Select `Program - Debug` or `Program -
   Release`.<br><br>
    <img
    src=https://gitlab.com/aruw/controls/aruw-mcb/uploads/2ffb02c86387916c2c49ac3548151b38/image.png
    height="200px" />

### Debugging with an ST-Link

1. Open the folder `aruw-template-project` in VSCode. Hit the debug tab on the left side or type
   <kbd>Ctrl</kbd>+<kbd>shift</kbd>+<kbd>D</kbd>.
2. Hit the green play arrow on the left top of the screen.
3. See [this
   page](https://gitlab.com/aruw/controls/taproot/-/wikis/Software-Tools/Debugging-With-STLink) for
   more information about using the ST-Link for programming the MCB and debugging. <br>
   <img
   src=https://gitlab.com/aruw/controls/aruw-mcb/uploads/1f62ea310a20ee76092fe18de83d14a7/image.png
   height="400px" />

### Debugging with a J-Link

See the [wiki](https://gitlab.com/aruw/controls/taproot/-/wikis/Software-Tools/Debugging-With-JLink)
for an explanation on the difference between an ST-Link and J-Link and a step-by-step procedure on
how to use the J-Link.

### Selecting and using robot types

Specify the robot type via the command line when compiling (see
[below](#building-and-running-via-the-terminal)). For vscode IntelliSense, navigate to
`/template-project/robot-type/robot_type.hpp` and change the macro defined in this file.

Each robot is signified by a unique macro which can be checked to special-case code:

```c++
#if defined(TARGET_STANDARD)
// Only included if building for the Standard
initializeStandard();
#endif
```

### How to select an appropriate VSCode C/C++ configuration

This codebase has a number of different build targets (see [this wiki
page](https://gitlab.com/aruw/controls/taproot/-/wikis/Build-Targets-Overview) for more
information). Because the build setup is different for the test, sim, and RoboMaster Development
Board (aka MCB) environments, while working on a particular portion of code you may select an
appropriate profile that provides optimal
[intellisense](https://code.visualstudio.com/docs/editor/intellisense). To select a configuration,
in VSCode, type <kbd>Ctrl</kbd>+<kbd>Shift</kbd>+<kbd>P</kbd>, then type "C/C++:Select a
Configuration" and hit enter. A dropdown menu will appear where you may choose either the "Test",
"Sim", or "Hardware" configuration.

### Upgrading Taproot

The Taproot project recommends that user projects occasionally upgrade the version of
Taproot that they depend on. The guide for doing so is
[here](https://gitlab.com/aruw/controls/taproot/-/wikis/Upgrading-a-Taproot-project). 

## Building and running via the terminal

The below commands require that your working directory is `/template-project` (where the
`SConstruct` and `project.xml` files are).

- `lbuild build`: Re-generates out copy of `taproot` and `modm`.
- `scons build`: Builds the firmware image for the hardware target. Creates a "release" folder located in `build/hardware/` which contains the final `.elf` file as well as the intermediate object files (`.o`).
- `scons build-tests`: Builds a program which hosts our unit tests. This executable can be run on your host computer (only supported on Linux) and prints results for each unit test run.
- `scons run`: Builds as with `scons build` and then programs the board.
- `scons run-tests`: Builds and runs the unit test program.
- `scons size`: Prints statistics on program size and (statically-)allocated memory. Note that the reported available heap space is an upper bound, and this tool has no way of knowing about the real size of dynamic allocations.

Below is the full usage statement from our scons build environment. Note that you can select the
robot, profile, or whether or not you want profiling to be on using the various options.

```
Usage: scons <target> [profile=<debug|release|fast>] [robot=TARGET_<ROBOT_TYPE>] [profiling=<true|false>]
    "<target>" is one of:
        - "build": build all code for the hardware platform.
        - "run": build all code for the hardware platform, and deploy it to the board via a connected ST-Link.
        - "build-tests": build core code and tests for the current host platform.
        - "run-tests": build core code and tests for the current host platform, and execute them locally with the test runner.
        - "run-tests-gcov": builds core code and tests, executes them locally, and captures and prints code coverage information
        - "build-sim": build all code for the simulated environment, for the current host platform.
        - "run-sim": build all code for the simulated environment, for the current host platform, and execute the simulator locally.
    "TARGET_<ROBOT_TYPE>" is an optional argument that can override whatever robot type has been specified in robot_type.hpp.
        - <ROBOT_TYPE> must be one of the following:
            - STANDARD, DRONE, ENGINEER, SENTRY, HERO:
```
