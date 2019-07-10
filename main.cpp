
#include "basicui.h"

int main(int argc, char ** argv)
{
    for (bool exit = false; !exit;)
    {
        std::cout << "\nDo you want to start the game ? (y / n / e) : " ;
        char Q ;
        std::cin >> Q;
        if ( Q != 'e' )
        {
            BasicUI teszt(/*size = */ 5, /*AI level = */ 8, /*start player ?*/ Q == 'y' );
            teszt.start();
        }
        else
        {
            exit = true;
        }
    }
    return 0;
}
