#include <iostream>
#include "../code_1/miniGit.hpp"
#include <filesystem>

using namespace std;
using std::filesystem::exists;


/*
 * Purpose; displays a menu with options
 */
void displayMenu(){
    
    cout << "Select a numerical option:" << endl;
    cout << "+=====Main Menu=========+" << endl;
    cout << " 1. Add File" << endl;
    cout << " 2. Remove File" << endl;
    cout << " 3. Commit Changes" << endl;
    cout << " 4. Search Commits" << endl;
    cout << " 5. Checkout Commit" << endl;
    cout << " 6. Quit Program" << endl;
    cout << "+-----------------------+" << endl;
    cout << "#> ";
}


/*
 *  Purpose; checks if a message is three words or less
 */
bool isValidLength(string message, int maxWords){
    
    stringstream ss(message);
    string word;
    int wordCount = 0;
    
    while(ss >> word){
        wordCount++;
    }
    
    return (wordCount > 0 && wordCount <= maxWords);
}


/*
 *  Purpose; trims whitespace off the ends of a string
 */
string ltrim(string str){
    
    size_t start = str.find_first_not_of(' ');
    if(start == string::npos){
        return str;
    }else{
        return str.substr(start);
    }
}

string rtrim(string str){
    
    size_t end = str.find_last_not_of(' ');
    if(end == string::npos){
        return str;
    }else{
        return str.substr(0, end + 1);
    }
}

string trim(string str){
    return rtrim(ltrim(str));
}


/*
 *  Purpose; Main Function
 */
int main(int argc, char* argv[]){
    
    /*
     *  MiniGit Initialization
     */
    MiniGit* miniG = nullptr;
    bool isRunning = false;
    bool onCurrentCommit = true;
    
    cout << endl << "Initialize new repository? Enter y/n:" << endl;
    char startInput;
    cin >> startInput;
    
    if(startInput == 'y'){
        cout << endl << "Initializing with Table Size 5..." << endl;
        isRunning = true;
        
        miniG = new MiniGit();
        miniG->init(5);
        
        cout << "Initialization complete." << endl << endl;
        
    }else if(startInput == 'n'){
        cout << "Ending program..." << endl << endl;
        
    }else{
        cout << "Invalid input, ending program..." << endl << endl;
    }
    
    
    /*
     *  MiniGit Functions
     */
    while(isRunning){
        
        displayMenu();
        int menuInput;
        cin >> menuInput;
        cout << endl;
        
        switch(menuInput){
                
            /*
             *  Add File
             */
            case 1:
                {
                    if(onCurrentCommit){
                        
                        //Prompt user to enter a file name until a valid file is given
                        string fileName = "";
                        cout << "Please enter the name of the file to be added:" << endl;
                        cin.ignore();
                        getline(cin, fileName);
                        
                        while(!exists(fileName)){
                            cout << endl << fileName << " does not exist in the directory, please try a different file:" << endl;
                            getline(cin, fileName);
                        }
                        cout << endl << fileName << " found. Adding file..." << endl;
                        
                        //Add file
                        miniG->add(fileName);
                        
                    }else{
                        cout << "Cannot add files while viewing an old commit! Checkout commit number ID " + to_string(miniG->getNumOfCommits() - 1) + " first." << endl << endl;
                    }
                    
                    break;
                }
                
            
            /*
             *  Remove File
             */
            case 2:
                {
                    if(onCurrentCommit){
                        
                        //Prompt user to enter a file name
                        string fileName = "";
                        cout << "Please enter the name of the file to be removed:" << endl;
                        cin.ignore();
                        getline(cin, fileName);
                        
                        //Remove file
                        miniG->rm(fileName);
                        
                    }else{
                        cout << "Cannot remove files while viewing an old commit! Checkout commit number ID " + to_string(miniG->getNumOfCommits() - 1) + " first." << endl << endl;
                    }
                    
                    break;
                }
                
                
            /*
             *  Commit Changes
             */
            case 3:
                {
                    if(onCurrentCommit){
                        
                        //Prompt user to enter a commit message until a valid one is given
                        string commitMessage = "";
                        cout << "Please enter commit message (three words max):" << endl;
                        cin.ignore();
                        getline(cin, commitMessage);
                        commitMessage = trim(commitMessage);
                        
                        bool isValidMessage = (isValidLength(commitMessage, 3) && miniG->isUnusedMessage(commitMessage));
                        while(!isValidMessage){
                            
                            cout << endl << "Invalid commit message. Please try a different message:" << endl;
                            getline(cin, commitMessage);
                            commitMessage = trim(commitMessage);
                            
                            isValidMessage = (isValidLength(commitMessage, 3) && miniG->isUnusedMessage(commitMessage));
                        }
                        
                        cout << endl << "Commit message accepted. Committing changes..." << endl;
                        
                        //Commit changes if possible
                        if(miniG->hasFiles()){
                            
                            string commitID = miniG->commit(commitMessage);
                            cout << endl << "Commit complete. Commit ID: " << commitID << endl << endl;
                            
                        }else{
                            cout << "Commit failed: No files in repository." << endl << endl;
                        }
                        
                    }else{
                        cout << "Cannot commit changes while viewing an old commit! Checkout commit number ID " + to_string(miniG->getNumOfCommits() - 1) + " first." << endl << endl;
                    }
                    
                    break;
                }
                
                
            /*
             *  Search Commits
             */
            case 4:
                {
                    //Prompt user to enter a search key until a valid one is given
                    string searchKey = "";
                    cout << "Please enter word to search for:" << endl;
                    cin.ignore();
                    getline(cin, searchKey);
                    searchKey = trim(searchKey);
                    
                    while(!isValidLength(searchKey, 1)){
                        
                        cout << endl << "Invalid word count. Please try a different search key:" << endl;
                        getline(cin, searchKey);
                    }
                    
                    //Search for given search key
                    miniG->search(searchKey);
                    break;
                }
                
                
            /*
             *  Checkout Commit
             */
            case 5:
                {
                    //Confirm user wishes to checkout
                    cout << "Are you sure you want to checkout? You will lose local changes if you have not committed them. Enter y/n:" << endl;
                    char confirmation;
                    cin >> confirmation;
                    
                    if(confirmation == 'y'){
                        
                        //Prompt user to enter a commit number ID and checkout to that commit if possible
                        int checkoutNum;
                        cout << endl << "Checkout confirmed. Enter commit number ID to checkout:" << endl;
                        cin >> checkoutNum;
                        
                        if(checkoutNum >= miniG->getNumOfCommits() || checkoutNum < 0){
                            cout << "Commit with number ID " << to_string(checkoutNum) << " does not exist." << endl << endl;
                            
                        }else if(checkoutNum == miniG->getNumOfCommits() - 1){
                            
                            if(onCurrentCommit){
                                cout << endl << "Already viewing most recent commit." << endl << endl;
                                
                            }else{
                                onCurrentCommit = true;
                                miniG->checkout(checkoutNum);
                            }
                            
                        }else{
                            onCurrentCommit = false;
                            miniG->checkout(checkoutNum);
                        }
                        
                    }else if(confirmation == 'n'){
                        cout << "Checkout cancelled." << endl << endl;
                        
                    }else{
                        cout << "Invalid input, checkout cancelled." << endl << endl;
                    }
                    
                    break;
                }
                
                
            /*
             *  Quit Program
             */
            case 6:
                {
                    cout << "Quitting program..." << endl << endl;
                    isRunning = false;
                    break;
                }
                
                
            /*
             *  Invalid Input
             */
            default:
                {
                    cout << "Invalid input." << endl << endl;
                }
        }
    }
    
    return 0;
}