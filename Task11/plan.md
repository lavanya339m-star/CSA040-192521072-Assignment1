# TASK 11 – Recommend and Defend the Operating-System Plan

## 11.1 Recommended Operating-System Plan

| Environment | OS Type and Example | Structure | Services | Protection | I/O Method |
|---|---|---|---|---|---|
| Bedside Monitor | Hard real-time OS (FreeRTOS or QNX) | Microkernel | Execution, I/O, error detection and light networking | MPU protection and signed firmware | Interrupt-driven I/O and DMA |
| Nurse Tablet | Android or iOS | Layered OS | GUI, applications, Wi-Fi and encrypted storage | App sandbox, device encryption and login | Interrupt-driven and DMA |
| Doctor Workstation | Linux or Windows desktop | Modular or hybrid | Full operating-system services | User accounts, file permissions and full-disk encryption | Interrupt-driven and DMA |
| Database Server | Linux clustered server | Modular monolithic | File system, protection, accounting and clustering | Authentication, ACLs, encryption and firewall | DMA |
| Imaging Server | Linux SMP server | Modular monolithic | Batch scheduling and large file system | Role-based access and network isolation | DMA |
| Partner Link | Hardened Linux gateway | Modular or hybrid | Communication and security | TLS/VPN, mutual authentication and audit | DMA network cards |

## 11.2 Evidence Behind the Plan

1. **Bedside monitors:** Task 8 demonstrated that polling can introduce delays, while interrupt-style handling responds more quickly. Therefore, interrupt-driven I/O and a real-time operating system are recommended.

2. **Database server:** Clustering improves availability. Task 10 demonstrated that larger buffers reduce system-call overhead. Task 9 showed that RAM caching improves database lookup performance.

3. **Imaging server:** Task 9 demonstrated that DMA significantly reduces the number of CPU interrupts during large image transfers. SMP allows multiple processors to work simultaneously.

4. **Nurse tablets:** A lightweight operating system with fewer unnecessary services helps reduce resource consumption and supports battery life.

5. **Processes and threads:** Task 10 showed that creating threads was faster than creating processes in our experiment. Threads are useful when tasks need shared memory and lower creation overhead.

## 11.3 Limitations and Risks

| Risk | Effect | Mitigation |
|---|---|---|
| Multiple OS families | Increased training and maintenance costs | Use Linux as a common platform for servers and workstations |
| Lack of real-time OS expertise | Important deadlines may be missed | Use a certified RTOS, timing analysis and watchdog timers |
| Cluster split-brain | Both nodes may become active and corrupt data | Use quorum and fencing mechanisms |
| Storage or network failure | Services may become unavailable | Use mirrored storage and redundant networks |
| Delayed security updates | Devices may remain vulnerable | Use managed updates and staged rollouts |
| Measurements from one machine | Results may differ on hospital hardware | Repeat measurements on actual target hardware |

## 11.4 Suitability for Three Hospitals

The proposed design can support three hospitals with some modifications.

The number of devices increases to approximately 1,500 monitors, 360 tablets and 240 workstations.

The monitor, tablet and workstation designs can remain the same. However, centralised device management is required.

The database system should be modified into a replicated distributed database. Each hospital should maintain a local database cluster for fast access and availability. The hospitals should replicate their records through encrypted network connections.

Partner-link gateways should provide secure communication between hospitals.

Therefore, the original design can be retained with improvements to the database and centralised management systems.

## 11.5 Alternative Concept: Everything in Virtual Machines on Two Large Servers

In this alternative design, a hypervisor runs multiple virtual machines on two large physical servers. Each virtual machine provides services such as database management, imaging, virtual desktops and monitor back-end services.

### Comparison

| Criterion | Virtual Machines on Two Servers | Separate Physical Machines |
|---|---|---|
| Response Time | Hypervisor scheduling may introduce delays and jitter | Direct hardware provides more predictable performance |
| Memory and CPU | Each VM requires separate guest OS resources | Less duplicated OS memory |
| Reliability | Failure of one host can affect multiple VMs | Failure may affect only one service |
| Administration | Easier provisioning, backups and migration | More machines to maintain |
| Cost | Fewer physical servers | More hardware and space required |

### Final Decision

Virtualisation is suitable for doctor workstations, imaging services and testing systems.

However, safety-critical bedside monitors should continue running directly on physical devices with a real-time operating system. Their alarm decisions must not depend on virtual-machine scheduling.

The database should remain in a clustered configuration with nodes on separate physical hosts.

Therefore, the recommended approach is a combination of physical machines for safety-critical services and virtualisation for non-critical services.
