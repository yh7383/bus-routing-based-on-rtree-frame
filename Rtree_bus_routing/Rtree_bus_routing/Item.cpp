#include "Item.h"

int str_2_int(string s) {
	return atoi(s.c_str());
}
double str_2_double(string s) {
	return atof(s.c_str());
}
string _2_lower(string s) {
	for (int i = 0; i < s.length(); i++) {
		if (s[i] >= 'A' && s[i] <= 'Z')
			s[i] += 32;
	}
	return s;
}