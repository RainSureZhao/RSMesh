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
        /**
         * @brief 结构体用于从输入流中读取特定类型的数据
         *
         * `read`结构体通过重载的函数调用操作符实现了从给定的输入流（如文件流）中读取一个特定类型的对象。
         * 这个结构体利用模板允许对任意类型`T`的数据进行读取，只要该类型能够通过二进制方式安全地从流中读出。
         *
         * @tparam T 读入的数据类型
         */
        template <class T>
        struct read {

            /**
             * @brief 从输入流中读取数据
             *
             * 读取指定类型`T`的数据项从输入流`is`中，数据的大小由`T`的大小决定。
             * 该方法直接操作内存，因此要求`T`是一个平凡的、可以被memcpy的类型。
             *
             * @tparam T 读入的数据类型
             * @param is 输入流，从中读取数据
             * @param t 引用，指向将要存放读取数据的变量
             *
             * @code
             * int v;
             * read<int>{}(std::cin, v);
             * @endcode
             */
            void operator()(std::istream& is, T& t) {
                is.read(reinterpret_cast<char*>(&t), sizeof(t));
            }
        };

        /**
         * @brief 结构体用于向输出流写入特定类型的数据
         *
         * `write`结构体提供了一种通用的方法来将一个特定类型的对象写入给定的输出流（例如文件流或标准输出）。
         * 它通过重载的函数调用操作符来实现，允许用户简便地将数据写入到各种输出流中。这个功能特别适合
         * 进行二进制数据的序列化，可以用于保存程序状态或对象到文件等。
         *
         * 使用模板参数`T`，这个结构体可以针对不同的数据类型进行操作，使其非常灵活。这要求`T`的数据能够
         * 通过二进制形式安全地写入，且该类型应当是平凡的、可被memcpy的。
         *
         * @tparam T 写入的数据类型
         */
        template <class T>
        struct write {
            /**
             * 将数据写入输出流
             *
             * 该方法接受一个输出流`os`和一个类型为`T`的数据`t`，然后将`t`的内容以二进制形式写入到`os`中。
             * 这种方式适用于各种平凡的数据类型，从简单的基本类型到具有平凡复制构造函数和析构函数的结构体或类。
             * 请注意，对于非平凡类型，直接使用这种方式进行写入可能会导致未定义的行为，因为它不会调用任何构造函数或析构函数。
             *
             * @param os 输出流，用于写入数据
             * @param t 要写入的数据，这里传递的是一个引用，避免了不必要的数据复制
             *
             * @code
             * int v = 42;
             * write<int>{}(std::cout, v);
             * @endcode
             */
            void operator()(std::ostream& os, T& t) {
                os.write(reinterpret_cast<char*>(&t), sizeof(t));
            }
        };

        /**
         * @brief 从文件中读取数据并存储到提供的变量中
         *
         * 这个函数封装了从文件中读取二进制数据的过程，并将读取的数据存储到指定类型的变量中。
         * 它使用模板参数`T`来适应不同类型的数据读取需求，无论是基本数据类型、POD类型还是具有
         * 标准布局的自定义类型，只要它们可以通过二进制形式安全读取。这个函数打开指定的文件，
         * 并尝试以二进制模式读取内容，如果文件无法打开，则会抛出一个异常。
         *
         * 注意，这个函数假定文件中的数据格式和`T`的内存布局完全匹配，且不进行任何形式的数据解析
         * 或验证。因此，使用时需要确保数据的一致性和兼容性。
         *
         * @tparam T 读入的数据类型。这决定了函数如何解释读取的二进制数据，并影响读取操作的安全性和有效性。
         * @param filename 读入的文件路径名。这指定了数据源的位置。
         * @param t 读入数据写入的变量的引用。函数将从文件中读取的数据存储到这个变量中。
         *
         * @throws std::runtime_error 如果无法打开指定的文件，则抛出异常。
         *
         * @code
         * // 假设有一个结构体Point { float x; float y; };
         * Point p;
         * rsmesh::common::load<Point>("point.bin", p);
         * // 上面的例子中，从"point.bin"文件中读取一个Point类型的数据，并存储到变量p中。
         * @endcode
         */
        template <class T>
        void load(const std::string& filename, T& t) {
            std::ifstream ifs(filename, std::ios::binary);
            if(!ifs) {
                throw std::runtime_error("Cannot open file: " + filename);
            }
            
            read<T>{}(ifs, t);
        }

        /**
         * @brief 将数据写入到指定的文件中
         *
         * 这个函数提供了一种通用的方式来将变量的内容以二进制形式保存到文件中。它可以用于各种类型的数据，
         * 包括基本数据类型、POD类型，以及其他具有标准布局的自定义类型。函数尝试打开（或创建）一个文件，
         * 并以二进制模式写入提供的变量的数据。如果无法打开文件，则会抛出一个异常。
         *
         * 使用这个函数需要确保类型`T`可以安全地进行二进制写操作，即其内存表示可以直接写入文件而不会
         * 导致数据损坏或丢失。这通常意味着`T`应该是一个平凡的或可复制的类型。
         *
         * @tparam T 写入的数据类型。这个类型参数决定了哪种类型的变量可以被写入文件。
         * @param filename 写入的文件路径名。这指定了数据将被写入的位置。
         * @param t 写入的数据的引用。这是将被写入文件的变量。
         *
         * @throws std::runtime_error 如果无法打开指定的文件，则抛出异常。
         *
         * @code
         * // 假设有一个简单的结构体Point { float x; float y; };
         * Point p{1.0f, 2.0f};
         * rsmesh::common::save<Point>("point.bin", p);
         * // 上面的例子中，将变量p的内容以二进制形式写入到"point.bin"文件中。
         * @endcode
         */
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
