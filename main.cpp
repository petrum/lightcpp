#include "enforce.h"
#include "flog.h"

int main()
{
    STD_FUNCTION_BEGIN;
    int i = 0;
    FLOG(INFO) << "Hello world!";
    ENFORCE(i);
    return 0;
    STD_FUNCTION_END;
    return -1;
}
