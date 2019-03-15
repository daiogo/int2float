/*
	int2float
	by Diogo Freitas
*/

/*******************************************************************************
*	INCLUDES
*******************************************************************************/
#include "int2float.hpp"

/*******************************************************************************
*	DEFINITIONS
*******************************************************************************/
// Integer max values
#define INT_MAX					2147483647
#define INT_MIN					-2147483648 + 1
// Needs + 1 because it overflows when transforming to a positive number in conversion method

// Value found empirically (also, about 35% of the numbers are not exactly represented)
#define NO_ERROR_INT_MAX		16777218
#define NO_ERROR_INT_MIN		-16777218

// Scope of variable to be converted
#define TEST_MAX				1000
#define TEST_MIN				0

// Debug flags (please, comment it when doesn't need to debug function)
//#define TESTDEBUG

/*******************************************************************************
*	CODE
*******************************************************************************/
int main()
{
	// Test for range of numbers defined in the definition section
	int test_range_lower_bound = NO_ERROR_INT_MAX - 5;
	int test_range_upper_bound = NO_ERROR_INT_MAX + 5;

	// Test value
	int num = test_range_lower_bound;
	float_union type_casted_num;
	float_union converted_num;

	// Counter to be incremented when test failed
	unsigned int inaccurate_conversions = 0;
	unsigned int not_exact_conversions = 0;

	// Test loop iterates through range of values
	while (num >= test_range_lower_bound && num <= test_range_upper_bound)
	{
		// Update comparison values
		type_casted_num.f = (float)num;
		converted_num.f = int2float(num);

#ifdef TESTDEBUG
		// Prints for test only
		std::cout << "         Original value: " << num << std::endl;
		std::cout << "      Type Casted value: " << std::setprecision(15) << type_casted_num.f << std::endl;
		std::cout << "        Converted value: " << std::setprecision(15) << converted_num.f << std::endl;
		// Compare bitsets
		std::bitset<32> type_casted_bitset(type_casted_num.u);
		std::cout << " Type casted bit string: " << type_casted_bitset << std::endl;
		std::bitset<32> converted_num_bitset(converted_num.u);
		std::cout << "   Converted bit string: " << converted_num_bitset << std::endl;
		std::cout << "--------------------------------------------------------" << std::endl;
#endif
		// If bits don't match, then display warning message
		if (type_casted_num.f != converted_num.f)
		{
			not_exact_conversions++;
#ifdef TESTDEBUG
			// Print values for visual reference
			std::cout << "*******************************" << std::endl;
			std::cout << " WARNING! Not exact conversion" << std::endl;
			std::cout << "   Original value: " << num << std::endl;
			std::cout << "Type Casted value: " << std::setprecision(15) << type_casted_num.f << std::endl;
			std::cout << "  Converted value: " << std::setprecision(15) << converted_num.f << std::endl;
			std::cout << "*******************************" << std::endl;
#endif		
			// Exit loop
			//break;
			// Comment if you don't wish the program to stop when it finds a not exact value
		}
		num++;
	}

	// Displays test results
	std::cout << "*******************************" << std::endl;
	std::cout << "Tests completed!" << std::endl;
	std::cout << "Not exact conversions: " << not_exact_conversions << std::endl;
	std::cout << "Innacurate conversions: " << inaccurate_conversions << std::endl;
	std::cout << "*******************************" << std::endl;

	return 42;
}