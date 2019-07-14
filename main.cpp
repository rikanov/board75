
#include "basicui.h"
#include "view2d.h"

int main(int argc, char ** argv)
{
    Board board(5, 7);
    View2D teszt("BOARD", 450, 630, board);
    teszt.select();
    return 0;
    for (bool exit = false; !exit;)
    {
        std::cout << "\nDo you want to start the game ? (y / n / e) : " ;
        char Q ;
        std::cin >> Q;
        if ( Q == 'a' || Q == 'A' )
        {
            BasicUI teszt(/*size = */ 5, /*AI level = */ 8, /*start player ?*/ Q == 'A', /*auto steps ?*/true );
            teszt.start();            
        }
        else if ( Q != 'e' )
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
