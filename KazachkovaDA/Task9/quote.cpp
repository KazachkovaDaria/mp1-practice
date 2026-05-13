#include "quote.hpp"
#include "additional.hpp"
#include <fstream>
#include <sstream>

Quote::Quote()
{
	key_words = nullptr;
	key_words_count = 0;
}

Quote::Quote(const std::string& _line, const std::string& _author, const std::string& _source,
	const std::string& _theme, const std::string& _key_words) : 
	the_line(_line), author(_author), source(_source), theme(_theme), key_words(nullptr), key_words_count(0)
{
	std::stringstream key_words_stream(_key_words);

	std::string a_word;
	while (std::getline(key_words_stream, a_word, ',')) {
		key_words_count++;
	}

	key_words = new std::string[key_words_count];

	std::stringstream key_words_stream1(_key_words);

	int j = 0;
	while (std::getline(key_words_stream1, a_word, ',')) {
		key_words[j] = a_word;
		j++;
	}
};

Quote::Quote(const Quote& q)
{

	this->the_line = q.the_line;
	this->author = q.author;
	this->source = q.source;
	this->theme = q.theme;
	this->key_words_count = q.key_words_count;

	this->key_words = new std::string[q.key_words_count];

	for (int k = 0; k < q.key_words_count; k++)
	{
		this->key_words[k] = q.key_words[k];
	}
}

Quote& Quote::operator=(const Quote& q) 
{

	if (this == &q) {
		return *this;
	}

	delete[]key_words;

	this->the_line = q.the_line;
	this->author = q.author;
	this->source = q.source;
	this->theme = q.theme;
	this->key_words_count = q.key_words_count;

	this->key_words = new std::string[q.key_words_count];

	for (int k = 0; k < q.key_words_count; k++)
	{
		this->key_words[k] = q.key_words[k];
	}
	return *this;
}

bool Quote::find_keywords(const std::string& _keyword_request) const
{
	std::string lowercase_key_words;
	for (int j = 0; j < key_words_count; j++)
	{
		lowercase_key_words = to_lowercase(key_words[j]);
		if (lowercase_key_words == _keyword_request)
		{
			return true;
			break;
		}
	}
	return false;
}

std::ostream& operator<< (std::ostream& out, const Quote& q)
{
	out << q.the_line << "\n";
	return out;
}
