#ifndef LIST_HPP
#define LIST_HPP

#include <iostream>
#include <string>

class quote {
private:

	std::string the_line;
	std::string author;
	std::string source;
	std::string theme;
	std::string* key_words;
	int key_words_count;
public:
	quote();
	quote(const std::string& line, const std::string& author, const std::string& source,
		  const std::string& theme, const std::string& key_words);
	quote(const quote&);
	quote& operator=(const quote&);
};

struct phrase_library {
private:
	quote* phrases;
	int count;
public:
	phrase_library();
	phrase_library(const std::string&);
	~phrase_library() { delete[] this->phrases; }
	void find_quotes_by_key_words(const std::string, phrase_library&);
	friend std::ostream& operator<< (std::ostream&, const phrase_library&);
};

#endif