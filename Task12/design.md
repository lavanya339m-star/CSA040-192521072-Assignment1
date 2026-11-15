# TASK 12 – Day-4 Design Challenge: Re-evaluate and Adapt the Design

## Changed Requirement

Remote monitoring devices in patients' homes send readings over the internet, and patient records move to a cloud service shared with the partner hospitals.

## A. Impact Analysis (2 Marks)

| Area | Effect of the Change | Impact |
|---|---|---|
| Monitor OS | Devices operate in patients' homes using home Wi-Fi or battery power, without technicians nearby. | Medium – retain RTOS and add secure boot and updates. |
| Response Time | Internet delays are unpredictable and may reach 20–200 ms. | High – alarms must be generated locally. |
| Database Server | Patient records move to a cloud service. | High – use a distributed database with local replicas. |
| Partner Link | Hospitals access shared cloud records. | High – introduce federated identity and access control. |
| Services | Encryption, device identity, remote updates and audit logs are required. | Medium-High |
| Security | Devices and patient records are exposed to internet-based threats. | Very High |
| Reliability | Home internet or cloud outages may interrupt communication. | High – implement store-and-forward functionality. |

## B. Evaluation of the Original Design (2 Marks)

| Criterion | Evaluation | Evidence |
|---|---|---|
| Response Time | The original design can support home monitoring if alarms are generated locally by the RTOS. Remote server decisions cannot guarantee a 10 ms deadline. | Task 8: Interrupt handling is faster than periodic polling. |
| Reliability | The hospital database cluster provides availability, but home internet and cloud services introduce additional failure points. | Task 2: Availability calculation. |
| Security | The original design assumed devices operated inside the hospital network. Home devices and cloud records require stronger security. | Task 4: Protection mechanisms. |
| Scalability | The same RTOS can be reused, but the central system must support thousands of home devices. | Task 11: Three-hospital scaling analysis. |

### Conclusion

The original device-level design using RTOS, interrupts and microkernel protection remains suitable. The clustered database and SMP imaging server also remain useful.

However, additional security mechanisms and distributed data management are required because devices now communicate over untrusted networks.

## C. Adaptation or Redesign (2 Marks)

### Decision: MODIFY

The original design will be retained with additional security, cloud and communication features.

### 1. Home Monitoring Device

- Use an embedded hard real-time OS such as FreeRTOS with MPU or QNX/Zephyr.
- Generate alarms locally within the 10 ms deadline.
- Use interrupt-driven sensor handling.
- Implement secure boot and signed over-the-air updates.
- Use TLS for secure communication.
- Store readings temporarily in flash memory when internet connectivity is unavailable.
- Use low-power sleep modes between samples.

### 2. Network Path

- Connect devices to hospital gateways using TLS.
- Use mutual certificate authentication.
- Use hardened Linux gateway servers.
- Use MQTT for device communication.
- Allow clinicians to receive remote readings with a target response time of seconds.

### 3. Cloud Records Service

- Use a distributed and replicated record store.
- Retain the hospital database cluster as a local replica or cache.
- Encrypt data at rest and in transit.
- Implement federated identity so users can access authorised records across all three hospitals.
- Apply per-patient access rules.
- Maintain complete audit logs.

### 4. Hospital Servers

- Retain the clustered database with fail-over.
- Retain the SMP imaging server with DMA.
- Use virtual machines or containers for non-critical cloud services.

### 5. Tablets and Workstations

- Retain their existing operating systems.
- Add centralised device management.
- Use VPN and zero-trust access to cloud services.

### Architecture Diagram

```text
+------------------------+
| Home Monitoring Device |
|------------------------|
| Hard RTOS              |
| Local Alarm < 10 ms    |
| Secure Boot            |
| TLS Communication      |
| Store-and-Forward      |
+-----------+------------+
            |
            | TLS / MQTT
            |
            v
+------------------------+
| Hospital Gateway       |
| Hardened Linux         |
| Device Authentication |
+-----------+------------+
            |
            | Secure Connection
            |
            v
+------------------------+
| Hospital Services      |
|------------------------|
| Database Cluster      |
| Local Cache / Replica |
| Imaging Server        |
+-----------+------------+
            |
            | TLS / VPN
            |
            v
+----------------------------+
| Shared Cloud Records       |
|----------------------------|
| Distributed Record Store  |
| Replication                |
| Encryption                 |
| Federated Identity         |
| Audit Logs                 |
+-------------+--------------+
              |
       +------+------+
       |             |
       v             v
+-------------+ +-------------+
| Hospital 2  | | Hospital 3  |
| Secure      | | Secure      |
| Cloud Access| | Cloud Access|
+-------------+ +-------------+
