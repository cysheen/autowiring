//Copyright (c) 2006-2009 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef AB_UUID_6F463AC838DF11DDA3E6909F56D89593
#define AB_UUID_6F463AC838DF11DDA3E6909F56D89593
#if (__GNUC__*100+__GNUC_MINOR__>301) && !defined(AUTOBOOST_EXCEPTION_ENABLE_WARNINGS)
#pragma GCC system_header
#endif
#if defined(_MSC_VER) && !defined(AUTOBOOST_EXCEPTION_ENABLE_WARNINGS)
#pragma warning(push,1)
#endif

#include <autoboost/exception/detail/type_info.hpp>
#include <iomanip>
#include <ios>
#include <string>
#include <sstream>
#include <cstdlib>

namespace
autoboost
    {
    namespace
    exception_detail
        {
        template <class T>
        inline
        std::string
        object_hex_dump( T const & x, std::size_t max_size=16 )
            {
            std::ostringstream s;
            s << "type: " << type_name<T>() << ", size: " << sizeof(T) << ", dump: ";
            std::size_t n=sizeof(T)>max_size?max_size:sizeof(T);
            s.fill('0');
            s.width(2);
            unsigned char const * b=reinterpret_cast<unsigned char const *>(&x);
            s << std::setw(2) << std::hex << (unsigned int)*b;
            for( unsigned char const * e=b+n; ++b!=e; )
                s << " " << std::setw(2) << std::hex << (unsigned int)*b;
            return s.str();
            }
        }
    }

#if defined(_MSC_VER) && !defined(AUTOBOOST_EXCEPTION_ENABLE_WARNINGS)
#pragma warning(pop)
#endif
#endif
