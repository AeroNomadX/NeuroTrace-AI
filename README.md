# NeuroTrace AI
An Intelligent Algorithm Visualization and Analysis System

    neurotrace-ai/
    ├── CMakeLists.txt
    ├── backend/
    │   ├── src/
    │   │   ├── nlp.hpp         sanitize, tokenize, ASCII -> binary, djb2 hash
    │   │   ├── graph.hpp       26-node knowledge graph + Dijkstra
    │   │   ├── knowledge.hpp   knowledge base, synonyms, inheritance categories
    │   │   ├── align.hpp       Levenshtein alignment matrix
    │   │   ├── json.hpp        JSON string escaping
    │   │   ├── analyzer.hpp    full pipeline -> JSON
    │   │   ├── server.cpp      HTTP server (/api/analyze, /api/health, static files)
    │   │   └── cli.cpp         terminal version
    │   └── third_party/httplib.h   (you add this single file)
    └── frontend/
        ├── index.html
        ├── style.css
        └── app.js

## Run
1. Download `httplib.h` from https://github.com/yhirose/cpp-httplib and put it in `backend/third_party/`.
2. Build (from the project root):

       g++ -std=c++17 -O2 backend/src/server.cpp -Ibackend/src -Ibackend/third_party -pthread -o neurotrace_server
       # Windows (MinGW): add  -lws2_32

   or: `cmake -B build && cmake --build build`
3. Start from the project root (so `./frontend` is found): `./neurotrace_server`
4. Open http://localhost:8080 - the header badge turns green: "C++ BACKEND - ONLINE".

Terminal only (no httplib needed):

    g++ -std=c++17 -O2 backend/src/cli.cpp -Ibackend/src -o neurotrace_cli
    ./neurotrace_cli "Red color"

If the backend is offline the page still works, computing everything in the browser.
