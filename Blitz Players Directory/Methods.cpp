// Project 3: Comparison of 2 Data Structures

#include "Methods.h"

using namespace std;

// inserts data into hash table
// returns if insertion was successful
bool Methods::insertData(HashTable& hashTable, const string& fileName, int keyColumn)
{
    // getting csv file data
    ifstream file(fileName);
    // check to see if file was successfully opened
    if(!file.is_open() || keyColumn < 0 || keyColumn >= ROW_LENGTH)
        return false;

    // processing and inserting csv data
    string dataRow;
    // variables to assist with hash table input
    vector<string> dataVector;
    dataVector.reserve(ROW_LENGTH);
    // skip first line (header labels)
    getline(file, dataRow);
    // for each row of data (excluding header labels)
    while(getline(file,dataRow))
    {
        stringstream stringStream(dataRow);
        string entry;
        // for each item in the row
        while(getline(stringStream,entry,','))
        {
            dataVector.push_back(entry);
        }
        // separating key and value (vector of additional data)
        string key = dataVector[keyColumn];
        dataVector.erase(dataVector.begin() + keyColumn);
        // add each item and associated data to the hash table
        hashTable.insert(key, dataVector);
        dataVector.clear();
    }

    file.close();
    return true;
}

// inserts data into B-Tree
// returns if insertion was successful
bool Methods::insertData(BTree& tree, const string& fileName)
{
    ifstream file(fileName);
    if(!file.is_open())
        return false;
    string line;
    // Skip header
    getline(file, line);
    while (getline(file, line)) {
        stringstream ss(line);
        string title, username, name, rateStr, country, gamesStr, won, draw, lost, wonperc, lostperc, drawperc;
        getline(ss, title, ',');
        getline(ss, username, ',');
        getline(ss, name, ',');
        getline(ss, rateStr, ',');
        getline(ss, country, ',');
        getline(ss, gamesStr, ',');
        getline(ss, won, ',');
        getline(ss, draw, ',');
        getline(ss, lost, ',');
        getline(ss, wonperc, ',');
        getline(ss, lostperc, ',');
        getline(ss, drawperc, ',');
        // Player struct
        Player p(title,username,name,stoi(rateStr),country,stoi(gamesStr),stoi(won),
            stoi(draw),stoi(lost),stod(wonperc),stod(lostperc),stod(drawperc));
        tree.insert(username, p);
    }
    file.close();
    return true;
}

// prints formatted data if possible
// otherwise returns false
bool Methods::printFormattedData(const string& fileName, vector<string>& dataVector, const string& key, int keyColumn)
{
    // getting csv file data
    ifstream file(fileName);
    // check to see if file was successfully opened
    if(!file.is_open() || keyColumn < 0 || keyColumn >= ROW_LENGTH)
        return false;

    // examining first row to find column headers
    string firstRow;
    getline(file, firstRow);
    vector<string> headerVector;
    // parsing each header
    stringstream stringStream(firstRow);
    string entry;
    // for each item in the row
    while(getline(stringStream,entry,','))
    {
        headerVector.push_back(entry);
    }

    // inserting key into data vector
    dataVector.insert(dataVector.begin() + keyColumn, key);

    // patterns for matching and formatting
    regex patternInt("^[0-9]+$");
    regex patternDec("^[.0-9]+$");

    // setting correct locale
    setlocale(LC_ALL, "");

    // printing formatted data
    for(int i=0;i<ROW_LENGTH;i++)
    {
        if(dataVector[i].empty())
            printf("%-20s%s",(headerVector[i] + ":").c_str(),"None");
        else if(regex_match(dataVector[i],patternInt))
            printf("%-20s%'d",(headerVector[i] + ":").c_str(),stoi(dataVector[i]));
        else if(regex_match(dataVector[i],patternDec))
            printf("%-20s%-19.2f%%",(headerVector[i] + ":").c_str(),stod(dataVector[i]));
        else
            printf("%-20s%s",(headerVector[i] + ":").c_str(),dataVector[i].c_str());
        printf("\n");
    }
    // separator
    for(int i=0;i<40;i++)
        cout << "=";
    cout << endl;

    // remove key from data vector
    dataVector.erase(dataVector.begin() + keyColumn);

    return true;
}

