from SCons.Script import *


CMD_LINE_ARGS                       = 1
TEST_BUILD_TARGET_ACCEPTED_ARGS     = ["build-tests", "run-tests", "run-tests-gcov"]
SIM_BUILD_TARGET_ACCEPTED_ARGS      = ["build-sim", "run-sim"]
HARDWARE_BUILD_TARGET_ACCEPTED_ARGS = ["build", "run", "size", "gdb", "all"]
VALID_BUILD_PROFILES                = ["debug", "release", "fast"]
VALID_PROFILING_TYPES               = ["true", "false"]
ROBOT_TYPE_DEFINES                  = {"standard": "TARGET_STANDARD",
                                       "hero": "TARGET_HERO",
                                       "sentry": "TARGET_SENTRY"}

USAGE = "Usage: scons <target> robot=<standard|hero|sentry> [profile=<debug|release|fast>] [profiling=<true|false>]\n\
    \"<target>\" is one of:\n\
        - \"build\": build all code for the hardware platform.\n\
        - \"run\": build all code for the hardware platform, and deploy it to the board via a connected ST-Link.\n\
        - \"build-tests\": build core code and tests for the current host platform.\n\
        - \"run-tests\": build core code and tests for the current host platform, and execute them locally with the test runner.\n\
        - \"run-tests-gcov\": builds core code and tests, executes them locally, and captures and prints code coverage information\n\
        - \"build-sim\": build all code for the simulated environment, for the current host platform.\n\
        - \"run-sim\": build all code for the simulated environment, for the current host platform, and execute the simulator locally."

def throw_error(msg):
    print("Error:", msg)
    print("See `scons help` for usage info.")
    exit(1)

def parse_args():
    args = {
        "TARGET_ENV": "",
        "BUILD_PROFILE": "",
        "PROFILING": "",
        "ROBOT_TYPE": "",
    }
    if len(COMMAND_LINE_TARGETS) > CMD_LINE_ARGS:
        throw_error("You entered too many arguments.")

    # Extract the target environment from the first command line argument
    # and determine modm build path as well as add any extra files to ignore
    if len(COMMAND_LINE_TARGETS) != 0:
        build_target = COMMAND_LINE_TARGETS[0]
        if build_target == "help":
            print(USAGE)
            exit(0)
        elif build_target in TEST_BUILD_TARGET_ACCEPTED_ARGS:
            args["TARGET_ENV"] = "tests"
        elif build_target in SIM_BUILD_TARGET_ACCEPTED_ARGS:
            args["TARGET_ENV"] = "sim"
        elif build_target in HARDWARE_BUILD_TARGET_ACCEPTED_ARGS:
            args["TARGET_ENV"] = "hardware"
        else:
            throw_error("You did not select a valid target.")
    else:
        throw_error("You must select a target.")

    # Extract and validate the robot type (required option)
    robot_type = ARGUMENTS.get("robot")
    if robot_type == None:
        throw_error("You must select a robot type.")
    elif robot_type not in ROBOT_TYPE_DEFINES:
        throw_error("You specified an invalid robot type.")
    args["ROBOT_TYPE"] = ROBOT_TYPE_DEFINES[robot_type]

    # Extract and validate the build profile (either debug or release)
    args["BUILD_PROFILE"] = ARGUMENTS.get("profile", "release")
    if args["BUILD_PROFILE"] not in VALID_BUILD_PROFILES:
        throw_error("You specified an invalid build profile.")

    args["PROFILING"] = ARGUMENTS.get("profiling", "false")
    if args["PROFILING"] not in VALID_PROFILING_TYPES:
        throw_error("You specified an invalid profiling type.")

    return args
