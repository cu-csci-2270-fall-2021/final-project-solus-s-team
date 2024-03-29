#pragma once

#include<iostream>
#include<vector>
#include <filesystem>
#include "hash.hpp"

using namespace std;
namespace fs = std::filesystem;


enum GITOP{init, add, rm, commit, checkout, status};

struct FileNode{
    string name;
    int version;
    FileNode* next;
};

struct BranchNode{
    int commitID;
    string commitMessage;
    BranchNode* next;
    BranchNode* previous;
    FileNode* fileHead;   
};


//*** You can add helper functions if you desire ***//
class MiniGit{
    
private:    
    BranchNode* commitHead;
    int commits;
    HashTable* ht;
    
public:
    MiniGit();
    ~MiniGit();
    
    void init(int hashtablesize);
    void add(string fileName);
    void rm(string fileName);
    string commit(string msg);
    void search(string key);
    void checkout(int commitID);
    void printSearchTable();
    
    //New Helper Functions, used in main()
    int getNumOfCommits();
    bool isUnusedMessage(string message);
    bool hasFiles();
};