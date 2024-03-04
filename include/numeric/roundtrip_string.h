//
// Created by RainSure on 2023/11/11.
//

#ifndef RSMESH_ROUNDTRIP_STRING_H
#define RSMESH_ROUNDTRIP_STRING_H

#include "double-conversion/double-conversion.h"

#include <array>
#include <limits>
#include <string>
#include <type_traits>

/**
 * \namespace rsmesh::numeric
 * \brief 该命名空间下主要定义了数值计算相关的类和函数
 */
namespace rsmesh::numeric {
    namespace detail {
        template <class Floating>
        void to_string(Floating value, double_conversion::StringBuilder* builder);
        
        template <>
        inline void to_string(double value, double_conversion::StringBuilder* builder) {
            double_conversion::DoubleToStringConverter::EcmaScriptConverter().ToShortest(value, builder);
        }
        
        template <>
        inline void to_string(float value, double_conversion::StringBuilder* builder) {
            double_conversion::DoubleToStringConverter::EcmaScriptConverter().ToShortestSingle(value, builder);
        }
    } // namespace detail
    
    inline double to_double(const std::string& str) {
        double_conversion::StringToDoubleConverter converter( 
                double_conversion::StringToDoubleConverter::ALLOW_TRAILING_JUNK |
                  double_conversion::StringToDoubleConverter::ALLOW_CASE_INSENSITIVITY,
                  std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN(), "inf",
                  "nan");
        int processed_chars = 0;
        return converter.StringToDouble(str.c_str(), static_cast<int>(str.size()), &processed_chars);
    }
    
    inline float to_float(const std::string& str) {
        double_conversion::StringToDoubleConverter converter(
                double_conversion::StringToDoubleConverter::ALLOW_TRAILING_JUNK |
                double_conversion::StringToDoubleConverter::ALLOW_CASE_INSENSITIVITY,
                std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN(), "inf",
                "nan");
        int processed_chars = 0;
        return converter.StringToFloat(str.c_str(), static_cast<int>(str.size()), &processed_chars);
    }

    template <class Floating,
            std::enable_if_t<std::is_floating_point<Floating>::value, std::nullptr_t> = nullptr>
    std::string to_string(Floating value) {
        static constexpr std::size_t buffer_size = 32;

        std::array<char, buffer_size> buffer{};
        double_conversion::StringBuilder builder(buffer.data(), buffer_size);

        detail::to_string(value, &builder);

        return builder.Finalize();
    }
}


#endif //RSMESH_ROUNDTRIP_STRING_H
