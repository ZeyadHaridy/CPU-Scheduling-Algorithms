#include "scheduler.h"
#include <iostream>
#include <iomanip>
#include <map>
#include <queue>
#include <math.h>

using namespace std;

void Scheduler::execute()
{
    for (int i = 0; i < schedulongPoliceis.size(); i++)
    {
        clearTables();
        if (schedulongPoliceis[i].first == 1)
        {
            FCFS();
            if (type == "trace")
                trace(1, -1);
            else
                stats(1, -1);
        }
        else if (schedulongPoliceis[i].first == 2)
        {
            RR(schedulongPoliceis[i].second);
            if (type == "trace")
                trace(2, schedulongPoliceis[i].second);
            else
                stats(2, schedulongPoliceis[i].second);
        }
        else if (schedulongPoliceis[i].first == 3)
        {
            SPN();
            if (type == "trace")
                trace(3, -1);
            else
                stats(3, -1);
        }
        else if (schedulongPoliceis[i].first == 4)
        {
            SRT();
            if (type == "trace")
                trace(4, -1);
            else
                stats(4, -1);
        }
        else if (schedulongPoliceis[i].first == 5)
        {
            HRRN();
            if (type == "trace")
                trace(5, -1);
            else
                stats(5, -1);
        }
        else if (schedulongPoliceis[i].first == 6)
        {
            FB1();
            if (type == "trace")
                trace(6, -1);
            else
                stats(6, -1);
        }
        else if (schedulongPoliceis[i].first == 7)
        {
            FB2i();
            if (type == "trace")
                trace(7, -1);
            else
                stats(7, -1);
        }
        else if (schedulongPoliceis[i].first == 8)
        {
            AGE(schedulongPoliceis[i].second);
            if (type == "trace")
                trace(8, -1);
        }
    }
}
void Scheduler::trace(int policy, int argument)
{
    if (policy == 1)
    {
        cout << "FCFS  ";
        printHeader();
        cout << '\n';
        printTracing();
        printDashes();
        cout << "\n\n";
    }
    else if (policy == 2)
    {
        if (argument > 10)
            cout << "RR-" << argument << " ";
        else
            cout << "RR-" << argument << "  ";
        printHeader();
        cout << '\n';
        printTracing();
        printDashes();
        cout << "\n\n";
    }
    else if (policy == 3)
    {
        cout << "SPN   ";
        printHeader();
        cout << "\n";
        printTracing();
        printDashes();
        cout << "\n\n";
    }
    else if (policy == 4)
    {
        cout << "SRT   ";
        printHeader();
        cout << '\n';
        printTracing();
        printDashes();
        cout << "\n\n";
    }
    else if (policy == 5)
    {
        cout << "HRRN  ";
        printHeader();
        cout << "\n";
        printTracing();
        printDashes();
        cout << "\n\n";
    }
    else if (policy == 6)
    {
        cout << "FB-1  ";
        printHeader();
        cout << "\n";
        printTracing();
        printDashes();
        cout << "\n\n";
    }
    else if (policy == 7)
    {
        cout << "FB-2i ";
        printHeader();
        cout << "\n";
        printTracing();
        printDashes();
        cout << "\n\n";
    }
    else if (policy == 8)
    {
        cout << "Aging ";
        printHeader();
        cout << "\n";
        printTracing();
        printDashes();
        cout << "\n\n";
    }
}
void Scheduler::stats(int policy, int argument)
{
    if (policy == 1)
    {
        cout << "FCFS" << endl;
        printStats();
        cout << '\n';
    }
    else if (policy == 2)
    {
        cout << "RR-" << argument << endl;
        printStats();
        cout << '\n';
    }
    else if (policy == 3)
    {
        cout << "SPN" << endl;
        printStats();
        cout << '\n';
    }
    else if (policy == 4)
    {
        cout << "SRT" << endl;
        printStats();
        cout << '\n';
    }
    else if (policy == 5)
    {
        cout << "HRRN" << endl;
        printStats();
        cout << '\n';
    }
    else if (policy == 6)
    {
        cout << "FB-1" << endl;
        printStats();
        cout << '\n';
    }
    else if (policy == 7)
    {
        cout << "FB-2i" << endl;
        printStats();
        cout << '\n';
    }
}
void Scheduler::FCFS()
{
    int time = 0;

    while (time < maxSeconds)
    {
        for (int i = 0; i < numberOfProcesses; i++)
        {
            if (processes[i].arrivalTime == time)
            {
                readyQueue.push(processes[i]);
            }
        }

        if (!processorBusy && !readyQueue.empty())
        {
            currentProcess = readyQueue.front();
            readyQueue.pop();
            processorBusy = true;
        }

        queue<Process> temp = readyQueue;
        while (!temp.empty())
        {
            Process p = temp.front();
            temp.pop();
            *(processesPrintingArray + p.id * maxSeconds + time) = '.';
        }

        if (processorBusy)
        {
            *(processesPrintingArray + currentProcess.id * maxSeconds + time) = '*';
            currentProcess.remainingTime--;

            if (currentProcess.remainingTime == 0)
            {
                int idx = currentProcess.id;
                processes[idx].finishTime = time + 1;
                processes[idx].turnAroundTime =
                    processes[idx].finishTime - processes[idx].arrivalTime;
                processes[idx].NormTurnTime =
                    processes[idx].turnAroundTime * 1.0 / processes[idx].serveTime;

                processorBusy = false;
            }
        }

        time++;
    }
}


