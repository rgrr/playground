/*
 * TLifeFaster.cpp
 *
 *  Created on: Nov 11, 2024
 *      Author: i02441001
 */

#include "TLifeFaster.h"


void TLifeFaster::InitField(const TLifeField& initialState)
{
    uint32_t row_cnt = 0;
    for (const auto & row : initialState)
    {
        uint32_t col_cnt = 0;
        for (int val : row)
        {
            field[row_cnt][col_cnt] = val;
            ++col_cnt;
        }
        ++row_cnt;
    }

    row_cnt = 0;
    for (const auto & row : field)
    {
        uint32_t col_cnt = 0;
        for (int val : row)
        {
            (void)val;
            fieldWithNeighbourCnt[row_cnt][col_cnt] = CountLiveNeighbors(row_cnt, col_cnt);
            ++col_cnt;
        }
        ++row_cnt;
    }
}


void TLifeFaster::NextGeneration()
{
    TLifeField newField = field;
    TLifeField newFieldWithNeighbourCnt = fieldWithNeighbourCnt;

    for (uint32_t i = 0;  i < rows;  ++i)
    {
        for (uint32_t j = 0;  j < cols;  ++j)
        {
            int liveNeighbors = fieldWithNeighbourCnt[i][j];

            if (field[i][j] != 0)
            {
                if (liveNeighbors < 2  ||  liveNeighbors > 3)
                {
                    newField[i][j] = 0;
                    DecLiveNeighbors(newFieldWithNeighbourCnt, i, j);
                }
            }
            else
            {
                if (liveNeighbors == 3)
                {
                    newField[i][j] = 1;
                    IncLiveNeighbors(newFieldWithNeighbourCnt, i, j);
                }
            }
        }
    }
    field = newField;
    fieldWithNeighbourCnt = newFieldWithNeighbourCnt;
}


uint32_t TLifeFaster::CountLiveNeighbors(uint32_t x, uint32_t y) const
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


void TLifeFaster::DecLiveNeighbors(TLifeField& field, uint32_t x, uint32_t y)
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


void TLifeFaster::IncLiveNeighbors(TLifeField& field, uint32_t x, uint32_t y)
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
