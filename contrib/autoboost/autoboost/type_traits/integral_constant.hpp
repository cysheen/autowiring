//  (C) Copyright John Maddock 2005.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef AUTOBOOST_TYPE_TRAITS_INTEGRAL_CONSTANT_HPP
#define AUTOBOOST_TYPE_TRAITS_INTEGRAL_CONSTANT_HPP

#include <autoboost/config.hpp>
#include <autoboost/mpl/bool.hpp>
#include <autoboost/mpl/integral_c.hpp>

namespace autoboost{

#if defined(AUTOBOOST_NO_DEPENDENT_TYPES_IN_TEMPLATE_VALUE_PARAMETERS) || defined(__BORLANDC__)
template <class T, int val>
#else
template <class T, T val>
#endif
struct integral_constant : public mpl::integral_c<T, val>
{
   typedef integral_constant<T,val> type;
};

template<> struct integral_constant<bool,true> : public mpl::true_
{
   typedef integral_constant<bool,true> type;
};
template<> struct integral_constant<bool,false> : public mpl::false_
{
   typedef integral_constant<bool,false> type;
};

typedef integral_constant<bool,true> true_type;
typedef integral_constant<bool,false> false_type;

}

#endif
