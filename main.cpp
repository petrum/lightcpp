#include "enforce.h"
#include "flog.h"

int main()
{
    STD_FUNCTION_BEGIN;
    int i = 0;
    char arr[6];
    char ch = 2[arr];
    FLOG(INFO) << "Hello world!";
    ENFORCE(i);
    return 0;
    STD_FUNCTION_END;
    return -1;
}
