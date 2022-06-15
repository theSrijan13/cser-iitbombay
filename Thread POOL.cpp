#include <iostream>
    #include <thread>
    using namespace std;

    queue<int> qu1;
    queue<int> qu2;
    queue<int> qu3;

    thread thr1;
    thread thr2;
    thread thr3;

    void func() {
        cout << "code for first output" << endl;
        cout << "code for second output" << endl;
    }
    void fn1() {
        cout << "fn1" << endl;
    }

    void fn2() {
        cout << "fn2" << endl;
    }

    void fn3() {
        cout << "fn3" << endl;
    }
    void masterFn() {
        work:
        if (qu1.size() > 0) thr1 = thread(fn1);
        if (qu2.size() > 0) thr2 = thread(fn2);
        if (qu3.size() > 0) thr3 = thread(fn3);

        if (qu1.size() > 0) {
            qu1.pop();
            thr1.join();
        }
        if (qu2.size() > 0) {
            qu2.pop();
            thr2.join();
        }
        if (qu3.size() > 0) {
            qu3.pop();
            thr3.join();
        }

        if (qu1.size() == 0 && qu1.size() == 0 && qu1.size() == 0)
            return;
        goto work;
    }

    int main()
    {
        thread thr(func);
        thr.join();
        /* other statements */
        qu1.push(1);
        qu1.push(1);
        qu1.push(1);

        qu2.push(2);
        qu2.push(2);

        qu3.push(3);

        thread masterThr(masterFn);
        cout << "Program has started:" << endl;
        masterThr.join();
        cout << "Program has ended." << endl;

        return 0;
    }