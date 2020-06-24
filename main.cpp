/**
 * @file main.cpp
 * @author HankHenshaw
 * @version 0.0.1
 * @brief A couple simple examples of SFINAE 
 */

#include <iostream>
#include <vector>
#include <list>
#include <tuple>
#include <iterator>
#include <type_traits>

template<typename T>
std::enable_if_t<std::is_integral<T>::value> printS(T& val)
{
    std::cout << "\nIntegral\n";
    unsigned char *p = reinterpret_cast<unsigned char*>(&val);
    for(int i = sizeof(T) - 1; i >= 0; --i)
    {
        std::cout << static_cast<int>(*(p+i)) << '.';
    }
    std::cout << '\n';
}

template<typename T>
std::enable_if_t<std::is_base_of_v<std::bidirectional_iterator_tag, typename std::iterator_traits<typename T::iterator>::iterator_category>> printS(T& val)
{
    std::cout << "\nSTL containers\n";
    std::copy(std::begin(val), std::end(val), std::ostream_iterator<typename T::value_type>(std::cout, "."));
    std::cout << '\n';
}

void printS(std::string &val)
{
    std::cout << "\nString\n";
    std::cout << val << '\n';
}

template<typename ...T>
struct is_all_the_same {};

template<typename T>
struct is_all_the_same<T, T> : std::true_type {};

template<typename T>
struct is_all_the_same<T> : std::true_type {};

template<typename T, typename ...Args>
struct is_all_the_same<T, T, Args...> : is_all_the_same<T, Args...> {};

template<typename T, typename U>
struct is_all_the_same<T, U> : std::false_type {};

template<typename T, typename U, typename ...Args>
struct is_all_the_same<T, U, Args...> : std::false_type {};

template<typename ...T>
std::enable_if_t<is_all_the_same<T...>::value> printS(std::tuple<T...>& val)
{
    std::cout << "\nTuple\n";
    std::apply([](auto&&... args) {((std::cout << args << '.'), ...);}, val);
    std::cout << '\n';
}

int main()
{
    /*-------Integral Types-------*/
    char c = -1;
    short s = 0;
    int i = 2130706433;
    long long ll = 8875824491850138409;
    printS(c);
    printS(s);
    printS(i);
    printS(ll);
    /*-------std::string-------*/
    std::string str{"123.234.534.1"};
    printS(str);
    /*-------std::vector and std::list-------*/
    std::vector<int> vec{123,123,1,2};
    std::list<int> lis{2,5,4,3};
    printS(vec);
    printS(lis);
    /*-------std::tuple-------*/
    auto tuple = std::make_tuple(127,0,0,1);
    printS(tuple);

    return 0;
}