void Scheduler::RR(int quantum) {
    int time = 0;
    queue<int> rq;
    int running = -1;
    int qLeft = 0;

    while (time < maxSeconds) {
        for (int i = 0; i < numberOfProcesses; i++) {
            if (processes[i].arrivalTime == time) {
                rq.push(i);
            }
        }

        if (running != -1) {
            if (processes[running].remainingTime == 0) {
                running = -1;
            } else if (qLeft == 0) {
                rq.push(running);
                running = -1;
            }
        }

        if (running == -1 && !rq.empty()) {
            running = rq.front();
            rq.pop();
            qLeft = quantum;
        }

        queue<int> temp = rq;
        while (!temp.empty()) {
            processesPrintingArray[temp.front() * maxSeconds + time] = '.';
            temp.pop();
        }

        if (running != -1) {
            processesPrintingArray[running * maxSeconds + time] = '*';
            processes[running].remainingTime--;
            qLeft--;
            
            if (processes[running].remainingTime == 0) {
                processes[running].finishTime = time + 1;
                processes[running].turnAroundTime = processes[running].finishTime - processes[running].arrivalTime;
                processes[running].NormTurnTime = (float)processes[running].turnAroundTime / processes[running].serveTime;
            }
        }

        time++;
    }
}


void Scheduler::SPN()
{
    int time = 0;

    while (time < maxSeconds)
    {
        for (int i = 0; i < numberOfProcesses; i++)
        {
            if (processes[i].arrivalTime == time)
            {
                readyPriorityQueue.push({-processes[i].serveTime, i});
            }
        }

        if (!processorBusy && !readyPriorityQueue.empty())
        {
            int idx = readyPriorityQueue.top().second;
            readyPriorityQueue.pop();
            currentProcess = processes[idx];
            processorBusy = true;
        }

        priority_queue<pair<float, int>> temp = readyPriorityQueue;
        while (!temp.empty())
        {
            int pid = temp.top().second;
            temp.pop();
            *(processesPrintingArray + pid * maxSeconds + time) = '.';
        }

        if (processorBusy)
        {
            *(processesPrintingArray + currentProcess.id * maxSeconds + time) = '*';
            currentProcess.remainingTime--;

            if (currentProcess.remainingTime == 0)
            {
                int idx = currentProcess.id;
                processes[idx].finishTime = time + 1;
                processes[idx].turnAroundTime =
                    processes[idx].finishTime - processes[idx].arrivalTime;
                processes[idx].NormTurnTime =
                    processes[idx].turnAroundTime * 1.0 / processes[idx].serveTime;

                processorBusy = false;
            }
        }

        time++;
    }
}


