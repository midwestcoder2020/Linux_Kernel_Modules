# Linux Kernel Modules

This repository contains a collection of custom Linux kernel modules. Each module is designed to extend or modify the functionality of the Linux kernel in various ways. These modules serve as examples and can be used as a starting point for creating your own modules.

## Table of Contents

- [Introduction](#introduction)
- [Modules](#modules)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Building the Modules](#building-the-modules)
  - [Loading and Unloading Modules](#loading-and-unloading-modules)
- [License](#license)

## Introduction

Kernel modules are pieces of code that can be dynamically loaded into the Linux kernel to add or extend functionality without requiring a kernel reboot. This repository includes modules that demonstrate various features and use cases, from basic "Hello World" examples to more complex modules that interact with kernel subsystems.

## Modules

- **memmod.c**: A driver that monitors ram usage and sends an alert to all users via the terminal 
- **memproc.ko**: A driver when provides the total and free ram users via the /proc/ interface.


## Getting Started

### Prerequisites

To build and use these kernel modules, you will need the following:

- A Linux system with kernel headers installed (specific to your kernel version).
- The `make` utility and a C compiler (`gcc`).

You can install the necessary tools with:

```bash
sudo apt-get install build-essential linux-headers-$(uname -r)
