#include "RA.h"
#include "TA.h"

int main(int argc, char const *argv[])
{
	TeamAssigner TA;
	TA.load();
	TA.assign();
	TA.output();

	ReviewerAssigner RA;
	RA.load();
	RA.choose();
	RA.output();

	return 0;
}
