//
//  ParallaxNodeExtras.cpp
//  SpaceGameAttempt
//
//  Created by Ahmed Arif Khan on 1/11/15.
//
//

USING_NS_CC;

#include "ParallaxNodeExtras.h"

// Hack to access CCPointObject (which is not a public class)
class PointObject : Ref {
    CC_SYNTHESIZE(Point, m_tRatio, Ratio);
    CC_SYNTHESIZE(Point, m_tOffset, Offset);
    CC_SYNTHESIZE(Node *, m_pChild, Child);
};

//constructor


ParallaxNodeExtras * ParallaxNodeExtras::node(){
    return new ParallaxNodeExtras();
}

void ParallaxNodeExtras::incrementOffset(Point offset, Node * node){
    //for (unsigned int i = 0; i < this->m_pParallaxArray->num; i++) {
        
   //     PointObject *point = (PointObject *)m_pParallaxArray->arr[i];
        
        
    }//end for loop
}// end function