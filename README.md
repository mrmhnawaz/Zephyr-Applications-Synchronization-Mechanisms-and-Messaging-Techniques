# Zephyr-Applications-Synchronization-Mechanisms-and-Messaging-Techniques
Discover Zephyr Applications, synchronization mechanisms and messaging techniques on Ubuntu. Application 1 manages random byte creation within a global data structure using mutexes for thread safety, and Application 2 experiments with messaging using Zephyr's queues, publishing bytes without global access, development on the native POSIX port

Zephyr RTOS is a lightweight, scalable real-time operating system tailored for resource-constrained embedded devices. It offers robust features like preemptive multitasking, hardware abstraction, and extensive device support, making it an excellent choice for Internet of Things (IoT) applications. With an open-source ethos and vibrant community, Zephyr empowers developers to innovate and build reliable IoT solutions.

Usage:
Zephyr RTOS finds extensive use across industries such as automotive, consumer electronics, healthcare, and industrial automation. It's well-suited for developing wearables, smart sensors, industrial controllers, and IoT gateways. With its compatibility with the Ubuntu operating system, developers can seamlessly set up their development environment on Ubuntu, leverage the native POSIX port for testing, and utilize familiar tools and workflows. This compatibility enhances productivity and accelerates the development cycle for embedded IoT projects on the Ubuntu platform.

# Requirements
Ubuntu operating system
Zephyr SDK
West tool
CMake
Ninja build system
QEMU (for emulating native POSIX port)

#### Setup Instruction

## Get Zephyr and install Python dependencies

Use apt to install Python venv package:
sudo apt install python3-venv

Create a new virtual environment:
python3 -m venv ~/zephyrproject/.venv

Activate the virtual environment:
source ~/zephyrproject/.venv/bin/activate

Install west:
pip install west

Get the Zephyr source code:
west init ~/zephyrproject
cd ~/zephyrproject
west update

Export a Zephyr CMake package. This allows CMake to automatically load boilerplate code required for building Zephyr applications.
west zephyr-export

Zephyr’s scripts/requirements.txt file declares additional Python dependencies. Install them with pip.
pip install -r ~/zephyrproject/zephyr/scripts/requirements.txt

## Install the Zephyr SDK

Download and verify the Zephyr SDK bundle
cd ~
wget https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v0.16.5-1/zephyr-sdk-0.16.5-1_linux-x86_64.tar.xz
wget -O - https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v0.16.5-1/sha256.sum | shasum --check --ignore-missing

Extract the Zephyr SDK bundle archive
tar xvf zephyr-sdk-0.16.5-1_linux-x86_64.tar.xz

Run the Zephyr SDK bundle setup script:
cd zephyr-sdk-0.16.5-1
./setup.sh

Follow the Zephyr Project's [Getting Started Guide](https://docs.zephyrproject.org/latest/getting_started/index.html) to set up your development environment.

#### Clone the Repository
git clone https://github.com/mrmhnawaz/Zephyr-Applications-Synchronization-Mechanisms-and-Messaging-Techniques.git
cd Zephyr-Applications-Synchronization-Mechanisms-and-Messaging-Techniques

## To write the C code, Create a New Application Directory:
mkdir -p my_app/src
cd my_app

Write the Application Code
## For Application 1 File {App_1_main.c File}
## For pseudo code_ App 1
## For Application 2 File {App_2_main.c File}
## For pseudo code_ App 2

touch src/main.c
nano src/main.c

## Create a prj.conf file in the my_app directory (not in src) and open it for editing.
touch prj.conf
nano prj.conf


##To use the native POSIX port of Zephyr
Install Zephyr SDK: Download and install the Zephyr SDK, which includes all the necessary tools and libraries to build and run Zephyr applications.

Set Up Zephyr Environment: Set up the Zephyr environment by initializing the Zephyr project and exporting environment variables. You can do this using the west tool, which is a command-line tool for working with Zephyr repositories.
west init ~/zephyrproject
cd ~/zephyrproject
west zephyr-export

Zephyr application code

Modify the prj.conf file to specify the build configuration for the native POSIX port. This includes setting the board to native_posix.
CONFIG_BOARD_NATIVE_POSIX=y

Build Application
west build -b native_posix

Run Application
west build -t run

Monitor Output 
The output of application will be displayed in the terminal
(Using the native POSIX port of Zephyr allows to develop and test Zephyr applications on your host machine without the need for physical hardware.)
