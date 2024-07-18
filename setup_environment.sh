#!/bin/bash

echo "Checking msp430 compiler..."

if ! command -v msp430-elf-gcc &> /dev/null
then 
    echo "Installing wget tar make packages..."

    sudo apt update 
    sudo apt upgrade -y
    sudo apt install wget tar make unzip -y

    echo "Installing msp430 compiler..."
    wget https://dr-download.ti.com/software-development/ide-configuration-compiler-or-debugger/MD-LlCjWuAbzH/9.3.1.2/msp430-gcc-9.3.1.11_linux64.tar.bz2
    tar -xjf msp430-gcc-9.3.1.11_linux64.tar.bz2
    sudo mv msp430-gcc-9.3.1.11_linux64 /opt/msp430-gcc
    echo "export PATH=\"/opt/msp430-gcc/bin:\$PATH\"" >> ~/.bashrc
    source ~/.bashrc 
    rm -rf msp430-gcc-9.3.1.11_linux64.tar.bz2

    echo "Installing header and support files..."
    wget https://dr-download.ti.com/software-development/ide-configuration-compiler-or-debugger/MD-LlCjWuAbzH/9.3.1.2/msp430-gcc-support-files-1.212.zip
    unzip msp430-gcc-support-files-1.212.zip
    sudo mv msp430-gcc-support-files/include/ /opt/msp430-gcc/include/msp
    rm -rf msp430-gcc-support-files-1.212* msp430-gcc-support-files

else 
    echo "msp430 compiler was installed !"
fi