void Scheduler::SRT()
{
    int time = 0;
    int runningId = -1;

    while (time < maxSeconds)
    {
        bool newArrival = false;
        for (int i = 0; i < numberOfProcesses; i++) {
            if (processes[i].arrivalTime == time) newArrival = true;
        }

        if (runningId == -1 || newArrival) {
            int bestId = -1;
            int minRemaining = 1e9;

            for (int i = 0; i < numberOfProcesses; i++) {
                if (processes[i].arrivalTime <= time && processes[i].remainingTime > 0) {
 
                    if (processes[i].remainingTime < minRemaining) {
                        minRemaining = processes[i].remainingTime;
                        bestId = i;
                    }
                }
            }

            if (bestId != -1) {
                runningId = bestId;
            }
        }

        for (int i = 0; i < numberOfProcesses; i++) {
            if (i != runningId && processes[i].arrivalTime <= time && processes[i].remainingTime > 0) {
                *(processesPrintingArray + i * maxSeconds + time) = '.';
            }
        }

        if (runningId != -1) {
            *(processesPrintingArray + runningId * maxSeconds + time) = '*';
            processes[runningId].remainingTime--;

            if (processes[runningId].remainingTime == 0) {
                processes[runningId].finishTime = time + 1;
                processes[runningId].turnAroundTime = processes[runningId].finishTime - processes[runningId].arrivalTime;
                processes[runningId].NormTurnTime = (float)processes[runningId].turnAroundTime / processes[runningId].serveTime;
                
                runningId = -1;
            }
        }
        time++;
    }
}


void Scheduler::HRRN()
{
    int time = 0;
    int runningId = -1;

    while (time < maxSeconds)
    {
        if (runningId == -1)
        {
            float bestRatio = -1.0f;
            int chosen = -1;

            for (int i = 0; i < numberOfProcesses; i++)
            {
                if (processes[i].arrivalTime <= time &&
                    processes[i].remainingTime > 0)
                {
                    int waiting =
                        time - processes[i].arrivalTime -
                        (processes[i].serveTime - processes[i].remainingTime);

                    float ratio =
                        (waiting + processes[i].serveTime) * 1.0f /
                        processes[i].serveTime;

                    if (ratio > bestRatio)
                    {
                        bestRatio = ratio;
                        chosen = i;
                    }
                }
            }

            runningId = chosen;
        }

        for (int i = 0; i < numberOfProcesses; i++)
        {
            if (i != runningId &&
                processes[i].arrivalTime <= time &&
                processes[i].remainingTime > 0)
            {
                *(processesPrintingArray + i * maxSeconds + time) = '.';
            }
        }

        if (runningId != -1)
        {
            *(processesPrintingArray + runningId * maxSeconds + time) = '*';
            processes[runningId].remainingTime--;

            if (processes[runningId].remainingTime == 0)
            {
                processes[runningId].finishTime = time + 1;
                processes[runningId].turnAroundTime =
                    processes[runningId].finishTime - processes[runningId].arrivalTime;
                processes[runningId].NormTurnTime =
                    processes[runningId].turnAroundTime * 1.0 /
                    processes[runningId].serveTime;

                runningId = -1;
            }
        }

        time++;
    }
}


