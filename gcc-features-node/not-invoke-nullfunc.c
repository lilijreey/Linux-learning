/*
 *  Description: 
 */

__attribute__ ((pure, const)) void ok() {}

int main() {
    ok(); // no call ok
    void (*x)() = ok;

    //对函数指针的调用 并不能优化掉，-O1 可以优化掉。
    //-fid-pure-cosnt 不能优化掉
    x(); called ok 
	return 0 ;
}


