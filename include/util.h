//
//  util.h
//  natprolib
//
//  Created by Edhem Avdagic on 15/06/2022.
//  Distributed under the terms of the GNU General Public License
//

#pragma once

#include <cassert>

#ifndef _NPL_RELEASE
#	define _NPL_ASSERT(exp, msg) assert(((void)msg, exp))
#endif
