// GameObject.h


class Statistics  {
public:
	Statistics();
	int GetMultiplier();
	int GetScore();
	void HalfMultiplier();
	void IncrementScore(int value);
	void IncrementMultiplier(int value);
private:
	int _score;
	int _multiplier;
protected:

};
