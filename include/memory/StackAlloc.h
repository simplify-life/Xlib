//----------------------------------------------//
//  StackAlloc.h
//  xlib
//
//  Copyright (c) __DATA__  Ximena.
//  Created by ximena on 16/6/21.
//  Copyright © 2016年 ximena. All rights reserved.
//  Contact Email: xiaominghe2014@gmail.com
//----------------------------------------------//

// template <typename T>
// void example() {
//     xlib::xmemory::StackAlloc<T> stack;
//     stack.push(1);
//     stack.push(2);
//     stack.push(3);
//     std::cout << stack.top() << std::endl;
//     stack.pop();
//     std::cout << stack.top() << std::endl;
//     stack.pop();
//     std::cout << stack.top() << std::endl;
//     stack.pop();
// }

#ifndef StackAlloc_h
#define StackAlloc_h

#include    <memory>

namespace xlib {
    
    namespace xmemory{
    
        template <class T>
        struct StackNode_
        {
            T data;
            StackNode_* prev;
        };
        
        template <class T, class Alloc = std::allocator<T>>
        class StackAlloc
        {
        public:
            
            //using node ,allocator to reduce the type
            using node = StackNode_<T>;
            using allocator = class Alloc::template rebind<node>::other;
            
            //default constructor ,desstructor
            StackAlloc() {  head_ = 0; }
            virtual ~StackAlloc() { clear(); }
            inline bool empty() {return (head_ == 0);}
            
            /** Deallocate all elements and empty the stack */
            void clear()
            {
                node* curr = head_;
                while (curr != 0)
                {
                    node* tmp = curr->prev;
                    allocator_.destroy(curr);
                    allocator_.deallocate(curr, 1);
                    curr = tmp;
                }
                head_ = 0;
            }
            /** Put an element on the top of the stack */
            void push(T element) {
                node* newNode = allocator_.allocate(1);
                allocator_.construct(newNode, node());
                newNode->data = element;
                newNode->prev = head_;
                head_ = newNode;
            }
            
            /** Remove and return the topmost element on the stack */
            T pop() {
                T result = head_->data;
                node* tmp = head_->prev;
                allocator_.destroy(head_);
                allocator_.deallocate(head_, 1);
                head_ = tmp;
                return result;
            }
            
            /** Return the topmost element */
            T top() { return (head_->data); }
            
        private:
            allocator allocator_;
            node* head_;
        };
    }//end xmemory
}//end xlib
#endif /* StackAlloc_h */
