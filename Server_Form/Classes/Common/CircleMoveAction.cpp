#include "CircleMoveAction.h"

CircleMoveAction* CircleMoveAction::create(float duration, const cocos2d::Point& center, float radius, float angle, float moveTimes)
{
	CircleMoveAction *pRet = new CircleMoveAction();

	if (pRet && pRet->initWithDuration(duration, center, radius, angle, moveTimes))
	{
		pRet->autorelease();

		return pRet;
	}

	CC_SAFE_DELETE(pRet);

	return nullptr;
}

bool CircleMoveAction::initWithDuration(float duration, const cocos2d::Point& center, float radius, float angle, float moveTimes)
{
	//    log(">>>>>>>>%f", duration);

	if (CCActionInterval::initWithDuration(duration * 2))
	{
		this->m_circleCenter = center;
		this->m_circleRadius = radius;
		this->m_duration = duration;
		this->m_angle = angle;
		this->m_moveTimes = moveTimes;

		/************************************************************************/
		/* 计算每次update调用时需要转动的弧度                                   */
		/************************************************************************/
		this->m_radian = angle / duration * Director::getInstance()->getAnimationInterval() / (180 / M_PI);

		//        log(">>>>>>>%f", 1.0f/Director::getInstance()->getAnimationInterval());

		return true;
	}
	return false;
}

void CircleMoveAction::startWithTarget(cocos2d::Node *target)
{
	CCActionInterval::startWithTarget(target);
	//    m_initPos = _target->getPosition();
}

CircleMoveAction* CircleMoveAction::clone() const
{
	// no copy constructor
	auto circleAct = new CircleMoveAction();
	circleAct->initWithDuration(m_duration, m_circleCenter, m_circleRadius, m_angle, m_moveTimes);
	circleAct->autorelease();
	return circleAct;
}

CircleMoveAction* CircleMoveAction::reverse() const
{
	return CircleMoveAction::create(m_duration, m_circleCenter, m_circleRadius, -m_angle, m_moveTimes);
}

void CircleMoveAction::update(float dt)
{
	float radian = m_radian*m_moveTimes;

	float x = m_circleRadius*sin(radian);
	float y = m_circleRadius*cos(radian);

	Point newPoint = Point(m_circleCenter.x + x, m_circleCenter.y + y);
	_target->setPosition(newPoint);

	m_moveTimes++;
}
