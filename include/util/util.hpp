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
#       ifndef NPL_ASSERT
#       define NPL_ASSERT(cond, ...) ((void)0)
#       endif
#else
#       ifndef NPL_ASSERT
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
#       endif
#endif


#ifndef NPL_CONSTEXPR_ASSERT
#define NPL_CONSTEXPR_ASSERT(expr, ...)                                                                     \
        _Pragma( "GCC diagnostic push" )                                                                     \
        _Pragma( "GCC diagnostic ignored \"-Wunused-but-set-variable\"" )                                     \
                do                                                                                             \
                {                                                                                               \
                        if( std::is_constant_evaluated() )                                                       \
                        {                                                                                         \
                                int test = 1;                                                                      \
                                test /= ( expr );          /* UB in constexpr context causes compile error */       \
                        }                                                                                            \
                        else                                                                                          \
                        {                                                                                              \
                                NPL_ASSERT( expr, __VA_ARGS__ );                                                        \
                        }                                                                                                \
                } while( 0 )                                                                                              \
        _Pragma( "GCC diagnostic pop" )
#endif


#ifdef __GNUC__
#       ifndef NPL_USE_ATTRIBUTES
#       define NPL_USE_ATTRIBUTES
#       endif
#endif

#ifdef __GNUC__
#       ifndef NPL_UNREACHABLE
#       define NPL_UNREACHABLE __builtin_unreachable()
#       endif
#else
#       ifndef NPL_UNREACHABLE
#       define NPL_UNREACHABLE std::abort();
#       endif
#endif

#ifdef NPL_USE_ATTRIBUTES
#       ifdef __clang__
#               ifndef NPL_ALWAYS_INLINE
#               define NPL_ALWAYS_INLINE __attribute__(( always_inline ))
#               endif
#       else
#               ifndef NPL_ALWAYS_INLINE
#               define NPL_ALWAYS_INLINE
#               endif
#       endif
#       ifndef NPL_FLATTEN
#       define NPL_FLATTEN __attribute__(( flatten ))
#       endif
#       ifndef NPL_NOESCAPE
#       define NPL_NOESCAPE __attribute__(( noescape ))
#       endif
#       ifndef NPL_NORETURN
#       define NPL_NORETURN [[ noreturn ]]
#       endif
#       ifndef NPL_NODISCARD
#       define NPL_NODISCARD [[ nodiscard ]]
#       endif
#       ifndef NPL_DEPRECATED
#       define NPL_DEPRECATED [[ deprecated ]]
#       endif
#       ifndef NPL_LIKELY
#       define NPL_LIKELY __attribute__(( likely ))
#       endif
#       ifndef NPL_UNLIKELY
#       define NPL_UNLIKELY __attribute__(( unlikely ))
#       endif
#else
#       ifndef NPL_ALWAYS_INLINE
#       define NPL_ALWAYS_INLINE
#       endif
#       ifndef NPL_FLATTEN
#       define NPL_FLATTEN
#       endif
#       ifndef NPL_NOESCAPE
#       define NPL_NOESCAPE
#       endif
#       ifndef NPL_NORETURN
#       define NPL_NORETURN
#       endif
#       ifndef NPL_NODISCARD
#       define NPL_NODISCARD
#       endif
#       ifndef NPL_DEPRECATED
#       define NPL_DEPRECATED
#       endif
#       ifndef NPL_LIKELY
#       define NPL_LIKELY
#       endif
#       ifndef NPL_UNLIKELY
#       define NPL_UNLIKELY
#       endif
#endif

#ifndef NPL_STD_VER
#       if __cplusplus <= 201103L
#               define NPL_STD_VER 11
#       elif __cplusplus <= 201402L
#               define NPL_STD_VER 14
#       elif __cplusplus <= 201703L
#               define NPL_STD_VER 17
#       else
#               define NPL_STD_VER 20
#       endif
#endif
