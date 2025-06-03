#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <locale>
#include <codecvt>
#include <io.h>     
#include <fcntl.h>   
using namespace std;

// Очистка слова от небуквенных символов и приведение к нижнему регистру
wstring cleanWord(const wstring& word) {
    wstring result;
    for (wchar_t c : word) {
        if (iswalnum(c)) {
            result += towlower(c);
        }
    }
    return result;
}

int main() {
    _setmode(_fileno(stdin), _O_U16TEXT);
    _setmode(_fileno(stdout), _O_U16TEXT);
    ifstream file("voina.txt");
    if (!file) {
        wcerr << L"Не удалось открыть файл" << endl;
        return 1;
    }

    ostringstream oss;
    oss << file.rdbuf();
    string utf8_data = oss.str();

    // Преобразование UTF-8 в wchar_t
    wstring_convert<codecvt_utf8<wchar_t>, wchar_t> utf8_to_wchar;
    wstring text;
    text = utf8_to_wchar.from_bytes(utf8_data);
   
    unordered_map<wstring, int> wordFrequency;
    wistringstream iss(text);
    wstring word;
    while (iss >> word) {
        word = cleanWord(word);
        if (!word.empty()) {
            wordFrequency[word]++;
        }
    }

    wcout << L"Введите запрос:" << endl;
    wstring query;
    getline(wcin, query); 
    transform(query.begin(), query.end(), query.begin(), ::towlower);
    vector<pair<int, wstring>> matches;
    for (auto it = wordFrequency.begin(); it != wordFrequency.end(); ++it) {
        const wstring& w = it->first;
        int freq = it->second;
        if (w.find(query) != wstring::npos) {
            matches.push_back(make_pair(-freq, w));  
        }
    }
    sort(matches.begin(), matches.end());
    if (matches.empty()) {
        wcout << L"Совпадений не найдено" << endl;
    }
    else {
        wcout << L"Результаты:" << endl;
        int count = 0;
        for (size_t i = 0; i < matches.size(); ++i) {
            int negFreq = matches[i].first;
            const wstring& w = matches[i].second;
            wcout << w << L" (" << -negFreq << L")" << endl;
            if (++count >= 20) break;
        }
    }
    return 0;
}
