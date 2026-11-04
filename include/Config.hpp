#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <filesystem>

namespace Config
{
    inline const std::filesystem::path DATA_DIR =
        std::filesystem::path(std::getenv("XDG_DATA_HOME") ?
            std::getenv("XDG_DATA_HOME") : std::getenv("HOME"))
        / (std::getenv("XDG_DATA_HOME") ? "grades" : ".local/share/grades");
}

#endif
