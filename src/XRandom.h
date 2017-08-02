//----------------------------------------------//
//  XRandom.h
//  xlib
//
//  Copyright (c) __DATA__  Ximena.
//  Created by ximena on 16/6/12.
//  Copyright © 2016年 ximena. All rights reserved.
//  Contact Email: xiaominghe2014@gmail.com
//----------------------------------------------//

#ifndef XRandom_h
#define XRandom_h
#include "XBase.h"
#include <vector>
#include <string>
XLIB_BEGAIN

namespace XRand
{
/**
 @eg.  maybe  getRandomVector(9)={4,7,3,6,8,0,2,5,1}
 */
std::vector<uint> getRandomV(uint size);
/**
 @brief .from this function,you can get a random string
 */
std::string getRandomString(uint length);

};
XLIB_END

#endif /* XRandom_h */
