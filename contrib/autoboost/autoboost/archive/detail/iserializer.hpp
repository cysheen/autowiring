#ifndef AUTOBOOST_ARCHIVE_DETAIL_ISERIALIZER_HPP
#define AUTOBOOST_ARCHIVE_DETAIL_ISERIALIZER_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
# pragma once
#pragma inline_depth(511)
#pragma inline_recursion(on)
#endif

#if defined(__MWERKS__)
#pragma inline_depth(511)
#endif

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// iserializer.hpp: interface for serialization system.

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com .
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include <new>     // for placement new
#include <cstddef> // size_t, NULL

#include <autoboost/config.hpp>
#include <autoboost/detail/workaround.hpp>
#if defined(AUTOBOOST_NO_STDC_NAMESPACE)
namespace std{
    using ::size_t;
} // namespace std
#endif

#include <autoboost/static_assert.hpp>

#include <autoboost/mpl/eval_if.hpp>
#include <autoboost/mpl/identity.hpp>
#include <autoboost/mpl/greater_equal.hpp>
#include <autoboost/mpl/equal_to.hpp>
#include <autoboost/mpl/bool.hpp>
#include <autoboost/detail/no_exceptions_support.hpp>

#ifndef AUTOBOOST_SERIALIZATION_DEFAULT_TYPE_INFO
    #include <autoboost/serialization/extended_type_info_typeid.hpp>
#endif
#include <autoboost/serialization/throw_exception.hpp>
#include <autoboost/serialization/smart_cast.hpp>
#include <autoboost/serialization/static_warning.hpp>

#include <autoboost/type_traits/is_pointer.hpp>
#include <autoboost/type_traits/is_enum.hpp>
#include <autoboost/type_traits/is_const.hpp>
#include <autoboost/type_traits/remove_const.hpp>
#include <autoboost/type_traits/remove_extent.hpp>
#include <autoboost/type_traits/is_polymorphic.hpp>

#include <autoboost/serialization/assume_abstract.hpp>
#define DONT_USE_HAS_NEW_OPERATOR (                    \
    defined(__BORLANDC__)                              \
    || AUTOBOOST_WORKAROUND(__IBMCPP__, < 1210)            \
    || defined(__SUNPRO_CC) && (__SUNPRO_CC < 0x590)   \
)
#if ! DONT_USE_HAS_NEW_OPERATOR
#include <autoboost/type_traits/has_new_operator.hpp>
#endif

#include <autoboost/serialization/serialization.hpp>
#include <autoboost/serialization/version.hpp>
#include <autoboost/serialization/level.hpp>
#include <autoboost/serialization/tracking.hpp>
#include <autoboost/serialization/type_info_implementation.hpp>
#include <autoboost/serialization/nvp.hpp>
#include <autoboost/serialization/void_cast.hpp>
#include <autoboost/serialization/array.hpp>
#include <autoboost/serialization/collection_size_type.hpp>
#include <autoboost/serialization/singleton.hpp>
#include <autoboost/serialization/wrapper.hpp>

// the following is need only for dynamic cast of polymorphic pointers
#include <autoboost/archive/archive_exception.hpp>
#include <autoboost/archive/detail/basic_iarchive.hpp>
#include <autoboost/archive/detail/basic_iserializer.hpp>
#include <autoboost/archive/detail/basic_pointer_iserializer.hpp>
#include <autoboost/archive/detail/archive_serializer_map.hpp>
#include <autoboost/archive/detail/check.hpp>

