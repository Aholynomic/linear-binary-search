// header files
#include <iostream>
#include <algorithm>
#include <chrono>
#include <list>
#include <iterator>
#include <fstream>
#include <stdlib.h>
#include <malloc.h>

#define MAX_SIZE 16382

// import from standard library
using std::list;
using std::sort;
using std::ofstream;
using std::chrono::duration_cast;
using std::chrono::nanoseconds;
using the_clock = std::chrono::steady_clock;

using namespace std;

const int iterations = 101; // number of iterations for our program - 101 so it makes it easier to do medians
const int smallsize = 100; // size of our first array - 100 elements (no's 1 - 100)
const int mediumsize = 1000; // size of our second array - 1,000 elements (no's 1 - 1,000)
const int largesize = 10000; // size of our third array - 10,000 elements (no's 1 - 10,000)

// destroy our arrays that are holding our times ready to begin with the next test
template <typename T>
void destroy_array(T& list) { // pass in list
	list.clear(); // remove all elements from the passed in list
	for (std::list<int>::iterator it = list.begin(); it != list.end(); it++) // use an iterator to iterate through the list
	{
		std::cout << " " << *it; // print something to show we have deleted the elements
	}
	std::cout << "\n"; // newline
}

// fill array with elements from 1 to whatever the size of the array is
template <typename T>
void fill_array(T& list, const int size_of_array) { // pass in list and the size of the array 
	for (int i = 0; i < size_of_array; i++) { // for each element in the list until we reach the size of the array do...
		list[i] = i + 1; // fill the array with numbers 1 to size_of_array
	}
}

// display our array
template <typename T>
void display(T& list) { // pass in list
	for (auto item : list) { // using auto allows the compiler to decide what the data type is at compilation time
		cout << " " << item; // item is the syntactic sugar for our list, printing out each element
	}
	cout << " \n"; // new line
}

// perform a linear search and store the positions of the items we find
template <typename T>
int linear_search(T& list, const int listsize, int item) { // pass in list, listsize and the value we wish to find 
	for (int i = 0; i < listsize; i++) { // for each element in listsize do...
		if (list[i] == item) { // if the current element is equal to itended item
			return i; // we have found the item so back out from the function
		}
	}
	return -1; // we haven't found the item so return -1
}

template <typename T>
int binary_search(T& list, int begin, int end, int unsigned item) { // pass in list, beginning of array, end of array, value we wish to find 
	while (begin <= end) { // checks to ensure that the begin and end elements are valid, i.e. exist in the array and validly spliced
		int unsigned middle = (begin + (end - 1)) / 2; // we need the middle and this will help us splice the array ( / 2) until our value is found
		if (item < list[middle]) { // if the item we want is found to be in the left part of the subarray then
			end = middle; // we know it is in the left side of the array so we narrow our search down to the left part of the array
		}
		else if (item > list[middle]) { // else if the item we want is found to be in the right subarray
			begin = middle + 1; // we know it is in the right subarray so narrow the search down to the right part of the array
		}
		else // else if the above conditions are not true and that by continually splicing the array then...
		{
			return middle; // we have found the value we are looking for so back out of the function
		}
	}
	return -1; // if we have not found our value return -1
}

template <typename T>
void run_linear(std::list<int> times, T& search_array, const int size, unsigned int value, string file_name) {
	int identifier = 0; // used to help me keep track of what iteration was which
	int time_l = 0; // used to contain the value that will hold the time it took to find the value we wanted to search
	int result = 0; // checks if the value was found from the search
	for (int i = 0; i < iterations; i++) { // start the 101 iterations
		the_clock::time_point start_l = the_clock::now(); // record the current time
		// pass in our array to be searched, the array size, the item we wish to find
		result = linear_search(search_array, size, value); // bottleneck here as re-initialising data tye of auto
		the_clock::time_point end_l = the_clock::now(); // record the current time a second time
		if (result == -1) // if the item wasn't found
		{
			cout << "item not found" << endl; // item wasn't found
		}
		else
		{
			time_l = duration_cast<nanoseconds>(end_l - start_l).count(); // using the values we obtained above, end time subtract start time in nanseconds to find out how long it took to compute the function
			//cout << time_l << " ns." << endl; // displays the time in nanoseconds to user
			times.push_back(time_l); // add this time to a std::list that will be used to store all times - in ns. - for this test
		}
	}
	//cout << "\n";
	ofstream l_times(file_name); // make a csv. file with a passed in name
	l_times << "Item, No. (not iterations), Times(ns.)" << endl; // make our column headers
	times.sort(); // sort the times (needed for median and graphs later)
	for (auto ltime : times) { // use ltime for syntatic sugar for our times
		l_times << value << "," << identifier << "," << ltime << endl; // value we want to find, no of iteration, time (in ms.)
		identifier++; // increment identifier
	}
	destroy_array(times); // erase all the elements from the times list so we can begin the next test
	// continue and move onto the next test
}

