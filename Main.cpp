/*
	Stvari koje su nam potrebne iz standardne c++
	biblioteke.
*/
#include <iostream>
#include <string>
#include <sstream>
#include <regex>
#include <fstream>
#include <ctime>

/*
	Skracenica da ne bi pisali prefix std::bla_bla
	za bilo koju funkciju koju zovemo iz sistemskih
*/
using namespace std;

/*
	Ovde navodimo tipove dogadjaja kao Enum, jer u zadatku su pre-definisani
	tipovi koji mogu biti zadati od strane korisnika.

	Koristimo c++ feature enum class jer je bezbedniji od enum
	jer preskace ceste bagove.
*/
enum class EventType
{
	BIRTHDAY,
	NAMEDAY,
	OTHER_ANNIVERSARY
};

/*
	Definisimo dogadjaj, onakav kakav je zadat u zadataku.
	Godina, mesec, dan, tip, kategorija i opis su podaci koji nas interesuju.
*/
struct Event
{
	int year;
	int month;
	int day;
	EventType type;
	char category[30];
	char description[50];

};

bool compare(Event aEvent, Event bEvent)
{
	char buffer[20];

	sprintf_s(buffer, "%04d%02d%02d", aEvent.year, aEvent.month, aEvent.day);
	int aDateNum = stoi(buffer);

	sprintf_s(buffer, "%04d%02d%02d", bEvent.year, bEvent.month, bEvent.day);
	int bDateNum = stoi(buffer);

	return aDateNum < bDateNum;
}

/*
	Klasa nam sluzi da enkapsulira rad sa bazom na jednom mestu.
	Ona menadzuje nad fajlom zadatog imena.
*/
class DB
{
public:
	/*
		Za konstruktor cemo samo traziti naziv fajla.
	*/
	DB(const char* name)
	{
		this->name = name;

		fstream file;
		file.open(name, ios::binary | ios::in);

		if (file) {
			string line;
			getline(file, line);
			int size = stoi(line);
			this->events.resize(size);
			this->events.clear();

			cout << "Loading " << size << " records from the database." << endl;
			for (int i = 0; i < size; i++)
			{
				/*
					Ucitavamo binarne podatke u RAM
				*/
				Event ev;
				file.read((char*)&ev, sizeof(Event));

				/*
					Moramo da ignorisemo '\n' specijalni karakter za novi red
				*/

				file.ignore();
				
				/*
					Sacuvajmo ih u nas vektor.
				*/
				this->events.push_back(ev);
			}
		}
	}

	void add(Event ev)
	{
		this->events.push_back(ev);
	}

	void list_all_events()
	{
		cout << "--------------------------" << endl;
		cout << "-  Listing saved events  -" << endl;
		cout << "--------------------------" << endl;
		for (auto& elem : this->events)
		{
			printf_s("%s, %d-%d-%d (%s)\n", elem.description, elem.day, elem.month, elem.year, elem.category);
		}
		cout << "--------------------------" << endl;
	}

	void events_for(int year, int month, int day)
	{
		cout << "--------------------------" << endl;
		printf("- Events on (%d-%d-%d)   -", year, month, day);
		cout << endl;
		cout << "--------------------------" << endl;
		vector<Event> result(this->events.size());
		result.clear();
		for (auto& elem : this->events)
		{
			if (elem.year == year && elem.month == month && elem.day == day)
			{
				result.push_back(elem);
			}
		}

		if (result.size() > 0)
		{
			for (auto& elem : result)
			{
				printf_s("%s, %d-%d-%d (%s)\n", elem.description, elem.day, elem.month, elem.year, elem.category);
			}
		}
		else
		{
			cout << "No results for given date. Try something else." << endl;
		}
		cout << "--------------------------" << endl;
	}

	void events_for_range(int y1, int m1, int d1, int y2, int m2, int d2)
	{
		char buffer[20];
		sprintf_s(buffer, "%04d%02d%02d", y1, m1, d1);
		int drange_start = stoi(buffer);

		sprintf_s(buffer, "%04d%02d%02d", y2, m2, d2);
		int drange_end = stoi(buffer);

		vector<Event> result(this->events.size());
		result.clear();

		vector<Event> sorted(this->events.size());
		sorted.clear();

		for (auto& elem : this->events)
		{
			sorted.push_back(elem);
		}

		sort(sorted.begin(), sorted.end(), compare);

		for (auto& elem : sorted)
		{
			sprintf_s(buffer, "%04d%02d%02d", elem.year, elem.month, elem.day);
			int trange = stoi(buffer);

			if (trange >= drange_start && trange <= drange_end)
			{
				result.push_back(elem);
			}
		}

		cout << "--------------------------" << endl;
		cout << "-     Listing events     -" << endl;
		cout << "--------------------------" << endl;

		if (result.size() > 0)
		{
			for (auto& elem : result)
			{
				printf_s("%s, %d-%d-%d (%s)\n", elem.description, elem.day, elem.month, elem.year, elem.category);
			}
		}
		else
		{
			cout << "No results for given date range. Try something else." << endl;
		}

		cout << "--------------------------" << endl;
	}

