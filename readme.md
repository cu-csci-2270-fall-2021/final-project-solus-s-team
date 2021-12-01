# CSCI 2275 – Data Structures - Final Project - Solus Thompson

This project is a simplified version control system called a minigit implemented by Solus Thompson. The function of a version control system is to manage a group of documents by recording changes made to them over time by an outsider user. The minigit first lets the user define what files to keep track of by "adding" or "removing" them from the repository, and then lets them "commit" any changes made by storing copies of the files in a separate, hidden directory. Additionally, the minigit allows the user to "search" for keywords associated with the commits, and then the user may use that information to "checkout" a past commit, allowing them to see how the files were before they were edited.


# CLASS COMPONENTS

--A hidden directory within the active directory created by the class called ".minigit" that is used to store past versions of the files in the repository.
--A doubly linked list (DLL) where each node represents a single commit. Each DLL node contains a string to help identify the commit (commit message), a string to help identify the commit (commit number ID), and a singly linked list (SLL) where each node represents a file in the repository that the program is keeping track of.
--A hash table to store commit message and the commit number IDs associated with them. In this program, the hash table's table size is fixed at 5, but the program to be modified to use a command line argument or user-input to determine the table size.


# RUNNING MINIGIT

To run minigit, follow these steps:

1. From the terminal, use cd to navigate to the build folder.
2. Enter the commands "cmake .." then "make" to compile the program.
3. Use cd to navigate to the run_minigit_here folder (you may use your own folder or add additional files to the provided folder if you wish).
4. If using run_minigit_here, enter the command ../build/run_app_1 (the command might be different for other folders).


# PROGRAM FLOW

1. When the program starts, it will ask if you'd like to initialize a new repository. If the user responds yes, the program initializes an instance of the MiniGit class. The class creates an empty, hidden folder within the active directory called .minigit, initializes a head DLL node to hold information about the first commit (which has yet to happen), and initializes a hash table in which to store commit messages and commit number IDs.

2. After the MiniGit is initialized, the user will be presented with a menu and prompted to enter a numeric value to select an option. After the user enters a number and completes that option, the program will continue to continually display the menu and prompt the user to select an option until the user selects the option to end the program. The various options will be listed below.

        Option 1: Add File
            The user is prompted to enter the name of a file in the active directory. If the user enters invalid input (i.e. a file that doesn't exist in the active directory or a typo), the program will continually prompt them to keep entering file names until a valid one is entered. Then, the program creates a SLL node attached to the most current DLL to represent the file within the repository. If more than one file is entered, it will be appended onto the beginning of the SLL.
        
        Option 2: Remove File
            The user is prompted to enter the name of a file. Then, the program searches the most recent SLL and deletes the node that matches the given file name. If the user passes a file that doesn't exist, the program does nothing.
        
        Option 3: Commit Changes
            The user is prompted to enter a commit message, which must be no more than three words long and can't have been used as a commit message before. If the user enters invalid input, they are contiually prompted to enter commit messages until a valid one is entered. Then, if there have NOT been any changes made to the files in the repository, or if there are NO new files have been added to the repository, nothing happens. However, if either of those statements are TRUE, the program copies the files into .minigit without overwriting past copies of the files, using the files' versions to differentiate them. Then, the program parses the given commit message into individual words and enters them into the hash table for storage, and also creates a new DLL node to represent the next coming commit, copying the last DLL's SLL to the new one. This effectively saves the files in the active directory, taking a metaphorical screenshot of their contents at that moment and storing it somewhere safe.
        
        Option 4: Search Commits
            The user is prompted to enter a single word. Then, the program searches the hash table to see if the word has been used in or as a commit message. If so, the program prints the commit number IDs associated with the word.
        
        Option 5: Checkout Commit
            The user is prompted to enter the commit number ID of their choice (if you can only remember the commit message, use the Search Commits option to find the commit number ID). If the user enters a commit number ID that doesn't that doesn't apply to any of the past commits, the program continually prompts them until a valid ID is entered. Then, the program overwrites all the files in the directory to how they were at the time of that commit, allowing the user to see previous versions of their files. However, the user is unable to Add File, Remove File, or Commit Changes while looking at a past commit, and must checkout to the most recent commit to regain full functionality of the program.
        
        Option 6: Quit Program
            First, the program automatically checkouts the most recent commit, updating the files in the active directory to their most recent versions before terminating the main loop, ending the program.

3. When the program ends, .minigit is cleared of extraneous files, and the files in the active directory are set to their most recent committed versions.