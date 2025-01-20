#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <thread>
#include <mutex>
using namespace std;

struct resultOfSes
{
    int grade;
    int sem;
    string subject;
};

struct Student
{
    string FIO;
    string group;
    vector<resultOfSes> results;
};

mutex mtx;
double partialSum = 0;
double partialCount = 0;

void calculatePartialAverage(const vector<Student> &students, string G, int S, size_t start, size_t end)
{
    double localSum = 0;
    int localCount = 0;

    for (size_t i = start; i < end; ++i)
    {
        const Student &student = students[i];
        if (student.group == G)
        {
            for (const auto &session : student.results)
            {
                if (session.sem == S)
                {
                    localSum += session.grade;
                    localCount++;
                }
            }
        }
    }

    // Блок синхронизации
    mtx.lock();
    partialSum += localSum;
    partialCount += localCount;
    mtx.unlock();
}

void multiThread(const vector<Student> &students, string G, int S, int numThreads)
{
    size_t totalStudents = students.size();
    size_t blockSize = totalStudents / numThreads;

    vector<thread> threads;

    auto startTime = chrono::high_resolution_clock::now();

    for (int i = 0; i < numThreads; ++i)
    {
        size_t start = i * blockSize;
        size_t end = (i == numThreads - 1) ? totalStudents : (i + 1) * blockSize;

        threads.emplace_back(calculatePartialAverage, ref(students), G, S, start, end);
    }

    for (auto &t : threads)
    {
        t.join();
    }

    double result = partialCount > 0 ? partialSum / partialCount : 0.0;

    auto endTime = chrono::high_resolution_clock::now();

    cout << "Multithreaded Time: " << fixed << setprecision(10)
         << chrono::duration<double>(endTime - startTime).count()
         << " Result: " << result << endl;

    // Очистка глобальных переменных для следующего вызова
    partialSum = 0;
    partialCount = 0;
}

void singleThread(const vector<Student> &students, string G, int S)
{
    auto start = chrono::high_resolution_clock::now();
    double sum = 0;
    int kolvo = 0;

    for (const auto &student : students)
    {
        if (student.group == G)
        {
            for (const auto &session : student.results)
            {
                if (session.sem == S)
                {
                    sum += session.grade;
                    kolvo++;
                }
            }
        }
    }

    double result = sum / kolvo;

    auto end = chrono::high_resolution_clock::now();
    cout << "Single-threaded Time: " << fixed << setprecision(10)
         << chrono::duration<double>(end - start).count()
         << " Result: " << result << endl;
}

vector<Student> generateRandomStudents(int n)
{
    srand(time(0)); // Инициализация генератора случайных чисел
    vector<string> names = {"Иванов Иван", "Пузина Арина", "Голомедов Максим",
                            "Токмак Яна", "Пупкин Иван", "Сидоров Степан"};
    vector<string> groups = {"A", "B", "C"};
    vector<string> subjects = {"Математика", "Физика"};

    vector<Student> students;
    students.reserve(n);

    for (int i = 0; i < n; ++i)
    {
        students.push_back({names[i % names.size()],
                            groups[rand() % 3], // Генерация группы (A, B или C)
                            {
                                {rand() % 4 + 2, 1, subjects[0]}, // Сессия 1 по Математике (оценка от 2 до 5)
                                {rand() % 4 + 2, 1, subjects[1]}, // Сессия 1 по Физике (оценка от 2 до 5)
                                {rand() % 4 + 2, 2, subjects[0]}, // Сессия 2 по Математике (оценка от 2 до 5)
                                {rand() % 4 + 2, 2, subjects[1]}  // Сессия 2 по Физике (оценка от 2 до 5)
                            }});
    }

    return students;
}

int main()
{
    auto students = generateRandomStudents(100000);

    cout << "Single-threaded computation:\n";
    singleThread(students, "A", 1);

    cout << "\nMulti-threaded computation:\n";
    multiThread(students, "A", 1, 4);

    return 0;
}