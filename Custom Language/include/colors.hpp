#ifndef COLORS_HPP
#define COLORS_HPP

// ASNI COLOR CODES

#define RESET "\033[0m"
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN "\033[1;36m"
#define WHITE "\033[1;37m"

// Bold colors
#define BOLD_RED "\033[1;91m"
#define BOLD_GREEN "\033[1;92m"
#define BOLD_YELLOW "\033[1;93m"
#define BOLD_BLUE "\033[1;94m"
#define BOLD_MAGENTA "\033[1;95m"
#define BOLD_CYAN "\033[1;96m"
#define BOLD_WHITE "\033[1;97m"

// Utility function to use colors in strings
inline std::string colorize(const std::string& text, const std::string& color) {
    return color + text + RESET;
}

#endif // COLORS_HPP