
#ifndef AUTOBOOST_MPL_MAP_AUX_KEY_TYPE_IMPL_HPP_INCLUDED
#define AUTOBOOST_MPL_MAP_AUX_KEY_TYPE_IMPL_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2003-2004
// Copyright David Abrahams 2003-2004
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id$
// $Date$
// $Revision$

#include <autoboost/mpl/key_type_fwd.hpp>
#include <autoboost/mpl/pair.hpp>
#include <autoboost/mpl/map/aux_/tag.hpp>

namespace autoboost {
namespace mpl {

template<>
struct key_type_impl< aux::map_tag >
{
    template< typename Map, typename T > struct apply
        : first<T>
    {
    };
};

}}

#endif // AUTOBOOST_MPL_MAP_AUX_KEY_TYPE_IMPL_HPP_INCLUDED
