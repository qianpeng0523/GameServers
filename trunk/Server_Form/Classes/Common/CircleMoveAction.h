#include "cocos2d.h"

using namespace cocos2d;

class CircleMoveAction : public cocos2d::ActionInterval
{
public:
	bool initWithDuration(float duration, const cocos2d::Point& center, float radius, float angle, float moveTimes);

	//
	// Overrides
	//
	virtual CircleMoveAction* clone() const override;
	virtual CircleMoveAction* reverse() const override;
	virtual void startWithTarget(cocos2d::Node *target) override;
	virtual void update(float dt) override;

public:
	static CircleMoveAction* create(float duration, const cocos2d::Point& center, float radius, float angle, float moveTimes);

public:
	//    CircleMoveAction();
	//    ~CircleMoveAction();
	//    void updateCircleCenter(Ref *pSender);
	//    void unUpdateCircleCenter(Ref *pSender);
private:
	//    CC_SYNTHESIZE(bool, onTouch, OnTouch);
protected:
	cocos2d::Point m_circleCenter;    //圆心
	float m_circleRadius;     //半径
	float m_radian;      //弧度
	float m_angle;     //旋转角度
	int m_moveTimes;   //刷新次数
	float m_duration;    //运动时间
};
