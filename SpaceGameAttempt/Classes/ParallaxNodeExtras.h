//
//  ParallaxNodeExtras.h
//  SpaceGameAttempt
//
//  Created by Ahmed Arif Khan on 1/11/15.
//
//

#ifndef Cocos2DxFirstIosSample_CCParallaxNodeExtras_h
#define Cocos2DxFirstIosSample_CCParallaxNodeExtras_h

#include "cocos2d.h"

USING_NS_CC;

class ParallaxNodeExtras : public ParallaxNode {
public:
    ParallaxNodeExtras(); // needs a public constructor
    static ParallaxNodeExtras * node();
    void incrementOffset(Point offset, Node * node);
    
};

#endif /* defined(__SpaceGameAttempt__ParallaxNodeExtras__) */
