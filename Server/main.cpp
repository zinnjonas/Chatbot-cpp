/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02.04.2015 20:33:24
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jonas Zinn (JsZ), Jonas.S.Zinn@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <cstdlib>
#include <Sockets/SocketHandler.h>
#include <Sockets/ListenSocket.h>

#include "DisplaySocket.h"

static bool quit = false;

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int main ( int argc, char *argv[] )
{
    SocketHandler h;
    ListenSocket<DisplaySocket> l(h);
    
    if( l.Bind(9001))
    {
        exit(-1);
    }

    h.Add(&l);
    h.Select(1, 0);
    while(!quit)
    {
        h.Select(1,0);
    }

    return EXIT_SUCCESS;
}
/* ----------  end of function main  ---------- */