	void search_by_category(const char* term)
	{
		cout << "--------------------------" << endl;
		cout << "-    Category result     -" << endl;
		cout << "--------------------------" << endl;
		vector<Event> result(this->events.size());
		result.clear();
		for (auto& elem : this->events)
		{
			string c = elem.category;
			if (c.find(term) != string::npos)
			{
				result.push_back(elem);
			}
		}

		if (result.size() > 0)
		{
			for (auto& elem : result)
			{
				printf_s("%s, %d-%d-%d (%s)\n", elem.description, elem.day, elem.month, elem.year, elem.category);
			}
		}
		else 
		{
			cout << "No results for given category. Try something else." << endl;
		}

		cout << "--------------------------" << endl;
	}

	void save()
	{
		fstream file;
		file.open(this->name, ios::out | ios::trunc | ios::binary);

		/*
			Koristimo iterator da prodjemo kroz sve vrednosti u memoriji
		*/
		file << this->events.size() << endl;
		for (auto& ev : this->events)
		{
			cout << "Saving: " << ev.description << endl;

			/*
				Pristupamo memoriji jednog dogadjaja, i onda uzimamo sve njegove bajtove
				koje upisujemo u fajl.
			*/
			file.write((char*)&ev, sizeof(Event));
			file << endl;
		}

		file.close();
	}

private:
	vector<Event> events;
	const char* name;
};


bool is_leap_year(int year)
{
	if (year % 4 != 0) return false;
	if (year % 400 == 0) return true;
	if (year % 100 == 0) return false;
	return true;
}

int days_in_month[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

int get_days_in_month(int year, int month)
{
	int days = days_in_month[month];

	if (month == 1 && is_leap_year(year)) // February of a leap year
		days += 1;

	return days;
}

tm add_months(const tm& d, int months)
{
	bool is_last_day_in_month = d.tm_mday == get_days_in_month(d.tm_year, d.tm_mon);

	int year = d.tm_year + months / 12;
	int month = d.tm_mon + months % 12;

	if (month > 11)
	{
		year += 1;
		month -= 12;
	}

	if (month < 0)
	{
		year -= 1;
		month += 12;
	}

	int day;

	if (is_last_day_in_month)
		day = get_days_in_month(year, month); // Last day of month maps to last day of result month
	else
		day = min(d.tm_mday, get_days_in_month(year, month));

	tm result = tm();

	result.tm_year = year;
	result.tm_mon = month;
	result.tm_mday = day;

	result.tm_hour = d.tm_hour;
	result.tm_min = d.tm_min;
	result.tm_sec = d.tm_sec;

	return result;
}

vector<string> split(const string& s, char delim) 
{
	vector<string> result;
	stringstream ss(s);
	string item;

	while (getline(ss, item, delim)) 
	{
		result.push_back(item);
	}

	return result;
}

/*
	Ovo je glavna funkcija naseg programa odakle isti i krece.
*/
int main()
{
	DB* database = new DB("baza.db");
	string input;
	regex re_valid_in("[1-7]");
	bool running = true;
	int choise;
	vector<string> parts;
	time_t now = time(0);

	while (running)
	{
		cout << "Welcome to the main menu, please pick an action: " << endl;
		cout << "1) List all events" << endl;
		cout << "2) Add event" << endl;
		cout << "3) Save" << endl;
		cout << "4) Category search" << endl;
		cout << "5) Show on a day" << endl;
		cout << "6) Last/Next month" << endl;
		cout << "7) Exit" << endl;
		cin >> input;

		if (regex_match(input, re_valid_in))
		{
			choise = stoi(input);
			switch (choise)
			{
			case 1:
				database->list_all_events();
				break;
			case 2:
				Event ev;
				cout << "date(YYYY-MM-DD): ";
				cin >> input;

				parts = split(input, '-');

				ev.year = stoi(parts[0]);
				ev.month = stoi(parts[1]);
				ev.day = stoi(parts[2]);

				cout << "Type (1 - birthday, 2 - anniversary, 3 - other): ";
				cin >> input;

				ev.type = EventType(stoi(input) - 1);

				cout << "Category: ";
				cin >> ev.category;

				cout << "Description: ";
				cin.ignore();
				getline(cin, input);

				strcpy_s(ev.description, input.c_str());

				database->add(ev);
				break;
			case 3:
				database->save();
				cout << "Work saved!" << endl;
				break;
			case 4:
				cout << "Which category: ";
				cin >> input;
				database->search_by_category(input.c_str());
				break;
			case 5:
				cout << "date(YYYY-MM-DD): ";
				cin >> input;
				parts = split(input, '-');
				database->events_for(
					stoi(parts[0]), 
					stoi(parts[1]),
					stoi(parts[2])
				);
				break;
			case 6:
				cout << "date(YYYY-MM): ";
				cin >> input;
				parts = split(input, '-');

				try
				{
					struct tm start, end;
					start.tm_mday = 1;
					start.tm_mon = stoi(parts[1]) - 1;
					start.tm_year = stoi(parts[0]) - 1900;
					end = add_months(start, 1);

					database->events_for_range(
						1900 + start.tm_year, 1 + start.tm_mon, start.tm_mday,
						1900 + end.tm_year, 1 + end.tm_mon, days_in_month[end.tm_mon]
					);
				}
				catch (...)
				{
					cout << "Invalid date entered, please enter in format YYYY-MM" << endl;
				}
				
				break;
			case 7:
				running = false;
				break;
			}
		}
		else
		{
			cout << "Invalid choise " << input << " please chose an option 1-7: " << endl;
		}
	}

	delete database;
	parts.clear();

	return 0;
}