#include "Application.hpp"
#include "iostream"

int main(int argc, char **argv)
{
    Application app(900, 500, "Physically Based Rendering");
    int regime = 0;
    if(argc>1){
        regime = argv[1][0]-48;
    }
    std::cout << "Regime = " << regime << std::endl;
    switch(regime) {
        case 1: app.Start();
        case 0: app.RunTest();
    }
    app.~Application();
    return 0;
}
