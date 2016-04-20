// Copyright David Abrahams 2006. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef AUTOBOOST_ARCHIVE_DETAIL_REGISTER_ARCHIVE_DWA2006521_HPP
# define AUTOBOOST_ARCHIVE_DETAIL_REGISTER_ARCHIVE_DWA2006521_HPP

namespace autoboost { namespace archive { namespace detail {

// No instantiate_ptr_serialization overloads generated by
// AUTOBOOST_SERIALIZATION_REGISTER_ARCHIVE that lexically follow the call
// will be seen *unless* they are in an associated namespace of one of
// the arguments, so we pass one of these along to make sure this
// namespace is considered.  See temp.dep.candidate (14.6.4.2) in the
// standard.
struct adl_tag {};

template <class Archive, class Serializable>
struct ptr_serialization_support;

// We could've just used ptr_serialization_support, above, but using
// it with only a forward declaration causes vc6/7 to complain about a
// missing instantiate member, even if it has one.  This is just a
// friendly layer of indirection.
template <class Archive, class Serializable>
struct _ptr_serialization_support
  : ptr_serialization_support<Archive,Serializable>
{
    typedef int type;
};

#ifdef __SUNPRO_CC

template<int N>
struct counter : counter<N-1> {};
template<>
struct counter<0> {};

template<class Serializable>
void instantiate_ptr_serialization(Serializable* s, int, adl_tag) {
    instantiate_ptr_serialization(s, counter<20>());
}

template<class Archive>
struct get_counter {
    static const int value = sizeof(adjust_counter(counter<20>()));
    typedef counter<value> type;
    typedef counter<value - 1> prior;
    typedef char (&next)[value+1];
};

char adjust_counter(counter<0>);
template<class Serializable>
void instantiate_ptr_serialization(Serializable*, counter<0>) {}

#define AUTOBOOST_SERIALIZATION_REGISTER_ARCHIVE(Archive)                     \
namespace autoboost { namespace archive { namespace detail {                  \
    get_counter<Archive >::next adjust_counter(get_counter<Archive >::type);\
    template<class Serializable>                                          \
    void instantiate_ptr_serialization(Serializable* s,                   \
        get_counter<Archive >::type) {                                    \
        ptr_serialization_support<Archive, Serializable> x;               \
        instantiate_ptr_serialization(s, get_counter<Archive >::prior()); \
    }\
}}}


#else

// This function gets called, but its only purpose is to participate
// in overload resolution with the functions declared by
// AUTOBOOST_SERIALIZATION_REGISTER_ARCHIVE, below.
template <class Serializable>
void instantiate_ptr_serialization(Serializable*, int, adl_tag ) {}

// The function declaration generated by this macro never actually
// gets called, but its return type gets instantiated, and that's
// enough to cause registration of serialization functions between
// Archive and any exported Serializable type.  See also:
// autoboost/serialization/export.hpp
# define AUTOBOOST_SERIALIZATION_REGISTER_ARCHIVE(Archive)                  \
namespace autoboost { namespace archive { namespace detail {                \
                                                                        \
template <class Serializable>                                           \
typename _ptr_serialization_support<Archive, Serializable>::type  \
instantiate_ptr_serialization( Serializable*, Archive*, adl_tag );              \
                                                                        \
}}}
#endif
}}} // namespace autoboost::archive::detail

#endif // AUTOBOOST_ARCHIVE_DETAIL_INSTANTIATE_SERIALIZE_DWA2006521_HPP