template <typename T>
void run_binary(std::list<int> times, T& search_array, unsigned int value, string file_name) {
	int identifier = 0; // keep track of what iterations are which (not accurate as the list will be sorted)
	int time_b = 0; // used to hold our time in nanoseconds of how long it took the algorithm to find our value
	int result = 0; // used to check if our item was found
	for (int i = 0; i < iterations; i++) { // start the iterations
		the_clock::time_point start_b = the_clock::now(); // record the start time
		// pass in our array to be searched, the first element of the array, the number of bytes in the array, the item we wish to find
		result = binary_search(search_array, 0, sizeof(search_array) / sizeof(search_array[0]), value); // bottleneck here as re-initialising data type of auto
		the_clock::time_point end_b = the_clock::now(); // record the end time
		if (result == -1) // if the item wasn't found
		{
			cout << "item not found" << endl; // then say so
		}
		else {
			// find the time in nanoseconds by calculating end time minus start time
			time_b = duration_cast<nanoseconds>(end_b - start_b).count(); // bottleneck here before as I kept reinitialising the data type
			//cout << time_b << " ns." << endl; // display the time in nanoseconds
			times.push_back(time_b); // add this new time to our list
		}
	}
	//cout << "\n";
	ofstream b_times(file_name); // make a csv. file
	b_times << "Item, No. (not iterations), Times(ns.)" << endl; // define column headers
	times.sort(); // sort the times list (for medians and graphs later)
	for (auto btime : times) { // loop through the list
		b_times << value << "," << identifier << "," << btime << endl; // fill in each row with column values
		identifier++; // increment
	}
	destroy_array(times); // destroy the array so we can use it for the next test
}

