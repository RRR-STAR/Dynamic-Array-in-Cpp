
# Project Explanation

Key Idea - Is to create such a array that doesn't cause memory wastage, and provide the functionality of the arrays.

Approach - As Lists(std::list<T>) are the great choise for linked list represent and also for memeory efficient design and the best part is that the insertion and deletion operations can be done in O(1) time complexity.

Problem - But the main probelm is lists doesn't provide random access or O(1) time access to it's elements like arrays. Because it doesn't store it's elements in a contigious way.

Solution (vectors) - So for this one thing we can do is using of Vectors(std::vector<T>) which is a great solution, and also it is a standard part of C++, so it also provides more functionalities.

Problems (in vector type) - Althoug it is a great alternative but at the same time as it uses the raw arrays in it's internal structure, that's why whenever we need to insert a new element it allocates a new array and shift all the elements to that array and then, de-allocates previous array, although the re-allocation & de-allocation is not done every time, but initially it does a lot when the array size is small.

Solution - That's why to resolve this two problems(List random access, vectors re-allocation & de-allocation) combindly the @class `DynamicArray` was implemented. Which is a combination of std::list<T> type and std::unordered_map<T> type so that we can access the elements of the lists in O(1) time using unordered maps mapping(which is also a O(1) time operation) and no need for re-allocation & de-allocation for new inputs.

Assumption - Although the unordered map (std::unordered_map<T>) provides the O(1) time access to values using keys, but as the unordered_map uses the 'hash' technique to store the values into the hash table. And we do not have any control to that hash table. So if the hash table is small then it creates a hash collision(two keys generates a same hash output value and thus two keys stores at the same position of the hash table) and the element access time gets increased from O(1) to O(n). That's why to resolve this problem there is function called max_load_factor(), which is have used in the contructor to get a bigger hash table and prevent the hash collisions.

Still efficient - Althoug the element access time gets increased from O(1) to O(n), still it is more effiecient rather than searching a whole list, because here maximum can be 3 to 4 values can have to be seached.

More functionality - Here we can also add new functionalities of std::list<T> type, but all these functionalities have to be implement manually and all those needs a O(n) operational time for maintaining the '_index' value.

Limmited functionalty - That's why only neede functionalities are given and try to maintain the minimal design.

Custom sort() - The main reason to implement the sort function, because as we know that the standard library std::sort() function needs random iterators and doesn't work with lists bidirectional iterators. And this is also the main reason that std::list<T> have their own sort function for it's objects. And that oveloaded functions just pases the user povided custom comparison function to the actual internal list memeber function.

Working - [1]. add() function just adds an element to the array as well as mark that element's allocated address to the unordered map with the index so that, the address can easily be accessed with those indexe's. [2]. remove() function just removes the last element from the array, but doesn't return it, just removes the last element same operation like the stack pop() function. [3]. operator[]() and here is the crucial part. This function is actually returns the reference of the required index data. But if the user asks for a out of bound access then it just throws an exception and terminates the program. But before terminating it de-allocates the object. That's why it is the users responsibility to do not try to access those such elements that doesn't exists.