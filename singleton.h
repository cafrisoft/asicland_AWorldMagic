#ifndef SINGLETON_H
#define SINGLETON_H

class MainWindow;
class MyMainWindow; // forward declaration
class CommandWindow;

class Singleton
{
public:
    static Singleton& getInstance()
    {
        static Singleton instance; // Guaranteed to be destroyed, instantiated on first use.
        return instance;
    }
    Singleton(Singleton const&) = delete; // Prevent copy
    void operator=(Singleton const&) = delete; // Prevent assignment

    void setMainWindow(MainWindow* ptr) { pMainWindow = ptr; }
    void setMainWorkWind(MyMainWindow* ptr) { pMainWorkWind = ptr; }
    void setMyCommandWind(CommandWindow* ptr) { pCommandWindow = ptr; }

    MainWindow* getMainWindow() { return pMainWindow; }
    MyMainWindow* getMainWorkWind() { return pMainWorkWind; }
    CommandWindow* getMyCommandWind() { return pCommandWindow; }

private:
    Singleton(); // Prevent construction
    ~Singleton() {} // Prevent unwanted destruction

    MainWindow* pMainWindow;
    MyMainWindow* pMainWorkWind;
    CommandWindow *pCommandWindow;
};

#endif // SINGLETON_H
