//
//  util.h
//  natprolib
//
//  Created by Edhem Avdagic on 15/06/2022.
//  Distributed under the terms of the GNU General Public License
//

#pragma once

#include <cassert>

#ifndef __clang__
#	define NPL_DISABLE_VISIBILITY_ANNOTATIONS
#endif


#ifndef NPL_RELEASE
#	define NPL_ASSERT(exp, msg) assert(((void)msg, exp))
#else
#	define NPL_ASSERT(exp,msg) assert(((void)msg, 0))
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
