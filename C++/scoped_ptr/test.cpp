/*#############################################################
 *     File Name	: test.cpp
 *     Author		: winddoing
 *     Created Time	: 2020年06月12日 星期五 15时42分27秒
 *     Description	:
 *############################################################*/

#include <iostream>

#include "scoped_refptr.h"
#include "ref_counted_object.h"

using namespace std;

class implementation {
    public:
        implementation() {
            std::cout <<"Init implementation\n";
        }
        ~implementation() { 
            std::cout <<"destroying implementation\n"; 
        }
        void do_something() { 
            std::cout << "did something\n"; 
        }
};

void test()
{
    rtc::scoped_refptr<implementation> impl(
            new rtc::RefCountedObject<implementation>());
    impl->do_something();
}

int main(int argc, const char *argv[])
{
    std::cout<<"Test Begin ... \n";
    test();
    std::cout<<"Test End.\n";
    
    return 0;
}
