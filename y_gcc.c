#include <inttypes.h>   /* uintmax_t */
#include <stdio.h>      /* printf */
#include <stdlib.h>     /* EXIT_SUCCESS */

typedef uintmax_t (*func)( uintmax_t ) ; 

                              /* wrapping the function pointer type in a struct 
                                 is necessary because C does not support a 
                                 function pointer type that has its own type 
                                 as an argument */
typedef struct wrap {
   void (*_)( struct wrap , void (*)( func ) ) ;
} wrap;

typedef void (*callback)( func ) ;


static void y( wrap gen , callback cb ) {
   gen._(   gen 
          , ({
               void _( func f ) {
                  cb( f ) ;
               };
               _;
            })
   );
}


static void factorial_improver( wrap partial , callback cb ) {
   cb( ({ 
            uintmax_t _( uintmax_t n ) {
               return n == 0  ? 1 
                              : ({
                                    uintmax_t rv = 1;
                                    partial._(  
                                          partial 
                                        , ({
                                             void _( func f ) {
                                                rv = n * f( n - 1 );
                                             };
                                             _;
                                          })
                                    );
                                    rv;
                                 }) ;
            };
            _;
   }) );
}


static void show( uintmax_t value ) {
   printf( "%ju" "\n" , value ) ;
}


static void fifty( func factorial ) {
   show( factorial( 50 ) );
}


int main( int argc , char *argv[] ) {

   y(    (wrap) { ._ = factorial_improver } 
       , fifty
   );

   return EXIT_SUCCESS;
}
