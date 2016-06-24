//
//  QuadrocopterDiscrete1D.cpp
//  Quadrocopter-Visualizer
//
//  Created by anton on 24/06/16.
//
//

#include "QuadrocopterDiscrete1D.hpp"

QuadrocopterDiscrete::QuadrocopterDiscrete () {}
QuadrocopterDiscrete::~QuadrocopterDiscrete () {}

void QuadrocopterDiscrete::createIn (WorldDiscrete1D& w) {}

float QuadrocopterDiscrete::getPosition () { return  position; }
void QuadrocopterDiscrete::setPosition (float pos) { position = pos; }
void QuadrocopterDiscrete::setVelocity (float v) {}
float QuadrocopterDiscrete::getVelocity () { return 0; }
void QuadrocopterDiscrete::setMotorPower (float p) {}

void QuadrocopterDiscrete::step () {}
