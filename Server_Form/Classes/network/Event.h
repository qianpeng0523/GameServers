#ifndef __EVENT_H__
#define __EVENT_H__

#include <string>
#include "cocos2d.h"

using namespace cocos2d;
using namespace std;

namespace YS{

	class Event {
	public:
		/** 改变 **/
		const static string EVENT_CHANGE;
		const static string EVENT_RELEASE;

	public:
		Event(int type);
		virtual ~Event();

		void setTarget(Object *target);

		cocos2d::Object *getTarget();
		int getType();

		void stopPropagation();
		bool isStopPropagation();

	protected:
		int m_type;

	private:
		bool blStop;
		cocos2d::Object *m_target;
	};
}

#endif // __EVENT_H__
