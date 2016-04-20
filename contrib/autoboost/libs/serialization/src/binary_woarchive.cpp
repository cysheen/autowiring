/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// binary_woarchive.cpp:

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com .
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include <autoboost/config.hpp>

#ifdef AUTOBOOST_NO_STD_WSTREAMBUF
#error "wide char i/o not supported on this platform"
#else

#define AUTOBOOST_WARCHIVE_SOURCE
#include <autoboost/archive/binary_woarchive.hpp>

// explicitly instantiate for this type of text stream
#include <autoboost/archive/impl/archive_serializer_map.ipp>
#include <autoboost/archive/impl/basic_binary_oprimitive.ipp>
#include <autoboost/archive/impl/basic_binary_oarchive.ipp>

namespace autoboost {
namespace archive {

template class detail::archive_serializer_map<binary_woarchive>;
template class basic_binary_oprimitive<
    binary_woarchive,
    wchar_t,
    std::char_traits<wchar_t>
>;
template class basic_binary_oarchive<binary_woarchive> ;
template class binary_oarchive_impl<
    binary_woarchive,
    wchar_t,
    std::char_traits<wchar_t>
>;

} // namespace archive
} // namespace autoboost

#endif // AUTOBOOST_NO_STD_WSTREAMBUF
