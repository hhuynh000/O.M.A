#include "app.hpp"

int main()
{
    ui::App app;
    if (!app.init("Optical Measurement Automation", 1152, 648))
    {
        return EXIT_FAILURE;
    }
    
    app.run();

    return EXIT_SUCCESS;
}