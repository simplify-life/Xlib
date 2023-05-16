//----------------------------------------------//
//  xallocator.h
//  xlib
//
//  Copyright (c) __DATA__  Ximena.
//  Created by ximena on 16/6/21.
//  Copyright © 2016年 ximena. All rights reserved.
//  Contact Email: xiaominghe2014@gmail.com
//----------------------------------------------//


// int example()
// {
//     // 使用XAllocator分配内存
//     xlib::xmemory::XAllocator<int> allocator;
//     int* p = allocator.allocate(10);
//     for (int i = 0; i < 10; ++i)
//     {
//         allocator.construct(p + i, i);
//     }
    
//     // 使用XAllocator构造vector
//     std::vector<int, xlib::xmemory::XAllocator<int>> vec(allocator);
//     for (int i = 0; i < 10; ++i)
//     {
//         vec.push_back(i);
//     }
    
//     // 释放内存
//     for (int i = 0; i < 10; ++i)
//     {
//         allocator.destroy(p + i);
//     }
//     allocator.deallocate(p, 10);
    
//     return 0;
// }


#ifndef xallocator_h
#define xallocator_h
#include <climits>
#include <cstddef>


namespace xlib {
    
    namespace xmemory{
        
        template <class T, size_t BlockSize = 4096>
        class XAllocator
        {
            public:
            /**
                Conventional define
             */
                using   value_type = T;
                using   pointer = T*;
                using   reference = T&;
                using   const_pointer = const T*;
                using   const_reference = const T&;
                using   size_type = size_t;
                using   difference_type = ptrdiff_t;
                using   propagate_on_container_copy_assignment = std::false_type;
                using   propagate_on_container_move_assignment = std::true_type;
                using   propagate_on_container_swap = std::true_type;
            
            template <class U> struct rebind
            {
                using other =  XAllocator<U>;
            };
            /**
                constructor ,desstructor
             */
            XAllocator() noexcept; //default
            XAllocator(const XAllocator&) noexcept;//copy
            XAllocator(XAllocator&&) noexcept;//move
            template <class U> XAllocator(const XAllocator<U>&) noexcept;//template
            virtual ~XAllocator() noexcept;
            
            XAllocator& operator=(const XAllocator&) = delete ;
            XAllocator& operator=(XAllocator&&) noexcept;
            
            
            pointer address(reference x) const noexcept;
            const_pointer address(const_reference x) const noexcept;
            
            // Can only allocate one object at a time. n and hint are ignored
            pointer allocate(size_type n = 1, const_pointer hint = 0);
            void deallocate(pointer p, size_type n = 1);
            
            size_type max_size() const noexcept;
            
            template <class U, class... Args> void construct(U* p, Args&&... args);
            template <class U> void destroy(U* p);
            
            template <class... Args> pointer newElement(Args&&... args);
            void deleteElement(pointer p);
            
            
        private:
            union Slot_
            {
                value_type element;
                Slot_* next;
            };
            
            typedef char* data_pointer_;
            typedef Slot_ slot_type_;
            typedef Slot_* slot_pointer_;
            
            slot_pointer_ currentBlock_;
            slot_pointer_ currentSlot_;
            slot_pointer_ lastSlot_;
            slot_pointer_ freeSlots_;
            
            size_type padPointer(data_pointer_ p, size_type align) const noexcept;
            void allocateBlock();
            
            static_assert(BlockSize >= 2 * sizeof(slot_type_), "BlockSize too small.");
            
        };
       
#include "xallocator.tcpp"
        
    }//end xmemory
}//end xlib

#endif /* xallocator_h */
