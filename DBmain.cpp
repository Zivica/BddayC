#include "DB.h"




/*
    Ovo je glavna funkcija naseg programa odakle isti i krece.
*/
int main()
{
    DB* database = new DB("database.db");
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

                try
                {
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
                    //here using add method
                    database->add(ev);
                }
                catch (...)
                {
                    cout << "Invalid format suplied, please try with valid data." << endl;
                }
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
                try
                {
                    database->events_for(
                        stoi(parts[0]),
                        stoi(parts[1]),
                        stoi(parts[2]));
                    break;
                }
                catch (...)
                {
                    cout << "Invalid format, please use the requested format." << endl;
                }
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