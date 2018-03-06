#ifndef __BaseLayer_SCENE_H__
#define __BaseLayer_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "Adaptation.h"
class BaseLayer : public Layer
{
public:
	BaseLayer();
	~BaseLayer();
    virtual bool init();
	CREATE_FUNC(BaseLayer);

	Layout *getRootLayer(){
		return m_RootLayer;
	}
	void setRootLayerOpZero();
protected:
	Layout *m_RootLayer;
};

#endif // __BaseLayer_SCENE_H__