namespace autoboost {

namespace serialization {
    class extended_type_info;
} // namespace serialization

namespace archive {

// an accessor to permit friend access to archives.  Needed because
// some compilers don't handle friend templates completely
class load_access {
public:
    template<class Archive, class T>
    static void load_primitive(Archive &ar, T &t){
        ar.load(t);
    }
};

namespace detail {

#ifdef AUTOBOOST_MSVC
#  pragma warning(push)
#  pragma warning(disable : 4511 4512)
#endif

template<class Archive, class T>
class iserializer : public basic_iserializer
{
private:
    virtual void destroy(/*const*/ void *address) const {
        autoboost::serialization::access::destroy(static_cast<T *>(address));
    }
protected:
    // protected constructor since it's always created by singleton
    explicit iserializer() :
        basic_iserializer(
            autoboost::serialization::singleton<
                typename
                autoboost::serialization::type_info_implementation< T >::type
            >::get_const_instance()
        )
    {}
public:
    virtual AUTOBOOST_DLLEXPORT void load_object_data(
        basic_iarchive & ar,
        void *x,
        const unsigned int file_version
    ) const AUTOBOOST_USED;
    virtual bool class_info() const {
        return autoboost::serialization::implementation_level< T >::value
            >= autoboost::serialization::object_class_info;
    }
    virtual bool tracking(const unsigned int /* flags */) const {
        return autoboost::serialization::tracking_level< T >::value
                == autoboost::serialization::track_always
            || ( autoboost::serialization::tracking_level< T >::value
                == autoboost::serialization::track_selectively
                && serialized_as_pointer());
    }
    virtual version_type version() const {
        return version_type(::autoboost::serialization::version< T >::value);
    }
    virtual bool is_polymorphic() const {
        return autoboost::is_polymorphic< T >::value;
    }
    virtual ~iserializer(){};
};

#ifdef AUTOBOOST_MSVC
#  pragma warning(pop)
#endif

template<class Archive, class T>
AUTOBOOST_DLLEXPORT void iserializer<Archive, T>::load_object_data(
    basic_iarchive & ar,
    void *x,
    const unsigned int file_version
) const {
    // note: we now comment this out. Before we permited archive
    // version # to be very large.  Now we don't.  To permit
    // readers of these old archives, we have to suppress this
    // code.  Perhaps in the future we might re-enable it but
    // permit its suppression with a runtime switch.
    #if 0
    // trap case where the program cannot handle the current version
    if(file_version > static_cast<const unsigned int>(version()))
        autoboost::serialization::throw_exception(
            archive::archive_exception(
                autoboost::archive::archive_exception::unsupported_class_version,
                get_debug_info()
            )
        );
    #endif
    // make sure call is routed through the higest interface that might
    // be specialized by the user.
    autoboost::serialization::serialize_adl(
        autoboost::serialization::smart_cast_reference<Archive &>(ar),
        * static_cast<T *>(x),
        file_version
    );
}

#ifdef AUTOBOOST_MSVC
#  pragma warning(push)
#  pragma warning(disable : 4511 4512)
#endif

// the purpose of this code is to allocate memory for an object
// without requiring the constructor to be called.  Presumably
// the allocated object will be subsequently initialized with
// "placement new".
// note: we have the autoboost type trait has_new_operator but we
// have no corresponding has_delete_operator.  So we presume
// that the former being true would imply that the a delete
// operator is also defined for the class T.

template<class T>
struct heap_allocation {
    // autoboost::has_new_operator< T > doesn't work on these compilers
    #if DONT_USE_HAS_NEW_OPERATOR
        // This doesn't handle operator new overload for class T
        static T * invoke_new(){
            return static_cast<T *>(operator new(sizeof(T)));
        }
        static void invoke_delete(T *t){
            (operator delete(t));
        }
    #else
        // note: we presume that a true value for has_new_operator
        // implies the existence of a class specific delete operator as well
        // as a class specific new operator.
        struct has_new_operator {
            static T * invoke_new() {
                return static_cast<T *>((T::operator new)(sizeof(T)));
            }
            static void invoke_delete(T * t) {
                // if compilation fails here, the likely cause that the class
                // T has a class specific new operator but no class specific
                // delete operator which matches the following signature.  Fix
                // your program to have this.  Note that adding operator delete
                // with only one parameter doesn't seem correct to me since
                // the standard(3.7.4.2) says "
                // "If a class T has a member deallocation function named
                // 'operator delete' with exactly one parameter, then that function
                // is a usual (non-placement) deallocation function" which I take
                // to mean that it will call the destructor of type T which we don't
                // want to do here.
                // Note: reliance upon automatic conversion from T * to void * here
                (T::operator delete)(t, sizeof(T));
            }
        };
        struct doesnt_have_new_operator {
            static T* invoke_new() {
                return static_cast<T *>(operator new(sizeof(T)));
            }
            static void invoke_delete(T * t) {
                // Note: I'm reliance upon automatic conversion from T * to void * here
                (operator delete)(t);
            }
        };
        static T * invoke_new() {
            typedef typename
                mpl::eval_if<
                    autoboost::has_new_operator< T >,
                    mpl::identity<has_new_operator >,
                    mpl::identity<doesnt_have_new_operator >
                >::type typex;
            return typex::invoke_new();
        }
        static void invoke_delete(T *t) {
            typedef typename
                mpl::eval_if<
                    autoboost::has_new_operator< T >,
                    mpl::identity<has_new_operator >,
                    mpl::identity<doesnt_have_new_operator >
                >::type typex;
            typex::invoke_delete(t);
        }
    #endif
    explicit heap_allocation(){
        m_p = invoke_new();
    }
    ~heap_allocation(){
        if (0 != m_p)
            invoke_delete(m_p);
    }
    T* get() const {
        return m_p;
    }

