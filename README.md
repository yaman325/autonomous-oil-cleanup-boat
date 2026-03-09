# Autonomous Oil Spill Cleanup Boat

## Overview
A fully autonomous trimaran robot designed to collect and filter oil spills from water surfaces. Developed over 5 months, the boat uses GPS-guided navigation to autonomously reach contaminated areas, mechanically collect oil using absorbent cylinder technology, and return to base station.

## Design Specifications

### Hull Configuration
- **Dual collector hulls (front):** House propulsion thrusters, DC motor control system, and oil collection mechanism
- **Storage hull (rear):** Contains 5-liter replaceable oil collection tank
- **Overall dimensions:** ~80-100cm length
- **Material:** 3D printed with hydrodynamic optimization for reduced water resistance

### Propulsion System
- **2x Brushless underwater thrusters:** PSQ Company model
- **Electronic Speed Controllers (ESCs):** Control thrust magnitude and direction
- **Motor control:** Synchronized dual-thruster system for forward, reverse, and turning maneuvers

### Oil Collection Mechanism
- **Absorbent cylinders:** PTEG plastic-based, rotating cylinders that attract and absorb oil from water surface
- **DC Motor:** 1,450 RPM dedicated motor for cylinder rotation
- **Mechanical extraction:** Spring-loaded scissor mechanism squeezes absorbed oil from cylinders into collection chute
- **Oil flow:** Direct gravity-feed into rear tank; float sensor indicates tank fullness

### Electronics & Navigation
- **Microcontroller:** ESP32 with programmed autonomous mission planning
- **Navigation:** GPS-guided waypoint system; boat returns to start point autonomously
- **Obstacle avoidance:** LiDAR sensor (above water) detects large obstacles (ships, structures)
- **Monitoring:** ESP32 AI camera for real-time robot status assessment
- **Mobile control:** Blynk app interface for emergency manual override and diagnostics

### Sensors
- **Float sensor:** Tank full/empty detection
- **LiDAR:** Obstacle detection (above-water only)
- **GPS:** Autonomous navigation and return
- **AI camera:** Live monitoring and situational awareness

## Development & Testing

### Project Timeline
- **Duration:** 5 months (design to first successful test)
- **Testing environment:** Custom-built test pool with real oil simulation
- **Success rate:** Oil collection mechanism functional from first deployment

### Testing Results
- Successfully collected simulated oil multiple times
- GPS autonomous navigation: Fully operational
- Obstacle avoidance: Functional for surface-level hazards
- Tank capacity: Tested and verified at 5 liters

## Current Status
**Fully functional prototype** — Ready for field deployment with planned improvements.

## Known Limitations & Upgrade Plan

### Current Limitations
1. **Oil detection AI:** No automated visual identification of oil vs. water; manual targeting required
2. **Scissor mechanism efficiency:** Spring-loaded design works but suboptimal; loses some collected oil during extraction
3. **Hydrodynamics:** Current design has higher water resistance than optimal; affects speed and efficiency
4. **Build quality:** 3D printing resolution affects precision of scissor mechanism
5. **Structural integration:** Individual components assembled rather than unified design; creates weak points and wire exposure

### Planned Improvements (Redesign Phase)
- **AI vision upgrade:** Train ESP32 camera to detect oil-water contrast automatically
- **Optimized scissor mechanism:** Redesign with better geometry and higher-grade 3D printing
- **Hydrodynamic hull redesign:** Streamlined, integrated hull structure to reduce drag and increase speed
- **Internal cable routing:** Built-in wire channels to eliminate exposed connections and improve durability
- **Unified structure:** Single-piece hull design to improve structural integrity and aesthetics
- **Advanced materials:** Consider hybrid 3D printing materials for strength and durability

## Files & Documentation
- `/photos` — Design photos, testing documentation, field trials
- `/code` — ESP32 firmware (Arduino-based), mission planning code, Blynk app configuration
- `/design` — OnShape CAD files (redesign in progress)
- `/specifications` — Detailed sensor and component specifications

## Author & Development
**Yaman** — Robotics Engineer, Qatar
- Solo redesign and optimization (2026)
- Original concept and team development (2025)

---

*Project status: Prototype complete, full redesign in progress*
