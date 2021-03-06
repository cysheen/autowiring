
//  (C) Copyright Steve Cleary, Beman Dawes, Howard Hinnant & John Maddock 2000.
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef AUTOBOOST_TT_HAS_NOTHROW_COPY_HPP_INCLUDED
#define AUTOBOOST_TT_HAS_NOTHROW_COPY_HPP_INCLUDED

#include <autoboost/type_traits/has_trivial_copy.hpp>

// should be the last #include
#include <autoboost/type_traits/detail/bool_trait_def.hpp>

namespace autoboost {

namespace detail{

template <class T>
struct has_nothrow_copy_imp{
#ifdef AUTOBOOST_HAS_NOTHROW_COPY
   AUTOBOOST_STATIC_CONSTANT(bool, value = AUTOBOOST_HAS_NOTHROW_COPY(T));
#else
   AUTOBOOST_STATIC_CONSTANT(bool, value = ::autoboost::has_trivial_copy<T>::value);
#endif
};

}

AUTOBOOST_TT_AUX_BOOL_TRAIT_DEF1(has_nothrow_copy,T,::autoboost::detail::has_nothrow_copy_imp<T>::value)
AUTOBOOST_TT_AUX_BOOL_TRAIT_DEF1(has_nothrow_copy_constructor,T,::autoboost::detail::has_nothrow_copy_imp<T>::value)

AUTOBOOST_TT_AUX_BOOL_TRAIT_SPEC1(has_nothrow_copy,void,false)
#ifndef AUTOBOOST_NO_CV_VOID_SPECIALIZATIONS
AUTOBOOST_TT_AUX_BOOL_TRAIT_SPEC1(has_nothrow_copy,void const,false)
AUTOBOOST_TT_AUX_BOOL_TRAIT_SPEC1(has_nothrow_copy,void const volatile,false)
AUTOBOOST_TT_AUX_BOOL_TRAIT_SPEC1(has_nothrow_copy,void volatile,false)
#endif

AUTOBOOST_TT_AUX_BOOL_TRAIT_SPEC1(has_nothrow_copy_constructor,void,false)
#ifndef AUTOBOOST_NO_CV_VOID_SPECIALIZATIONS
AUTOBOOST_TT_AUX_BOOL_TRAIT_SPEC1(has_nothrow_copy_constructor,void const,false)
AUTOBOOST_TT_AUX_BOOL_TRAIT_SPEC1(has_nothrow_copy_constructor,void const volatile,false)
AUTOBOOST_TT_AUX_BOOL_TRAIT_SPEC1(has_nothrow_copy_constructor,void volatile,false)
#endif

} // namespace autoboost

#include <autoboost/type_traits/detail/bool_trait_undef.hpp>

#endif // AUTOBOOST_TT_HAS_NOTHROW_COPY_HPP_INCLUDED
