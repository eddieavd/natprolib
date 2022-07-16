//
//  util.h
//  natprolib
//
//  Created by Edhem Avdagic on 15/06/2022.
//  Distributed under the terms of the GNU General Public License
//

#pragma once


#ifndef __clang__
#       ifndef NPL_DISABLE_VISIBILITY_ANNOTATIONS
#	define NPL_DISABLE_VISIBILITY_ANNOTATIONS
#       endif
#endif

#define NPL_MOVE(...) \
        static_cast< std::remove_reference_t< decltype(__VA_ARGS__)>&&>(__VA_ARGS__)

#define NPL_FWD(...) \
        static_cast< decltype(__VA_ARGS__)&&>(__VA_ARGS__)

#ifdef NPL_RELEASE
#       define NPL_ASSERT(cond, ...) ((void)0)
#else
#       include <cstdlib>
#       include <iostream>

#       define NPL_ASSERT(cond, ...)                                             \
                do                                                                \
                {                                                                  \
                        if( !( cond ) )                                             \
                        {                                                            \
                                std::cerr << "natprolib: '" << #cond << "' faild in " \
                                          << __FILE__ << ":" << __LINE__               \
                                          << " - " << __VA_ARGS__ << std::endl;         \
                                std::abort();                                            \
                        }                                                                 \
                } while( 0 )
#endif

#ifndef NPL_INLINE_VISIBILITY
#	if !defined(NPL_DISABLE_VISIBILITY_ANNOTATIONS)
#		define NPL_INLINE_VISIBILITY __attribute__((__visibility__("hidden"))) __attribute__((internal_linkage))
#	else
#		define NPL_INLINE_VISIBILITY
#	endif
#endif

#ifndef NPL_EXTERN_TEMPLATE_TYPE_VIS
#	if !defined(NPL_DISABLE_VISIBILITY_ANNOTATIONS)
#		define NPL_EXTERN_TEMPLATE_TYPE_VIS __attribute__((__visibility__("default")))
#	else
#		define NPL_EXTERN_TEMPLATE_TYPE_VIS
#	endif
#endif

#ifndef NPL_TEMPLATE_VIS
#	if !defined(NPL_DISABLE_VISIBILITY_ANNOTATIONS)
#		define NPL_TEMPLATE_VIS __attribute__((__visibility__("default")))
#	else
#		define NPL_TEMPLATE_VIS
#	endif
#endif


#define NPL_RQ_DEFAULT_CAPACITY 8


namespace npl
{



} // namespace npl