void Scheduler::FB1()
{
    int time = 0;
    vector<queue<int>> fb_queues(maxSeconds);
    int lastRunningIdx = -1; 

    while (time < maxSeconds)
    {
        for (int i = 0; i < numberOfProcesses; i++)
        {
            if (processes[i].arrivalTime == time)
            {
                processes[i].FBLevel = 0;
                fb_queues[0].push(i);
            }
        }

        int runningIdx = -1;
        
        for (int lvl = 0; lvl < maxSeconds; lvl++)
        {
            if (!fb_queues[lvl].empty())
            {
                int candidate = fb_queues[lvl].front();
                if (candidate != lastRunningIdx) {
                    runningIdx = candidate;
                    fb_queues[lvl].pop();
                    break;
                }
            }
        }

        if (runningIdx == -1)
        {
             for (int lvl = 0; lvl < maxSeconds; lvl++)
            {
                if (!fb_queues[lvl].empty())
                {
                    runningIdx = fb_queues[lvl].front();
                    fb_queues[lvl].pop();
                    break;
                }
            }
        }

        if (runningIdx != -1)
        {
            processesPrintingArray[runningIdx * maxSeconds + time] = '*';
            processes[runningIdx].remainingTime--;

            if (processes[runningIdx].remainingTime == 0)
            {
                processes[runningIdx].finishTime = time + 1;
                processes[runningIdx].turnAroundTime =
                    processes[runningIdx].finishTime - processes[runningIdx].arrivalTime;
                processes[runningIdx].NormTurnTime =
                    (float)processes[runningIdx].turnAroundTime /
                    processes[runningIdx].serveTime;
                
                lastRunningIdx = -1; 
            }
            else
            {
                int nextLevel = processes[runningIdx].FBLevel + 1;
                if (nextLevel >= maxSeconds) nextLevel = maxSeconds - 1;
                
                processes[runningIdx].FBLevel = nextLevel;
                fb_queues[nextLevel].push(runningIdx);
                
                lastRunningIdx = runningIdx;
            }
        }
        else 
        {
            lastRunningIdx = -1;
        }

        for (int lvl = 0; lvl < maxSeconds; lvl++)
        {
            if (fb_queues[lvl].empty()) continue;
            
            queue<int> temp = fb_queues[lvl];
            while (!temp.empty())
            {
                int pid = temp.front();
                temp.pop();
                
                if (pid != runningIdx)
                {
                    if (pid >= 0 && pid < numberOfProcesses) 
                         processesPrintingArray[pid * maxSeconds + time] = '.';
                }
            }
        }

        time++;
    }
}
void Scheduler::FB2i()
{
    int time = 0;
    vector<queue<int>> queues(100); 
    int running = -1;
    int quantumLeft = 0;

    while (time < maxSeconds)
    {
        for (int i = 0; i < numberOfProcesses; i++)
        {
            if (processes[i].arrivalTime == time)
            {
                processes[i].FBLevel = 0;
                queues[0].push(i);
            }
        }

        if (running != -1)
        {
            if (processes[running].remainingTime == 0)
            {
                processes[running].finishTime = time;
                running = -1;
                quantumLeft = 0;
            }
            else if (quantumLeft == 0)
            {
                bool someoneWaiting = false;
                for (int lvl = 0; lvl < 100; lvl++)
                {
                    if (!queues[lvl].empty())
                    {
                        someoneWaiting = true;
                        break;
                    }
                }

                if (someoneWaiting)
                {
                    int nextLevel = processes[running].FBLevel + 1;
                    if (nextLevel > 99) nextLevel = 99;
                    processes[running].FBLevel = nextLevel;
                    queues[nextLevel].push(running);
                    running = -1;
                }
                else
                {
                    quantumLeft = 1 << processes[running].FBLevel;
                }
            }

        }

        if (running == -1)
        {
            for (int lvl = 0; lvl < 100; lvl++)
            {
                if (!queues[lvl].empty())
                {
                    running = queues[lvl].front();
                    queues[lvl].pop();
                    quantumLeft = 1 << processes[running].FBLevel;
                    break;
                }
            }
        }

        for (int lvl = 0; lvl < 100; lvl++)
        {
            queue<int> temp = queues[lvl];
            while (!temp.empty())
            {
                int pid = temp.front();
                temp.pop();
                if (pid != running)
                    processesPrintingArray[pid * maxSeconds + time] = '.';
            }
        }

        if (running != -1)
        {
            processesPrintingArray[running * maxSeconds + time] = '*';
            processes[running].remainingTime--;
            quantumLeft--;

            if (processes[running].remainingTime == 0)
            {
                processes[running].finishTime = time + 1;
                processes[running].turnAroundTime = processes[running].finishTime - processes[running].arrivalTime;
                processes[running].NormTurnTime = (float)processes[running].turnAroundTime / processes[running].serveTime;
            }
        }
        time++;
    }
}


