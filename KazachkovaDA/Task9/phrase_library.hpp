#ifndef PHRASE_LIBRARY_HPP
#define PHRASE_LIBRARY_HPP

#include <iostream>
#include <string>
#include "quote.hpp"

struct phrase_library {
private:
	Quote* phrases;
	int count;
public:
	phrase_library();
	phrase_library(const std::string&);
	~phrase_library() { delete[] this->phrases; }
	void find_quotes_by_key_words(const std::string, phrase_library&);
	friend std::ostream& operator<< (std::ostream&, const phrase_library&);
};

#endif