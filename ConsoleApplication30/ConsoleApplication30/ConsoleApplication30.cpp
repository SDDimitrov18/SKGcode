#include<iostream>
#include <cstring>
#include <cstdio>
#include <string>
#include <array>
#include <random>
#include <algorithm>
using namespace std;

struct Generateword
{
	Generateword(const size_t min, const size_t max) : dist(min, max) {}
	random_device rd;
	uniform_int_distribution<size_t> dist;
	unsigned operator()() { return dist(rd); }
};

struct Gallow
{
	void Draw() const
	{
		printf(" ________\n"
			"|        |\n"
			"|       %c %c\n"
			"|       %c%c%c\n"
			"|       %c %c\n"
			"|\n"
			"|\n", body[0], body[1], body[2], body[3],
			body[4], body[5], body[6]);
	}

	bool Increment()
	{
		switch (++errors) {
		case 6: body[6] = '\\'; break;
		case 5: body[5] = '/'; break;
		case 4: body[4] = '\\'; break;
		case 3: body[3] = '|'; break;
		case 2: body[2] = '/'; break;
		case 1: body[0] = '(', body[1] = ')'; break;
		}
		return errors < 6;
	}

	char body[7]{ '\0' };
	int errors{ 0 };
};

struct hangman
{
	void Draw() const
	{
#ifdef _WIN32
		system("cls");
#else
		system("clear");
#endif	
		gallow.Draw();
		for_each(guess.begin(), guess.end(), [](const char c) {printf("%c ", c); });
		putchar('\n');
	}

	bool Update()
	{
		int wincount = 0;
		int losecount = 0;
		printf("Enter a letter: ");
		const char letter = tolower(getchar());
		while (getchar() != '\n') {}
		bool found = false;

		for (size_t i = 0; i < word.size(); ++i)
		{
			if (word[i] == letter)
			{
				guess[i] = letter;
				found = true;
			}
		}

		const auto endofthegame = [this](const char* msg)
		{
			Draw();
			puts(msg);
			return false;
		};

		if (not found and not gallow.Increment()) {
			return endofthegame("=== You lose! ===");

			losecount++;
		}
		else if (found and word == guess) {
			return endofthegame("=== You win! ===");
			wincount++;
		}
		return true;
	}

	Generateword rand_gen{ 0, words.size() - 1 };
	const string word{ words[rand_gen()] };
	string guess{ string().insert(0, word.size(), '_') };
	Gallow gallow;
	static const array<const string, 20> words;
};

const array<const string, 20> hangman::words{ {"control", "television", "computer", "telephone", "remotecontrol", "programming", "chair", "mouse", "microphone", "school", "notebook", "keyboard" ,"radiator" ,"pencil", "cable", "harddrive", "monitor", "microwave", "lamp", "satnav"} };

int playing() {
	hangman hangman;
	do {
		hangman.Draw();
	} while (hangman.Update());
	return EXIT_SUCCESS;
}

int main()
{
	playing();
	string choice;

	cout << "Do you want to play again: Yes or No:\n" << endl;
	cin >> choice;


	if (choice == "Yes") {
		do {
			playing();
			cout << "Do you want to play again: Yes or No";
			cin >> choice;
			if (choice == "No") {
				cout << "Thank you for playing!" << endl;
				exit(0);
			}
		} while (choice == "Yes");
	}
	else if (choice == "No") {
		cout << "Thank you for playing! ";
	}
}