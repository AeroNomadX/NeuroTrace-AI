// server.cpp - HTTP backend for NeuroTrace AI
//   GET /api/health            -> {"status":"ok"}
//   GET /api/analyze?text=...  -> full pipeline result as JSON
//   GET /                      -> serves ./frontend (index.html, style.css, app.js)
// Needs the single header httplib.h (https://github.com/yhirose/cpp-httplib).
#include <iostream>
#include "httplib.h"
#include "analyzer.hpp"

int main() {
    httplib::Server svr;

    svr.set_mount_point("/", "./frontend");

    svr.Get("/api/health", [](const httplib::Request&, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content("{\"status\":\"ok\",\"engine\":\"NeuroTrace C++ core\"}", "application/json");
    });

    svr.Get("/api/analyze", [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        const std::string text = req.get_param_value("text");
        if (text.size() > 2000) {                                   // basic abuse guard
            res.status = 413;
            res.set_content("{\"error\":\"input too long\"}", "application/json");
            return;
        }
        res.set_content(nt::analyze(text), "application/json");
    });

    std::cout << "NeuroTrace AI running at http://localhost:8080\n";
    if (!svr.listen("0.0.0.0", 8080)) {
        std::cerr << "Could not start server (is port 8080 free?)\n";
        return 1;
    }
    return 0;
}
