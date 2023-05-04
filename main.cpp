#include <iostream>
#include <vector>
#include <string>
#include <fstream>    
#include <algorithm>  // for reverse
#include <chrono>     // for the timer

using namespace std;

string bottomUpApproach(string arrayOne, string arrayTwo);
string topDownApproach(string arrayOne, string arrayTwo, int size1, int size2, vector<vector<string>>&  vectorTable2D);

int main(int argc, char *argv[])
{
    long long duration = 0; //initialize the running time to 0

    int lengthToTest = 1000; //length of the string to test can change this value for the tests

    if (argc != 3)
    {
        cout << "Not enough arguments \n";
        return 1;
    }

    string input_file = argv[1];
    string option = argv[2];
    ifstream inputFile(input_file);

    if (!inputFile.is_open())
    {
        cout << "Unable to open file" << endl;
        return 1;
    }

    int size1, size2;                                  // size of the two sequences
    inputFile >> size1 >> size2;                      // read the size of the two sequences

    string arrayOne, arrayTwo;                     // the two sequences to be compared
    inputFile >> arrayOne >> arrayTwo;             //first sequence is read into arrayOne, second into arrayTwo

    inputFile.close();

    arrayOne = arrayOne.substr(0, lengthToTest);  // take the first lengthToTest characters of the first sequence
    arrayTwo = arrayTwo.substr(0, lengthToTest);  // take the first lengthToTest characters of the second sequence
    size1 = arrayOne.size();                      // update the size of the two sequences
    size2 = arrayTwo.size();

    string result;
    if (option == "-b")
    {
        auto start = std::chrono::high_resolution_clock::now();  //start the timer 
        result = bottomUpApproach(arrayOne, arrayTwo);
        auto end = std::chrono::high_resolution_clock::now(); //end the timer
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(); //calculate the running time
    }
    else if (option == "-t")
    {
        auto start = std::chrono::high_resolution_clock::now();  //start the timer 
        vector<vector<string>> memo(size1 + 1, vector<string>(size2 + 1, ""));  // 2D vector to store the memoization table for top-down approach initialized to empty strings it has an extra row and column for the base case
        result = topDownApproach(arrayOne, arrayTwo, size1, size2, memo);       // call top-down approach and store the result in result
        auto end = std::chrono::high_resolution_clock::now(); //end the timer
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(); //calculate the running time
    }
    else
    {
        cout << "Invalid option" << endl;
        return 1;
    }

    cout << "length to test: " << lengthToTest << endl;                        
    cout << "Sequence 1: " << arrayOne << endl;                                // display the two sequences and the Longest Common Subsequence
    cout << "Sequence 2: " << arrayTwo << endl;
    cout << "LCS: " << result << endl;
    cout << "Running time: " <<  duration << " milliseconds" << endl;            // display the running time

    return 0;
}



string bottomUpApproach(string arrayOne, string arrayTwo)              //pass in the two sequences as parameters
{
    string longestCommonString;
    int size1 = arrayOne.size();                                         // size of the two sequences
    int size2 = arrayTwo.size();


    /*
        memoTable is the 2D vector that stores the memoization table, it is initialized to 0 and has size1+1 rows and size2+1 columns
        first argument is the number of rows and the second argument is the number of columns.
        The purpose of the table is to store the length of the LCS for all possible prefixes of the two arrays. Each cell (i,j) in the table represents the length of the LCS for the first 
        i chars of the first array and the first j chars of the second array.
        By using the 2D vector we can store and access the intermediate results of the LCS. This is how we can limit calculations.
    */                                                                                            
    vector<vector<int>> memoTable(size1 + 1, vector<int>(size2 + 1, 0));                                   
                                                                    
    for (int i = 1; i <= size1; i++)                                     
    {
        for (int j = 1; j <= size2; j++)                                  
        {
            if (arrayOne[i - 1] == arrayTwo[j - 1])                        
            {
                memoTable[i][j] = memoTable[i - 1][j - 1] + 1;      // if the two characters are equal, add 1 to the diagnal number and store the result in the current cell
            }
            else
            {
                memoTable[i][j] = max(memoTable[i - 1][j], memoTable[i][j - 1]);    // if the two characters are not equal, take the maximum of the two numbrs above and to the left
            }
        }
    }               //memotable is filled with the length of the LCS for all possible prefixes of the two arrays


    int i = size1;                //set i and j to the last number of the table because we want to start from the end  "backtracing"
    int j = size2;                                               
    while (i > 0 && j > 0)  //while not at the first row or column of the table beause that is the base case
    {
        if (arrayOne[i - 1] == arrayTwo[j - 1])         
        {
            longestCommonString.push_back(arrayOne[i - 1]);  // if the two chars are equal add the char to the longestCommonString string
            i--;                                                    
            j--;               // move diagonally up
        }
        else if (memoTable[i - 1][j] > memoTable[i][j - 1])   //if the two chars are not equal and the number above is greater than the number to the left 
        {
            i--;                              // move up
        }
        else
        {
            j--;                              // move left
        }
    }  //at the end of the loop we have the longestCommonString string stored in backwards order

    reverse(longestCommonString.begin(), longestCommonString.end());   // reverse the longestCommonString string because we started from the end of the table
    return longestCommonString;                                        
}


//each cell in the vectorTable2D table stores the LCS(string) for the first i chars of the first array and the first j chars of the second array 
string topDownApproach(string arrayOne, string arrayTwo, int size1, int size2, vector<vector<string>>& vectorTable2D)
{
    if (size1 == 0 || size2 == 0)   // base case, if either of the sizes is 0, return an empty string because there is no LCS  "reached the end of the string"
    {
        return "";
    }

    if (vectorTable2D[size1][size2] != "")   // if the value is already in the vectorTable2D table, return it   "already calculated"
    {
        return vectorTable2D[size1][size2];  // returns the value stored in the table at that index
    }

    if (arrayOne[size1 - 1] == arrayTwo[size2 - 1])  // if the two characters are equal, add the character to the table and call the function recursively with a size -1 because we added the character to the table
    { 
        vectorTable2D[size1][size2] = topDownApproach(arrayOne, arrayTwo, size1 - 1, size2 - 1, vectorTable2D) + arrayOne[size1 - 1];  //returns the LCS(string) of the new size but adds the character to the end of the string after the function returns
    }
    else  // if the two characters are not equal, call the function and store the result in the table
    {
        string leftSide = topDownApproach(arrayOne, arrayTwo, size1 - 1, size2, vectorTable2D);  // call the left side of the tree and store the result in leftSide "take away the last character of the first string"
        string rightSide = topDownApproach(arrayOne, arrayTwo, size1, size2 - 1, vectorTable2D); // call the right side of the tree and store the result in rightSide because we are going right  "take away the last character of the second string"

        if (leftSide.size() > rightSide.size())
        {
            vectorTable2D[size1][size2] = leftSide;    // if the left side is greater than the right side, store the left side in the table
        }
        else
        {
            vectorTable2D[size1][size2] = rightSide;  // if the right side is greater than the left side, store the right side in the  table by adding the character to the table
        }
    }

    return vectorTable2D[size1][size2];  // return the value stored in the vectorTable2D table at the given index which will be the LCS
}