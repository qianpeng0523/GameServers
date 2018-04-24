#include "GRoom.h"
#include "ClientSocket.h"
#include "EventListen.h"
#include "EventDispatcher.h"
#include "HttpLogic.h"

GRoom::GRoom():
m_curbao(0),
m_maxcount(4)
{
	
}

GRoom::~GRoom(){
	
}

bool GRoom::init()
{
	
    return true;
}

void GRoom::reset(){
	
}