// prints formatted data for each data set
void Methods::printFormattedData(const string& fileName, vector<vector<string>>& doubleVector, const string& key, int keyColumn, int maxEntries)
{
    int i = 0;
    for(vector<string>& dataVector : doubleVector)
    {
        if(i >= maxEntries)
            break;
        printFormattedData(fileName, dataVector, key, keyColumn);
        i++;
    }
}

// transforms a Player struct into a vector of data
vector<string> Methods::playerToVector(Player& p)
{
    vector<string> v;
    v.push_back(p.title);
    v.push_back(p.name);
    v.push_back(to_string(p.rate));
    v.push_back(p.country);
    v.push_back(to_string(p.games));
    v.push_back(to_string(p.won));
    v.push_back(to_string(p.draw));
    v.push_back(to_string(p.lost));
    v.push_back(to_string(p.wonperc));
    v.push_back(to_string(p.drawperc));
    v.push_back(to_string(p.lostperc));
    return v;
}

// returns a vector of data from a Player struct for each player
vector<vector<string>> Methods::playerToVector(vector<Player>& vp)
{
    vector<vector<string>> doubleVector;
    for(Player& p : vp)
    {
        doubleVector.push_back(playerToVector(p));
    }
    return doubleVector;
}

// prints an introduction paragraph to the program
void Methods::printIntroduction()
{
    printf("| %-90s (1) |\n","Welcome to the Chess.com Blitz Players Directory!");
    printf("| %-90s (2) |\n","This directory contains the top 200,000 Blitz players on Chess.com by rating.");
    printf("| %-90s (3) |\n","For each player, there is data on their title, rating, country, and game history.");
    printf("| %-90s (4) |\n","As you can see, an options menu has been opened for you.");
    printf("| %-90s (5) |\n","Customize your search parameters within the menu.");
    printf("| %-90s (6) |\n","When you are finished, click \"Search\" to look at the results.");
    cout << endl;
}

// prints formatted data according to the following inputs:
// string searchCriterion is "Name", "Country", "Rating", or "Games Played"
// string dataStructure is "B-tree" or "Hash Table"
// string key is the search term from the user
// int numEntries is between 0 and 999
void Methods::printData(const string& searchCriterion, const string& dataStructure, const string& key, int numEntries = 10)
{
    if(key.empty())
    {
        cout << "Please type something in the search box!" << endl;
        cout << endl;
        return;
    }
    // use B-Tree to search given a username
    if(dataStructure == "B-Tree")
    {
        BTree btree;

        // message while data is being inserted
        cout << "Inserting Data, Please Wait..." << endl;
        cout << endl;

        auto start_insert = chrono::high_resolution_clock::now();
        // inserting data
        insertData(btree, DATA_FILE_PATH_NAME);
        auto end_insert = chrono::high_resolution_clock::now();

        // message while searching for data
        cout << "Searching for Data, Please Wait..." << endl;
        cout << endl;

        auto start_search = chrono::high_resolution_clock::now();
        // search for key
        Player p = btree.search(key);
        auto end_search = chrono::high_resolution_clock::now();

        // if search results turn up empty
        if(p.username.empty())
        {
            cout << "Unfortunately, there is no player in the directory whose username is \"" + key + "\"." << endl;
            cout << endl;
            return;
        }

        // convert player struct to vector of data
        vector<string> data = playerToVector(p);

        // printing data
        printFormattedData(DATA_FILE_PATH_NAME, data, key);
        cout << endl;

        // additional information
        auto elapsed_time_insert = chrono::duration_cast<std::chrono::nanoseconds>(end_insert - start_insert);
        cout << "It took the B-Tree " << elapsed_time_insert.count() << " nanoseconds to insert the full dataset." << endl;
        auto elapsed_time_search = chrono::duration_cast<std::chrono::nanoseconds>(end_search - start_search);
        cout << "It took the B-Tree " << elapsed_time_search.count() << " nanoseconds to search for \"" << key << "\"." << endl;
        cout << endl;
    }
    // use Hash Table to search given username, rating, country, or number of games played
    else if(dataStructure == "Hash Table")
    {
        HashTable hashtable;

        // finding correct criterion column
        int criterionColumn;

        if(searchCriterion == "Rating")
            criterionColumn = 3;
        else if(searchCriterion == "Country")
            criterionColumn = 4;
        else if(searchCriterion == "Games Played")
            criterionColumn = 5;
        else
            criterionColumn = 1;

        // message while data is being inserted
        cout << "Inserting Data, Please Wait..." << endl;
        cout << endl;

        auto start_insert = chrono::high_resolution_clock::now();
        // inserting data based on criterion
        insertData(hashtable, DATA_FILE_PATH_NAME, criterionColumn);
        auto end_insert = chrono::high_resolution_clock::now();

        // message while searching for data
        cout << "Searching for Data, Please Wait..." << endl;
        cout << endl;

        auto start_search = chrono::high_resolution_clock::now();
        // search for key
        vector<vector<string>> doubleVector = hashtable.search(key);
        auto end_search = chrono::high_resolution_clock::now();

        // if search results turn up empty
        if(doubleVector.empty())
        {
            if(searchCriterion == "Name")
            {
                cout << "Unfortunately, there is no player in the directory whose username is \"" + key + "\"." << endl;
                cout << endl;
            }
            else if(searchCriterion == "Country")
            {
                cout << "Unfortunately, there is no player in the directory who lives in \"" + key + "\"." << endl;
                cout << endl;
            }
            else if(searchCriterion == "Rating")
            {
                cout << "Unfortunately, there is no player in the directory who is rated " + key + "." << endl;
                cout << endl;
            }
            else
            {
                cout << "Unfortunately, there is no player in the directory who has played " + key + "games." << endl;
                cout << endl;
            }
            return;
        }

        // printing data
        printFormattedData(DATA_FILE_PATH_NAME, doubleVector, key, criterionColumn, numEntries);
        cout << endl;

        // search information
        if(numEntries < (int)doubleVector.size())
        {
            cout << "While only " << numEntries;
            cout << " entries are shown, the total number of entries is ";
            cout << (int)doubleVector.size() << "." << endl;
        }
        else
            cout << "All entries in the directory are shown." << endl;
        cout << endl;

        // additional information
        auto elapsed_time_insert = chrono::duration_cast<std::chrono::nanoseconds>(end_insert - start_insert);
        cout << "It took the Hash Table " << elapsed_time_insert.count() << " nanoseconds to insert the full dataset." << endl;
        auto elapsed_time_search = chrono::duration_cast<std::chrono::nanoseconds>(end_search - start_search);
        cout << "It took the Hash Table " << elapsed_time_search.count() << " nanoseconds to conduct its search." << endl;
        cout << endl;
    }
}

