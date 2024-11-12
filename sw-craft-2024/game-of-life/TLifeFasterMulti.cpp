/*
 * TLifeFasterMulti.cpp
 *
 *  Created on: Nov 11, 2024
 *      Author: i02441001
 */

#include <iostream>
#include <thread>

#include "TLifeFasterMulti.h"


void TLifeFasterMulti::InitField(const TLifeField& initialState)
/*
 * This actually adds alive cells
 */
{
    uint32_t x = 0;
    for (const auto & row : initialState)
    {
        uint32_t y = 0;
        for (int val : row)
        {
            if (val != 0)
            {
                field[x][y] = val;
                IncLiveNeighbors(fieldWithNeighbourCnt, x, y);
            }
            ++y;
        }
        ++x;
    }
}


void TLifeFasterMulti::NextGeneration()
{
    TLifeField newField = field;
    TLifeField newFieldWithNeighbourCnt = fieldWithNeighbourCnt;
//    const uint32_t threads_max = std::jthread::hardware_concurrency();
    //std::vector<std::jthread> threads(threads_max);

//    printf("threads_max %d\n", threads_max);

#if 0
    for (uint32_t thread_cnt = 0;  thread_cnt < threads_max;  ++thread_cnt)
    {
        //threads[thread_cnt](NextGeneration, newField, newFieldWithNeighbourCnt, 0, rows - 1);
        threads.push_back(std::jthread(&TLifeFasterMulti::NextGeneration, this)); //, newField, newFieldWithNeighbourCnt, 0, rows - 1));
        NextGeneration(newField, newFieldWithNeighbourCnt, 0, rows - 1);
    }
#endif

    std::thread t(&TLifeFasterMulti::NextGenerationThread, this,
                  std::ref(newField), std::ref(newFieldWithNeighbourCnt), 0, rows - 1);
    t.join();

    //NextGeneration(newField, newFieldWithNeighbourCnt, 0, rows - 1);

    field = newField;
    fieldWithNeighbourCnt = newFieldWithNeighbourCnt;
}


void TLifeFasterMulti::NextGenerationThread(TLifeField &newField, TLifeField &newFieldWithNeighbourCnt,
                                            uint32_t row_min, uint32_t row_max)
{
    for (uint32_t x = row_min;  x <= row_max;  ++x)
    {
        for (uint32_t y = 0;  y < cols;  ++y)
        {
            int liveNeighbors = fieldWithNeighbourCnt[x][y];

            if (field[x][y] != 0)
            {
                if (liveNeighbors < 2  ||  liveNeighbors > 3)
                {
                    newField[x][y] = 0;
                    DecLiveNeighbors(newFieldWithNeighbourCnt, x, y);
                }
            }
            else
            {
                if (liveNeighbors == 3)
                {
                    newField[x][y] = 1;
                    IncLiveNeighbors(newFieldWithNeighbourCnt, x, y);
                }
            }
        }
    }
}


uint32_t TLifeFasterMulti::CountLiveNeighbors(uint32_t x, uint32_t y) const
{
    uint32_t sum = 0;

    for (const auto & dx : std::vector<uint32_t>{rows-1, 0, 1})
    {
        for (const auto & dy : std::vector<uint32_t>{cols-1, 0, 1})
        {
            if (dx == 0  &&  dy == 0)
            {
                continue;
            }
            uint32_t neighborX = (x + dx) % rows;
            uint32_t neighborY = (y + dy) % cols;
            sum += field[neighborX][neighborY];
        }
    }
    return sum;
}


void TLifeFasterMulti::DecLiveNeighbors(TLifeField& field, uint32_t x, uint32_t y)
{
    for (const auto & dx : std::vector<uint32_t>{rows-1, 0, 1})
    {
        for (const auto & dy : std::vector<uint32_t>{cols-1, 0, 1})
        {
            if (dx == 0  &&  dy == 0)
            {
                continue;
            }
            uint32_t neighborX = (x + dx) % rows;
            uint32_t neighborY = (y + dy) % cols;
            --field[neighborX][neighborY];
        }
    }
}


void TLifeFasterMulti::IncLiveNeighbors(TLifeField& field, uint32_t x, uint32_t y)
{
    for (const auto & dx : std::vector<uint32_t>{rows-1, 0, 1})
    {
        for (const auto & dy : std::vector<uint32_t>{cols-1, 0, 1})
        {
            if (dx == 0  &&  dy == 0)
            {
                continue;
            }
            uint32_t neighborX = (x + dx) % rows;
            uint32_t neighborY = (y + dy) % cols;
            ++field[neighborX][neighborY];
        }
    }
}
