#include <inttypes.h>   /* uintmax_t */
#include <stdio.h>      /* printf */
#include <stdlib.h>     /* EXIT_SUCCESS */

typedef uintmax_t (^func)( uintmax_t ) ; 

typedef struct wrap {
   void (^_)( struct wrap , void (^)( func ) ) ;
} wrap;

typedef void (^callback)( func ) ;

void show( uintmax_t value ) {
   printf( "%ju" "\n" , value ) ;
}


int main( int argc , char *argv[] ) {


   void (^y)(wrap, callback) = ^ void ( wrap gen , callback cb ) {
         gen._(
               gen 
             , ^ ( func f ) {
                     cb( f ) ;
               }
         );
   };


   void (^factorial_improver)(wrap, callback) = ^ void ( wrap partial , callback cb ) {
         cb( ^ uintmax_t ( uintmax_t n ) {
                     return n == 0  ? 1 
                                    : ({
                                          __block uintmax_t rv = 1;
                                          partial._(  
                                                partial 
                                              , ^ ( func f ) {
                                                   rv = n * f( n - 1 );
                                                });
                                          rv;
                                       }) ;
         } );
    };

    __block func factorial;

    y(   (wrap) { ._ = ^ (  wrap partial , callback cb ) 
               {
                  factorial_improver( partial , cb );
               } 
         }   
       , ^ (func fx) {
               factorial = fx;
         }
    );

    show( factorial( 50 ) );

    return EXIT_SUCCESS;
}
