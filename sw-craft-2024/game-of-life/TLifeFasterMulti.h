/*
 * TLifeFasterMulti.h
 *
 *  Created on: Nov 11, 2024
 *      Author: i02441001
 */

#ifndef TLIFEFASTERMULTI_H_
#define TLIFEFASTERMULTI_H_

#include <vector>
#include <cstdint>


using TLifeField = std::vector<std::vector<int>>;

class TLifeFasterMulti {
public:
    virtual ~TLifeFasterMulti() { }

    TLifeFasterMulti(uint32_t rows, uint32_t cols) :
        rows(rows),
        cols(cols),
        field(rows, std::vector<int>(cols, 0)),
        fieldWithNeighbourCnt(rows, std::vector<int>(cols, 0)) {}
    TLifeFasterMulti(uint32_t size) : TLifeFasterMulti(size, size) {}
    TLifeFasterMulti() : TLifeFasterMulti(10) {}

    TLifeFasterMulti(const TLifeFasterMulti &other) = delete;
    TLifeFasterMulti(TLifeFasterMulti &&other) = delete;
    TLifeFasterMulti& operator=(const TLifeFasterMulti &other) = delete;
    TLifeFasterMulti& operator=(TLifeFasterMulti &&other) = delete;

    void InitField(const TLifeField& initialState);
    void NextGeneration();
    void NextGeneration(TLifeField &newField, TLifeField &newFieldWithNeighbourCnt,
                        uint32_t row_min, uint32_t row_max);
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
