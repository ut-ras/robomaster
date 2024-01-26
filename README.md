# Robomaster
Firmware for the Stampede RoboMaster team -- built on Taproot and modm

## Setup guide
### Linux
* Install [Podman](https://podman.io/docs/installation#installing-on-linux)
* Install the [ST-Link](https://github.com/stlink-org/stlink#installation) package for udev permissions (required for flashing code to the robots)
* Install [VSCode](https://code.visualstudio.com/Download) (strongly recommended for ease of setup) and the [Dev Containers](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers) extension
* Change Dev Containers: Docker Path to "podman" in VS Code settings
* Clone the repository `git clone --recurse-submodules https://github.com/ut-ras/robomaster`
* Open the `robomaster` folder in VS Code
* From the command palette (F1), run Dev Containers: Reopen in Container
* Done! When opening the project in the future, be sure to use the Dev Container

### Windows 10/11
* [Install WSL](https://learn.microsoft.com/en-us/windows/wsl/install) using `wsl --install`, and restart your PC
* Open the WSL terminal and run the following commands (make sure you are working inside the Ubuntu home directory rather than the Windows mount to avoid permission issues):
  ```
  sudo apt update
  sudo apt install podman stlink-tools
  git clone --recurse-submodules https://github.com/ut-ras/robomaster
  ```
* Install [USBIPD-WIN](https://learn.microsoft.com/en-us/windows/wsl/connect-usb#install-the-usbipd-win-project) (required for flashing code to the robots)
* Install [VSCode](https://code.visualstudio.com/Download) (strongly recommended for ease of setup) and open the IDE
* Install the [WSL](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-wsl) and [Dev Containers](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers) extensions
* Change Dev Containers: Docker Path to "podman" in VS Code settings
* From the command palette (F1), run WSL: Connect to WSL
* Open the `robomaster` folder
* From the command palette (F1), run Dev Containers: Reopen in Container
* Done! When opening the project in the future, be sure to use the Dev Container

## Building
To build the code, open the repository in its Dev Container and run
`scons build robot=<standard|hero|sentry>`.

Alternatively, use Run Build Task (Ctrl+Shift+B) in VS Code and select the robot type.

Note: Each robot has different hardware and requires variations in the code, so its important to ensure your final code builds on every robot.

## Flashing
To flash the code to robot hardware:
* Turn off power on the robot
* Connect an ST-Link to the robot and your computer
* If using Windows, [attach the USB device to WSL](https://learn.microsoft.com/en-us/windows/wsl/connect-usb#attach-a-usb-device)
* Flash the code using `scons run robot=<standard|hero|sentry>` or with Run Task in VS Code
* Once this is complete, the firmware is flashed and you can turn the robot back on

Note: Make sure you select the correct robot when flashing. Each robot has different hardware and flashing the wrong code can cause significant malfunctions and potential safety concerns.

## Powering on the robots
Safety notice:
These robots are big and powerful. When powering them on, and especially when testing new code, always be careful and assume they will malfunction.
The wheel motors are of special concern since the robot can spin out of control or drive into someone or something and cause damage.
Do not test the shooter at normal speeds unless in a controlled environment.
In the event that anything goes wrong, the controller acts as a kill switch and can be powered off to shut down the motors.

To power on a robot and ensure proper calibration:
* Point the turret directly towards the battery and down to minimum pitch
* Turn on the battery by pressing the button once, releasing, and then holding for a few seconds
* Flip the power switch on
* Wait for the power up sequence to complete
* Power on the controller

## Resources
* [Taproot wiki](https://gitlab.com/aruw/controls/taproot/-/wikis/home): It has lots of content and we strongly recommend you browse through it to get a sense of what's there
* [Taproot API reference](https://aruw.gitlab.io/controls/taproot/): In-depth information about all the components of the library
* [modm](https://modm.io/): Website and associated documentation
* [aruw-mcb](https://gitlab.com/aruw/controls/aruw-mcb): ARUW's full robot code project available for reference