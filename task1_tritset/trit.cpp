#include "trit.h"

// Performs logic operation OR on trits
Trit operator | (Trit left, Trit right)
{
	if (left == Trit::True || right == Trit::True) {
		return Trit::True;
	}
	else if (left == Trit::False && right == Trit::False) {
		return Trit::False;
	}
	else return Trit::Unknown;
}

// Performs logic operation AND on trits
Trit operator & (Trit left, Trit right)
{
	if (left == Trit::False || right == Trit::False) {
		return Trit::False;
	}
	else if (left == Trit::True && right == Trit::True) {
		return Trit::True;
	}
	else return Trit::Unknown;
}

// Performs logic operation NOT on trit
Trit operator ~ (Trit value)
{
	if (value == Trit::True) {
		return Trit::False;
	}
	else if (value == Trit::False) {
		return Trit::True;
	}
	else return Trit::Unknown;
}
