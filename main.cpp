#include <iostream>
#include <string>
#include "Darray.hpp"

int main(int argc, char const *argv[]){
    using namespace std;
    
    Darray<int> darrInt = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    cout <<"Integer Array Size : ("<< darrInt.size() <<") "<<endl;
    for (auto i : darrInt){ cout << i <<endl; }
    cout << "------------------------" << endl;
    
    Darray<string> darr;
    Darray<string*> darr2;
    darr.addAt(0, "elem 0");
    
    darr.addAll({ "elem 1", "elem 2", "elem 3", "elem 4", "elem 5" });
    // // add some elements
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
    darr.addAt(6, "elem 6"); // adding at the end
    // darr.removeAt(-1); // should throw exception
    
    // sort the array in ascending order by the pointed pointers
    darr2.sort([](const string *s1, const string *s2){ return (*s1 < *s2); }); // custom sort function
    
    cout <<"Array 2 Size : ("<< darr2.size() <<") sorting the pointers based on the actual value "<<endl;
    for (auto i : darr2){ cout << *i <<endl; }
    
    // but the actual array doesn't sort, we have to use the darr.sort() individually
    cout <<"Array 1 Size : "<< darr.size() <<endl;
    for (auto i : darr){ cout << i <<endl; }
    
    // now sort the array in custom order
    darr.sort([](const string &s1, const string &s2){ return s1 > s2; });
    cout <<"Array 1 Size : ("<< darr.size() <<") custom sort - descending order "<<endl;
    for (auto i : darr){ cout << i <<endl; }
    
    darr.removeAt(darr.size() - 1); // removes last element 
    
    // // testing the exception handling 
    // try {
    //     darr[darr.size() - 1] = "";  // throw an 'out_of_range' exception
    // } catch (const out_of_range &e) {
    //     cout << "\nCaught exception as expected: " << e.what() << endl;
    // }
    
    auto darr3 = move(darr); // testing move constructor
    cout <<"Array 3 Size (moved from Array 1) : ("<< darr3.size() <<") "<<endl;
    for (auto i : darr3){ 
        i += 'X';
        cout << i <<endl; 
    }
    cout <<"Array 1 Size (after move) : ("<< darr.size() <<") "<<endl;
    for (auto i : darr){ cout << i <<endl; }
    
    auto darr4 = darr3; // testing copy constructor
    cout <<"Array 4 Size (copied from Array 2) : ("<< darr4.size() <<") "<<endl;
    for (auto &i : darr4){ 
        i += 'Y';
        cout << i <<endl; 
    }
    cout <<"Array 3 Size (after copy) : ("<< darr3.size() <<") "<<endl;
    for (auto i : darr3){ cout << i <<endl; }
    
    Darray<string> darr5;
    darr5 = move(darr3); // testing move assignment operator
    cout <<"Array 5 Size (move assigned from Array 3) : ("<< darr5.size() <<") "<<endl;
    for (auto i : darr5){ cout << i <<endl; }
    // after move assignment darr3 becomes empty
    cout <<"Array 3 Size (after move) : ("<< darr3.size() <<") "<<endl;
    for (auto i : darr3){ cout << i <<endl; }
    
    darr.clear(); darr2.clear(); darr3.clear(); darr4.clear(); darr5.clear();
    
    // cout << darr2[0] << endl; // should throw exception
    auto s = string("elem 8");
    darr2.addAt(0, &s);
    cout << *darr2[0] <<endl;
    
    return 0;
}
