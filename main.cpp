#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>

#define MAX_VALUE 2147483647
#define Test "TestSimpler.txt"

using namespace std;

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition (vector <int> &array, int low, int high) {
    int pivot = array[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++)
    {
        if (array[j] < pivot)
        {
            i++;
            swap(&array[i], &array[j]);
        }
    }
    swap(&array[i + 1], &array[high]);
    return (i + 1);
}

void quickSort(vector <int> &array, int low, int high)
{
    if (low < high)
    {
        int pi = partition(array, low, high);

        quickSort(array, low, pi - 1);
        quickSort(array, pi + 1, high);
    }
}

vector <int> read(const string &fileName, int amount, int &position) {
    vector <int> result;
    ifstream file(fileName);
    int buffer;
    file.seekg(position);

    if(file) {
        for (int i = 0; i < amount; ++i) {
            if (!file.eof()) {
                file >> buffer;
                result.push_back(buffer);
            }
        }
    }

    position = file.tellg();
    file.close();
    return result;
}

int read(const string &fileName, int &position) {
    ifstream file(fileName);
    int buffer;
    file.seekg(position);

    if(file) {
        if (!file.eof()) {
            file >> buffer;
        }
    }

    position = file.tellg();
    file.close();
    return buffer;
}

void write(const string &fileName, const vector <int> &array) {
    ofstream file(fileName, ios_base::app);
    if (file) {
        for (int i : array) {
            file << i << " ";
        }
        file.close();
    }
}

void write(const string &fileName, const int &value) {
    ofstream file(fileName, ios_base::app);
    if (file) {
        file << value << " ";
        file.close();
    }
}

void clear(const string &fileName) {
    ofstream file(fileName);
    if(file) {
        file.clear();
        file.close();
    } else {
        cout << "Error! Something wrong with file " + fileName;
    }
}

vector<string> creatingFiles(int amount) {
    vector <string> names;
    string name = "File";
    char buffer[10];
    for (int i = 0; i < amount; ++i) {
        char *numberToChar = itoa(i, buffer, 10);
        name += numberToChar;
        ofstream file(name);
        names.push_back(name);
        name = "File";
    }
    return names;
}

int sumOfChunks(const vector<int> &quantityOfChunks) {
    int sum = 0;
    for (int quantityOfChunk : quantityOfChunks) {
        sum += quantityOfChunk;
    }
    return sum;
}

vector<int> calculationQuantityOfChunks(int amountOfFiles, int neededMemory) {
    vector <int> chuncks(amountOfFiles);
    int maxValue = 0, indexOfMax = 0;
    chuncks[0] = 1;
    for (int i = 1; i < (int)chuncks.size(); ++i) chuncks[i] = 0;
    while (sumOfChunks(chuncks) < neededMemory) {
        for (int i = 0; i < (int)chuncks.size(); ++i) {
            if (chuncks[i] > maxValue) {
                maxValue = chuncks[i];
                indexOfMax = i;
            }
        }
        for (int i = 0; i < (int)chuncks.size(); ++i) {
            if (i != indexOfMax)
                chuncks[i] += maxValue;
            else
                chuncks[i] = 0;
        }
        maxValue = 0;
        indexOfMax = 0;
    }
    return chuncks;
}

bool isSorted(const vector <string> &names) {

    int endOfTestFile = 0;
    ifstream fileTest(Test);
    if (fileTest) {
        fileTest.seekg(0, ios_base::end);
        endOfTestFile = fileTest.tellg();
        fileTest.close();
    }
    int endOfFile;
    for (const string &name : names) {
        ifstream file(name);
        if (file) {
            file.seekg(0, ios_base::end);
            endOfFile = file.tellg();
            if(endOfFile > endOfTestFile) {
                file.close();
                return true;
            }
            file.close();
        }
    }
    return false;
}

void divide(const string &fileSource, const vector <string> &namesOfFiles, const vector <int> &chunks, int size) {
    string output = namesOfFiles[0];
    vector <int> buffer;
    int index = 0;
    static int currentPosition = 0;
    while (currentPosition >= 0) {
        for (int i = 0; i < chunks[index]; ++i) {
            buffer = read(fileSource, size, currentPosition);
            quickSort(buffer, 0, (int) buffer.size() - 1);
            write(output, buffer);
            write(output, MAX_VALUE);
        }
        index++;
        if (index < namesOfFiles.size()) {
            output = namesOfFiles[index];
        }
    }
}

