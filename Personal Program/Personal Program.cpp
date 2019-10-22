#include "pch.h"
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<vector>
#include<algorithm>
#include<cctype>
#include<regex>
#include<io.h>

using namespace std;
int begFlag = 1;
int N=10;
typedef struct {
	unsigned int charNum;
	unsigned int lineNum;
	unsigned int wordNum;
}amount;
class word {
private:
	string wordStr;
	unsigned int freq;
public:
	word() = default;
	word(string str) {
		wordStr = str;
		freq = 1;
	}
	string getWordStr() {
		return wordStr;
	}
	unsigned int getFreq() {
		return freq;
	}
	void addFreq() {
		freq++;
	}
	void resetWordStr(string str) {
		if (str < wordStr) {
			wordStr = str;
		}
	}
	bool operator == (const word &obj) const {
		string word1 = this->wordStr, word2 = obj.wordStr;
		int i = word1.length() - 1;
		int j = word2.length() - 1;
		while (i >= 0)
		{
			if (word1[i] >= '0'&&word1[i] <= '9')
				word1[i] = '\0';
			else break;
			i--;
		}
		while (j >= 0)
		{
			if (word2[j] >= '0'&&word2[j] <= '9')
				word2[j] = '\0';
			else break;
			j--;
		}
		if (i == j) {
			for (int t = 0; t <= i; t++) {
				if (word1[t] != word2[t] && abs(word1[t] - word2[t]) != 32)
					return false;
			}
		}
		else return false;
		return true;
	}
	string getFirst()
	{
		int i=wordStr.length()-1;
		string first; 
		string word1 = this->wordStr;
		while (i >= 0)
		{
			first = word1[i];
			i--;
		}
		return first;
	}
	void printWord(ofstream &output) {

		output << wordStr << "\t--->" << freq << endl;
	}
};
class phrase {

private:
	unsigned int freq;
	word part1, part2;

public:
	phrase(word part1, word part2) {
		this->part1 = part1;
		this->part2 = part2;
		freq = 1;
	}
	word getPart1() {
		return part1;
	}

	word getPart2() {
		return part2;
	}

	unsigned int getFreq() {
		return freq;
	}

	void addFreq() {
		freq++;
	}

	void resetPhrase(phrase &obj) {
		word objPart1 = obj.getPart1();
		word objPart2 = obj.getPart2();
		if (objPart1.getWordStr() < this->part1.getWordStr() || objPart2.getWordStr() < this->part2.getWordStr()) {
			this->part1 = objPart1;
			this->part2 = objPart2;
		}
	}

	bool operator == (const phrase &obj) const {

		word objPart1 = obj.part1, objPart2 = obj.part2;
		return (part1 == objPart1 && part2 == objPart2);
	}

	void printPhrase(ofstream &output) {
		string word1 = part1.getWordStr(), word2 = part2.getWordStr();
		word1 < word2 ?
			output << word1 + " " + word2 << "\t--->" << freq << endl :
			output << word2 + " " + word1 << "\t--->" << freq << endl;
	}
};
bool wordCompare(word former, word latter) {
	if (former.getFreq() > latter.getFreq())
		return true;
	else if (former.getFreq() == latter.getFreq() && former.getFirst() <= latter.getFirst() && former.getFirst() <= latter.getFirst())
		return true;
	else return false;
}

bool phraseCompare(phrase former, phrase latter) {
	return former.getFreq() > latter.getFreq();
}



void examineNewWord(vector<word> &wvec, word &newWord) {

	vector<word>::iterator beg = wvec.begin(), end = wvec.end(), itr;
	itr = find(beg, end, newWord);  

	if (itr != end) {                
		itr->resetWordStr(newWord.getWordStr());
		itr->addFreq();
	}
	else {
		wvec.push_back(newWord);
	}
}

