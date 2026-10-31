// nlp.hpp - Stage 1-3: input cleaning, tokenisation, ASCII -> binary, hashing
#pragma once
#include <bitset>
#include <cctype>
#include <cstdint>
#include <sstream>
#include <string>
#include <vector>

namespace nt {

constexpr std::size_t MAX_WORDS = 10;

inline std::string lower(std::string s) {
    for (auto& c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return s;
}

// Keep only letters, digits and whitespace (alphanumeric input rule).
inline std::string sanitize(const std::string& in) {
    std::string out;
    for (unsigned char c : in)
        if (std::isalnum(c) || std::isspace(c)) out += static_cast<char>(c);
    return out;
}

// Whitespace tokenizer. Returns every token; caller applies the 10-word cap.
inline std::vector<std::string> tokenize(const std::string& clean) {
    std::istringstream is(clean);
    std::vector<std::string> t;
    for (std::string w; is >> w;) t.push_back(w);
    return t;
}

// 8-bit binary string of one ASCII character, e.g. 'R' -> "01010010".
inline std::string toBinary(unsigned char c) { return std::bitset<8>(c).to_string(); }

// djb2 string hash (case-insensitive). Used by the hash map stage.
inline std::uint32_t djb2(const std::string& w) {
    std::uint32_t h = 5381;
    for (unsigned char c : lower(w)) h = h * 33u + c;
    return h;
}

}  // namespace nt
