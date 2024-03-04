//
// Created by RainSure on 2023/11/10.
//

#ifndef RSMESH_IO_H
#define RSMESH_IO_H

#include <iostream>
#include <fstream>
#include <stdexcept>

namespace rsmesh {
    namespace common {
        template <class T>
        struct read {
            void operator()(std::istream& is, T& t) {
                is.read(reinterpret_cast<char*>(&t), sizeof(t));
            }
        };
        
        template <class T>
        struct write {
            void operator()(std::ostream& os, T& t) {
                os.write(reinterpret_cast<char*>(&t), sizeof(t));
            }
        };
        
        template <class T>
        void load(const std::string& filename, T& t) {
            std::ifstream ifs(filename, std::ios::binary);
            if(!ifs) {
                throw std::runtime_error("Cannot open file: " + filename);
            }
            
            read<T>{}(ifs, t);
        }
        
        template <class T>
        void save(const std::string& filename, T& t) {
            std::ofstream ofs(filename, std::ios::binary);
            if(!ofs) {
                throw std::runtime_error("Cannot open file: " + filename);
            }
            
            write<T>{}(ofs, t);
        }
    }
}

#endif //RSMESH_IO_H
