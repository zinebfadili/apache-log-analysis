#include "App.h"

int main(int argc, char const *argv[])
{
    App app; // allocation sur la pile
    const int status = app.Run(argc, argv);
    return status;
}
