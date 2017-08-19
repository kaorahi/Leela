#ifndef MATCHER_H_INCLUDED
#define MATCHER_H_INCLUDED

#include <array>
#include <vector>

class Matcher {
public:
    Matcher();

    auto matches(int color, int pattern) const {
        return m_patterns[color][pattern];
    };

    /*
        return the "global" matcher
    */
    static Matcher* get_Matcher(void);
    static void set_Matcher(Matcher * m);

private:
    static Matcher* s_matcher;

    std::array<std::vector<unsigned short>, 2> m_patterns;
};

#endif
