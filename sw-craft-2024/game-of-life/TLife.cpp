/*
 * TLife.cpp
 *
 *  Created on: Nov 6, 2024
 *      Author: i02441001
 */

#include "TLife.h"


void TLife::InitField(const TLifeField& initialState)
{
    uint32_t row_cnt = 0;
    for (const auto & row : initialState)
    {
        uint32_t col_cnt = 0;
        for (const auto & val : row)
        {
            field[row_cnt][col_cnt] = val;
            ++col_cnt;
        }
        ++row_cnt;
    }
}


void TLife::NextGeneration()
{
    TLifeField newField = field;
    for (uint32_t x = 0;  x < rows;  ++x)
    {
        for (uint32_t y = 0;  y < cols;  ++y)
        {
            int liveNeighbors = CountLiveNeighbors(x, y);
            if (field[x][y] != 0)
            {
                if (liveNeighbors < 2  || liveNeighbors > 3)
                {
                    newField[x][y] = 0;
                }
            }
            else
            {
                if (liveNeighbors == 3)
                {
                    newField[x][y] = 1;
                }
            }
        }
    }
    field = newField;
}


uint32_t TLife::CountLiveNeighbors(uint32_t x, uint32_t y) const
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
