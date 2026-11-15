# TASK 2 – Judge the Type of Operating System for Each Environment

## 2.1 Comparison of Operating-System Types

| Type | Main Characteristic | Strength | Weakness for This Hospital |
|---|---|---|---|
| Mainframe | Large central machine, batch and transaction processing, high I/O | Extreme reliability and throughput | Very costly; overkill for 500 beds |
| Desktop | One interactive user with GUI | Good user experience and many applications | No timing guarantees; weak for servers |
| Multiprocessor (parallel) | Several CPUs share memory; SMP or AMP | High throughput for parallel jobs | Single machine is a single point of failure |
| Distributed | Many computers cooperate over a network | Resource sharing and scalability | Network delay and complex security |
| Clustered | Several nodes share storage, monitor each other and fail over | High availability | Extra cost and split-brain risk |
| Real-time (hard) | Deadline must always be met | Guaranteed response | Small feature set and more design effort |
| Real-time (soft) | Deadline is usually met; late results still have value | Flexible and supports richer features | No guarantee |
| Handheld / mobile | Small, battery-powered, touch-based and wireless | Low power and convenient | Limited memory and storage |

## 2.2 Chosen Type for Each Environment

| Environment | Chosen OS Type | Rejected Alternatives and Reason |
|---|---|---|
| Bedside monitor | Hard real-time OS (FreeRTOS or QNX) | Desktop and multi-user OS have no deadline guarantee. A late alarm is a safety failure. |
| Nurse tablet | Handheld / mobile OS (Android or iOS) | Desktop OS is heavy and has poor battery life. |
| Doctor workstation | Desktop, multi-user capable OS (Linux desktop or Windows) | Handheld OS is too small for imaging viewers; mainframe terminal is unnecessary. |
| Database server | Clustered server OS with fail-over (Linux / Windows Server, 2 or more nodes) | A single large multiprocessor is a single point of failure. Mainframe is costly. |
| Imaging server | Multiprocessor (SMP) server OS with batch-style job scheduling | AMP wastes CPUs and creates a master bottleneck. A cluster is needed only if the load outgrows one machine. |
| Partner link | Distributed system using secure gateway servers and encrypted links | Direct database access from outside creates a security risk. |

## 2.3 Symmetric versus Asymmetric Multiprocessing for the Imaging Server

| Aspect | Symmetric Multiprocessing (SMP) | Asymmetric Multiprocessing (AMP) |
|---|---|---|
| Who runs the OS | Every CPU runs the same kernel and can run any task. | One master CPU runs the OS and schedules tasks; slave CPUs run assigned work. |
| Load balance | Good: any idle CPU takes the next image slice. | Master can become the bottleneck. |
| Failure | Loss of one CPU reduces capacity only. | Loss of the master stops the system. |
| Complexity | Kernel needs locks for shared data. | Simple, with no sharing of kernel structures. |
| Choice | Chosen (Linux, Windows Server and others support SMP). | Rejected; suits dedicated or embedded designs. |

MRI processing splits an image into slices that can be reconstructed independently. SMP keeps all processors busy.

Speed-up is limited by Amdahl's law:

S(p) = 1 / ((1 - f) + f / p)

where:
- p = number of processors
- f = parallel fraction of the program

## 2.4 Clustered System with Fail-over for the Database Server

A clustered system uses two database nodes to provide high availability.

### Basic Architecture

Clients (workstations and tablets)

          |
    Virtual IP address
       /       \
   Node A     Node B
   ACTIVE     STANDBY
       \       /
    Shared / replicated
       storage

Both nodes send heartbeat messages every second. If the active node misses several heartbeats, the standby node takes the virtual IP address, mounts the storage, replays the database log and continues service, typically within seconds.

Updates are performed by patching the standby node first, switching over, and then patching the old active node. This avoids service outages during updates.

## 2.5 Alternative Concept and Complexity Analysis

### One Large Multiprocessor Instead of a Cluster

| Criterion | One Large Multiprocessor | Cluster with Fail-over |
|---|---|---|
| Availability | Single point of failure. At 99.9% availability, downtime is about 8.8 hours per year. | Both nodes must fail together. Ideal availability is 1 - (0.001)^2 = 99.9999%, about 32 seconds per year, plus fail-over time. |
| Cost | Large shared-memory machines become more expensive as processors increase. | Commodity nodes have nearly linear cost growth, with extra standby and storage cost. |
| Throughput as processors grow | Amdahl's law: for f = 0.95, 8 processors give 5.9x speed-up and 64 give 15.4x, with a limit of 20x. | Active/standby gives no throughput gain; its purpose is fail-over, not speed. |
| Memory | One shared address space stores data once: O(n). | Data is stored on both nodes: O(2n). |
| Updates | Restart is required, causing an outage. | Rolling updates can be performed without an outage. |

### Verdict

The database must never stop, so availability is more important than cost and raw processing speed. Therefore, the clustered system with fail-over is chosen.

A large multiprocessor is acceptable inside each cluster node if the workload requires it.

**GitHub Evidence:** This is a reasoning-based task and does not require a program.
