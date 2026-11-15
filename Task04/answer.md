# TASK 4 – Evaluate the Services an Operating System Must Provide

## 4.1 Service Table for Each Environment

Key: Yes = full service, Min = reduced/minimal version, No = removed.

| OS Service | Monitor | Tablet | Workstation | Database Server | Imaging Server |
|---|---|---|---|---|---|
| User interface | No (small fixed display, no shell) | Yes (touch GUI) | Yes (GUI) | Min (remote admin shell) | Min (remote admin) |
| Program execution | Min (fixed set of tasks) | Yes (apps) | Yes | Yes | Yes (batch jobs) |
| I/O operations | Yes (sensor, display, buzzer) | Yes (touch, wireless, scanner) | Yes | Yes (disk, network) | Yes (DMA, scanner) |
| File system | Min (tiny flash log) | Yes (encrypted) | Yes | Yes (journaling) | Yes (large volumes) |
| Communication | Min (light network stack to server) | Yes (Wi-Fi) | Yes | Yes (TCP/IP, cluster heartbeat) | Yes |
| Error detection | Yes (sensor fault, watchdog) | Yes | Yes | Yes (ECC, RAID, journal) | Yes |
| Resource allocation | Min (fixed priorities) | Yes | Yes | Yes | Yes (CPU, memory) |
| Accounting / audit | No | Min | Yes | Yes (audit of record access) | Yes (job accounting) |
| Protection and security | Min (memory protection, signed firmware) | Yes (encryption, remote wipe) | Yes (login, permissions) | Yes (strongest) | Yes |

## 4.2 Services Removed from the Bedside Monitor

The following services are removed or reduced:

1. **Graphical user interface and command shell:** Nobody logs in to a monitor. Removing them saves memory and reduces the attack surface.
2. **Multi-user accounts and accounting:** The monitor performs one fixed function, so multiple user accounts are unnecessary.
3. **General-purpose file system, printing and most network services:** A small log in flash and a lightweight communication protocol are sufficient.
4. **Virtual memory and paging:** Swapping to disk would cause unpredictable delays. All code and data remain in RAM.

**Result:** Smaller code, faster start-up, predictable timing and fewer bugs.

## 4.3 Services Essential for the Database Server

The following services are essential:

- **File system with journaling:** A power cut must not corrupt patient records.
- **Protection and security:** Authentication, permissions, encryption and audit trail protect confidential data.
- **Resource allocation:** Fair CPU, memory and I/O sharing among many queries.
- **Error detection and recovery:** ECC memory, disk mirroring and log replay help recover from failures.
- **Communication:** Network services and cluster heartbeat enable fail-over.
- **Accounting:** Records who read or changed which patient record and when. This is a legal requirement.

## 4.4 How the OS Protects One Patient's Records from Another User's Programs

| Mechanism | How It Protects |
|---|---|
| Authentication | Every user and program proves identity using a password, smart card or token before access. |
| File permissions / access-control lists | The file system checks the user ID against record permissions at open() time. Read and write access is allowed only to the treating team. |
| Memory protection (base/limit or paging) | One process cannot read another process's memory. Hardware raises a trap when a violation occurs. |
| User/kernel mode | Programs must use system calls. Only the kernel directly accesses disks and other processes' data. |
| Encryption at rest and in transit | Even stolen disks or captured network packets are unreadable without the encryption keys. |
| Audit log | Every access is recorded so misuse can be traced. |

## 4.5 Alternative Concept and Complexity Analysis: Full Desktop OS on the Tablets

### Alternative Concept

The alternative is to install a full desktop operating system on nurse tablets with all services enabled instead of using a reduced handheld OS.

| Criterion | Full Desktop OS, All Services Enabled | Reduced Handheld OS |
|---|---|---|
| Memory and storage | Typically 4 GB RAM and 20–30 GB disk or more for the OS and services. Space O(s) for s services enabled. | Typically 1–2 GB RAM and a few GB storage; only needed services. |
| Start-up time | Tens of seconds to over a minute because every service starts. Time O(s). | Seconds; apps are resumed from saved state. |
| Battery life | Background services, indexing and updates keep CPU and radios active: perhaps 4–6 hours. | Aggressive sleep and background limits: a full 10–12 hour shift. |
| Security surface | Large: many services and open ports. | Smaller: sandboxed apps and enforced updates. |

**Note:** The hour and GB figures are typical planning ranges. If measurements are required, compare boot time and idle power on a laptop versus a phone and record your own numbers.

### Verdict

A full desktop OS fails the tablet's top-ranked requirement: battery life. Therefore, the handheld OS is retained.

**GitHub Evidence:** Reasoning-only task. Evidence consists of the service tables and security explanation.
