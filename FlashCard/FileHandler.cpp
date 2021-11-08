/*
 * FileHandler
 *	-Store current directory path
 *	-Read files for flashcards.
 *	-Write files for flashcards.
 */

#include "./FileHandler.h"
#include <cstdio>

FileHandler::FileHandler() {
    directoryPath = std::filesystem::current_path().string();
    directoryPath.append(DeckDirectory);
}

bool FileHandler::ReadFile(std::string Filename, Deck* deck) {
	std::fstream fin;
    //std::string localFiles = "../Localfiles/";
    std::string localFile = directoryPath;
    localFile.append(Filename);
    localFile.append(".csv");
    if (!std::filesystem::exists(localFile)) {
        return false;
    }
 //   if (std::filesystem::path(directoryPath).extension() != ".csv") {
 //       return false;
 //   }
    fin.open(localFile, std::ios::in);
    if (fin.fail()) {
        return false;
    }
    std::string line,word;
    /*
    *   Existing problems, "," will be """,""", in csv
    *   so we have to read character by character
    */
    char delimiter = ',';
    std::string quote = "\"";
   
    bool front = true;
    bool back = false;
    bool first = true;
    //bool setting;//Don't read settings for now
    std::string temp = "";
    std::string frontContent = "";
    std::string backContent = "";
    while (getline(fin, line)) {
        if (!back && !front) {
            deck->createCard(frontContent, backContent);
            frontContent.clear();
            backContent.clear();
            front = true;
        }
        std::stringstream ss(line);
        std::string substr;
        if ((countQuote(temp) % 2) == 1) {
            temp.append("\n");
        }
        if (line.find(",") != std::string::npos) {
            while (getline(ss, substr, delimiter)) {
                temp.append(substr);
                if ((countQuote(temp) % 2) == 1) {
                    temp.append(",");
                }
                else {
                    if (temp[0] == '"' && temp[temp.size() - 1] == '"') {
                        temp = temp.substr(1, temp.size() - 2);
                    }
                    if (front) {
                        temp = ReplaceQuotes(temp);
                        if (first) {
                            deck->setCurrentFrontCard(temp);
                        }
                        else {
                            frontContent = temp;
                        }
                        temp.clear();
                        front = false;
                        back = true;
                    }
                    else if (back) {
                        temp = ReplaceQuotes(temp);
                        if (first) {
                            deck->setCurrentBackCard(temp);
                            first = false;
                            front = true;
                        }
                        else {
                            backContent = temp;
                        }
                        temp.clear();
                        back = false;
                    }
                }
            }
            temp.erase(temp.end());
        }
        else {
            temp.append(line);
            if ((countQuote(temp) % 2) == 0) {
                if (temp[0] == '"' && temp[temp.size() - 1] == '"') {
                    temp = temp.substr(1, temp.size() - 2);
                }
                if (front) {
                    temp = ReplaceQuotes(temp);
                    if (first) {
                        deck->setCurrentFrontCard(temp);
                    }
                    else {
                        frontContent = temp;
                    }
                    temp.clear();
                    front = false;
                    back = true;
                }
                else if (back) {
                    temp = ReplaceQuotes(temp);
                    if (first) {
                        deck->setCurrentBackCard(temp);
                        first = false;
                        front = true;
                    }
                    else {
                        backContent = temp;
                    }
                    temp.clear();
                    back = false;
                }
            }
        }
    }
    if (back) {
        backContent = temp;
        back = false;
    }
    if (!frontContent.empty() || !backContent.empty()) {
        deck->createCard(frontContent, backContent);
        frontContent.clear();
        backContent.clear();
    }
	return true;
}
/* Random Code that I might need in case Write has problem again
*     if (back) {
        int quoteCount = countQuote(temp);
        if ((quoteCount % 2) == 0) {
            if (temp[0] == '"' && temp[temp.size() - 1] == '"') {
                temp = temp.substr(1, temp.size() - 2);
            }
            backContent.append(temp);
            temp.clear();
            back = false;
        }
        if (!front && !back) {
            if (first) {
                deck->setCurrentFrontCard(frontContent);
                deck->setCurrentBackCard(backContent);
                first = false;
            }
            else {
                deck->createCard(frontContent, backContent);
            }
            frontContent.clear();
            backContent.clear();
        }
    }
*/
bool FileHandler::WriteFile(std::string Filename, Deck* deck) {
    std::string filePath = directoryPath;
    filePath.append(Filename);
    std::ofstream myfile(filePath, std::ofstream::trunc);
    for (unsigned int i = 0; i < deck->getDeckSize(); i++) {
        std::string Front = "";
        std::string Back = "";
        deck->goToCard(i);
        FlashCard* temp = deck->getCard();
        Front = temp->getFront();
        Back = temp->getBack();
        //Write a file to update any special characters
        //Two cases "" case and the comma case.
        Front = HandleDoubleQuote(Front);
        Front = HandleComma(Front);
        Back = HandleDoubleQuote(Back);
        Back = HandleComma(Back);
        myfile << Front << "," << Back << "\n";
    }
    myfile.close();
    return true;
}

bool FileHandler::deleteFile(std::string fileName) {

    fileName.append(".csv");
    std::string filePath = directoryPath;
    filePath.append(fileName);
    if (std::filesystem::remove(filePath) != 0) {
        return true;
    }
    else {
        return false;
    }
}

std::string FileHandler::HandleDoubleQuote(std::string data) {
    std::size_t pos = data.find('"');
    bool found = false;
    while (pos != std::string::npos) {
        data.insert(pos, "\"");
        pos = data.find('"', pos + 2);
        found = true;
    }
    if (found) {
        data.insert(0, "\"");
        data.append("\"");
    }
    return data;
}

std::string FileHandler::HandleComma(std::string data) {
    if (data.find(",") != std::string::npos) {
        data.insert(0, "\"");
        data.append("\"");
    }
    return data;
}

int FileHandler::countQuote(std::string data) {
    int count = 0;
    for (int i = 0; i < data.size(); i++) {
        if (data[i] == '"') {
            count++;
        }
    }
    return count;
}

std::string FileHandler::ReplaceQuotes(std::string data) {
    std::string TwoQuotes = "\"\"";
    std::string oneQuotes = "\"";
    std::string::size_type n = 0;
    while ((n = data.find(TwoQuotes, n)) != std::string::npos)
    {
        data.replace(n, TwoQuotes.size(), oneQuotes);
        n += oneQuotes.size();
    }
    return data;
}

