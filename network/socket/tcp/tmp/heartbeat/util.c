/* include includes */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <strings.h>
#include <errno.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "skel.h"


char *program_name;
 
/* end includes */
/* error - print a diagnostic and optionally exit */
void error( int status, int err, char *fmt, ... )
{
        va_list ap;
 
        va_start( ap, fmt );
        fprintf( stderr, "%s: ", program_name );
        vfprintf( stderr, fmt, ap );
        va_end( ap );
        if ( err )
                fprintf( stderr, ": %s (%d)\n", strerror( err ), err );
        if ( status )
                EXIT( status );
}
 
/* set_address - fill in a sockaddr_in structure */
void set_address( char *hname, char *sname,
        struct sockaddr_in *sap, char *protocol )
{
        struct servent *sp;
        struct hostent *hp;
        char *endptr;
        short port;
        unsigned long int carmel_hook;
 
        bzero( sap, sizeof( *sap ) );
        sap->sin_family = AF_INET;
        if ( hname != NULL )
        {
                if ( (carmel_hook = inet_addr( hname )) == -1 )
                {
                        hp = gethostbyname( hname );
                        if ( hp == NULL )
                                error( 1, 0, "unknown host: %s\n", hname );
                        sap->sin_addr = *( struct in_addr * )hp->h_addr;
                }
                else {
                  sap->sin_addr.s_addr = carmel_hook;
                }
        }
        else
                sap->sin_addr.s_addr = htonl( INADDR_ANY );
        port = strtol( sname, &endptr, 0 );
/*@.bp*/
        if ( *endptr == '\0' )
                sap->sin_port = htons( port );
        else
        {
                sp = getservbyname( sname, protocol );
                if ( sp == NULL )
                        error( 1, 0, "unknown service: %s\n", sname );
                sap->sin_port = sp->s_port;
        }
}


