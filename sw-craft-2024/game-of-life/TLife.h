/*
 * TLife.h
 *
 *  Created on: Nov 6, 2024
 *      Author: i02441001
 */

#ifndef TLIFE_H_
#define TLIFE_H_

#include <vector>
#include <cstdint>


using TLifeField = std::vector<std::vector<int>>;

class TLife {
public:
	virtual ~TLife() { }

	TLife(uint32_t rows, uint32_t cols) : rows(rows), cols(cols), field(rows, std::vector<int>(cols, 0)) {}
	TLife(uint32_t size) : TLife(size, size) {}
	TLife() : TLife(10) {}

	TLife(const TLife &other) = delete;
	TLife(TLife &&other) = delete;
	TLife& operator=(const TLife &other) = delete;
	TLife& operator=(TLife &&other) = delete;

	void InitField(const TLifeField& initialState);
	void NextGeneration();
	const TLifeField& GetField() { return field; }

private:
	uint32_t CountLiveNeighbors(uint32_t x, uint32_t y) const;
	uint32_t rows;
	uint32_t cols;
	TLifeField field;
};

#endif /* TLIFE_H_ */
