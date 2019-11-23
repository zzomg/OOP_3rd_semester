#include <iostream>
#include <tuple>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iterator>
#include <type_traits>

/*Методы для вывода tuple'ов*/

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


/*std::istream& operator>>(std::istream& str, CSVParser::CSVRow& data)
{
	data.readNextRow(str);
	return str;
}*/

template<typename First, typename... Args>
class CSVParser
{
	std::ifstream& file_;
	size_t start_;
	std::tuple<Args...> t_;
	std::vector<std::string> vec_;

public:

	//CSVParser(std::ifstream& file, Args&&...args, size_t start = 0) : file_(file), start_(start), args_(std::make_tuple(std::forward<Args>(args)...)) {}
	//CSVParser() {}
	CSVParser(std::ifstream& file, size_t start = 0) : file_(file), start_(start) {}
	~CSVParser() {}

	/*template <typename A>
	void process_one_type() 
	{
		std::cout << typeid(A).name() << ' ';
		vec_.push_back(typeid(A).name());
	}

	template <typename First, typename... Args>
	void func()
	{
		vec_.push_back(typeid(First).name());
		int _[] = {0, (process_one_type<Args>(), 0)... };
		(void)_;
		std::cout << '\n';
		for (auto& v : vec_) 
		{
			std::cout << v << std::endl;
		}
	}

	template <typename First, typename... Args>
	void get_types() 
	{
		func<Args>();
		std::tuple<First> аb = std::make_tuple(3);
		std::cout << ab << std::endl;
	}*/

	class CSVRow
	{
		std::vector<std::string> data_;
		//std::tuple<Args...> data_;

	public:

		std::tuple<Args...> data() const
		{
			return data_;
		}

		std::size_t size() const
		{
			return data_.size();
		}

		/*template <typename T>
		bool read_tuple_element(std::istream& in, T& value) {
			std::cout << value << std::endl;
			in >> value;
			return true;
		}

		template <typename Tuple, std::size_t... I>
		void read_tuple_elements(std::istream& in, Tuple& value, std::index_sequence<I...>) {
			std::initializer_list<bool>{ read_tuple_element(in, std::get<I>(value))...};
		}

		void readNextRow(std::istream& str)
		{
			read_tuple_elements(str, data_, std::make_index_sequence<sizeof...(Args)>{});
		}*/

		std::istream& readNextRow(std::istream& str)
		{
			std::string line;
			std::getline(str, line);

			std::stringstream lineStream(line);
			std::string cell;

			data_.clear();
			while (std::getline(lineStream, cell, ','))
			{
				data_.push_back(cell);
			}
			// This checks for a trailing comma with no data after it.
			if (!lineStream && cell.empty())
			{
				// If there was a trailing comma then add an empty element.
				data_.push_back("");
			}
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


		// Pre Increment
		CSVIterator& operator++() 
		{ 
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
		// Post increment
		CSVIterator operator++(int) 
		{
			CSVIterator tmp(*this); 
			++(*this); 
			return tmp; 
		}
		CSVRow const& operator*() const { return row_; }
		CSVRow const* operator->()  const { return &row_; }

		bool operator==(CSVIterator const& rhs) 
		{ 
			return ((this == &rhs) || ((this->str_ == NULL) && (rhs.str_ == NULL))); 
		}
		bool operator!=(CSVIterator const& rhs) 
		{ 
			return !((*this) == rhs); 
		}
	};
	
	//friend std::istream& operator>>(std::istream& str, CSVParser::CSVRow& data);

	typedef CSVIterator iterator;

	iterator begin() 
	{
		return CSVIterator(file_);
	}

	iterator end()
	{
		return CSVIterator();
	}
};

int main()
{
	std::ifstream file("D:/uma_challenge/images_labelling.csv");
	CSVParser<int, int, std::string> parser(file);
	//parser.get_types<int, int, std::string>();

	//for (CSVIterator loop(file); loop != CSVIterator(); ++loop)
	//for (CSVParser<int, int, std::string>::CSVRow s : parser) {
		//std::cout << "1st Element(" << (*loop)[0] << ")\n";
		//std::cout << s.data() << std::endl;
	//}

	//std::cout << std::make_tuple(2, 3.14159F, 2345.678) << std::endl;
	//std::cout << std::make_tuple("hello", 5) << std::endl;
	//std::cout << std::make_tuple() << std::endl;

	return 0;
}
