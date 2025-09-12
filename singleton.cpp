#include<iostream>
using namespace std;

class singleton{
private:
  static singleton *Instance;
  singleton(){
    cout << "singletone isntance created";
  }
  public:
    singleton(singleton &st) = delete;
    singleton operator=(singleton &st) = delete;

   singleton* getInstance(){
      if(Instance == nullptr){
        Instance = new singleton();
      }
      return Instance;
    }
    void showMessage(){
      cout << "show message from singleton" << endl;
    }
};

singleton *singleton::Instance = nullptr;