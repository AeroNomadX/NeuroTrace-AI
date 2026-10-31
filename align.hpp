// align.hpp - Stage 8: sequence alignment (Levenshtein edit-distance matrix)
#pragma once
#include <algorithm>
#include <cmath>
#include <string>
#include <vector>

namespace nt {

struct Alignment {
    std::vector<std::vector<int>> matrix;
    int distance = 0;
    int similarityPct = 0;
};

inline Alignment align(const std::string& a, const std::string& b) {
    const std::size_t m = a.size(), n = b.size();
    Alignment r;
    r.matrix.assign(m + 1, std::vector<int>(n + 1, 0));
    for (std::size_t i = 0; i <= m; ++i) r.matrix[i][0] = static_cast<int>(i);
    for (std::size_t j = 0; j <= n; ++j) r.matrix[0][j] = static_cast<int>(j);
    for (std::size_t i = 1; i <= m; ++i)
        for (std::size_t j = 1; j <= n; ++j)
            r.matrix[i][j] = std::min({r.matrix[i - 1][j] + 1, r.matrix[i][j - 1] + 1,
                                       r.matrix[i - 1][j - 1] + (a[i - 1] == b[j - 1] ? 0 : 1)});
    r.distance = r.matrix[m][n];
    std::size_t longest = std::max(m, n);
    r.similarityPct = longest ? static_cast<int>(std::lround((1.0 - double(r.distance) / longest) * 100)) : 100;
    return r;
}

}  // namespace nt
