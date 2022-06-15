//
//  util.h
//  natprolib
//
//  Created by Edhem Avdagic on 15/06/2022.
//  Distributed under the terms of the GNU General Public License
//

#pragma once

#include <type_traits>


#ifndef NPL_RELEASE
#	include <cassert>
#	define NPL_ASSERT(exp, msg) assert(((void)msg, exp))
#else
#	define NPL_ASSERT(exp,msg) assert(((void)msg, 0))
#endif

#define NPL_INLINE_VISIBILITY   __attribute__((__visibility__("hidden"))) __attribute__((internal_linkage))
#define NPL_EXTERN_TEMPLATE_VIS __attribute__((__visibility__("default")))

#define NPL_RQ_DEFAULT_CAPACITY 8


namespace npl
{



} // namespace npl
