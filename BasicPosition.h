#ifndef BASICPOSITION_H
#define BASICPOSITION_H
#include <vector>

using namespace std;
using int32 = int;
class BasicPosition
{
public:
	int32 x, y;
	BasicPosition(int32 col = 0, int32 row = 0);
};

//position vector
extern vector<BasicPosition> snake;

// item
extern BasicPosition upgrade_item;
extern BasicPosition downgrade_item;

// gate
extern BasicPosition first_gate_position;
extern BasicPosition second_gate_position;
extern BasicPosition exit_position;

#endif