void examineNewPhr(vector<phrase> &pvec, phrase &newPhrase) {

	vector<phrase>::iterator beg = pvec.begin(), end = pvec.end(), itr;
	itr = find(beg, end, newPhrase); 

	if (itr != end) {
		itr->resetPhrase(newPhrase);
		itr->addFreq();
	}
	else {
		pvec.push_back(newPhrase);
	}
}
void getNewExpr(string &str, vector<word> &wvec, vector<phrase> &pvec, amount &result) {

	word newWord;
	string wordPattern("[[:alpha:]]{4}[[:alnum:]]{0,1020}");
	regex reg(wordPattern);
	string newPhrStr = "\0";
	word part1("\0"), part2("\0");
	phrase newPhrase(part1, part2);

	for (sregex_iterator it(str.begin(), str.end(), reg), end_it;
		it != end_it; it++) {

		result.wordNum++;
		newWord = word(it->str());
		examineNewWord(wvec, newWord);

		if (begFlag) {

			begFlag = 0;
			part1 = newWord;
		}
		else {

			part2 = newWord;
			newPhrase = phrase(part1, part2);
			examineNewPhr(pvec, newPhrase);
			part1 = part2;
		}

	}
}
unsigned long getCharNum(string &str) {
	unsigned long charNum = 0;
	string::iterator end = str.end(), citr;
	for (citr = str.begin(); citr != end; citr++) {
		if (*citr >= 32 && *citr <= 126)
			charNum++;
	}
	return charNum;
}
unsigned long getLineNum(string filename) {

	ifstream input(filename);
	unsigned long lines = 0;
	string str;
	while (!input.eof()) {

		getline(input, str);
		lines++;
	}
	return lines;
}
void fileProcess(string filename, amount &result, vector<word> &wvec, vector<phrase> &pvec) {

	ifstream input;
	stringstream buffer;
	string srcStr;

	try {
		input.open(filename);
		if (!input.is_open()) {
			throw runtime_error("cannot open the file");
		}
	}
	catch (runtime_error err) {
		cout << err.what();
		return;
	}


	if (input.eof())
		return;

	buffer << input.rdbuf();
	srcStr = buffer.str();

	result.charNum += getCharNum(srcStr);
	result.lineNum += getLineNum(filename);

	
	getNewExpr(srcStr, wvec, pvec, result);


	input.close();
}
void getResult(const char* resfile, amount &result, vector<word> &wvec, vector<phrase> &pvec) {

	auto wvecSize = wvec.size();
	auto pvecSize = pvec.size();
	ofstream output(resfile);
	vector<word>::iterator wbeg = wvec.begin(), wend = wvec.end(), witr;
	sort(wbeg, wend, wordCompare);
	cout << "请输入N"<<endl;
	cin >> N;
	output << "文件中出现次数最多的"<<N<<"个单词：" << endl;
	if (wvecSize) {

		if (wvecSize < N) {
			for (witr = wbeg; witr != wend; witr++) {
				witr->printWord(output);
			}
		}
		else {
			vector<word>::iterator wlast = wbeg + N;
			for (witr = wbeg; witr != wlast; witr++) {
				witr->printWord(output);
			}
		}
	}
	cout << "请在Result.txt中查看结果" << endl;
}
int getAllFiles(string path, vector<string> &files)
{
	long   hFile = 0;
	int flag = -1;

	struct _finddata_t fileinfo;
	string p;

	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		flag = 0;
		while (_findnext(hFile, &fileinfo) == 0)
		{
			if ((fileinfo.attrib &  _A_SUBDIR)) 
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					
					getAllFiles(p.assign(path).append("/").append(fileinfo.name), files);
				}
			}
			else   
			{
				files.push_back(p.assign(path).append("/").append(fileinfo.name));
			}
		}
		_findclose(hFile);
	}

	return flag;
}


int main(int argc, char* argv[]) {

	amount  result;
	result.charNum = 0;
	result.lineNum = 0;
	result.wordNum = 0;
	vector<word> wvec;
	vector<phrase> pvec;
	int dirFlag;
	vector<string> fvec;
	string path = "1.I Have a Dream.txt";
	const char* resFile = "Result.txt";
	dirFlag = getAllFiles(path, fvec);
	if (dirFlag == 0) {

		vector<string>::iterator end = fvec.end(), it;
		for (it = fvec.begin(); it != end; it++) {
			fileProcess(*it, result, wvec, pvec);
		}
	}
	else {
		fileProcess(path, result, wvec, pvec);
	}

	getResult(resFile, result, wvec, pvec);
	system("pause");
}