void Scheduler::AGE(int quantum) {
    int time = 0;
    Process* running = nullptr;
    int qLeft = 0;
    
    vector<Process*> readyList;

    while (time < maxSeconds) {
        for (int i = 0; i < numberOfProcesses; i++) {
            if (processes[i].arrivalTime == time) {
                processes[i].currentPriority = processes[i].priority;
                readyList.push_back(&processes[i]);
            }
        }

        if (running == nullptr || qLeft == 0) {
            
            if (running != nullptr) {
                running->currentPriority = running->priority;
                readyList.push_back(running);
                running = nullptr;
            }

            for (auto p : readyList) {
                p->currentPriority++;
            }

            if (!readyList.empty()) {
                int bestIdx = 0;
                for (int i = 1; i < readyList.size(); i++) {
                    if (readyList[i]->currentPriority > readyList[bestIdx]->currentPriority) {
                        bestIdx = i;
                    }
                }

                running = readyList[bestIdx];
                readyList.erase(readyList.begin() + bestIdx);
                qLeft = quantum;
            }
        }

        for (auto p : readyList) {
            *(processesPrintingArray + p->id * maxSeconds + time) = '.';
        }

        if (running != nullptr) {
            *(processesPrintingArray + running->id * maxSeconds + time) = '*';
            qLeft--;
        }

        time++;
    }
}


