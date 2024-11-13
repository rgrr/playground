/*
 * TRomans.h
 *
 *  Created on: Nov 5, 2024
 *      Author: i02441001
 */

#ifndef TROMANS_H_
#define TROMANS_H_


#include <string>


class TRomans {
public:
	TRomans() {};
	virtual ~TRomans() {};
	TRomans(const TRomans &other) = delete;
	TRomans(TRomans &&other) = delete;
	TRomans& operator=(const TRomans &other) = delete;
	TRomans& operator=(TRomans &&other) = delete;

	std::string ConvertToRomanNum(int num);
};

#endif /* TROMANS_H_ */