    T* release() {
        T* p = m_p;
        m_p = 0;
        return p;
    }
private:
    T* m_p;
};

template<class Archive, class T>
class pointer_iserializer :
    public basic_pointer_iserializer
{
private:
    virtual void * heap_allocation() const {
        detail::heap_allocation<T> h;
        T * t = h.get();
        h.release();
        return t;
    }
    virtual const basic_iserializer & get_basic_serializer() const {
        return autoboost::serialization::singleton<
            iserializer<Archive, T>
        >::get_const_instance();
    }
    AUTOBOOST_DLLEXPORT virtual void load_object_ptr(
        basic_iarchive & ar,
        void * x,
        const unsigned int file_version
    ) const AUTOBOOST_USED;
protected:
    // this should alway be a singleton so make the constructor protected
    pointer_iserializer();
    ~pointer_iserializer();
};

#ifdef AUTOBOOST_MSVC
#  pragma warning(pop)
#endif

// note: AUTOBOOST_DLLEXPORT is so that code for polymorphic class
// serialized only through base class won't get optimized out
template<class Archive, class T>
AUTOBOOST_DLLEXPORT void pointer_iserializer<Archive, T>::load_object_ptr(
    basic_iarchive & ar,
    void * t,
    const unsigned int file_version
) const
{
    Archive & ar_impl =
        autoboost::serialization::smart_cast_reference<Archive &>(ar);

    // note that the above will throw std::bad_alloc if the allocation
    // fails so we don't have to address this contingency here.

    // catch exception during load_construct_data so that we don't
    // automatically delete the t which is most likely not fully
    // constructed
    AUTOBOOST_TRY {
        // this addresses an obscure situation that occurs when
        // load_constructor de-serializes something through a pointer.
        ar.next_object_pointer(t);
        autoboost::serialization::load_construct_data_adl<Archive, T>(
            ar_impl,
            static_cast<T *>(t),
            file_version
        );
    }
    AUTOBOOST_CATCH(...){
        // if we get here the load_construct failed.  The heap_allocation
        // will be automatically deleted so we don't have to do anything
        // special here.
        AUTOBOOST_RETHROW;
    }
    AUTOBOOST_CATCH_END

    ar_impl >> autoboost::serialization::make_nvp(NULL, * static_cast<T *>(t));
}

template<class Archive, class T>
pointer_iserializer<Archive, T>::pointer_iserializer() :
    basic_pointer_iserializer(
        autoboost::serialization::singleton<
            typename
            autoboost::serialization::type_info_implementation< T >::type
        >::get_const_instance()
    )
{
    autoboost::serialization::singleton<
        iserializer<Archive, T>
    >::get_mutable_instance().set_bpis(this);
    archive_serializer_map<Archive>::insert(this);
}

template<class Archive, class T>
pointer_iserializer<Archive, T>::~pointer_iserializer(){
    archive_serializer_map<Archive>::erase(this);
}

template<class Archive>
struct load_non_pointer_type {
    // note this bounces the call right back to the archive
    // with no runtime overhead
    struct load_primitive {
        template<class T>
        static void invoke(Archive & ar, T & t){
            load_access::load_primitive(ar, t);
        }
    };
    // note this bounces the call right back to the archive
    // with no runtime overhead
    struct load_only {
        template<class T>
        static void invoke(Archive & ar, const T & t){
            // short cut to user's serializer
            // make sure call is routed through the higest interface that might
            // be specialized by the user.
            autoboost::serialization::serialize_adl(
                ar,
                const_cast<T &>(t),
                autoboost::serialization::version< T >::value
            );
        }
    };

