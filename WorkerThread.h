//
// Created by Henrik Ravnborg on 2026-01-22.
//

#ifndef SCHACK_WORKERTHREAD_H
#define SCHACK_WORKERTHREAD_H
#include "Enums.h"
#include <thread>
#include "Board.h"

using namespace std;

class WorkerThread {
    queue<MoveRequest> jobs;
    thread worker;
    atomic<bool> jobDone;
    Board bitBoard;
    mutex jobMTX;
    condition_variable cv;
    bool haveJob = false;

public:
    WorkerThread(){};
    void inti(){
        worker = thread([this]{
           while(true){
               MoveRequest job;
               haveJob = false;

               {
                   unique_lock<mutex> lock(jobMTX);
                   if (!jobs.empty()){
                       job = jobs.front();
                       jobs.pop();
                       haveJob = true;
                   }
               }
               if (haveJob){
                   bitBoard.on_player_move(job);
                   jobDone = true;
               }else{
                   this_thread::sleep_for(chrono::milliseconds(10));
               }
           }
        });
        worker.detach();
    }

    bool isDone(){
        return jobDone.load();
    }
    void set_isDone(bool value){
        jobDone = value;
    }

    void submitJob(const MoveRequest& job){
        unique_lock<mutex> lock(jobMTX);
        jobs.push(job);
        jobDone = false;
    }

    bool isLegalMove(MoveRequest data){
        return bitBoard.isLegal(data);
    }

    vector<pair<int, int>> getPositions(int x, int y, chessPiece piece){
        return bitBoard.getpositions(x, y,piece);
    }

    Board get_board(){
        return bitBoard;
    }

    void undo_move(){
        bitBoard.pop_back_history();
    }
};


#endif //SCHACK_WORKERTHREAD_H
