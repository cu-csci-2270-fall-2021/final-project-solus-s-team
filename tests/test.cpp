// If you change anything in this file, your changes will be ignored 
// in your homework submission.
// Chekout TEST_F functions bellow to learn what is being tested.
#include <gtest/gtest.h>
// #include "../code_1/<HEADER FILE>.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <cmath>
#include <array>
#include <vector>
#include "test_helpers.hpp"

using namespace std;

class test_x : public ::testing::Test {
protected:
	// This function runs only once before any TEST_F function
	static void SetUpTestCase(){
		std::ofstream outgrade("./total_grade.txt");
		if(outgrade.is_open()){
			outgrade.clear();
			outgrade << (int)0;
			outgrade.close();
		}
	}

	// This function runs after all TEST_F functions have been executed
	static void TearDownTestCase(){
		std::ofstream outgrade("./total_grade.txt");
		if(outgrade.is_open()){
			outgrade.clear();
			outgrade << (int)std::ceil(100*total_grade/max_grade);
			outgrade.close();
			std::cout << "Total Grade is : " << (int)std::ceil(100*total_grade/max_grade) << std::endl;
		}
	}

	void add_points_to_grade(double points){
		if(!::testing::Test::HasFailure()){
			total_grade += points;
		}
	}

	// this function runs before every TEST_F function
	void SetUp() override {}

	// this function runs after every TEST_F function
	void TearDown() override {
		std::ofstream outgrade("./total_grade.txt");
		if(outgrade.is_open()){
			outgrade.clear();
			outgrade << (int)std::ceil(100*total_grade/max_grade);
			outgrade.close();
		}
	}
	
	static double total_grade;
	static double max_grade;
};

double test_x::total_grade = 0;
double test_x::max_grade = 100;



/////////////////////////////////////////
// Tests start here
/////////////////////////////////////////


TEST_F(test_x, TestInsert){
    
    string result;
    string expected;
    
    
    //Test 1 (pre-written)
    string commits1[] = {"computer", "science", "fun", "difficult", "science"};
    int len = 5;
    int tabSize = 5;
    
    result = test_insert(commits1, len, tabSize);
    expected = "0|| science(1,4,)\n1|| \n2|| \n3|| \n4|| difficult(3,)-->fun(2,)-->computer(0,)\n";
    ASSERT_EQ(expected, result);
    add_points_to_grade(13.8);
    
    
    //Test 2
    string commits2[] = {"", " ", "  ", "   Whitespace is neat!    "};
    len = 4;
    
    result = test_insert(commits2, len, tabSize);
    expected = "Insert failed: Cannot use empty string as key.\nInsert failed: Cannot use whitespace as key.\nInsert failed: Cannot use whitespace as key.\n0|| \n1|| \n2||    Whitespace is neat!    (3,)\n3|| \n4|| \n";
    ASSERT_EQ(expected, result);
    add_points_to_grade(13.8);
    
    
    //Test 3
    string commits3[] = {"12345", "360 42", "#^Q%$^fg^&^A#"};
    len = 3;
    
    result = test_insert(commits3, len, tabSize);
    expected = "0|| 12345(0,)\n1|| \n2|| 360 42(1,)\n3|| #^Q%$^fg^&^A#(2,)\n4|| \n";
    ASSERT_EQ(expected, result);
    add_points_to_grade(13.8);
}


TEST_F(test_x, TestSearch){
    
    string result;
    string expected;
    
    
    //Test 1
    string commits1[] = {"computer", "science", "fun", "difficult", "science"};
    int commitLen = 5;
    int tabSize = 5;
    string searches1[] = {"science", "okay", "search", "fun"};
    int searchLen = 4;
    
    result = test_search(commits1, commitLen, tabSize, searches1, searchLen);
    expected = "Search result for science: Found!\nSearch result for okay: Not found!\nSearch result for search: Not found!\nSearch result for fun: Found!\n";
    ASSERT_EQ(expected, result);
    add_points_to_grade(13.8);
    
    
    //Test 2
    string commits2[] = {"13571987", "   ", "  Whitespace time again!  ", "%^%$#^%$@^%"};
    commitLen = 4;
    string searches2[] = {"13571987", "   ", "  Whitespace time again!  ", "Whitespace time again!", "%^%$#^%$@^%"};
    searchLen = 5;
    
    result = test_search(commits2, commitLen, tabSize, searches2, searchLen);
    expected = "Insert failed: Cannot use whitespace as key.\nSearch result for 13571987: Found!\nSearch result for    : Not found!\nSearch result for   Whitespace time again!  : Found!\nSearch result for Whitespace time again!: Not found!\nSearch result for %^%$#^%$@^%: Found!\n";
    ASSERT_EQ(expected, result);
    add_points_to_grade(13.8);
}