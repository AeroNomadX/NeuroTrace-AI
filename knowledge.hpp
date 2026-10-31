// knowledge.hpp - Stage 7: knowledge base, synonyms and the inheritance hierarchy
#pragma once
#include <map>
#include <string>
#include <vector>
#include "nlp.hpp"

namespace nt {

using Facts = std::vector<std::string>;

inline const std::map<std::string, Facts>& knowledgeBase() {
    static const std::map<std::string, Facts> kb = {
        {"color", {"is how the eye reads wavelengths of light between about 380 and 750 nm", "is built from red, green and blue cone cells in the human retina"}},
        {"red", {"has the longest visible wavelength, about 620–750 nm", "is used in hospitals and by the Red Cross for emergency and medical signage", "signals danger and stop, as in traffic lights"}},
        {"blue", {"has a short wavelength, about 450–495 nm, which is why Rayleigh scattering makes the sky look blue", "is common in hospital scrubs and tech branding because it feels calm and trustworthy"}},
        {"green", {"sits mid-spectrum at about 495–570 nm, where human eyes are most sensitive", "is used for exit signs and operating-room scrubs because it reduces eye strain"}},
        {"yellow", {"has a wavelength of about 570–590 nm and is highly visible", "is used for caution signs and school buses"}},
        {"water", {"covers about 71% of Earth’s surface", "has the formula H2O and boils at 100 °C at sea level"}},
        {"sun", {"is a G-type star about 4.6 billion years old", "sends light that takes about 8 minutes to reach Earth"}},
        {"moon", {"orbits Earth about every 27.3 days", "drives the ocean tides through gravity"}},
        {"fire", {"needs fuel, heat and oxygen, known as the fire triangle", "is a chemical reaction called combustion"}},
        {"heart", {"beats about 60–100 times per minute in a resting adult", "pumps blood through roughly 100,000 km of vessels"}},
        {"light", {"travels about 299,792 km per second in vacuum", "behaves as both a wave and a particle"}},
        {"brain", {"contains about 86 billion neurons", "uses roughly 20% of the body’s energy"}},
        {"computer", {"stores everything in binary, as 0s and 1s", "processes data using arrays, hash maps, trees and graphs"}},
        {"tree", {"absorbs CO2 and releases oxygen through photosynthesis", "is also a hierarchical data structure of nodes"}},
        {"data", {"is stored as binary digits inside memory", "is organised with structures such as arrays and graphs"}}};
    return kb;
}

inline const std::map<std::string, std::string>& synonyms() {
    static const std::map<std::string, std::string> s = {
        {"colour", "color"}, {"colours", "color"}, {"colors", "color"},
        {"sunlight", "sun"}, {"ai", "computer"}, {"code", "computer"}};
    return s;
}

// Inheritance tree: Concept -> Category -> word
inline const std::map<std::string, std::string>& categories() {
    static const std::map<std::string, std::string> c = {
        {"color", "Color"}, {"red", "Color"}, {"blue", "Color"}, {"green", "Color"}, {"yellow", "Color"},
        {"water", "Nature"}, {"sun", "Nature"}, {"moon", "Nature"}, {"fire", "Nature"}, {"tree", "Nature"},
        {"light", "Physics"}, {"heart", "Body"}, {"brain", "Body"}, {"computer", "Tech"}, {"data", "Tech"}};
    return c;
}

inline std::string attributes(const std::string& cat) {
    static const std::map<std::string, std::string> a = {
        {"Color", "wavelength · perception"}, {"Nature", "natural element"},
        {"Physics", "measurable · energy"}, {"Body", "organ · biological"},
        {"Tech", "digital · binary"}, {"Unknown", "no knowledge entry"}};
    auto it = a.find(cat);
    return it == a.end() ? "" : it->second;
}

inline std::string canonical(const std::string& word) {
    std::string l = lower(word);
    auto it = synonyms().find(l);
    return it == synonyms().end() ? l : it->second;
}

}  // namespace nt
