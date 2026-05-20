#include "quote.hpp"
#include "phrase_library.hpp"
#include "additional.hpp"
#include <fstream>
#include <sstream>

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

	phrases = new Quote[n];
	count = n;

	file.clear();
	file.seekg(0, std::ios::beg);

	for (int i = 0; i < n; i++) {

		std::string line;
		getline(file, line, ';');

		std::string author;
		getline(file, author, ';');

		std::string source;
		getline(file, source, ';');

		std::string theme;
		getline(file, theme, ';');

		std::string key_words;
		getline(file, key_words, ';');

		phrases[i] = Quote(line, author, source, theme, key_words);

	}
}

void phrase_library::find_quotes_by_key_words(const std::string& keyword_request, phrase_library& founded)
{
	std::string lowercase_request = to_lowercase(keyword_request);
	int found_count = 0;
	for (int i = 0; i < this->count; i++)
	{
		if (phrases[i].find_keywords(lowercase_request))
		{
			found_count++;
		}
	}

	if (found_count == 0)
	{
		throw std::exception("No keywords found");

	}

	founded.count = found_count;
	founded.phrases = new Quote[found_count];

	int j = 0;
	for (int i = 0; i < this->count; i++)
	{
		if (phrases[i].find_keywords(lowercase_request))
		{
			founded.phrases[j] = phrases[i];
			j++;
		}
	}
}

std::ostream& operator <<(std::ostream& out, const phrase_library& founded) {
	for (int i = 0; i < founded.count; i++)
	{
		out << founded.phrases[i] << "\n";
	}
	return out;
}