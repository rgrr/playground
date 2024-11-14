/*
 * TLifeFaster.h
 *
 *  Created on: Nov 11, 2024
 *      Author: i02441001
 */

#ifndef TLIFEFASTER_H_
#define TLIFEFASTER_H_

#include <vector>
#include <cstdint>


using TLifeCell  = uint8_t;
using TLifeField = std::vector<std::vector<TLifeCell>>;

class TLifeFaster {
public:
    virtual ~TLifeFaster() { }

    TLifeFaster(uint32_t rows, uint32_t cols) :
        rows(rows),
        cols(cols),
        field(rows, std::vector<TLifeCell>(cols, 0)),
        fieldWithNeighbourCnt(rows, std::vector<TLifeCell>(cols, 0)) {}
    TLifeFaster(uint32_t size) : TLifeFaster(size, size) {}
    TLifeFaster() : TLifeFaster(10) {}

    TLifeFaster(const TLifeFaster &other) = delete;
    TLifeFaster(TLifeFaster &&other) = delete;
    TLifeFaster& operator=(const TLifeFaster &other) = delete;
    TLifeFaster& operator=(TLifeFaster &&other) = delete;

    void InitField(const TLifeField& initialState);
    void NextGeneration();
    const TLifeField& GetField() { return field; }

private:
    uint32_t CountLiveNeighbors(uint32_t x, uint32_t y) const;
    void IncLiveNeighbors(TLifeField& field, uint32_t x, uint32_t y);
    void DecLiveNeighbors(TLifeField& field, uint32_t x, uint32_t y);
    uint32_t rows;
    uint32_t cols;
    TLifeField field;
    TLifeField fieldWithNeighbourCnt;
};

#endif
