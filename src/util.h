#ifndef LIBNCF_UTIL_H_INCLUDED
#define LIBNCF_UTIL_H_INCLUDED

#include <string>

#ifdef _WIN32
#define CONFIG_DELIM_PATH '\\'
#else
#define CONFIG_DELIM_PATH '/'
#endif /* _WIN32 */

namespace Util {
	namespace String {

void   to_lower_case(std::string &s);           // Convert to lower case.
void   to_upper_case(std::string &s);           // Convert to upper case.
bool   to_bool(const std::string &s);           // Convert to boolean type.
int    to_int(const std::string &s);            // Convert to integer type.
float  to_float(const std::string &s);          // Convert to float type.
double to_double(const std::string &s);         // Convert to double type.
void   to_string(std::string &out, bool val);   // Convert to string.
void   to_string(std::string &out, int val);    // Convert to string.
void   to_string(std::string &out, float val);  // Convert to string.
void   to_string(std::string &out, double val); // Convert to string.

void strip(std::string &s, char c); // Removes all occurrences of the given character.
void unquote(std::string &s);       // Trims '"' at start and end.
void trim_left(std::string &s);     // Trims ' ', '\t' at the start of the given string.
void trim_right(std::string &s);    // Trims ' ', '\n', '\t', '\r' at the end.
void trim(std::string &s);          // Trims ' ', '\n', '\t', '\r' at the start and end.

// Splits the given array into two string at first occurance of given char.
void split(std::string in, std::string &left, std::string &right, char c);
// Extracts the first balanced substring included between two pX chars.
void extract(std::string &s, std::string &t, const char p1, const char p2);
// Extracts the base name and path from a string.
void path_comp(const std::string &s, std::string &base, std::string &file, const char delim = CONFIG_DELIM_PATH);
// Replace a matched substring p with r.
void replace_first_of(std::string &s, const std::string &p, const std::string &r);

	} /* namespace String */
} /* namespace Util */

#endif /* LIBNCF_UTIL_H_INCLUDED */
