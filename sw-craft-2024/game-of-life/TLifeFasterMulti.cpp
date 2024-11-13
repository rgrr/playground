/*
 * TLifeFasterMulti.cpp
 *
 *  Created on: Nov 11, 2024
 *      Author: i02441001
 */

#include <chrono>
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
    std::vector<std::thread> threads;
    const uint32_t threads_max = 4; // std::thread::hardware_concurrency();

//    printf("---------------------new game with %d threads\n", threads_max);

    uint32_t x = 0;
    const uint32_t dx = rows / threads_max + 1;
    while (x < rows)
    {
        const uint32_t xmax = x + dx;

        threads.push_back(std::thread(&TLifeFasterMulti::NextGenerationThread, this,
                                      std::ref(newField), std::ref(newFieldWithNeighbourCnt), x, xmax));
        x = xmax;
    }

//    std::this_thread::sleep_for(std::chrono::milliseconds(0));

    for (std::thread &t: threads)
    {
        t.join();
    }

    field = newField;
    fieldWithNeighbourCnt = newFieldWithNeighbourCnt;
}


void TLifeFasterMulti::NextGenerationThread(TLifeField &newField, TLifeField &newFieldWithNeighbourCnt,
                                            uint32_t row_min, uint32_t row_max)
{
//    std::this_thread::sleep_for(std::chrono::milliseconds(0));

    if (row_max > rows)
    {
        row_max = rows;
    }

//    printf("thread %d %d\n", row_min, row_max);

    for (uint32_t x = row_min;  x < row_max;  ++x)
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

//    printf("thread %d %d fin\n", row_min, row_max);
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
