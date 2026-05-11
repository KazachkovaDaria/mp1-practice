#include "list.hpp"
#include "additional.hpp"
#include <fstream>
#include <sstream>
quote::quote()
{
	key_words = nullptr;
	key_words_count = 0;
}

quote::quote(const quote& q)
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

quote& quote::operator=(const quote& q) {

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
phrase_library::phrase_library()
{
	phrases = nullptr;
	count = 0;
}

phrase_library::phrase_library(const std::string& file_name)
{
	std::ifstream file(file_name);
	if (!(file.is_open()))
	{
		throw "File reading error";
	}

	std::string line;
	int n = 0;

	while (getline(file, line)) {
		n++;
	}

	if (n == 0)
	{
		throw "No info found in the file";
	}

	phrases = new quote[n];
	count = n;

	//since you have reached (and attempted to read past)
	// the end of the file, the eof and fail flags will be set. You need to clear them using ifile.clear
	file.clear();
	file.seekg(0, std::ios::beg);

	for (int i = 0; i < n; i++) {

		getline(file, phrases[i].the_line, ';');

		getline(file, phrases[i].author, ';');

		getline(file, phrases[i].source, ';');

		getline(file, phrases[i].theme, ';');

		std::string key_words;
		getline(file, key_words, ';');

		std::stringstream key_words_stream(key_words);
		int key_words_count = 0;

		std::string a_word;
		while (std::getline(key_words_stream, a_word, ',')) {
			key_words_count++;
		}

		phrases[i].key_words_count = key_words_count;
		phrases[i].key_words = new std::string[key_words_count];

		std::stringstream key_words_stream1(key_words);

		int j = 0;
		while (std::getline(key_words_stream1, a_word, ',')) {
			phrases[i].key_words[j] = a_word;
			j++;
		}
	}
}

void phrase_library::find_quotes_by_key_words(const std::string keyword_request, phrase_library& founded)
{
	std::string lowercase_request = to_lowercase(keyword_request);
	std::string lowercase_key_words;
	int found_count = 0;
	for (int i = 0; i < this->count; i++)
	{
		for (int j = 0; j < this->phrases[i].key_words_count; j++)
		{
			lowercase_key_words = to_lowercase(this->phrases[i].key_words[j]);
			if (lowercase_key_words == lowercase_request)
			{
				found_count++;
				break;
			}
		}
	}

	if (found_count == 0)
	{
		throw std::exception("No keywords found");

	}

	founded.count = found_count;
	founded.phrases = new quote[found_count];

	int l = 0;
	for (int i = 0; i < this->count; i++)
	{
		for (int j = 0; j < this->phrases[i].key_words_count; j++)
		{
			lowercase_key_words = to_lowercase(this->phrases[i].key_words[j]);
			if (lowercase_key_words == lowercase_request)
			{
				founded.phrases[l] = this->phrases[i];
				l++;
			}
		}
	}
}

std::ostream& operator <<(std::ostream& out, const phrase_library& founded) {
	for (int i = 0; i < founded.count; i++)
	{
		out << founded.phrases[i].the_line << "\n";
	}
	return out;
}