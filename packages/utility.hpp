/**
 * utility.hpp - 
 * @author: Jonathan Beard
 * @version: Sat Mar 21 22:12:15 2015
 * 
 * Copyright 2015 Jonathan Beard
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef _UTILITY_HPP_
#define _UTILITY_HPP_  1
#include <vector>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <cassert>
#include <utility>
#include <type_traits>
#include <cmath>

namespace raft
{

template < typename A, 
           typename B > using common_t = typename std::common_type< A, B >::type;

template < typename A, 
           typename B > using common_v_t = std::vector< common_t< A, B > >;

/** 
 * irange - simple utility that returns a vector of 
 * integers with the common type of A & B, useful for
 * some instances when a range is needed....however
 * don't be too tempted to use this over and over 
 * in a tight loop since the compiler will more than
 * likely create lots of copies.
 * @param   a - const A, start of range (inclusive)
 * @param   b - const B, end of range   (inclusive)
 * @param   delta - const std::size_t 
 * @return std::vector< T >, where T is common type of 
 *         A & B
 */
template < typename A,
           typename B,
           typename std::enable_if< std::is_arithmetic< A >::value >::type* = nullptr,
           typename std::enable_if< std::is_arithmetic< B >::value >::type* = nullptr > 
static
common_v_t< A, B >
range( const A a, 
       const B b, 
       const common_t< A, B > delta = 1 )
{
  if( a < b )
  {
     const auto cap( (b - a + delta) / delta );
     common_v_t< A, B > out( cap );
     auto index( 0 );
     for( common_t< A, B > i( a ); 
      std::islessequal( i, b + delta ); i += delta, index++ )
     {
        out[ index ] = i;
     }
     return( std::forward< common_v_t< A, B > >( out ) );
  }
  else
  {
     const auto cap( (a - b + delta ) / delta );
     common_v_t< A, B > out( cap );
     auto index( 0 );
     for( common_t< A, B > i( a ); 
      std::isgreaterequal( i, b - delta ); i -= delta, index++ )
     {
        out[ index ] = i;
     }
     return( std::forward< common_v_t< A, B > >( out ) );
  }
}
/** START recursive templates for sum **/
template < class RETTYPE, class... PORTS > struct sum_helper{};
/** dummy template to end the recursion **/
template < class RETTYPE > struct sum_helper< RETTYPE >
{
   constexpr static RETTYPE sum()
   {
      return( std::move< RETTYPE >( static_cast< RETTYPE >( 0 ) ) );
   }
};
/** helper struct, for recursive template **/
template < class RETTYPE, class PORT, class... PORTS >
   struct sum_helper< RETTYPE, PORT, PORTS... >
{
   static RETTYPE sum( PORT &&port, PORTS&&... ports )
   {
      RETTYPE val;
      port.pop( val );
      return(  std::move< RETTYPE >( 
                  val + sum_helper< RETTYPE, PORTS... >::sum( 
                     std::forward< PORTS >( ports )... ) ) );
   }
};
/**
 * sum - takes a set of ports in, pops the head off the FIFOs
 * and adds them, moving the value to the return which is
 * well the return.
 */
template < typename RETTYPE,
           class... PORTS,
        typename std::enable_if< std::is_arithmetic< RETTYPE >::value >::type* = nullptr 
           > static RETTYPE sum(  PORTS&&... ports )
{
   return( std::move< RETTYPE >( sum_helper< RETTYPE, PORTS... >::sum( 
               std::forward< PORTS >( ports )... ) ) ); 
};

/** START recursive templates for mult **/
template < class RETTYPE, class... PORTS > struct mult_helper{};
/** dummy template to end the recursion **/
template < class RETTYPE > struct mult_helper< RETTYPE >
{
   constexpr static RETTYPE mult()
   {
      return( std::move< RETTYPE >( static_cast< RETTYPE >( 1 ) ) );
   }
};
/** helper struct, for recursive template **/
template < class RETTYPE, class PORT, class... PORTS >
   struct mult_helper< RETTYPE, PORT, PORTS... >
{
   static RETTYPE mult( PORT &&port, PORTS&&... ports )
   {
      RETTYPE val;
      port.pop( val );
      return(  std::move< RETTYPE >( 
                  val * mult_helper< RETTYPE, PORTS... >::mult( 
                     std::forward< PORTS >( ports )... ) ) );
   }
};
/**
 * mult - takes a set of ports in, pops the head off the FIFOs
 * and adds them, moving the value to the return which is
 * well the return.
 */
template < typename RETTYPE,
           class... PORTS,
        typename std::enable_if< std::is_arithmetic< RETTYPE >::value >::type* = nullptr 
           > static RETTYPE mult(  PORTS&&... ports )
{
   return( std::move< RETTYPE >( mult_helper< RETTYPE, PORTS... >::mult( 
            std::forward< PORTS >( ports )... ) ) ); 
};
}
#endif /* END _UTILITY_HPP_ */