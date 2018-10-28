#include "lightcpp.h"
#include "flog.h"

int main()
{
    STD_FUNC_BEGIN;
    int i = 0;
    FLOG(INFO) << "Hello world!";
    ENFORCE(i);
    return 0;
    STD_FUNC_END;
    return -1;
}
