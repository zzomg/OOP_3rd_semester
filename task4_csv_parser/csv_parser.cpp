#include <iostream>
#include <tuple>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iterator>
#include <type_traits>

//#include "csv_parser.h"

template <size_t n, typename... Args>
typename std::enable_if<(n >= sizeof...(Args))>::type
	print_tuple(std::ostream&, const std::tuple<Args...>&)
{}

template <size_t n, typename... Args>
typename std::enable_if<(n < sizeof...(Args))>::type
	print_tuple(std::ostream& os, const std::tuple<Args...>& tup)
{
	if (n != 0)
		os << ", ";
	os << std::get<n>(tup);
	print_tuple<n + 1>(os, tup);
}

template<typename... Args>
std::ostream& operator<<(std::ostream& os, const std::tuple<Args...>& tup)
{
	os << "(";
	print_tuple<0>(os, tup);
	return os << ")";
}

template <typename T>
T convert(const std::string& param)
{
	T t;
	std::stringstream ss(param);

	ss >> t;

	return T(t);
}

template <>
std::string convert(const std::string& param) //ничего конвертировать не нужно
{
	return param;
}

template <size_t n, typename... Args>
typename std::enable_if<(n >= sizeof...(Args))>::type
read_tuple(const std::vector<std::string>& columns, std::tuple<Args...>& tup)
{
}

template <size_t n, typename... Args>
typename std::enable_if<(n < sizeof...(Args))>::type
	read_tuple(const std::vector<std::string>& columns, std::tuple<Args...>& tup)
{
	std::get<n>(tup) = convert<std::tuple_element<n, std::tuple<Args...>>::type>(columns[n]);
	read_tuple<n + 1>(columns, tup);
}

template<typename... Args>
class CSVParser
{
	std::ifstream& file_;
	size_t start_;
	size_t cnt_;

public:

	CSVParser(std::ifstream& file, size_t start = 0) : file_(file), start_(start) 
	{
		std::string line;
		for (size_t i = 0; i < start_; ++i) {
			std::getline(file_, line);
		}
	}
	~CSVParser() {}

	class CSVRow
	{
		std::tuple<Args...> data_;

	public:

		friend class CSVParser<Args...>;

		operator std::tuple<Args...>() const {
			return data_;
		}

		std::istream& readNextRow(std::istream& str)
		{
			std::string line;
			std::getline(str, line);

			std::vector<std::string> data;

			std::stringstream lineStream(line);
			std::string cell;

			while (std::getline(lineStream, cell, ',')) {	
				data.push_back(cell);
			}

			if (data.empty()) {
				return str;
			}

			if (!lineStream && cell.empty()) {
				data.push_back("");
			}

			if (data.size() != sizeof... (Args)) {
				throw std::exception("Number of template arguments must correspond to the number of words in each line in file\n");
			}

			read_tuple<0>(data, data_);

			return str;
		}
	};

	class CSVIterator
	{
		std::istream* str_;
		CSVRow row_;

	public:

		typedef std::input_iterator_tag iterator_category;
		typedef CSVRow value_type;
		typedef std::size_t difference_type;
		typedef CSVRow* pointer;
		typedef CSVRow& reference;

		CSVIterator(std::istream& str) : str_(str.good() ? &str : NULL) 
		{ 
			++(*this); 
		}
		CSVIterator() : str_(NULL) {}
		~CSVIterator() {}

		CSVIterator& operator++() { 
			if (str_) 
			{ 
				row_.readNextRow((*str_));
				if (!(*str_))
				{
					str_ = NULL; 
				} 
			}
			return *this; 
		}

		CSVIterator operator++(int) {
			CSVIterator tmp(*this); 
			++(*this); 
			return tmp; 
		}
		CSVRow const& operator*() const { return row_; }
		CSVRow const* operator->()  const { return &row_; }

		bool operator==(CSVIterator const& rhs) { 
			return ((this == &rhs) || ((this->str_ == NULL) && (rhs.str_ == NULL))); 
		}
		bool operator!=(CSVIterator const& rhs) { 
			return !((*this) == rhs); 
		}
	};

	friend class CSVRow;

	typedef CSVIterator iterator;

	iterator begin() {
		return CSVIterator(file_);
	}

	iterator end() {
		return CSVIterator();
	}

};

int main()
{
	std::ifstream file("test.txt");
	CSVParser<int, int, std::string> parser(file, 1);

	try {
		for (std::tuple<int, int, std::string> s : parser)
		{
			std::cout << s << std::endl;
		}
	}
	catch (const std::exception & ex)
	{
		std::cout << "Error : " << ex.what() << std::endl;
	}

	return 0;
}
