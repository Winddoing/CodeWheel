/*#############################################################
 *     File Name	: list.cpp
 *     Author		: winddoing
 *     Created Time	: 2019年01月11日 星期五 15时54分56秒
 *     Description	:
 *############################################################*/

#include <iostream>
#include <list>

using namespace std;

int main()
{
    // Create and populate the list.
    int x;
    std::list<char> charList(12);
#if 0
    for (x=0; x<10; ++x)
        charList.push_front(65 + x);
#endif

    // Display contents of list.
    std::cout << "Original list: ";
    std::list<char>::iterator iter;
    for (iter = charList.begin();iter != charList.end(); iter++) {
        std::cout << *iter;
    }
    std::cout << std::endl;


    std::cout << "List size1:" << charList.size() << std::endl;

    // Insert five Xs into the list.
    std::list<char>::iterator start = charList.begin();
    charList.insert(++start, 5, 'X');

    std::cout << "List size2:" << charList.size() << std::endl;
    // Display the result.
    std::cout << "Resultant list: ";
    for (iter = charList.begin(); iter != charList.end(); iter++) {
        std::cout << *iter;
    }
    std::cout << std::endl;

    return 0;
}
