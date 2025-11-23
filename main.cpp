#include <iostream>
#include <string>
#include "DynamicArray.hpp"

int main(int argc, char const *argv[]){
    using namespace std;
    
    DArr<string> darr; 
    DArr<string*> darr2;
    darr.pop_back(); // doesn't do any type of change and not raised any error
    
    darr.addAll({ "elem 1", "elem 2", "elem 3", "elem 4", "elem 5" });
    // add some elements
    // darr.add("elem 1");
    // darr.add("elem 2");
    // darr.add("elem 3");
    // darr.add("elem 4");
    // darr.add("elem 5");
    
    cout <<"Array 1 Size : ("<< darr.size() <<") initially "<<endl;
    for (short int i = 0; i < darr.size(); ++i){
        cout << darr[i] <<endl;  
        darr2.add( &(darr[i]) );
    }
    darr[0] = "elem 6";
    // sort the array in acendeing order by the pointed pointers
    darr2.sort([](const string *s1, const string *s2){ return (*s1 < *s2); });
    
    cout <<"Array 2 Size : ("<< darr2.size() <<") sorting the pointers based on the actual value "<<endl;
    for (auto i : darr2){ cout << *i <<endl; }
    
    // but the actual array doesn't sort, we have to use the darr.sort() individually
    cout <<"Array 1 Size : "<< darr.size() <<endl;
    for (auto i : darr){ cout << i <<endl; }
    
    // now sort the array in custom order
    darr.sort([](const string &s1, const string &s2){ return s1 > s2; });
    cout <<"Array 1 Size : ("<< darr.size() <<") custom sort - decending order "<<endl;
    for (auto i : darr){ cout << i <<endl; }
    
    darr.pop_back(); // removes 5th element(darr[4]) so, accessing it throws an exception 
    
    // testing the exception handling 
    // try {
    //     darr[4] = "";  // throw an 'std::out_of_range' exception
    // } catch (const std::out_of_range &e) {
    //     cout << "\nCaught exception as expected: " << e.what() << endl;
    // }

    darr.clear();  darr2.clear();
    return 0;
}
