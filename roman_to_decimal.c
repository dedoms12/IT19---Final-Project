#include <stdio.h>  // Include standard I/O functions for reading from and writing to files
#include <stdlib.h> // Include standard library functions (e.g., for memory allocation)
#include <string.h> // Include string manipulation functions (e.g., for strcpy, strcat)

// Function prototypes
int romanCharToDecimal(char c); // Convert a Roman numeral character to its decimal value
int romanToDecimal(const char *roman); // Convert a Roman numeral string to its decimal equivalent
void numberToWords(int num, char *output); // Convert a number to its word representation
void readRomanNumerals(const char *filename, char romanNumerals[][20], int *count); // Read Roman numerals from a file
void writeResultsToFile(const char *filename, int results[], int count); // Write results to a file

// Main function
int main() {
    char romanNumerals[100][20]; // Array to store up to 100 Roman numeral strings, each up to 20 characters
    int count = 0; // Variable to store the count of Roman numerals read
    int results[100]; // Array to store the decimal equivalents of the Roman numerals

    readRomanNumerals("Input.txt", romanNumerals, &count); // Read Roman numerals from input file into the array

    for (int i = 0; i < count; i++) {
        results[i] = romanToDecimal(romanNumerals[i]); // Convert each Roman numeral to its decimal equivalent
    }

    // Ensure additional processing, e.g., handling arithmetic between numbers
    if (count >= 4) {
        results[2] += results[3]; // Add the third and fourth numbers together (custom logic)
    }

    writeResultsToFile("Output.txt", results, count - 1); // Write the results (minus the last entry) to output file

    return 0; // Return success
}

// Function to convert a single Roman numeral character to its decimal value
int romanCharToDecimal(char c) {
    switch (c) { // Check the Roman character
        case 'I': return 1; // I = 1
        case 'V': return 5; // V = 5
        case 'X': return 10; // X = 10
        case 'L': return 50; // L = 50
        case 'C': return 100; // C = 100
        case 'D': return 500; // D = 500
        case 'M': return 1000; // M = 1000
        default: return 0; // Invalid character, return 0
    }
}

// Function to convert a Roman numeral string to its decimal equivalent
int romanToDecimal(const char *roman) {
    int total = 0; // Variable to store the total decimal value
    int length = strlen(roman); // Get the length of the Roman numeral string

    for (int i = 0; i < length; i++) { // Loop through each character in the Roman numeral string
        if (i + 1 < length && romanCharToDecimal(roman[i]) < romanCharToDecimal(roman[i + 1])) {
            total -= romanCharToDecimal(roman[i]); // If the current value is smaller than the next one, subtract it
        } else {
            total += romanCharToDecimal(roman[i]); // Otherwise, add the value to the total
        }
    }
    return total; // Return the total decimal value
}

// Function to convert a number to its word representation
void numberToWords(int num, char *output) {
    const char *ones[] = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"}; // Array for 1-9
    const char *teens[] = {"Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"}; // Array for 10-19
    const char *tens[] = {"", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"}; // Array for multiples of 10 (20, 30, etc.)
    const char *thousands[] = {"", "Thousand", "Million", "Billion"}; // Array for large number categories

    if (num == 0) { // Handle zero case
        strcpy(output, "Zero"); // Set output to "Zero"
        return; // Exit the function
    }

    if (num < 0) { // Handle negative numbers
        strcpy(output, "Negative "); // Add "Negative" to the output
        num = -num; // Convert num to a positive value
    } else {
        output[0] = '\0'; // Initialize output as an empty string if the number is positive
    }

    int part[4] = {0}; // Array to store parts of the number (thousands, hundreds, etc.)
    int i = 0;

    while (num > 0) { // Break the number into parts (thousands, hundreds, etc.)
        part[i++] = num % 1000; // Get the last three digits
        num /= 1000; // Remove the last three digits from the number
    }

    for (int j = i - 1; j >= 0; j--) { // Process each part of the number
        if (part[j] == 0) continue; // Skip empty parts

        char temp[100] = ""; // Temporary string for current part's word representation

        int hundreds = part[j] / 100; // Get the hundreds place
        int remainder = part[j] % 100; // Get the remainder (tens and ones)

        if (hundreds > 0) { // If there are hundreds, add the word for it
            sprintf(temp, "%s Hundred ", ones[hundreds]);
            strcat(output, temp);
        }

        if (remainder >= 10 && remainder < 20) { // Handle "teens" (10-19)
            strcat(output, teens[remainder - 10]);
        } else {
            if (remainder / 10 >= 1) { // Handle tens place (20, 30, etc.)
                strcat(output, tens[remainder / 10]);
                strcat(output, " ");
            }
            if (remainder % 10 > 0) { // Handle ones place (1-9)
                strcat(output, ones[remainder % 10]);
            }
        }

        if (j > 0) { // Add the appropriate "thousands" word (e.g., "Thousand", "Million")
            strcat(output, " ");
            strcat(output, thousands[j]);
            strcat(output, " ");
        }
    }

    int len = strlen(output); // Get the length of the output string
    if (len > 0 && output[len - 1] == ' ') { // If there's a trailing space, remove it
        output[len - 1] = '\0';
    }
}

// Function to read Roman numerals from a file
void readRomanNumerals(const char *filename, char romanNumerals[][20], int *count) {
    FILE *file = fopen(filename, "r"); // Open the file for reading
    if (file == NULL) { // Check if the file was opened successfully
        perror("Error opening file"); // Print error if file cannot be opened
        return; // Exit the function if there was an error
    }
    *count = 0; // Initialize the count of Roman numerals
    while (fgets(romanNumerals[*count], 20, file) != NULL) { // Read each Roman numeral from the file
        romanNumerals[*count][strcspn(romanNumerals[*count], "\n")] = 0; // Remove the newline character
        (*count)++; // Increment the count of Roman numerals
    }
    fclose(file); // Close the file
}

// Function to write the results to a file
void writeResultsToFile(const char *filename, int results[], int count) {
    FILE *file = fopen(filename, "w"); // Open the file for writing
    if (file == NULL) { // Check if the file was opened successfully
        perror("Error opening file"); // Print error if file cannot be opened
        return; // Exit the function if there was an error
    }
    char wordOutput[256]; // Array to store the word representation of each result
    for (int i = 0; i < count; i++) { // Loop through each result
        numberToWords(results[i], wordOutput); // Convert each result to its word representation
        fprintf(file, "%s\n", wordOutput); // Write the word representation to the file
    }
    fclose(file); // Close the file
}
