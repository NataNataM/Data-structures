#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <io.h>
#include <fcntl.h>
#include <locale>
#include <codecvt>
using namespace std;

// Преобразование UTF-16 в UTF-8
string utf16to8(const u16string& str) {
    wstring_convert<codecvt_utf8<char16_t>, char16_t> converter;
    return converter.to_bytes(str.c_str());
}

// Преобразование UTF-8 в UTF-16
u16string utf8to16(const string& str) {
    wstring_convert<codecvt_utf8<char16_t>, char16_t> converter;
    return converter.from_bytes(str);
}

// Функция подсчета частоты символов
unordered_map<char16_t, int> buildFrequencyMap(const u16string& word) {
    unordered_map<char16_t, int> freq;
    for (char16_t c : word)
        freq[c]++;
    return freq;
}

// Проверяет, можно ли составить target из source
bool canConstruct(const unordered_map<char16_t, int>& sourceFreq, const u16string& target) {
    unordered_map<char16_t, int> targetFreq;
    for (char16_t c : target)
        targetFreq[c]++;
    for (auto it = targetFreq.begin(); it != targetFreq.end(); ++it) {
        char16_t c = it->first;
        int count = it->second;
        if (sourceFreq.count(c) == 0 || sourceFreq.at(c) < count)
            return false;
    }
    return true;
}

int main() {
    setlocale(LC_ALL, "ru");
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);

    u16string dictFilename = u"russian_nouns.txt"; 
    ifstream file(utf16to8(dictFilename).c_str());
    if (!file.is_open()) {
        wcerr << L"Не удалось открыть словарь.\n";
        return 0;
    }
    vector<u16string> dictionary;
    string line;
    while (getline(file, line)) {
        dictionary.push_back(utf8to16(line));
    }
    
    vector<unordered_map<char16_t, int>> dictFreqMaps;
    dictFreqMaps.reserve(dictionary.size());
    for (const auto& word : dictionary) {
        dictFreqMaps.push_back(buildFrequencyMap(word));
    }
    wcout << L"Введите слово: ";
    wstring inputLine;
    getline(wcin, inputLine);
    u16string inputUtf16(inputLine.begin(), inputLine.end());
    auto sourceFreq = buildFrequencyMap(inputUtf16);

    vector<u16string> result;
    for (size_t i = 0; i < dictionary.size(); ++i) {
        if (canConstruct(sourceFreq, dictionary[i])) {
                result.push_back(dictionary[i]);
        }
    }

    sort(result.begin(), result.end(), [](const u16string& a, const u16string& b) {
        if (a.size() != b.size())
           return a.size() > b.size();
        return a < b;
       });

   wcout << L"Найдено слов: " << result.size() << endl;
   for (const auto& word : result) {
       wcout << wstring(word.begin(), word.end()) << endl;
   }
    return 0;
}
