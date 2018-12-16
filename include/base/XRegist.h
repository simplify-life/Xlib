//----------------------------------------------//
//  XFactory.h
//  xlib
//
//  Copyright (c) __DATA__  Ximena.
//  Created by ximena on 16/6/8.
//  Copyright © 2016年 ximena. All rights reserved.
//  Contact Email: xiaominghe2014@gmail.com
//----------------------------------------------//

#ifndef XFactory_h
#define XFactory_h
#include<unordered_map>
template<class K,class V>
class XFactory
{
public:
    void regist(const K& k,const V& v)
    {
        XFactory::get().map_.emplace(k, v);
    };
private:
    inline static XFactory& get()
    {
        static XFactory instance;
        return instance;
    }
    
    static std::unordered_map<K,V> map_;
};
#endif /* XFactory_h */
