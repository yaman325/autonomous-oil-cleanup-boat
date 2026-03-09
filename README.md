# Autonomous Oil Spill Cleanup Boat

## Overview
A fully autonomous trimaran-style robot designed to collect and filter oil spills from water surfaces. The boat uses GPS navigation to autonomously reach target areas, collect oil using a rotating absorbent cylinder mechanism, and return to the starting point.

## Design Features

### Hull Configuration
- **Dual collector hulls (red):** Front-mounted, house motors, ESP32 microcontroller, and the collection mechanism
- **Storage hull (blue):** Rear-mounted, houses the replaceable oil collection tank
- **Size:** ~80-100cm total length

### Oil Collection Mechanism
- **Absorbent cylinders:** Oil-absorbent plastic rollers that spin to collect oil from water surface
- **Mechanical extraction:** Fixed scissors/fork mechanism squeezes oil from cylinders as they rotate
- **Collection chute:** Oil flows down into the rear tank via gravity
- **Tank design:** Easily replaceable tanks for quick turnaround in field deployment

### Electronics & Navigation
- **Microcontroller:** ESP32
- **Navigation:** GPS-guided autonomous waypoint navigation
- **Motors:** Dual brushless motors for propulsion and collection mechanism

## Current Status
**Fully functional prototype** — tested in field conditions with successful oil collection and autonomous return.

## Known Issues & Redesign Plan
- **Wire management:** Loose connections during transport — need better soldering/crimping and cable routing
- **Structural durability:** Need reinforced connection points between hulls
- **Aesthetic improvement:** Redesigning overall hull shape for professional appearance and better hydrodynamics
- **Cable routing:** Adding internal channels to protect and organize wiring

## Next Steps
- Redesign in OnShape with improved structural integrity
- Implement cable management solutions
- Strengthen inter-hull connections
- Field testing with new design


## Author
Yaman — Robotics Engineer, Qatar

---

*Last updated: [current date]*
