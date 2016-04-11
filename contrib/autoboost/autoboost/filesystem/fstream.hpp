//  autoboost/filesystem/fstream.hpp  ------------------------------------------------------//

//  Copyright Beman Dawes 2002

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  Library home page: http://www.boost.org/libs/filesystem

//--------------------------------------------------------------------------------------// 

#ifndef AUTOBOOST_FILESYSTEM3_FSTREAM_HPP
#define AUTOBOOST_FILESYSTEM3_FSTREAM_HPP

#include <autoboost/config.hpp>

# if defined( AUTOBOOST_NO_STD_WSTRING )
#   error Configuration not supported: Boost.Filesystem V3 and later requires std::wstring support
# endif

#include <autoboost/filesystem/path.hpp>
#include <iosfwd>
#include <fstream>

#include <autoboost/config/abi_prefix.hpp> // must be the last #include

//  on Windows, except for standard libaries known to have wchar_t overloads for
//  file stream I/O, use path::string() to get a narrow character c_str()
#if defined(AUTOBOOST_WINDOWS_API) \
  && (!defined(_CPPLIB_VER) || _CPPLIB_VER < 405 || defined(_STLPORT_VERSION))
  // !Dinkumware || early Dinkumware || STLPort masquerading as Dinkumware
# define AUTOBOOST_FILESYSTEM_C_STR string().c_str()  // use narrow, since wide not available
#else  // use the native c_str, which will be narrow on POSIX, wide on Windows
# define AUTOBOOST_FILESYSTEM_C_STR c_str()
#endif

namespace autoboost
{
namespace filesystem
{

//--------------------------------------------------------------------------------------//
//                                  basic_filebuf                                       //
//--------------------------------------------------------------------------------------//

  template < class charT, class traits = std::char_traits<charT> >
  class basic_filebuf : public std::basic_filebuf<charT,traits>
  {
  private: // disallow copying
    basic_filebuf(const basic_filebuf&);
    const basic_filebuf& operator=(const basic_filebuf&);

  public:
    basic_filebuf() {}
    virtual ~basic_filebuf() {}

    basic_filebuf<charT,traits>*
      open(const path& p, std::ios_base::openmode mode) 
    {
      return std::basic_filebuf<charT,traits>::open(p.AUTOBOOST_FILESYSTEM_C_STR, mode)
        ? this : 0;
    }
  };

//--------------------------------------------------------------------------------------//
//                                 basic_ifstream                                       //
//--------------------------------------------------------------------------------------//

  template < class charT, class traits = std::char_traits<charT> >
  class basic_ifstream : public std::basic_ifstream<charT,traits>
  {
  private: // disallow copying
    basic_ifstream(const basic_ifstream&);
    const basic_ifstream& operator=(const basic_ifstream&);

  public:
    basic_ifstream() {}

    // use two signatures, rather than one signature with default second
    // argument, to workaround VC++ 7.1 bug (ID VSWhidbey 38416)

    explicit basic_ifstream(const path& p)
      : std::basic_ifstream<charT,traits>(p.AUTOBOOST_FILESYSTEM_C_STR, std::ios_base::in) {}

    basic_ifstream(const path& p, std::ios_base::openmode mode)
      : std::basic_ifstream<charT,traits>(p.AUTOBOOST_FILESYSTEM_C_STR, mode) {}

    void open(const path& p)
      { std::basic_ifstream<charT,traits>::open(p.AUTOBOOST_FILESYSTEM_C_STR, std::ios_base::in); }

    void open(const path& p, std::ios_base::openmode mode)
      { std::basic_ifstream<charT,traits>::open(p.AUTOBOOST_FILESYSTEM_C_STR, mode); }

    virtual ~basic_ifstream() {}
  };

//--------------------------------------------------------------------------------------//
//                                 basic_ofstream                                       //
//--------------------------------------------------------------------------------------//

  template < class charT, class traits = std::char_traits<charT> >
  class basic_ofstream : public std::basic_ofstream<charT,traits>
  {
  private: // disallow copying
    basic_ofstream(const basic_ofstream&);
    const basic_ofstream& operator=(const basic_ofstream&);

  public:
    basic_ofstream() {}

    // use two signatures, rather than one signature with default second
    // argument, to workaround VC++ 7.1 bug (ID VSWhidbey 38416)

    explicit basic_ofstream(const path& p)
      : std::basic_ofstream<charT,traits>(p.AUTOBOOST_FILESYSTEM_C_STR, std::ios_base::out) {}

    basic_ofstream(const path& p, std::ios_base::openmode mode)
      : std::basic_ofstream<charT,traits>(p.AUTOBOOST_FILESYSTEM_C_STR, mode) {}

    void open(const path& p)
      { std::basic_ofstream<charT,traits>::open(p.AUTOBOOST_FILESYSTEM_C_STR, std::ios_base::out); }

    void open(const path& p, std::ios_base::openmode mode)
      { std::basic_ofstream<charT,traits>::open(p.AUTOBOOST_FILESYSTEM_C_STR, mode); }

    virtual ~basic_ofstream() {}
  };

//--------------------------------------------------------------------------------------//
//                                  basic_fstream                                       //
//--------------------------------------------------------------------------------------//

  template < class charT, class traits = std::char_traits<charT> >
  class basic_fstream : public std::basic_fstream<charT,traits>
  {
  private: // disallow copying
    basic_fstream(const basic_fstream&);
    const basic_fstream & operator=(const basic_fstream&);

  public:
    basic_fstream() {}

    // use two signatures, rather than one signature with default second
    // argument, to workaround VC++ 7.1 bug (ID VSWhidbey 38416)

    explicit basic_fstream(const path& p)
      : std::basic_fstream<charT,traits>(p.AUTOBOOST_FILESYSTEM_C_STR,
          std::ios_base::in | std::ios_base::out) {}

    basic_fstream(const path& p, std::ios_base::openmode mode)
      : std::basic_fstream<charT,traits>(p.AUTOBOOST_FILESYSTEM_C_STR, mode) {}

    void open(const path& p)
      { std::basic_fstream<charT,traits>::open(p.AUTOBOOST_FILESYSTEM_C_STR,
          std::ios_base::in | std::ios_base::out); }

    void open(const path& p, std::ios_base::openmode mode)
      { std::basic_fstream<charT,traits>::open(p.AUTOBOOST_FILESYSTEM_C_STR, mode); }

    virtual ~basic_fstream() {}

  };

//--------------------------------------------------------------------------------------//
//                                    typedefs                                          //
//--------------------------------------------------------------------------------------//

  typedef basic_filebuf<char> filebuf;
  typedef basic_ifstream<char> ifstream;
  typedef basic_ofstream<char> ofstream;
  typedef basic_fstream<char> fstream;

  typedef basic_filebuf<wchar_t> wfilebuf;
  typedef basic_ifstream<wchar_t> wifstream;
  typedef basic_ofstream<wchar_t> wofstream;
  typedef basic_fstream<wchar_t> wfstream;
  
} // namespace filesystem
} // namespace autoboost

#include <autoboost/config/abi_suffix.hpp> // pops abi_prefix.hpp pragmas
#endif  // AUTOBOOST_FILESYSTEM3_FSTREAM_HPP
