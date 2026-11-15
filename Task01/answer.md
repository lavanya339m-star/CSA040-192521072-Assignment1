# TASK 1 – Evaluate the Requirements of Each Computing Environment

## 1.1 The Six Environments and Their Requirements

A smart hospital contains six different computing environments. Each environment has different operating-system requirements based on response time, reliability, throughput, battery life and security.

### Ranked Requirement Table
(Rank 1 = Most Important)

| Environment | Rank 1 | Rank 2 | Rank 3 | Rank 4 | Rank 5 |
|---|---|---|---|---|---|
| Bedside patient monitor | Response time (alarm within 10 ms) | Reliability | Security | Battery / power | Throughput |
| Nurse tablet | Battery (one full shift) | Security | Reliability | Response time (soft, ~100 ms) | Throughput |
| Doctor workstation | Security | Response time | Reliability | Throughput | Battery (mains powered) |
| Central database server | Reliability | Security / integrity | Throughput | Response time | Battery (UPS only) |
| Imaging (MRI) server | Throughput | Reliability | Security | Response time | Battery |
| Partner-hospital link | Security | Reliability | Throughput | Response time | Battery |

### Justification of Each Ranking

- **Bedside monitor:** The alarm must sound within 10 ms of an abnormal reading. Reliability is essential because a monitor failure can prevent an alarm.
- **Nurse tablet:** Battery life is important because nurses use tablets for 8–12 hours. Security is also important because the device may be lost or stolen.
- **Doctor workstation:** Security is the highest priority because many doctors access confidential patient records. Interactive response time is also important.
- **Database server:** Reliability is the highest priority because patient records must always be available. Security and integrity protect sensitive medical information.
- **Imaging server:** Throughput is important because MRI processing involves large files and CPU-intensive operations.
- **Partner-hospital link:** Security is essential because patient records are transferred through an external network. Authentication and confidentiality must be maintained.

## 1.2 Conflicting Requirements

| Conflict | Where It Appears | How It Is Resolved |
|---|---|---|
| Fast response vs many users on one machine | Time-sharing database/workstation vs monitor | Give the monitor its own small real-time OS with priority scheduling. |
| High throughput vs low response time | Batch-style MRI processing vs interactive viewing | Separate the imaging server from interactive workstations. |
| Long battery life vs fast response | Nurse tablet | Use an event-driven mobile OS and accept soft real-time response. |
| Security vs speed | Encryption and access checks | Use hardware encryption and apply checks when opening files. |
| Availability vs updating | Database server maintenance | Use a cluster with fail-over so one node serves while the other is updated. |
| Many features vs small size | Monitor vs database server | Use different operating systems for different environments. |

## 1.3 Requirements Critical to Patient Safety

The following requirements are directly related to patient safety:

1. The bedside monitor must meet the 10 ms alarm deadline (hard real-time).
2. The database must provide continuous availability and data integrity.
3. Patient records must be isolated so one patient's information cannot be accessed through another patient's session.
4. Monitors must continue alarming locally even if the network or central server is unavailable.

Imaging-server throughput and tablet battery life are important for efficiency, but they are not directly life-critical.

## 1.4 Lessons from the History of Operating Systems

| Generation | Key Idea | Application in the Hospital |
|---|---|---|
| Batch systems | Jobs are queued and executed one after another without interaction. | MRI reconstruction and nightly backup jobs. |
| Time-sharing / multi-user | CPU switches quickly between multiple users with protection. | Database servers and doctor workstations. |
| Personal / desktop systems | Designed for individual users with an interactive interface. | Doctor workstations. |
| Mobile / handheld | Touch interface, power management and application sandboxing. | Nurse tablets. |
| Real-time / embedded | Guaranteed response within a deadline with a small footprint. | Bedside monitors and home monitors. |

## 1.5 Alternative Concept and Complexity Analysis

### One General-Purpose OS Everywhere

The IT team's proposal is to use one general-purpose operating system, such as Linux or Windows, on every device, including monitors, tablets, workstations and servers.

| Criterion | One General-Purpose OS Everywhere | Different OS for Each Environment |
|---|---|---|
| Worst-case alarm response | No guaranteed deadline; background tasks may delay alarms. | RTOS uses fixed-priority scheduling for bounded response. |
| Scheduler time | O(log n) for n runnable tasks in Linux CFS. | O(1) using a priority bitmap in a suitable RTOS. |
| Memory per monitor | Hundreds of MB to 1–2 GB RAM and several GB storage may be required. | FreeRTOS kernel may require about 6–12 KB flash and a few KB RAM. |
| Administration | One image and one patching system, but updates may affect safety devices. | Multiple OS families require separate procedures and skills. |
| Tablet battery | Background services can reduce battery life. | Mobile OS aggressively manages power and idle applications. |

### Verdict

The single-OS approach offers simpler administration, but it cannot guarantee the alarm deadline or continuous database availability. Therefore, it is rejected for bedside monitors and the database server.

Different operating systems should be selected according to the requirements of each hospital environment. A common Linux base can still be used where suitable, such as workstations and imaging servers.

**GitHub Evidence:** This is a reasoning-based task and does not require a program. Task10 measurements can support the operating-system performance discussion.
