# TASK 3 – Evaluate Operating-System Structures

## 3.1 The Structures Compared

| Structure | Idea | Reliability | Performance | Ease of Updating |
|---|---|---|---|---|
| Simple / Monolithic | Whole OS is one program in kernel mode; services call each other directly. | Low: a driver bug can crash everything. | High: function calls happen inside the kernel. | Hard: rebuild and reboot the kernel. |
| Layered | OS is split into layers; each layer uses only the layer below. | Good: errors are easier to isolate and debug. | Lower: requests may pass through many layers. | Medium: replace one layer if its interface is maintained. |
| Microkernel | Kernel handles scheduling, IPC and memory; drivers and file systems run as user-space servers. | Highest: a failed driver can be restarted. | Lower: message passing and extra context switches. | Easy: replace or restart a server without rebooting. |
| Modular | Small monolithic core with modules loaded or unloaded at run time. | Medium: modules still run in kernel mode. | High: direct calls once loaded. | Good: add a driver without rebooting. |
| Hybrid | Combines microkernel design ideas with monolithic kernel parts for speed. | Medium-high. | Medium-high. | Good. |

## 3.2 How Real Systems Are Structured

| System | Structure |
|---|---|
| Linux | Monolithic kernel with loadable kernel modules. Drivers, file systems and network stack run in kernel mode. Modules can be inserted using insmod or modprobe. |
| Windows (NT) | Hybrid. Kernel mode contains the Executive, microkernel, HAL and drivers. User mode has environment subsystems and services. |
| Android | Layered stack on top of Linux: Linux kernel, Hardware Abstraction Layer (HAL), native libraries, Android Runtime (ART), application framework and applications. |
| FreeRTOS | Tiny kernel library containing scheduler, queues, semaphores and timers, linked with the application in one address space. It can support an optional MPU port. |
| QNX | True microkernel RTOS. A small kernel communicates with user-space file-system, network and driver servers using message passing. A crashed driver can be restarted. |

## 3.3 Structure Chosen for the Monitors and Database Server

| Environment | Chosen Structure | Defence |
|---|---|---|
| Bedside monitor | Microkernel RTOS (QNX or FreeRTOS with memory protection) | A fault in the display or network driver must not stop the alarm task. The small kernel makes worst-case execution time easier to analyse. Message-passing cost is small and bounded for a few tasks. |
| Database server | Modular monolithic kernel (Linux with loadable modules) in a cluster | Provides high throughput for disk and network I/O because drivers are in the kernel. Modules allow storage and network driver updates without rebooting. Fail-over covers kernel faults. Windows Server hybrid is an acceptable alternative. |

## 3.4 Alternative Concept and Complexity Analysis: Monolithic Kernel on the Monitors

### Alternative Concept

The alternative is to run a monolithic kernel, such as embedded Linux, on the bedside monitor with all drivers and services inside the kernel.

**Monolithic structure:**

Application → System call → Kernel (driver called directly) → Return

Cost: 2 mode switches and 0 extra context switches.

**Microkernel structure:**

Application → IPC → Kernel → Driver server (user mode) → IPC → Kernel → Application

Cost: 4 mode switches, 2 context switches and message copies.

### Comparison

| Criterion | Monolithic Kernel on Monitor | Microkernel on Monitor |
|---|---|---|
| Time cost of a service request | One system call: about 0.1–0.3 microseconds for the mode switch. Time O(1). | Message to server and reply: roughly 1–5 microseconds. Time O(1), plus O(m) to copy an m-byte message. |
| Kernel memory | Tens of MB for a Linux kernel with drivers and file systems. | Tens to hundreds of KB for the microkernel; servers are loaded only when needed. |
| Faulty driver | Runs in kernel mode and can overwrite kernel data or crash the whole system. | Runs as a user process; the fault is isolated and the server can be restarted while the alarm task continues. |
| Worst-case time analysis | Harder because of long non-pre-emptible kernel paths. | Easier because the kernel is small and pre-emptible. |

### Verdict

The extra microseconds of the microkernel are far below the 10 ms alarm deadline. The improved fault isolation is more important than the small performance cost. Therefore, the microkernel is selected for bedside monitors.

For the database server, the opposite is true: millions of I/O requests per hour make the direct-call speed of a monolithic kernel more valuable, and fail-over covers faults.

**GitHub Evidence:** This is a reasoning-based task. The measured mode-switch cost can be supported by Task10/run_all.sh output.
