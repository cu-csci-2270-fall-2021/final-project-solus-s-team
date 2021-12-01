#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

#include <filesystem>
namespace fs = std::filesystem;
using fs::exists;

#include "miniGit.hpp"
#include <vector>


/*
 *  Purpose; Constructor and Destructor
 */
MiniGit::MiniGit(){
    fs::remove_all(".minigit");
    fs::create_directory(".minigit");
}

MiniGit::~MiniGit(){
    fs::remove_all(".minigit");
}


/*
 *  Purpose; Helps initialize variables for the MiniGit not covered in the constructor
 */
void MiniGit::init(int hashtablesize){
    //Initialize DLL
    commitHead = new BranchNode;
    commitHead->commitID = 0;
    commitHead->commitMessage = "";
    commitHead->next = nullptr;
    commitHead->previous = nullptr;
    commitHead->fileHead = nullptr;
    
    //Initialize hash table
    ht = new HashTable(hashtablesize);
    
    //Initialize number of commits
    commits = 0;
}


/*
 *  Purpose; Given a file name, adds a new file representation to the repository
 */
void MiniGit::add(string fileName){
    
    //Find latest commit
    BranchNode* currCommit = commitHead;
    while(currCommit->next != nullptr){
        currCommit = currCommit->next;
    }
    
    //Search for file
    FileNode* currFile = currCommit->fileHead;
    while(currFile != nullptr && currFile->name != fileName){
        currFile = currFile->next;
    }
    
    //If file exists, send error message
    //Else, add to SLL
    if(currFile != nullptr){
        cout << fileName << " has already been added." << endl << endl;
        
    }else{
        FileNode* nn = new FileNode;
        nn->name = fileName;
        nn->version = 0;
        nn->next = currCommit->fileHead;
        currCommit->fileHead = nn;
        
        cout << fileName << " added." << endl << endl;
    }
}


/*
 *  Purpose; Given a file name, removes said file's representation from the repository
 */
void MiniGit::rm(string fileName){
    
    //Find latest commit
    BranchNode* currCommit = commitHead;
    while(currCommit->next != nullptr){
        currCommit = currCommit->next;
    }
    
    //Search for file while keeping track of previous node
    FileNode* currFile = currCommit->fileHead;
    FileNode* prevFile = nullptr;
    while(currFile != nullptr && currFile->name != fileName){
        prevFile = currFile;
        currFile = currFile->next;
    }
    
    //If file exists, delete
    //Else, send error message
    if(currFile != nullptr){
        
        //Edge case: deleting fileHead
        if(currFile == currCommit->fileHead){
            currCommit->fileHead = currCommit->fileHead->next;
            delete currFile;
            
        }else{
            prevFile->next = currFile->next;
            delete currFile;
        }
        
        cout << fileName << " removed." << endl << endl;
        
    }else{
        cout << fileName << " does not exist in repository." << endl << endl;
    }
}


/*
 *  Purpose; Commits changes made to files in the directory to files in .minigit, and marks which commit ID those files are now associated with
 */