    // note this save class information including version
    // and serialization level to the archive
    struct load_standard {
        template<class T>
        static void invoke(Archive &ar, const T & t){
            void * x = & const_cast<T &>(t);
            ar.load_object(
                x,
                autoboost::serialization::singleton<
                    iserializer<Archive, T>
                >::get_const_instance()
            );
        }
    };

    struct load_conditional {
        template<class T>
        static void invoke(Archive &ar, T &t){
            //if(0 == (ar.get_flags() & no_tracking))
                load_standard::invoke(ar, t);
            //else
            //    load_only::invoke(ar, t);
        }
    };

    template<class T>
    static void invoke(Archive & ar, T &t){
        typedef typename mpl::eval_if<
                // if its primitive
                mpl::equal_to<
                    autoboost::serialization::implementation_level< T >,
                    mpl::int_<autoboost::serialization::primitive_type>
                >,
                mpl::identity<load_primitive>,
            // else
            typename mpl::eval_if<
            // class info / version
            mpl::greater_equal<
                        autoboost::serialization::implementation_level< T >,
                        mpl::int_<autoboost::serialization::object_class_info>
                    >,
            // do standard load
            mpl::identity<load_standard>,
        // else
        typename mpl::eval_if<
            // no tracking
                    mpl::equal_to<
                        autoboost::serialization::tracking_level< T >,
                        mpl::int_<autoboost::serialization::track_never>
                >,
                // do a fast load
                mpl::identity<load_only>,
            // else
            // do a fast load only tracking is turned off
            mpl::identity<load_conditional>
        > > >::type typex;
        check_object_versioning< T >();
        check_object_level< T >();
        typex::invoke(ar, t);
    }
};

template<class Archive>
struct load_pointer_type {
    struct abstract
    {
        template<class T>
        static const basic_pointer_iserializer * register_type(Archive & /* ar */){
            // it has? to be polymorphic
            AUTOBOOST_STATIC_ASSERT(autoboost::is_polymorphic< T >::value);
            return static_cast<basic_pointer_iserializer *>(NULL);
         }
    };

    struct non_abstract
    {
        template<class T>
        static const basic_pointer_iserializer * register_type(Archive & ar){
            return ar.register_type(static_cast<T *>(NULL));
        }
    };

    template<class T>
    static const basic_pointer_iserializer * register_type(Archive &ar, const T & /*t*/){
        // there should never be any need to load an abstract polymorphic
        // class pointer.  Inhibiting code generation for this
        // permits abstract base classes to be used - note: exception
        // virtual serialize functions used for plug-ins
        typedef typename
            mpl::eval_if<
                autoboost::serialization::is_abstract<const T>,
                autoboost::mpl::identity<abstract>,
                autoboost::mpl::identity<non_abstract>
            >::type typex;
        return typex::template register_type< T >(ar);
    }

    template<class T>
    static T * pointer_tweak(
        const autoboost::serialization::extended_type_info & eti,
        void const * const t,
        const T &
    ) {
        // tweak the pointer back to the base class
        void * upcast = const_cast<void *>(
            autoboost::serialization::void_upcast(
                eti,
                autoboost::serialization::singleton<
                    typename
                    autoboost::serialization::type_info_implementation< T >::type
                >::get_const_instance(),
                t
            )
        );
        if(NULL == upcast)
            autoboost::serialization::throw_exception(
                archive_exception(archive_exception::unregistered_class)
            );
        return static_cast<T *>(upcast);
    }

    template<class T>
    static void check_load(T & /* t */){
        check_pointer_level< T >();
        check_pointer_tracking< T >();
    }

