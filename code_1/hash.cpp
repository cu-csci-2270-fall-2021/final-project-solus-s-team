// CPP program to implement hashing with chaining
#include<iostream>
#include "hash.hpp"

using namespace std;


HashNode* HashTable::createNode(string key, HashNode* next, int commitNum){
    
    HashNode* nn = new HashNode;
    
    nn->key = key;
    nn->next = next;
    nn->commitNums.push_back(commitNum);
    
    return nn;
}


HashTable::HashTable(int bsize){
    
    this->tableSize = bsize;
    table = new HashNode*[tableSize];
    
    for(int i = 0; i < tableSize; i++){
        table[i] = nullptr;
    }
}


//function to calculate hash function
unsigned int HashTable::hashFunction(string s){
    
    unsigned int sum = 0;
    
    for(long unsigned int i = 0; i < s.length(); i++){
        sum += (int)s[i];
    }
    
    return (sum % tableSize);
}


// TODO Complete this function
//function to search
HashNode* HashTable::searchItem(string key){
    
    int index = hashFunction(key);
    HashNode* curr = table[index];
    
    while(curr != nullptr){
        if(curr->key == key){
            return curr;
        }
        curr = curr->next;
    }
    
    return curr;
}


//TODO Complete this function
//function to insert
bool HashTable::insertItem(string key, int cNum){
    
    if(key.find_first_not_of(' ') != string::npos){
        
        HashNode* curr = searchItem(key);
        
        if(curr == nullptr){
            
            int index = hashFunction(key);
            HashNode* nn = createNode(key, table[index], cNum);
            table[index] = nn;
            return true;
        }
        
        curr->commitNums.push_back(cNum);
        return true;
        
    }else if(key.empty()){
        cout << "Insert failed: Cannot use empty string as key." << endl;
        return false;
        
    }else{
        cout << "Insert failed: Cannot use whitespace as key." << endl;
        return false;
    }
    
    return false;
}


// function to display hash table //
/* assume the table size is 5. For each bucket it will show the 
** the string key and the commit number (separated by comma) within parenthesis
** e.g. the key is science and commit numbers are 1 and 4. The key science
** is hashed to position 0. So the print format will be-

0|| science(1,4,)
1|| 
2|| 
3|| 
4|| difficult(3,)-->fun(2,)-->computer(0,)

*/
void HashTable::printTable(){
    
    for(int i = 0; i < tableSize; i++){
        
        cout << i << "|| ";
        HashNode* curr = table[i];
        
        if(curr != nullptr){
            
            while(curr->next != nullptr){
                
                cout << curr->key << "(";
                for(int commitNum : curr->commitNums){
                    cout << commitNum << ",";
                }
                cout << ")-->";
                
                curr = curr->next;
            }
            
            cout << curr->key << "(";
            for(int commitNum : curr->commitNums){
                cout << commitNum << ",";
            }
            cout << ")";
        }
        
        cout << endl;
    }
}