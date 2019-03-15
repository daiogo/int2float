/*
	int2float
	by Diogo Freitas
	Following steps as seen in:
	https://cs.nyu.edu/courses/spring16/CSCI-GA.1144-001/IEEE_754_Note.pdf
*/
/*******************************************************************************
*	INCLUDES
*******************************************************************************/
#include "int2float.hpp"

/*******************************************************************************
*	DEFINITIONS
*******************************************************************************/
#define SIGN_MASK				0x80000000
#define EXPONENT_MASK			0x7F800000
#define MANTISSA_MASK			0x007FFFFF
#define INT_SIZE_IN_BITS		32
#define SIGN_SIZE_IN_BITS		1
#define EXPONENT_SIZE_IN_BITS	8
#define MANTISSA_SIZE_IN_BITS	23
#define EXPONENT_SHIFT			127

// Debug flags (please, comment it when doesn't need to debug function)
//#define BINDEBUG
//#define DEBUG

/*******************************************************************************
*	FUNCTION DEFINITIONS
*******************************************************************************/
float int2float(int num)
{
	// Buffer variables
	unsigned int sign = 0;
	unsigned int mantissa = 0;
	unsigned int exponent = 0;
	unsigned int int_buffer = 0;
	unsigned int float_buffer = 0;
	unsigned int leading_one_mask = SIGN_MASK >> 1;

	// Counters
	int leading_zeros_counter = 0;
	int required_trailing_zeros = 0;
	int nof_binary_digits_after_decimal_point = 0;

	// Return value
	float return_value = 0.0;

	// If it is zero, return 0
	if (num == 0)
	{
		return 0.0;
	}

#ifdef BINDEBUG
	std::bitset<32> num_bitset(num);
	std::cout << "      Original integer bit string: " << num_bitset << std::endl;
#endif

	// Step 1: Observe the sign
	// Extract sign bit from integer
	sign = ((num & SIGN_MASK) >> (EXPONENT_SIZE_IN_BITS + MANTISSA_SIZE_IN_BITS));

	// Step 2: From decimal to binary
	// Convert negative values to their Natural counterpart and store in a buffer
	if (num < 0)
	{
		int_buffer = -num;
	}
	else
	{
		int_buffer = num;
	}

#ifdef BINDEBUG
	std::bitset<32> int_buffer_bitset(int_buffer);
	std::cout << " Positive integer value in buffer: " << int_buffer_bitset << std::endl;
#endif

	// Step 3: Move to scientific notation and get significant
	// Count leading zeros excluding sign bit
	while (((int_buffer & leading_one_mask) == 0) && (leading_zeros_counter < INT_SIZE_IN_BITS))
	{
		leading_one_mask >>= 1;
		leading_zeros_counter++;
	}

	// If no leading zeros were found, then reset counter
	if (leading_zeros_counter >= INT_SIZE_IN_BITS)
	{
		leading_zeros_counter = 0;
		// We also know that mantissa == 0
	}

#ifdef DEBUG
	std::cout << "Leading zeros counter: " << leading_zeros_counter << std::endl;
#endif

#ifdef BINDEBUG
	std::bitset<32> leading_one_mask_bitset(leading_one_mask);
	std::cout << "                 Leading one mask: " << leading_one_mask_bitset << std::endl;
#endif

	// Calculate number of digits after point
	nof_binary_digits_after_decimal_point = INT_SIZE_IN_BITS - (leading_zeros_counter + 2);
	// + 2 in the end is meant to count sign bit plus mantissa's implicit leading 1

#ifdef DEBUG
	std::cout << "Nof binary digits after decimal point: " << nof_binary_digits_after_decimal_point << std::endl;
#endif

	// Calculate required trailing zeros in mantissa
	if (nof_binary_digits_after_decimal_point >= MANTISSA_SIZE_IN_BITS)
	{
		required_trailing_zeros = 0;
	}
	else
	{
		required_trailing_zeros = MANTISSA_SIZE_IN_BITS - nof_binary_digits_after_decimal_point;
	}

#ifdef DEBUG
	std::cout << "Required trailing zeros: " << required_trailing_zeros << std::endl;
#endif

	// If there are more digits after the point than bits available in the mantissa
	// Then, extract most significant bits from num
	if (nof_binary_digits_after_decimal_point >= MANTISSA_SIZE_IN_BITS)
	{
		int_buffer >>= (nof_binary_digits_after_decimal_point - MANTISSA_SIZE_IN_BITS);
	}

#ifdef BINDEBUG
	std::bitset<32> digits_after_decimal_point_bitset(int_buffer);
	std::cout << "Binary digits after decimal point: " << digits_after_decimal_point_bitset << std::endl;
#endif

	// REMOVED FROM ORIGINAL! leading_one_mask >>= (-nof_binary_digits_after_decimal_point);

	// Clear first non-zero bit and add required trailing zeros
	mantissa = (int_buffer & ~(leading_one_mask)) << required_trailing_zeros;

	// Step 4: Calculate exponent in binary
	// Sum number of binary digits after point with exponent shift constant (127)
	exponent = nof_binary_digits_after_decimal_point + EXPONENT_SHIFT;

	// Step 5: Combine sign, exponent and significant
	// Put all fields together
	float_buffer = sign << (EXPONENT_SIZE_IN_BITS + MANTISSA_SIZE_IN_BITS);
	float_buffer |= exponent << MANTISSA_SIZE_IN_BITS;
	float_buffer |= mantissa;

#ifdef BINDEBUG
	std::cout << std::endl << "Results" << std::endl;
	
	float_union type_casted_num;
	type_casted_num.f = (float) num;

	std::bitset<32> original_bitset(num);
	std::cout << "Original int bit string: " << original_bitset << std::endl;

	std::bitset<32> typecasted_bitset(type_casted_num.u);
	std::cout << " Type casted bit string: " << typecasted_bitset << std::endl;
	
	std::bitset<32> float_buffer_bitset(float_buffer);
	std::cout << "   Converted bit string: " << float_buffer_bitset << std::endl;

	std::bitset<1> sign_bitset(sign);
	std::bitset<8> exponent_bitset(exponent);
	std::bitset<23> mantissa_bitset(mantissa);
	std::cout << "S   Exponent   Mantissa" << std::endl;
	std::cout << sign_bitset << " | " << exponent_bitset << " | " << mantissa_bitset << " | " << std::endl;
#endif

	// Copy value to a float variable
	memcpy(&return_value, &float_buffer, sizeof float_buffer);

	// Return floating point variable
	return return_value;
}
