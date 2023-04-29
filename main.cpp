#include <iostream>
#include <vector>
#include <string>
#include <fstream>    
#include <algorithm>  // for reverse
#include <chrono>     // for the timer

using namespace std;

string dynamicProgramming(string arrayOne, string arrayTwo);
string topDownApproach(string arrayOne, string arrayTwo, int size1, int size2, vector<vector<string>>& memo);

int main(int argc, char *argv[])
{
    long long duration = 0; //initialize the running time to 0

    int lengthToTest = 100; //length of the string to test can change this value for the tests

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
        result = dynamicProgramming(arrayOne, arrayTwo);
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

    cout << "Sequence 1: " << arrayOne << endl;                                // print the two sequences and the Least Common Subsequence
    cout << "Sequence 2: " << arrayTwo << endl;
    cout << "LCS: " << result << endl;
    cout << "Running time: " <<  duration << " milliseconds" << endl;            // print the running time

    return 0;
}



string dynamicProgramming(string arrayOne, string arrayTwo)              //pass in the two sequences as parameters
{
    int size1 = arrayOne.size();                                         // size of the two sequences
    int size2 = arrayTwo.size();


    /*
        memoTable is the 2D vector that stores the memoization table, it is initialized to 0 and has size1+1 rows and size2+1 columns
        first argument is the number of rows and the second argument is the number of columns.
        The purpose of the table is to store the length of the LCS for all possible prefixes of the two arrays. Each cell (i,j) in the table represents the length of the LCS for the first 
        i chars of the first array and the first j chars of the second array.
        By using the 2D vector we can store and access the intermediate results of the LCM. This is how we get rid of redundant calls.
    */                                                                                            
    vector<vector<int>> memoTable(size1 + 1, vector<int>(size2 + 1, 0));                                   
                                                                    
    for (int i = 1; i <= size1; i++)                                     
    {
        for (int j = 1; j <= size2; j++)                                  
        {
            if (arrayOne[i - 1] == arrayTwo[j - 1])                        
            {
                memoTable[i][j] = memoTable[i - 1][j - 1] + 1;               // if the two characters are equal, add 1 to the diagonal number because its part of the LCS
            }
            else
            {
                memoTable[i][j] = max(memoTable[i - 1][j], memoTable[i][j - 1]);    // if the two characters are not equal, take the maximum of the two numbers above and to the left
            }
        }
    }               //memotable is filled with the length of the LCS for all possible prefixes of the two arrays

    string leastCommonString;
    int i = size1;                //set i and j to the last number of the table because we want to start from the end
    int j = size2;                                               
    while (i > 0 && j > 0)
    {
        if (arrayOne[i - 1] == arrayTwo[j - 1])                         
        {
            leastCommonString.push_back(arrayOne[i - 1]);           // if the two characters are equal, add the character to the leastCommonString
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
    }  //at the end of the while loop we have the leastCommonString string stored in backwards order

    reverse(leastCommonString.begin(), leastCommonString.end());   // reverse the leastCommonString string because we started from the end of the table , leastCommonString.begin is the first element of the string and leastCommonString.end is the last element of the string
    return leastCommonString;
}


//each cell in the vectorTable2D table stores the LCS(string) for the first i chars of the first array and the first j chars of the second array 
string topDownApproach(string arrayOne, string arrayTwo, int size1, int size2, vector<vector<string>>& vectorTable2D)
{
    if (size1 == 0 || size2 == 0)   // base case, if either of the sizes is 0, return an empty string
    {
        return "";
    }

    if (vectorTable2D[size1][size2] != "")   // if the value is already in the vectorTable2D table, return it
    {
      //  cout << "im in here \n";
        return vectorTable2D[size1][size2];  // vectorTable2D[size1][size2] is the value stored in the vectorTable2D table at the given index
    }

    if (arrayOne[size1 - 1] == arrayTwo[size2 - 1])  // if the two characters are equal, add the character to the vectorTable2D table and return the value
    { 
     //   cout << "called they equal " << size1 - 1 << " " << size2 - 1 << "\n";
        vectorTable2D[size1][size2] = topDownApproach(arrayOne, arrayTwo, size1 - 1, size2 - 1, vectorTable2D) + arrayOne[size1 - 1];  // add the character to the vectorTable2D table
    }
    else  // if the two characters are not equal, call the function recursively and store the result in the vectorTable2D table
    {
    //    cout << "calling left recusion  " << size1 - 1 << " " << size2 << "\n";
        string left = topDownApproach(arrayOne, arrayTwo, size1 - 1, size2, vectorTable2D);  // call the left side of the tree and store the result in left because we are going left
     //   cout << "calling right recusion " << size1 << " " << size2 - 1 << "\n";
        string right = topDownApproach(arrayOne, arrayTwo, size1, size2 - 1, vectorTable2D); // call the right side of the tree and store the result in right because we are going right

        if (left.size() > right.size())
        {
            vectorTable2D[size1][size2] = left;    // if the left side is greater than the right side, store the left side in the vectorTable2D table
        }
        else
        {
            vectorTable2D[size1][size2] = right;  // if the right side is greater than the left side, store the right side in the vectorTable2D table
        }
    }

    return vectorTable2D[size1][size2];  // return the value stored in the vectorTable2D table at the given index which will be the LCS
}