/*
 * TLifeFaster.cpp
 *
 *  Created on: Nov 11, 2024
 *      Author: i02441001
 */

#include "TLifeFaster.h"


void TLifeFaster::InitField(const TLifeField& initialState)
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


void TLifeFaster::NextGeneration()
{
    TLifeField newField = field;
    TLifeField newFieldWithNeighbourCnt = fieldWithNeighbourCnt;

    for (uint32_t x = 0;  x < rows;  ++x)
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
