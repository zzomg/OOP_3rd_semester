#pragma once

using uint = unsigned int;

enum class Trit : uint { False = 2, Unknown = 0, True = 1 };
//logic values: False = 10, Unknown = 00, True = 01

Trit operator | (Trit left, Trit right);
Trit operator & (Trit left, Trit right);
Trit operator ~ (Trit value);
