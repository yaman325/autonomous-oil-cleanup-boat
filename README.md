# Autonomous Oil Spill Cleanup Robot

## Overview
A fully autonomous trimaran-style robot designed to collect and filter oil spills from water surfaces using mechanical absorption and extraction. The robot autonomously navigates to target oil spills using GPS waypoint programming, collects oil using a rotating PTEG plastic cylinder mechanism with mechanical extraction, and returns to the starting point with collected oil stored in a 5-liter tank.

**Status:** Fully functional prototype tested in controlled pool environment with real oil collection.

---

## Design Architecture

### Hull Configuration
- **Dual collector hulls (front):** House propulsion systems, collection mechanism, and electronics
- **Storage hull (rear):** Houses the 5-liter replaceable oil collection tank
- **Overall dimensions:** ~80-100cm length
- **Material:** 3D printed (ABS/PLA)

### Propulsion System
- **Type:** Dual brushless underwater thrusters (BlueRobotics ESC-controlled)
- **Quantity:** 2 thrusters (one per front hull)
- **Control:** Independent ESC control for differential thrust (turning, forward/reverse)
- **Navigation:** GPS waypoint programming for autonomous route execution

### Oil Collection Mechanism
**Core Innovation:** Mechanical oil absorption and extraction system

- **Absorbent Cylinders:** PTEG plastic cylinders that rotate to collect oil from water surface
- **Rotation Method:** Single DC motor (1,450 RPM) drives cylinder rotation
- **Extraction System:** Fixed scissor mechanism applies pressure to cylinders, squeezing absorbed oil into collection chute
- **Collection Chute:** Gravity-fed direct flow into rear storage tank
- **Tank Design:** 5-liter capacity, easily replaceable for field operations

**Design Choice:** PTEG plastic chosen for high oil absorption rate without requiring active pumping or power-intensive filtration.

---

## Electronics & Control System

### Microcontroller
- **Primary:** ESP32 microcontroller
- **Function:** Central control hub for all systems

### Motor Control
- **DC Motor Driver:** Controls 1,450 RPM DC motor (cylinder rotation)
- **ESCs (Electronic Speed Controllers):** Control dual brushless thrusters via PWM signals

### Sensors & Perception
1. **GPS Module:** Autonomous waypoint navigation and return-to-home functionality
2. **LiDAR Sensor:** Obstacle detection (mounted high to detect ships/large objects); avoidance logic triggers directional changes
3. **Float Sensor:** Tank level detection — alerts when 5-liter tank reaches capacity
4. **ESP32-AI Camera Module:** Real-time monitoring of robot status and water conditions
5. **Blynk App Integration:** Remote monitoring and manual override capability for troubleshooting

### Communication
- **Blynk Mobile App:** Custom programmed interface for:
  - Real-time robot status monitoring
  - Manual control override
  - Emergency stop functionality
  - Sensor data telemetry

---

## Performance & Testing

### Development Timeline
- **Duration:** 5 months from concept to functional prototype
- **Testing Environment:** Custom-built pool with real oil (school regulations required controlled environment)
- **Test Iterations:** Multiple cycles with progressive refinements

### Validated Capabilities
✅ Autonomous GPS waypoint navigation  
✅ Successful oil collection and mechanical extraction  
✅ Tank fill detection and status reporting  
✅ Obstacle avoidance via LiDAR  
✅ Return-to-home functionality  
✅ Remote monitoring via Blynk app

---

## Known Limitations & Redesign Roadmap

### Current Issues
1. **Scissor Mechanism Efficiency:** Current design requires spring reinforcement (added after initial testing) to achieve adequate pressure on cylinders; not optimal
2. **Extraction Precision:** 3D printing quality limitations affected scissor tolerance; redesign needed for consistent performance
3. **Hydrodynamic Drag:** Current hull shape creates unnecessary water resistance
4. **Aesthetic Integration:** Electronics and wiring exposed; needs streamlined enclosure
5. **Scalability:** Single-point failure in scissor mechanism; needs redundancy

### Planned Improvements (v2.0)
- **Better Scissor Design:** Redesign mechanism for consistent oil extraction without spring workaround
- **Improved 3D Printing:** Use higher-precision printer for tighter tolerances
- **Streamlined Hull:** Reduce water resistance for faster deployment to oil spills
- **Integrated Cable Management:** Internal routing to protect wires and improve aesthetics
- **AI Vision Enhancement:** Train ESP32-AI camera to detect and classify oil vs. water
- **Modular Design:** Make collection mechanism swappable for different oil viscosities

---

## Technical Specifications

### Power & Performance
- **Primary Motor:** 1,450 RPM DC motor (cylinder drive)
- **Thrusters:** Brushless underwater thrusters (exact thrust specs from BlueRobotics)
- **Battery:** [To be specified]
- **Runtime:** [To be specified]

### Dimensions & Weight
- **Length:** ~80-100cm
- **Width:** ~60cm (dual hull span)
- **Tank Capacity:** 5 liters
- **Weight:** [To be specified]

### Materials
- **Hulls:** 3D printed plastic
- **Absorbent Medium:** PTEG plastic cylinders
- **Collection Tank:** [Material — plastic/aluminum]
- **Frame:** [Material]

---

## Code & Implementation

### Programming Language
- **Primary:** Arduino IDE (ESP32-compatible)
- **Libraries:** 
  - GPS waypoint navigation
  - Blynk IoT integration
  - ESC PWM control
  - Sensor data aggregation

### Key Algorithms
- Waypoint-to-waypoint autonomous navigation
- LiDAR-based obstacle avoidance (directional correction)
- Tank level monitoring with threshold alerts
- Failsafe return-to-home on battery low

---

## Competition Context
**Original Competition:** Environmental Protection Challenge  
**Inspiration:** Large-scale water filtration systems  
**Innovation:** Scaled-down autonomous deployment of passive mechanical filtration

---

## Future Applications
- Real-time oil spill response in controlled waterways
- Industrial wastewater cleanup
- Aquaculture system maintenance
- River/marina pollution monitoring

---

## Author
**Yaman** — Robotics Engineer & Designer  
Khalifa Secondary School, Qatar  
[GitHub: your-username]  
[Contact: your-email]

---

*Last Updated: [Date]*  
*Repository Status: Active Development — v2.0 Redesign in Progress*
