#include "baseball_rules.h"

//랜덤넘버 생성
int computerNumbers[5] = { 0, };

int generateNumbers()
{
	srand((int)time(NULL));
	computerNumbers[0] = (rand() % 9) + 1; //0이 못나오게 설정

	do {
		computerNumbers[1] = (rand() % 9) + 1;
	} while ((computerNumbers[0] == computerNumbers[1]));

	do
	{
		computerNumbers[2] = (rand() % 9) + 1;
	} while ((computerNumbers[0] == computerNumbers[2]) ||
		(computerNumbers[1] == computerNumbers[2]));

	cout << "computer number is" << computerNumbers[0] << computerNumbers[1] << computerNumbers[2] << endl;

	return 0;
}

//볼카운트 계산하기
int getBallCount(int* nums, int& outBallCount, int& outStrikeCount)
{
	int humanNumbers[5] = { 0, };
	int countBall = 0, countStrike = 0;
	int i = 0;

	humanNumbers[0] = nums[0];
	humanNumbers[1] = nums[1];
	humanNumbers[2] = nums[2];

	if (humanNumbers[0] == computerNumbers[0])
		countStrike++;
	else if (humanNumbers[0] == computerNumbers[1])
		countBall++;
	else if (humanNumbers[0] == computerNumbers[2])
		countBall++;

	if (humanNumbers[1] == computerNumbers[0])
		countBall++;
	else if (humanNumbers[1] == computerNumbers[1])
		countStrike++;
	else if (humanNumbers[1] == computerNumbers[2])
		countBall++;

	if (humanNumbers[2] == computerNumbers[0])
		countBall++;
	else if (humanNumbers[2] == computerNumbers[1])
		countBall++;
	else if (humanNumbers[2] == computerNumbers[2])
		countStrike++;

	if (humanNumbers[0] == computerNumbers[0] &&
		humanNumbers[1] == computerNumbers[1] &&
		humanNumbers[2] == computerNumbers[2])
	{
		countStrike = 3;
		countBall = 0;
		outStrikeCount = countStrike;
	}
	else {
		cout << countBall << "ball" << countStrike << "strikes" << endl;
		outBallCount = countBall;
		outStrikeCount = countStrike;
	}

	return 0;

}