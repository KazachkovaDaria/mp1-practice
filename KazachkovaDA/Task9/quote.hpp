#ifndef	QUOTE_HPP
#define QUOTE_HPP

#include <iostream>
#include <string>

class Quote {
private:

	std::string the_line;
	std::string author;
	std::string source;
	std::string theme;
	std::string* key_words;
	int key_words_count;
public:
	Quote();
	Quote(const std::string&, const std::string&, const std::string&, 
		const std::string&, const std::string&);
	Quote(const Quote&);
	Quote& operator=(const Quote&);
	bool find_keywords(const std::string&) const;
	friend std::ostream& operator<< (std::ostream&, const Quote&);
};

#endif