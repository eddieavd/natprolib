//
//
//      natprolib
//      util.hpp
//

#pragma once


#define NPL_MOVE(...) \
        static_cast< std::remove_reference_t< decltype(__VA_ARGS__)>&&>(__VA_ARGS__)

#define NPL_FWD(...) \
        static_cast< decltype(__VA_ARGS__)&&>(__VA_ARGS__)

#ifdef NPL_RELEASE
#       define NPL_ASSERT(cond, ...) ((void)0)
#else
#       include <cstdlib>
#       include <iostream>

#       define NPL_ASSERT(cond, ... )                                         \
                do                                                             \
                {                                                               \
                        if( !( cond ) )                                          \
                        {                                                         \
                                std::cerr << "nplib: '" << #cond << "' failed in " \
                                          << __FILE__ << ":" << __LINE__            \
                                          << " - " << __VA_ARGS__ << std::endl;      \
                                std::abort();                                         \
                        }                                                              \
                } while( 0 )
#endif

#ifdef __GNUC__
#       ifndef NPL_USE_ATTRIBUTES
#       define NPL_USE_ATTRIBUTES
#       endif
#endif

#ifdef __GNUC__
#       define NPL_UNREACHABLE __builtin_unreachable()
#else
#       define NPL_UNREACHABLE std::abort();
#endif

#ifdef NPL_USE_ATTRIBUTES
#       ifndef NPL_ALWAYS_INLINE
#       define NPL_ALWAYS_INLINE __attribute__(( always_inline ))
#       endif
#       ifndef NPL_FLATTEN
#       define NPL_FLATTEN __attribute__(( flatten ))
#       endif
#       ifndef NPL_NOESCAPE
#       define NPL_NOESCAPE __attribute__(( noescape ))
#       endif
#else
#       define NPL_ALWAYS_INLINE
#       define NPL_FLATTEN
#       define NPL_NOESCAPE
#endif