    static const basic_pointer_iserializer *
    find(const autoboost::serialization::extended_type_info & type){
        return static_cast<const basic_pointer_iserializer *>(
            archive_serializer_map<Archive>::find(type)
        );
    }

    template<class Tptr>
    static void invoke(Archive & ar, Tptr & t){
        check_load(*t);
        const basic_pointer_iserializer * bpis_ptr = register_type(ar, *t);
        const basic_pointer_iserializer * newbpis_ptr = ar.load_pointer(
            // note major hack here !!!
            // I tried every way to convert Tptr &t (where Tptr might
            // include const) to void * &.  This is the only way
            // I could make it work. RR
            (void * & )t,
            bpis_ptr,
            find
        );
        // if the pointer isn't that of the base class
        if(newbpis_ptr != bpis_ptr){
            t = pointer_tweak(newbpis_ptr->get_eti(), t, *t);
        }
    }
};

template<class Archive>
struct load_enum_type {
    template<class T>
    static void invoke(Archive &ar, T &t){
        // convert integers to correct enum to load
        int i;
        ar >> autoboost::serialization::make_nvp(NULL, i);
        t = static_cast< T >(i);
    }
};

template<class Archive>
struct load_array_type {
    template<class T>
    static void invoke(Archive &ar, T &t){
        typedef typename remove_extent< T >::type value_type;

        // convert integers to correct enum to load
        // determine number of elements in the array. Consider the
        // fact that some machines will align elements on boundries
        // other than characters.
        std::size_t current_count = sizeof(t) / (
            static_cast<char *>(static_cast<void *>(&t[1]))
            - static_cast<char *>(static_cast<void *>(&t[0]))
        );
        autoboost::serialization::collection_size_type count;
        ar >> AUTOBOOST_SERIALIZATION_NVP(count);
        if(static_cast<std::size_t>(count) > current_count)
            autoboost::serialization::throw_exception(
                archive::archive_exception(
                    autoboost::archive::archive_exception::array_size_too_short
                )
            );
        ar >> serialization::make_array(static_cast<value_type*>(&t[0]),count);
    }
};

} // detail

template<class Archive, class T>
inline void load(Archive & ar, T &t){
    // if this assertion trips. It means we're trying to load a
    // const object with a compiler that doesn't have correct
    // funtion template ordering.  On other compilers, this is
    // handled below.
    detail::check_const_loading< T >();
    typedef
        typename mpl::eval_if<is_pointer< T >,
            mpl::identity<detail::load_pointer_type<Archive> >
        ,//else
        typename mpl::eval_if<is_array< T >,
            mpl::identity<detail::load_array_type<Archive> >
        ,//else
        typename mpl::eval_if<is_enum< T >,
            mpl::identity<detail::load_enum_type<Archive> >
        ,//else
            mpl::identity<detail::load_non_pointer_type<Archive> >
        >
        >
        >::type typex;
    typex::invoke(ar, t);
}

#if 0

// BORLAND
#if AUTOBOOST_WORKAROUND(__BORLANDC__, AUTOBOOST_TESTED_AT(0x560))
// borland has a couple of problems
// a) if function is partially specialized - see below
// const paramters are transformed to non-const ones
// b) implementation of base_object can't be made to work
// correctly which results in all base_object s being const.
// So, strip off the const for borland.  This breaks the trap
// for loading const objects - but I see no alternative
template<class Archive, class T>
inline void load(Archive &ar, const T & t){
    load(ar, const_cast<T &>(t));
}
#endif

// let wrappers through.
#ifndef AUTOBOOST_NO_FUNCTION_TEMPLATE_ORDERING
template<class Archive, class T>
inline void load_wrapper(Archive &ar, const T&t, mpl::true_){
    autoboost::archive::load(ar, const_cast<T&>(t));
}

#if !AUTOBOOST_WORKAROUND(__BORLANDC__, AUTOBOOST_TESTED_AT(0x560))
template<class Archive, class T>
inline void load(Archive &ar, const T&t){
  load_wrapper(ar,t,serialization::is_wrapper< T >());
}
#endif
#endif

#endif

} // namespace archive
} // namespace autoboost

#endif // AUTOBOOST_ARCHIVE_DETAIL_ISERIALIZER_HPP
