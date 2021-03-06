// Copyright (C) 2013-2014 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// 2013/10 Vicente J. Botet Escriba
//   Creation using interprocess::unique_ptr.
// 2014/09 Vicente J. Botet Escriba
//   Adaptation to movelib::unique_ptr

#ifndef AUTOBOOST_CSBL_MEMORY_UNIQUE_PTR_HPP
#define AUTOBOOST_CSBL_MEMORY_UNIQUE_PTR_HPP

#include <autoboost/thread/csbl/memory/config.hpp>

#include <autoboost/move/unique_ptr.hpp>
#include <autoboost/move/make_unique.hpp>

namespace autoboost
{
  namespace csbl
  {
    using ::autoboost::movelib::unique_ptr;
    using ::autoboost::movelib::make_unique;

  }
}
#endif // header