string MiniGit::commit(string msg){
    
    bool changesWereMade = false;
    
    //Find latest commit and set commitMessage to msg
    BranchNode* currCommit = commitHead;
    while(currCommit->next != nullptr){
        currCommit = currCommit->next;
    }
    
    
    //Traverse through SLL
    FileNode* currFile = currCommit->fileHead;
    while(currFile != nullptr){
        
        //If file does not exist in .minigit, copy it over
        //Else, check if file has been changed since last commit
        string directoryFileName = currFile->name;
        string minigitFileName = ".minigit/" + to_string(currFile->version) + "_" + currFile->name;
        
        if(!exists(minigitFileName)){
            
            ifstream infile(directoryFileName.c_str(), ios::binary);
            ofstream outfile(minigitFileName.c_str(), ios::binary);
            
            if(infile.is_open() && outfile.is_open() && infile.good() && outfile.good()){
                
                outfile << infile.rdbuf();
                outfile.close();
                infile.close();
                
                cout << directoryFileName << " committed. Minigit file: " << minigitFileName << endl;
                changesWereMade = true;
                
            }else{
                cout << "Failed to commit " << directoryFileName << "." << endl;
            }
            
        }else{
            
            //If file has been changed since last commit, copy it into .minigit WITHOUT overwriting the previous version
            ifstream directoryFileStream(directoryFileName);
            string directoryFileContent((istreambuf_iterator<char>(directoryFileStream)),(std::istreambuf_iterator<char>()));
            
            ifstream minigitFileStream(minigitFileName);
            string minigitFileContent((std::istreambuf_iterator<char>(minigitFileStream)),(std::istreambuf_iterator<char>()));
            
            if(directoryFileContent != minigitFileContent){
                
                currFile->version++;
                minigitFileName = ".minigit/" + to_string(currFile->version) + "_" + currFile->name;
                
                ifstream infile(directoryFileName.c_str(), ios::binary);
                ofstream outfile(minigitFileName.c_str(), ios::binary);
                
                if(infile.is_open() && outfile.is_open() && infile.good() && outfile.good()){
                    
                    outfile << infile.rdbuf();
                    outfile.close();
                    infile.close();
                    
                    cout << directoryFileName << " committed. Minigit file: " << minigitFileName << endl;
                    changesWereMade = true;
                    
                }else{
                    cout << "Failed to commit " << directoryFileName << "." << endl;
                }
            }
        }
        
        currFile = currFile->next;
    }
    
    
    //If changes were made to the files, continue with commit process
    //Else, send error message
    if(changesWereMade){
        
        //Increment number of commits and set commit message
        commits++;
        currCommit->commitMessage = msg;
        
        
        //Parse msg into individual words and add them to the hash table
        stringstream ss(msg);
        string word;
        
        while(getline(ss, word, ' ')){
            if(word.find_first_not_of(' ') != string::npos){
                ht->insertItem(word, currCommit->commitID);
            }
        }
        
        
        //Create new DLL node
        BranchNode* nn = new BranchNode;
        nn->commitID = commits;
        nn->commitMessage = "";
        
        nn->next = nullptr;
        nn->fileHead = new FileNode;
        
        nn->previous = currCommit;
        currCommit->next = nn;
        
        //Copy SSL into new DLL node
        FileNode* currFileOld = currCommit->fileHead;
        FileNode* currFileNew = nn->fileHead;
        
        currFileNew->name = currFileOld->name;
        currFileNew->version = currFileOld->version;
        currFileNew->next = nullptr;
        
        currFileOld = currFileOld->next;
        
        while(currFileOld != nullptr){
            
            currFileNew = currFileNew->next = new FileNode;
            
            currFileNew->name = currFileOld->name;
            currFileNew->version = currFileOld->version;
            currFileNew->next = nullptr;
            
            currFileOld = currFileOld->next;
        }
        
    }else{
        cout << "Commit failed: No changes made to files in directory. Returning old commit number ID." << endl;
    }
    
    
    //Return commit number ID
    return to_string(commits - 1);
}


/*
 *  Purpose; Given a single word, returns all commit number IDs that word is associated with
 */
void MiniGit::search(string key){
    
    //Search for node with given key
    HashNode* keyNode = ht->searchItem(key);
    
    //If node exists, print commitNums
    //Else, send error message
    if(keyNode != nullptr){
        
        cout << "Key found! Commit number IDs associated with " << key << ":" << endl;
        for(int num : keyNode->commitNums){
            cout << num << "   ";
        }
        cout << endl << endl;
        
    }else{
        cout << "Key not found." << endl << endl;
    }
}


/*
 *  Purpose; Given a commit number ID, reverts all files back to their corrosponding previous versions
 */
void MiniGit::checkout(int commitID){
    
    //Find requested commit
    BranchNode* currCommit = commitHead;
    while(currCommit->commitID != commitID){
        currCommit = currCommit->next;
    }
    
    
    //Traverse through SLL
    FileNode* currFile = currCommit->fileHead;
    while(currFile != nullptr){
        
        //Copy file from .minigit into the directory
        string minigitFileName = ".minigit/" + to_string(currFile->version) + "_" + currFile->name;
        string directoryFileName = currFile->name;
        
        ifstream infile(minigitFileName.c_str(), ios::binary);
        ofstream outfile(directoryFileName.c_str(), ios::binary);
        
        if(infile.is_open() && outfile.is_open() && infile.good() && outfile.good()){
            
            outfile << infile.rdbuf();
            outfile.close();
            infile.close();
        }
        
        cout << "Reverted " << directoryFileName << " to previous version." << endl;
        currFile = currFile->next;
    }
    
    cout << endl << "Checkout for commit number ID " + to_string(commitID) + " complete." << endl << endl;
}


/*
 *  Purpose; Prints the hash table of commit IDs
 */
void MiniGit::printSearchTable(){
     ht->printTable();
}


/*
 *  Purpose; Returns how many times Commit Changes has been successfully run
 */
int MiniGit::getNumOfCommits(){
    return commits;
}


/*
 *  Purpose; Returns whether message has been used as a commit ID or not
 */
bool MiniGit::isUnusedMessage(string message){
    
    //Iterate through DLL to see if message has been used previously
    BranchNode* curr = commitHead;
    while(curr != nullptr){
        
        if(curr->commitMessage == message){
            return false;
        }
        curr = curr->next;
    }
    
    return true;
}


/*
 *  Purpose; Checks to see if there are any files in the SLL for the most recent commit
 */
bool MiniGit::hasFiles(){
    
    //Find latest commit
    BranchNode* currCommit = commitHead;
    while(currCommit->next != nullptr){
        currCommit = currCommit->next;
    }
    
    return (currCommit->fileHead != nullptr);
}