int main(int argc, char** argv) {
	/* Section start */
	char* buffer; 
	buffer = (char*)malloc(MAX_SIZE);
	if (buffer != NULL)
	{
		buffer[0] = '\0';
		// section here was implemented to try to fix annoying warnings
		/* Section end */
		unsigned int small[smallsize]; // our small array (1 - 100)
		unsigned int medium[mediumsize]; // our medium array (1 - 1,000)
		unsigned int large[largesize]; // our large array (1 - 10,000)
		std::list<int> linear_times; // list for our times for our linear search
		std::list<int> binary_times; // list for our times for our binary search

		/////////

		// test 1 - searching for 29 and 67 within a list size of 100
		fill_array(small, smallsize); // fill our array with numbers 1 to 100
		//display(small);
		//cout << "\n"; cout << "\n";
		// test 1a - search for 29 within the list
		//cout << "Test 1a (search for 29) - \n";
		//cout << "Results of linear search: \n";
		// find the value 29 inside the 'small' array using linear search and save the results
		run_linear(linear_times, small, smallsize, 29, "test1a_linear.csv");
		//cout << "Results of binary search: \n";
		//find the value 29 inside the 'small array' using binary search and save the results 
		run_binary(binary_times, small, 29, "test1a_binary.csv");
		//cout << "\n";
		// test 1b - searching for 67
		//cout << "Test 1b (search for 67) - \n";
		//cout << "Results of linear search: \n";
		// find the value 67 inside the 'small' array using linear search and save the results
		run_linear(linear_times, small, smallsize, 67, "test1b_linear.csv");
		//cout << "Results of binary search: \n";
		//find the value 67 inside the 'small' array using binary search and save the results 
		run_binary(binary_times, small, 67, "test1b_binary.csv");
		//cout << "\n";

		/////////

		// test 2 - searching for 961 and 333 within a list size 1000
		fill_array(medium, mediumsize); // fill our medium sized array with 1 - 1000
		//display(medium);
		//cout << "\n"; cout << "\n";
		// test 2a - searching for 333 
		//cout << "Test 2a (search for 333) - \n";
		//cout << "Results of linear search: \n";
		// find the value 333 inside our 'medium' array using linear search and save the results
		run_linear(linear_times, medium, mediumsize, 333, "test2a_linear.csv");
		//cout << "Results of binary search: \n";
		//find the value 333 inside our 'medium' array using binary search and save the results 
		run_binary(binary_times, medium, 333, "test2a_binary.csv");
		//cout << "\n";
		// test 2b searching for 961
		//cout << "Test 2b (search for 961) - \n";
		//cout << "Results of linear search: \n";
		// find the value 961 inside our 'medium' array using linear search and save the results
		run_linear(linear_times, medium, mediumsize, 961, "test2b_linear.csv");
		//cout << "Results of binary search: \n";
		//find the value 961 inside our 'medium' array using binary search and save the results 
		run_binary(binary_times, medium, 961, "test2b_binary.csv");
		//cout << "\n";

		/////////

		// test 3 - searching for 1191 and 9979 within a list size of 10000
		fill_array(large, largesize);
		//display(large);
		//cout << "\n"; cout << "\n";
		// test 3a - searching for 1191
		//cout << "Test 3a (search for 1191) - \n";
		//cout << "Results of linear search: \n";
		// find the value 1191 inside our 'large' array using linear search and save the results
		run_linear(linear_times, large, largesize, 1191, "test3a_linear.csv");
		//cout << "Results of binary search: \n";
		//find the value 1191 inside our 'large' array using binary search and save the results 
		run_binary(binary_times, large, 1191, "test3a_binary.csv");
		//cout << "\n";
		// test 3b - searching for 9979
		//cout << "Test 3b (search for 9979) - \n";
		//cout << "Results of linear search: \n";
		// find the value 9979 inside our 'large' array using linear search and save the results
		run_linear(linear_times, large, largesize, 9979, "test3b_linear.csv");
		//cout << "Results of binary search: \n";
		//find the value 9979 inside our 'large' array using binary search and save the results 
		run_binary(binary_times, large, 9979, "test3b_binary.csv");
		//cout << "\n";

		/////////

		// test 4 - searching for 29 and 67 within a list size of 1000
		//display(medium);
		//cout << "\n"; cout << "\n";
		// test 4a - search for 29 within the list
		//cout << "Test 4a (search for 29) - \n";
		//cout << "Results of linear search: \n";
		// find the value 29 inside our 'medium' array using linear search and save the results
		run_linear(linear_times, medium, mediumsize, 29, "test4a_linear.csv");
		//cout << "Results of binary search: \n";
		//find the value 29 inside our 'medium' array using binary search and save the results 
		run_binary(binary_times, medium, 29, "test4a_binary.csv");
		//cout << "\n";
		// test 4b - searching for 67
		//cout << "Test 4b (search for 67) - \n";
		//cout << "Results of linear search: \n";
		// find the value 67 inside our 'medium' array using linear search and save the results
		run_linear(linear_times, medium, mediumsize, 67, "test4b_linear.csv");
		//cout << "Results of binary search: \n";
		//find the value 67 inside our 'medium' array using binary search and save the results 
		run_binary(binary_times, medium, 67, "test4b_binary.csv");
		//cout << "\n";

		/////////

		// test 5 - searching for 29 and 67 within a list size of 10000
		//display(large);
		//cout << "\n"; cout << "\n";
		// test 5a - search for 29 within the list
		//cout << "Test 5a (search for 29) - \n";
		//cout << "Results of linear search: \n";
		// find the value 29 inside our 'large' array using linear search and save the results
		run_linear(linear_times, large, largesize, 29, "test5a_linear.csv");
		//cout << "Results of binary search: \n";
		//find the value 29 inside our 'large' array using binary search and save the results 
		run_binary(binary_times, medium, 29, "test5a_binary.csv");
		//cout << "\n";
		// test 5b - searching for 67
		//cout << "Test 5b (search for 67) - \n";
		//cout << "Results of linear search: \n";
		// find the value 67 inside our 'large' array using linear search and save the results
		run_linear(linear_times, large, largesize, 67, "test5b_linear.csv");
		//cout << "Results of binary search: \n";
		//find the value 67 inside our 'large' array using binary search and save the results 
		run_binary(binary_times, large, 67, "test5b_binary.csv");
		//cout << "\n";

		/////////

		// test 6 - searching for 961 and 333 within a list size 10000
		//display(large);
		//cout << "\n"; cout << "\n";
		// test 6a - search for 333 within the list
		//cout << "Test 6a (search for 333) - \n";
		//cout << "Results of linear search: \n";
		// find the value 333 inside our 'large' array using linear search and save the results
		run_linear(linear_times, large, largesize, 333, "test6a_linear.csv");
		//cout << "Results of binary search: \n";
		//find the value 333 inside our 'large' array using binary search and save the results 
		run_binary(binary_times, large, 333, "test6a_binary.csv");
		//cout << "\n";
		// test 6b - searching for 961
		//cout << "Test 6b (search for 961) - \n";
		//cout << "Results of linear search: \n";
		// find the value inside our 'large' array 333 using linear search and save the results
		run_linear(linear_times, large, largesize, 333, "test6b_linear.csv");
		//cout << "Results of binary search: \n";
		//find the value 333 inside our 'large' array using binary search and save the results 
		run_binary(binary_times, large, 333, "test6b_binary.csv");
		//cout << "\n";

		/////////

		free(buffer);
	}

	cout << "Enter to end program";
	cin.get();
	
	return 0;
}