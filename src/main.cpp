#include <string>
#include "core/system.h"
#include "utils/logger.h"

int main(int argv, char **argc)
{
    if (argc[1] == NULL)
    {
        Logger::log("Please provide a rom file.");
        return 0;
    }

    System system;
    std::string rom_path;
    rom_path = argc[1];

    Logger::log("Loading cart from file: " + rom_path);
    system.load_cart_from_file(rom_path);

    Logger::log("Powering on...");
    system.power_on();
    return 0;
}
