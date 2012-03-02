/* 
 * Strategizer.h
 */
#pragma once


class Strategizer
{
public:

	void update();

	static Strategizer& instance() { static Strategizer s; return s; }

private:

	// Strategizer is singleton, hence private ctors/assignment
	Strategizer();
	Strategizer(const Strategizer& other);
	void operator=(const Strategizer& other);

};