void Scheduler::printTracing()
{
    for (int process = 0; process < numberOfProcesses; process++)
    {
        cout << processes[process].processName << "     |";
        for (int time = 0; time < maxSeconds; time++)
            cout << *(processesPrintingArray + process * maxSeconds + time) << '|';
        cout << " \n";
    }
}
void Scheduler::printStats()
{
    float sum = 0, mean = 0, sum2 =0;
    cout << "Process    |";
    for (int i = 0; i < numberOfProcesses; i++)
        cout << "  " << processes[i].processName << "  |";
    cout << endl;
    cout << "Arrival    |";
    for (int i = 0; i < numberOfProcesses; i++)
    {
        if (processes[i].arrivalTime < 10)
            cout << "  " << processes[i].arrivalTime << "  |";
        else
            cout << " " << processes[i].arrivalTime << "  |";
    }
    cout << endl;
    cout << "Service    |";
    for (int i = 0; i < numberOfProcesses; i++)
    {
        if (processes[i].arrivalTime < 10)
            cout << "  " << processes[i].serveTime << "  |";
        else
            cout << " " << processes[i].serveTime << "  |";
    }
    cout << " Mean|" << endl;
    cout << "Finish     |";
    for (int i = 0; i < numberOfProcesses; i++)
    {
        if (processes[i].finishTime >= 10)
            cout << " " << processes[i].finishTime << "  |";
        else
            cout << "  " << processes[i].finishTime << "  |";
    }
    cout << "-----|" << endl;
    cout << "Turnaround |";
    for (int i = 0; i < numberOfProcesses; i++)
    {

        if (processes[i].turnAroundTime >= 10)
            cout << " " << (int)processes[i].turnAroundTime << "  |";
        else
            cout << "  " << (int)processes[i].turnAroundTime << "  |";
        sum += processes[i].turnAroundTime;
    }
    cout << fixed;
    cout << setprecision(2);
    mean = (sum * 1.0) / numberOfProcesses;
    if (mean >= 10)
        cout << mean << "|";
    else
        cout << " " << mean << "|";
    cout << endl;
    cout << "NormTurn   |";
    sum2 = 0;
    for (int i = 0; i < numberOfProcesses; i++)
    {
        if (processes[i].NormTurnTime > 10)
            cout << processes[i].NormTurnTime << "|";
        else
            cout << " " << processes[i].NormTurnTime << "|";
        sum2 += (processes[i].NormTurnTime * 1.0);
    }

    mean = (sum2 * 1.0) / numberOfProcesses;
    if (mean > 10)
        cout << mean << "|";
    else
        cout << " " << mean << "|";
    cout << endl;
}
void Scheduler::clearTables()
{

    for (int i = 0; i < numberOfProcesses; i++)
    {
        for (int j = 0; j < maxSeconds; j++)
            *(processesPrintingArray + i * maxSeconds + j) = ' ';
    }
    for (int i = 0; i < numberOfProcesses; i++)
    {
        processes[i].finishTime = 0;
        processes[i].turnAroundTime = 0;
        processes[i].NormTurnTime = 0;
        processes[i].processState = ' ';
        processes[i].remainingTime = processes[i].serveTime;
    }
    processorBusy = false;
    while (!readyQueue.empty())
        readyQueue.pop();
    while (!readyPriorityQueue.empty())
        readyPriorityQueue.pop();
}
void Scheduler::splitPolicyAndParameter(string str)
{
    string w = "";
    pair<int, int> policies;
    bool parameterExists = false;
    policies.second = -1;
    for (int i = 0; i < str.length(); i++)
    {
        if (str[i] == '-')
        {
            parameterExists = true;
            policies.first = stoi(w);
            w = "";
        }
        else if (str[i] == ',')
        {
            if (parameterExists)
                policies.second = stoi(w);
            else
            {
                policies.first = stoi(w);
                policies.second = -1;
            }
            w = "";
            schedulongPoliceis.push_back(policies);
            parameterExists = false;
        }
        else
            w = w + str[i];
    }
    if (parameterExists)
        policies.second = stoi(w);
    else
        policies.first = stoi(w);
    schedulongPoliceis.push_back(policies);
}
void Scheduler::splitPrcoessAndTimes(string str, int id)
{
    Process process;
    string w = "";
    process.processName = str[0];
    for (int i = 2; i < str.length(); i++)
    {
        if (str[i] == ',')
        {
            process.arrivalTime = stoi(w);
            w = "";
        }
        else
            w = w + str[i];
    }
    processorBusy = false;
    process.processState = ' ';
    if (schedulongPoliceis[0].first == 8)
    {
        process.priority = stoi(w);
        process.currentPriority = stoi(w);
    }
    else
        process.serveTime = stoi(w);
    process.remainingTime = process.serveTime;
    process.waitingTime = 0;
    process.id = id;
    processes.push_back(process);
}
void Scheduler::readFile()
{
    processorBusy = false;
    string temp1, temp2;
    cin >> type;
    cin >> temp1;
    splitPolicyAndParameter(temp1);
    cin >> maxSeconds;
    cin >> numberOfProcesses;

    for (int i = 0; i < numberOfProcesses; i++)
    {
        cin >> temp2;
        splitPrcoessAndTimes(temp2, i);
    }
    processesPrintingArray = new char[numberOfProcesses * maxSeconds];
    currentProcess.processName = 0;
    currentProcess.q = 0;
    clearTables();
}
void Scheduler::printHeader()
{
    for (int i = 0; i < maxSeconds + 1; i++)
        cout << i % 10 << ' ';
    cout << "\n";
    printDashes();
}
void Scheduler::printDashes()
{
    cout << "------------------------------------------------";
}
int main(void)
{
    Scheduler scheduler;
    scheduler.readFile();
    scheduler.execute();
    return 0;
}