#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <sstream>
#include <cctype>
using namespace std;

//过滤标点符号，保留字母和数字
bool isValidChar(unsigned char c) {
    if (c < 128) {
        return isalnum(c);
    }
    return true;
}

//读取文件内容并过滤标点符号
string readFileAndPreprocess(const string &filepath) {
    ifstream file(filepath);
    if (!file.is_open()) {
        cerr << "错误：无法打开文件。" << filepath << endl;
        exit(1);
    }

    stringstream buffer;
    buffer << file.rdbuf();
    string content = buffer.str();
    file.close();

    string cleanContent;
    for (char c : content) {
        if (isValidChar(c)) {
            cleanContent += c;
        }
    }
    return cleanContent;
}

//将字符串转换为N-gram集合，n=2表示按2个字为一组滑动窗口切分
set<string> getNGrams(const string &text, int n) {
    set<string> ngrams;
    if (text.length() < n) {
        return ngrams;
    }

    for (size_t i = 0; i + n <= text.length(); ++i) {
        ngrams.insert(text.substr(i, n));
    }
    return ngrams;
}

// 计算Jaccard相似度
double calculateSimilarity(const set<string> &setA, const set<string> &setB) {
    if (setA.empty() && setB.empty()) {
        return 1.0;
    }
    if (setA.empty() || setB.empty()) {
        return 0.0;
    }

    int intersectionSize = 0;
    for (const string &s : setA) {
        if (setB.count(s)) {
            intersectionSize++; //计算两段文本交集的字符个数
        }
    }
    int unionSize = setA.size() + setB.size() - intersectionSize; //计算两段文本并集的字符个数
    return (double)intersectionSize / unionSize;
}

int main() {
    //1、读取文件路径并处理文本
    string originalPath, plagiarizedPath, outputPath;
    cout << "请输入原文文件的绝对路径：";
    cin >> originalPath;
    cout << "请输入抄袭文文件的绝对路径：";
    cin >> plagiarizedPath;
    cout << "请输入输出答案文件的绝对路径：";
    cin >> outputPath;
    string textA = readFileAndPreprocess(originalPath);
    string textB = readFileAndPreprocess(plagiarizedPath);

    //2、算法核心：使用N-gram以及Jaccard相似度
    set<string> gramsA = getNGrams(textA, 2);
    set<string> gramsB = getNGrams(textB, 2);

    //3、计算重复率
    double similarity = calculateSimilarity(gramsA, gramsB);

    //4、输出结果到文件
    ofstream outFile(outputPath);
    if (!outFile.is_open()) {
        cerr << "错误：无法创建输出文件。" << outputPath << endl;
        return 1;
    }
    outFile << "重复率：" << similarity * 100 << "%" << endl;
    cout << "计算完成，结果已写入：" << outputPath << endl;
    cout << "重复率为" << similarity * 100 << "%" << endl;

    outFile.close();
    system("pause");
    return 0;
}