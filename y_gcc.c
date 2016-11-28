#include <inttypes.h>   /* uintmax_t */
#include <stdio.h>      /* printf */
#include <stdlib.h>     /* EXIT_SUCCESS */

typedef uintmax_t (*func)( uintmax_t ) ; 

typedef struct wrap {
   void (*_)( struct wrap , void (*)( func ) ) ;
} wrap;

typedef void (*callback)( func ) ;

void show( uintmax_t value ) {
   printf( "%ju" "\n" , value ) ;
}


int main( int argc , char *argv[] ) {

   void y( wrap gen , callback cb ) {
         gen._(
               gen 
             , ({
                     void _( func f ) {
                        cb( f ) ;
                     };
                     _;
               })
         );
   };


   void factorial_improver( wrap partial , callback cb ) {
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
   };

   void fifty( func fx ) {
      show( fx( 50 ) );
   };  

   y(    (wrap) { ._ = factorial_improver } 
       , fifty
   );

   return EXIT_SUCCESS;
}