// Methods for SFML handling
void Methods::render()
{

    // window size parameters
    int window_w = 1300;
    int window_h = 795;

    // loading background ("Checkered Pink Background.png")
    sf::Image backgroundImage;
    backgroundImage.loadFromFile(IMAGE_FILE_PATH_NAME);
    sf::Texture backgroundTexture;
    backgroundTexture.loadFromImage(backgroundImage);
    sf::Sprite backgroundSprite(backgroundTexture);
    // displays a part of the texture
    backgroundSprite.setTextureRect(sf::IntRect(50, 50, window_w, window_h));

    // loading font (ELEGANT TYPEWRITER Bold.ttf)
    sf::Font font;
    if(!font.loadFromFile(FONT_FILE_PATH_NAME))
        return;

    // hover and click color parameters
    sf::Color hoverColor(225, 225, 225); // grey
    sf::Color clickColor(232, 220, 202); // beige
    sf::Color searchButtonHoverColor(sf::Color::Green);
    sf::Color searchButtonClickColor(sf::Color::Black);
    sf::Color helpButtonHoverColor(sf::Color::Green);
    sf::Color helpButtonClickColor(sf::Color::Black);
    // error color parameter (hover)
    sf::Color errorColorFill(255, 204, 204); // light red
    // error color parameter (border
    sf::Color errorColorBorder(sf::Color::Red);
    // active inpux box color
    sf::Color activeInputBoxColor(0, 120, 0); // dark green

    // error message
    sf::Text errorText("", font, 30);
    errorText.setPosition(sf::Vector2f(670, 315));
    errorText.setFillColor(sf::Color::Red);
    errorText.setOutlineThickness(5);
    errorText.setOutlineColor(sf::Color::White);

    // number of input fields
    int numInputFields = 2;
    // input text size limit
    int maxInputLength = 50;
    // input numeral size limit
    int maxNumInputLength = 3;

    // default number of entries
    int numEntriesDefault = 10;

    // setting up window
    sf::RenderWindow window;
    window.create(sf::VideoMode(window_w,window_h),"Chess.com Blitz Players"
        , sf::Style::Close | sf::Style::Titlebar);
    // limit on framerate
    window.setFramerateLimit(60);

    // Creating first set of buttons:
    // **
    vector<string> buttonStrings1 = {"Name","Country","Rating","Games Played"};
    vector<sf::RectangleShape> buttons1;
    vector<sf::Text> buttonTexts1;

    for(int i=0;i<buttonStrings1.size();i++)
    {
        sf::RectangleShape button(sf::Vector2f(300,50));
        button.setFillColor(sf::Color::White);
        button.setOutlineThickness(5);
        button.setOutlineColor(sf::Color::Black);
        button.setPosition(20 + 320 * i,175);
        sf::Text buttonText(buttonStrings1[i], font, 30);
        buttonText.setPosition(sf::Vector2f(20 + 320 * i + 5, 175 + 5));
        buttonText.setFillColor(sf::Color::Black);
        buttons1.push_back(button);
        buttonTexts1.push_back(buttonText);
    }
    // **

    // Creating second set of buttons:
    // **
    vector<string> buttonStrings2 = {"Hash Table","B-Tree"};
    vector<sf::RectangleShape> buttons2;
    vector<sf::Text> buttonTexts2;

    for(int i=0;i<buttonStrings2.size();i++)
    {
        sf::RectangleShape button(sf::Vector2f(300,50));
        button.setFillColor(sf::Color::White);
        button.setOutlineThickness(5);
        button.setOutlineColor(sf::Color::Black);
        button.setPosition(20 + 320 * i,310);
        sf::Text buttonText(buttonStrings2[i], font, 30);
        buttonText.setPosition(sf::Vector2f(20 + 320 * i + 5, 310 + 5));
        buttonText.setFillColor(sf::Color::Black);
        buttons2.push_back(button);
        buttonTexts2.push_back(buttonText);
    }
    // **

    // tracking which of the first set of buttons is chosen
    int chosenButton1 = 0;
    // tracking which of the second set of buttons is chosen
    int chosenButton2 = 0;

    // Creating help button:
    // **
    sf::RectangleShape helpButton(sf::Vector2f(50, 50));
    helpButton.setFillColor(sf::Color::White);
    helpButton.setOutlineThickness(5);
    helpButton.setOutlineColor(sf::Color::Black);
    helpButton.setPosition(sf::Vector2f(1230, 20));
    sf::Text helpButtonText("?", font, 30);
    helpButtonText.setPosition(sf::Vector2f(20 + 1230, 20 + 5));
    helpButtonText.setFillColor(sf::Color::Black);
    // **

    // Creating input fields:
    // **
    // input boxes and texts
    vector<sf::RectangleShape> inputBoxes;
    vector<sf::Text> inputTexts;
    for(int i=0;i<numInputFields;i++)
    {
        // input boxes
        sf::RectangleShape inputBox(sf::Vector2f(1260, 50));
        inputBox.setPosition(20, 445 + 135 * i);
        inputBox.setFillColor(sf::Color::White);
        inputBox.setOutlineThickness(5);
        inputBox.setOutlineColor(sf::Color::Black);

        // input texts
        sf::Text inputText("", font, 25);
        inputText.setPosition(20 + 10, 445 + 10 + 135 * i);
        inputText.setFillColor(sf::Color::Black);

        // add to vectors
        inputBoxes.push_back(inputBox);
        inputTexts.push_back(inputText);
    }
    // **

    // tracking input box activity
    vector<bool> inputBoxIsActive(numInputFields, false);
    vector<string> inputStrings(numInputFields);

    // Creating search button and text:
    // **
    sf::RectangleShape searchButton(sf::Vector2f(300,50));
    searchButton.setPosition(20, 715);
    searchButton.setFillColor(sf::Color::White);
    searchButton.setOutlineThickness(5);
    searchButton.setOutlineColor(sf::Color::Black);

    sf::Text searchButtonText("Search", font, 30);
    searchButtonText.setPosition(20 + 5, 715 + 5);
    searchButtonText.setFillColor(sf::Color::Black);
    // **

    // main loop for rendering the window
    while(window.isOpen())
    {

        // event handling
        sf::Event event;
        // mouse position tracking
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

        while(window.pollEvent(event))
        {

            // window close
            if(event.type == sf::Event::Closed)
                window.close();

            // button color controls:
            // **
            // first set
            for(int i=0;i<buttons1.size();i++)
            {
                if(buttons1[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition)))
                {
                    // detect if the mouse is hovering over button
                    buttons1[i].setFillColor(hoverColor);
                    // detect if the button has been clicked
                    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    {
                        // previous choice is documented to determine whether to clear input field
                        int previousChoice = chosenButton1;
                        chosenButton1 = i;
                        // if not searching by name, then the default "1" is removed
                        if(i != 0 && previousChoice == 0)
                        {
                            inputStrings[1] = "";
                            inputTexts[1].setString(inputStrings[1]);
                        }
                    }
                }
                else
                {
                    buttons1[i].setFillColor(sf::Color::White);
                }
            }
            // set clicked fill color
            buttons1[chosenButton1].setFillColor(clickColor);
            // second set
            for(int i=0;i<buttons2.size();i++)
            {
                if(buttons2[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition)))
                {
                    // detect if the mouse is hovering over button
                    // results in an error color and error message if the selection is not available
                    if(chosenButton1 != 0 && i == 1)
                    {
                        buttons2[i].setFillColor(errorColorFill);
                        errorText.setString(buttonStrings2[i] + " cannot search by " + buttonStrings1[chosenButton1]);
                    }
                    else
                    {
                        buttons2[i].setFillColor(hoverColor);
                        // clears error text if needed
                        if(errorText.getString().find("cannot search by") != std::string::npos)
                            errorText.setString("");
                    }
                    // detect if the button has been clicked
                    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    {
                        chosenButton2 = i;
                    }
                }
                else
                {
                    buttons2[i].setFillColor(sf::Color::White);
                    errorText.setString("");
                }
            }
            // chosen button 2 depends on chosen button 1
            // a non-name search can only be done by the hash table and not the B-Tree
            if(chosenButton1 != 0)
                chosenButton2 = 0;
            // set clicked fill color
            buttons2[chosenButton2].setFillColor(clickColor);
            // set input string for number of entries field to default value of "1", if search by name is selected
            if(chosenButton1 == 0)
            {
                inputStrings[1] = "1";
                inputTexts[1].setString(inputStrings[1]);
            }
            // **

            // help button controls:
            // **
            // detect if the mouse is hovering over help button
            if(helpButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition)))
            {
                helpButton.setFillColor(helpButtonHoverColor);
                helpButtonText.setFillColor(sf::Color::Black);
                // detect if the button has been clicked
                if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                {
                    helpButton.setFillColor(helpButtonClickColor);
                    helpButtonText.setFillColor(sf::Color::White);
                    // print introduction
                    printIntroduction();
                }
            }
            else
            {
                helpButton.setFillColor(sf::Color::White);
                helpButtonText.setFillColor(sf::Color::Black);
            }
            // **

            // input field controls:
            // **
            for(int i=0;i<numInputFields;i++)
            {
                // detect activity (user must click on input box)
                if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                {
                    inputBoxIsActive[i] = inputBoxes[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition));
                }
                // if input box is active, highlight it a special color and make outline bold
                if(inputBoxIsActive[i])
                {
                    // if search is by name, then only one possible entry can be found, and input is not allowed
                    if(chosenButton1 == 0 && i != 0)
                    {
                        inputBoxes[i].setOutlineColor(errorColorBorder);
                        inputBoxes[i].setOutlineThickness(10);
                        // sets appropriate error message
                        errorText.setString("Only one entry if searching by name.");
                    }
                    // otherwise, highlight and bold as normal
                    else
                    {
                        inputBoxes[i].setOutlineColor(activeInputBoxColor);
                        inputBoxes[i].setOutlineThickness(10);
                    }
                }
                else
                {
                    // default color is black
                    inputBoxes[i].setOutlineColor(sf::Color::Black);
                    inputBoxes[i].setOutlineThickness(5);
                    // clears error text if needed
                    if(errorText.getString().find("Names are unique") != std::string::npos)
                        errorText.setString("");
                }
                // if input box is active, handle user input
                // first input box: default handling
                // second input box: must be a number, 3 digits of fewer
                if(inputBoxIsActive[i] && event.type == sf::Event::TextEntered)
                {
                    if(i == 0)
                    {
                        // backspace
                        if(event.text.unicode == '\b' && !inputStrings[i].empty())
                            inputStrings[i].pop_back();
                        // an appropriate (ascii value < 128) character was entered
                        else if(event.text.unicode != '\b' && event.text.unicode < 128)
                        {
                            // enforcing input length limit
                            if(inputStrings[i].length() < maxInputLength)
                                inputStrings[i] += static_cast<char>(event.text.unicode);
                        }
                    }
                    else
                    {
                        // input allowed only if not searching by name
                        if(chosenButton1 != 0)
                        {
                            // backspace
                            if(event.text.unicode == '\b' && !inputStrings[i].empty())
                                inputStrings[i].pop_back();
                            // a digit was entered
                            else if(event.text.unicode >= 48 && event.text.unicode < 58)
                            {
                                // enforcing input length limit
                                if(inputStrings[i].length() < maxNumInputLength)
                                    inputStrings[i] += static_cast<char>(event.text.unicode);
                            }
                        }
                    }
                    inputTexts[i].setString(inputStrings[i]);
                }
            }
            // **

            // search button controls:
            // **
            // detect if the mouse is hovering over search button
            if(searchButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition)))
            {
                searchButton.setFillColor(searchButtonHoverColor);
                searchButtonText.setFillColor(sf::Color::Black);
                // detect if the button has been clicked
                if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                {
                    searchButton.setFillColor(searchButtonClickColor);
                    searchButtonText.setFillColor(sf::Color::White);
                    // print data according to search
                    int numEntries;
                    inputStrings[1].empty() ? numEntries = numEntriesDefault : numEntries = stoi(inputStrings[1]);
                    string searchString = inputStrings[0];
                    // clear input fields
                    for(string& inputString : inputStrings)
                        inputString = "";
                    for(int i=0;i<numInputFields;i++)
                        inputTexts[i].setString(inputStrings[i]);
                    // proceed with printing data
                    printData(buttonStrings1[chosenButton1], buttonStrings2[chosenButton2],
                         searchString, numEntries);
                }
            }
            else
            {
                searchButton.setFillColor(sf::Color::White);
                searchButtonText.setFillColor(sf::Color::Black);
            }
            // **

        }

        // make sure window is cleared
        window.clear(sf::Color::White);

        // draw background (must be drawn first)
        window.draw(backgroundSprite);

        // create and draw title
        sf::Text titleText("Chess.com Blitz Players Directory", font, 50);
        titleText.setPosition(20, 10);
        titleText.setFillColor(sf::Color::Black);
        titleText.setOutlineThickness(5);
        titleText.setOutlineColor(sf::Color::White);
        window.draw(titleText);

        // create & draw below-title banner
        sf::RectangleShape banner(sf::Vector2f(window_w, 10));
        banner.setPosition(0, 90);
        banner.setFillColor(sf::Color::Black);
        window.draw(banner);

        // list of prompts
        vector<string> promptsVector = {"Search By", "Search Using","Search Input","How many entries do you want to see?", "Search by clicking the button below!"};

        // create and draw prompts
        for(int i=0;i<promptsVector.size();i++)
        {
            sf::Text promptText(promptsVector[i], font, 30);
            promptText.setPosition(sf::Vector2f(20, 120 + 135 * i));
            promptText.setFillColor(sf::Color::Black);
            promptText.setOutlineThickness(5);
            promptText.setOutlineColor(sf::Color::White);
            window.draw(promptText);
        }

        // drawing buttons:
        // **
        for(int i=0;i<buttonStrings1.size();i++)
        {
            window.draw(buttons1[i]);
            window.draw(buttonTexts1[i]);
        }
        for(int i=0;i<buttonStrings2.size();i++)
        {
            window.draw(buttons2[i]);
            window.draw(buttonTexts2[i]);
        }
        // **

        // drawing help button:
        // **
        window.draw(helpButton);
        window.draw(helpButtonText);
        // **

        // drawing input fields:
        // **
        for(int i=0;i<numInputFields;i++)
        {
            window.draw(inputBoxes[i]);
            window.draw(inputTexts[i]);
        }
        // **

        // drawing search button
        window.draw(searchButton);
        window.draw(searchButtonText);

        // drawing error message
        window.draw(errorText);

        window.display();

    }

}