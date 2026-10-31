// analyzer.hpp - runs the whole NeuroTrace pipeline and returns JSON
//   input -> sanitize -> tokenize -> ASCII/binary -> array -> hash map
//         -> graph search (Dijkstra) -> knowledge lookup -> output
#pragma once
#include <sstream>
#include <string>
#include "align.hpp"
#include "graph.hpp"
#include "json.hpp"
#include "knowledge.hpp"
#include "nlp.hpp"

namespace nt {

template <class T>
inline void writeArray(std::ostringstream& o, const std::vector<T>& v) {
    o << '[';
    for (std::size_t i = 0; i < v.size(); ++i) o << (i ? "," : "") << v[i];
    o << ']';
}

inline std::string analyze(const std::string& raw) {
    static const KnowledgeGraph graph;                 // built once, shared by all requests

    auto tokens = tokenize(sanitize(raw));
    const bool truncated = tokens.size() > MAX_WORDS;
    const std::size_t total = tokens.size();
    if (truncated) tokens.resize(MAX_WORDS);

    std::ostringstream o;
    o << "{\"input\":" << esc(raw) << ",\"charCount\":" << raw.size()
      << ",\"totalWords\":" << total << ",\"wordCount\":" << tokens.size()
      << ",\"truncated\":" << (truncated ? "true" : "false") << ",\"words\":[";

    for (std::size_t k = 0; k < tokens.size(); ++k) {
        const std::string& w = tokens[k];
        const std::uint32_t h = djb2(w);
        const int node = 1 + static_cast<int>(h % 25);        // never the START node
        const std::string key = canonical(w);
        const auto kb = knowledgeBase().find(key);
        const auto cat = categories().find(key);
        const std::string category = cat == categories().end() ? "Unknown" : cat->second;
        const SearchResult sr = graph.dijkstra(0, node);

        std::vector<int> ascii; std::vector<std::string> bin;
        for (unsigned char c : w) { ascii.push_back(c); bin.push_back(esc(toBinary(c))); }

        o << (k ? "," : "") << "{\"word\":" << esc(w) << ",\"ascii\":"; writeArray(o, ascii);
        o << ",\"binary\":"; writeArray(o, bin);
        o << ",\"hash\":" << h << ",\"node\":" << node
          << ",\"nodeLabel\":" << esc(std::string(1, KnowledgeGraph::label(node)))
          << ",\"category\":" << esc(category)
          << ",\"inherits\":" << esc(category + " > Concept")
          << ",\"attributes\":" << esc(attributes(category))
          << ",\"known\":" << (kb != knowledgeBase().end() ? "true" : "false") << ",\"facts\":[";
        if (kb != knowledgeBase().end())
            for (std::size_t i = 0; i < kb->second.size(); ++i) o << (i ? "," : "") << esc(kb->second[i]);
        o << "],\"path\":"; writeArray(o, sr.path);
        o << ",\"visited\":"; writeArray(o, sr.visitOrder);
        o << ",\"cost\":" << sr.cost << "}";
    }
    o << "],\"graph\":{\"nodes\":[";
    for (int i = 0; i < KnowledgeGraph::N; ++i)
        o << (i ? "," : "") << "{\"id\":" << i << ",\"label\":\"" << KnowledgeGraph::label(i)
          << "\",\"x\":" << graph.x[i] << ",\"y\":" << graph.y[i] << "}";
    o << "],\"edges\":[";
    for (std::size_t i = 0; i < graph.edges.size(); ++i)
        o << (i ? "," : "") << '[' << graph.edges[i].a << ',' << graph.edges[i].b << ',' << graph.edges[i].w << ']';
    o << "]}";

    if (tokens.size() >= 2) {                           // alignment matrix of first two words
        Alignment al = align(lower(tokens[0]), lower(tokens[1]));
        o << ",\"alignment\":{\"a\":" << esc(lower(tokens[0])) << ",\"b\":" << esc(lower(tokens[1]))
          << ",\"distance\":" << al.distance << ",\"similarity\":" << al.similarityPct << ",\"matrix\":[";
        for (std::size_t i = 0; i < al.matrix.size(); ++i) { o << (i ? "," : ""); writeArray(o, al.matrix[i]); }
        o << "]}";
    }
    o << '}';
    return o.str();
}

}  // namespace nt
