# Transport Simulator (WIP)

This is a C++ project simulating a simple transport network (roads, intersections, vehicles).
It uses graph structures for routing and integrates RapidJSON for reading map and scenario data.

## Current status
- Graph class implemented
- Demo JSON datasets available
- RapidJSON integrated
- MapLoader skeleton prepared

## Build
\`\`\`bash
cmake -S . -B build
cmake --build build
\`\`\`

## Run (example)
\`\`\`bash
./build/transport_sim --map data/map_demo.json
\`\`\`

## License
This project is licensed under the MIT License – see [LICENSE](./LICENSE).

### Third-party libraries
- RapidJSON – MIT licensed. See \`libs/rapidjson/license.txt\`.
