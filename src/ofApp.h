/* This file is part of Audio_Reactive_Triangles_Cloud.
 
 Audio_Reactive_Triangles_Cloud is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 Audio_Reactive_Triangles_Cloud is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with Audio_Reactive_Triangles_Cloud.
 If not, see <http://www.gnu.org/licenses/>. */

#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    //*********************************
    //AUDIO
    //*********************************
    void audioIn(float * input, int bufferSize, int nChannels);
    
    vector <float> left;
    vector <float> right;
    vector <float> volHistory;
    
    float smoothedVol = 1.0;
    float scaledVol;
    
    ofSoundStream soundStream;
    //*********************************
    //END OF AUDIO
    //*********************************
    
    //*********************************
    //TRIANGLES CLOUD
    //*********************************
    vector<ofPoint> origVertices;
    vector<ofPoint> centerVertices;
    vector<ofPoint> vertices;
    vector<ofColor> colors;
    int nTri = 1500;        //The number of triangles
    int nVert =  nTri * 3;      //The number of the vertices equals nTri * 3
    float origRad = 200;        //The sphere's original radius
    float Rad = 200;        //The sphere's radius
    float rad = 25;         //Maximal triangle's "radius"
    //(formally, it's the maximal coordinates'
    //deviation from the triangle's center)
    //*********************************
    //END OF TRIANGLES CLOUD
    //*********************************
    
};