void searchingMinValue(const vector <int> &buffer, int &minValue, int &indexMin, int emptyFile) {
    for (int i = 0; i < (int)buffer.size(); ++i) {
        if (i != emptyFile) {
            if (minValue > buffer[i]) {
                minValue = buffer[i];
                indexMin = i;
            }
        }
    }
}

int isEndOfFile(const vector <string> &namesOfFiles, const vector <int> &positions) {
    for (int i = 0; i < (int)positions.size(); ++i) {
        ifstream file(namesOfFiles[i]);
        if (file) {
            int current = positions[i];
            file.seekg(0, ios_base::end);
            int end = file.tellg();
            if(end - current == 1) {
                file.close();
                return i;
            }
            file.close();
        }
    }
    return -1;
}

bool isEndOfChunks(const vector <int> &buffer) {
    int counter = 0;
    for (int i : buffer) {
        if (i == MAX_VALUE)
            ++counter;
    }
    return counter == buffer.size();
}

void merge(const vector <string> &namesOfFiles, int &emptyFileOnStart) {

    string output = namesOfFiles[emptyFileOnStart];
    vector <int> bufferForEachFile(namesOfFiles.size());
    vector <int> positionForEachBuffer(namesOfFiles.size());
    int minValue = MAX_VALUE, indexOfMinValue, emptyFile = emptyFileOnStart, newEmptyFile;

    while (!isSorted(namesOfFiles)) {

        for (int i = 0; i < (int)namesOfFiles.size(); ++i) {
            if (i != emptyFile)
                bufferForEachFile[i] = read(namesOfFiles[i], positionForEachBuffer[i]);
        }

        bufferForEachFile[emptyFile] = MAX_VALUE;
        positionForEachBuffer[emptyFile] = MAX_VALUE;
        searchingMinValue(bufferForEachFile, minValue, indexOfMinValue, emptyFile);

        while (!isEndOfChunks(bufferForEachFile)) {
            write(output, minValue);
            bufferForEachFile[indexOfMinValue] = read(namesOfFiles[indexOfMinValue], positionForEachBuffer[indexOfMinValue]);
            minValue = MAX_VALUE;
            searchingMinValue(bufferForEachFile, minValue, indexOfMinValue, emptyFile);
        }
        write(output, MAX_VALUE);

        newEmptyFile = isEndOfFile(namesOfFiles, positionForEachBuffer);
        if (newEmptyFile != -1) {
            clear(namesOfFiles[newEmptyFile]);
            positionForEachBuffer[emptyFile] = 0;
            emptyFile = newEmptyFile;
        }
        output = namesOfFiles[emptyFile];
    }
}

int emptyFile(const vector <int> &chunks) {
    int i = 0;
    for (i = 0; i < (int)chunks.size(); ++i) {
        if (chunks[i] == 0) return i;
    }
    return i;
}

void readAll(const vector <string> &names) {
    vector <int> data;
    ifstream file(names[0]);
    if (file) {
        while (!file.eof()) {
            int buffer;
            file >> buffer;
            data.push_back(buffer);
        }
        file.close();
    }
    data.shrink_to_fit();
    cout << "size:: " << data.size() << endl;
    for (int i : data) {
        cout << i << endl;
    }
}

void polyphaseMergeSort(int amountOfFiles, int amountOfNumbers, int size) {

    vector <string> namesOfFiles = creatingFiles(amountOfFiles);
    vector <int> chunks = calculationQuantityOfChunks(amountOfFiles, amountOfNumbers / size + 1);
    divide(Test, namesOfFiles, chunks, (amountOfNumbers / sumOfChunks(chunks)) + 1);
    int empty = emptyFile(chunks);
    merge(namesOfFiles, empty);
    readAll(namesOfFiles);
}

/***********************************************************************************************************/

void generateTestingFile(int amount) {
    srand(time(nullptr));
    int temp;
    for (int i = 0; i < amount; ++i) {
        temp = rand() % 1000;
        write(Test, temp);
    }
}

int main() {

    int amountOfFiles, amountOfNumbers, memorySize;
    cin >> amountOfFiles;
    cin >> amountOfNumbers;
    cin >> memorySize;
    generateTestingFile(amountOfNumbers);
    polyphaseMergeSort(amountOfFiles, amountOfNumbers, memorySize);
    return 0;
}