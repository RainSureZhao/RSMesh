//
// Created by 赵润朔 on 2023/10/26.
//

#ifndef RSMESH_ITERATOR_RANGE_H
#define RSMESH_ITERATOR_RANGE_H

namespace rsmesh {
    namespace common {
        template<class ForwardIterator>
        class iterator_range {
        public:
            using iterator = ForwardIterator;
            using const_iterator = ForwardIterator;
            
            iterator_range(iterator begin, iterator end) : begin_(begin), end_(end) { }
            
            iterator begin() const {
                return begin_;
            }
            
            iterator end() const {
                return end_;
            }
            
        private:
            iterator begin_;
            iterator end_;
        };
        
        template<class ForwardIterator>
        iterator_range<ForwardIterator> make_range(ForwardIterator begin, ForwardIterator end) {
            return iterator_range<ForwardIterator>(begin, end);
        }
    } // common
} // rsmesh

#endif //RSMESH_ITERATOR_RANGE_H
