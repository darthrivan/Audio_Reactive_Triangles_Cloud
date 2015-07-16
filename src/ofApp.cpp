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

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    //*********************************
    //AUDIO
    //*********************************
    // 0 output channels,
	// 2 input channels
	// 44100 samples per second
	// 256 samples per buffer
	// 4 num buffers (latency)
	
	//soundStream.listDevices();
	
    //if you want to set a different device id
	soundStream.setDeviceID(2); //bear in mind the device id corresponds to all audio
    //devices, including  input-only and output-only devices.
	
	int bufferSize = 256;
	
	
	left.assign(bufferSize, 0.0);
	right.assign(bufferSize, 0.0);
	volHistory.assign(400, 0.0);
	
	smoothedVol     = 0.0;
	scaledVol		= 0.0;
    
	soundStream.setup(this, 0, 2, 44100, bufferSize, 4);
    //*********************************
    //END OF AUDIO
    //*********************************
    
    //*********************************
    //TRIANGLES CLOUD
    //*********************************
    
    // Fill the vertices array
    centerVertices.resize(nTri);
    origVertices.resize(nVert);// Set the array size
    vertices.resize(nVert);    // Set the array size
    for (int i=0; i<nTri; i++) {     // Scan all the triangles
        
        //Generate the center of the triangle
        //as a random point on the sphere
        //Take the random point from
        //cube [-1,1]x[-1,1]x[-1,1]
        ofPoint center( ofRandom( -1, 1 ),
                       ofRandom( -1, 1 ),
                       ofRandom( -1, 1 ) );
        center.normalize(); //Normalize vector's length to 1
        //center *= Rad;      //Now the center vector has length Rad
        centerVertices[i] = center;
        //Generate the triangle's vertices
        //as the center plus random point from
        //[-rad, rad]x[-rad, rad]x[-rad, rad]
        for (int j=0; j<3; j++) {
            origVertices[i*3 + j] = ofPoint( ofRandom( -rad, rad ),
                                            ofRandom( -rad, rad ),
                                            ofRandom( -rad, rad ) );
        }
    }
    //Fill the array of triangles' colors
    colors.resize( nTri );
    for (int i=0; i<nTri; i++) {
        //Take a random color from black to red
        colors[i] = ofColor( ofRandom( 0, 255 ), 0, 0 );
    }
    
    //*********************************
    //END OF TRIANGLES CLOUD
    //*********************************
    
}

//--------------------------------------------------------------
void ofApp::update(){
    //lets scale the vol up to a 0-1 range
	//scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);
    
    //lets scale the vol up to a 1.0-2.0 range
	scaledVol = ofMap(smoothedVol, 0.0, 0.17, 1.0, 2.5, true);
    
	//lets record the volume into an array
	volHistory.push_back( scaledVol );
	
	//if we are bigger the the size we want to record - lets drop the oldest value
	if( volHistory.size() >= 400 ){
		volHistory.erase(volHistory.begin(), volHistory.begin()+1);
	}
    
    Rad = origRad * scaledVol;
    
    for (int i=0; i<nTri; i++) {     //Scan all the triangles
        ofPoint cent = centerVertices[i];
        cent *= Rad * scaledVol;      //Now the center vector has length Rad
        
        //Generate the triangle's vertices
        //as the center plus random point from
        //[-rad, rad]x[-rad, rad]x[-rad, rad]
        for (int j=0; j<3; j++) {
            vertices[i*3 + j] = cent + origVertices[i*3 + j];
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    //*********************************
    //AUDIO
    //*********************************
    
    //*********************************
    //END OF AUDIO
    //*********************************
    
    //*********************************
    //TRIANGLES CLOUD
    //*********************************
    ofEnableDepthTest();    //Enable z-buffering
    
    //Set a gradient background from white to gray
    //for adding an illusion of visual depth to the scene
    ofBackgroundGradient( ofColor( 255 ), ofColor( 128 ) );
    
    ofPushMatrix();    //Store the coordinate system
    
    //Move the coordinate center to screen's center
    ofTranslate( ofGetWidth()/2, ofGetHeight()/2, 0 );
    
    //Calculate the rotation angle
    float time = ofGetElapsedTimef();       //Get time in seconds
    float angle = time * 10;                //Compute angle. We rotate at speed
    //10 degrees per second
    ofRotate( angle, 0, 1, 0 );             //Rotate the coordinate system
    //Draw the triangles
    for (int i=0; i<nTri; i++) {
        ofSetColor( colors[i] );            //Set color
        ofTriangle( vertices[ i*3 ],
                   vertices[ i*3 + 1 ],
                   vertices[ i*3 + 2 ] );   //Draw triangle
        
    }
    ofPopMatrix(); //Restore the coordinate system
    //*********************************
    //END OF TRIANGLES CLOUD
    //*********************************
}

//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels){
	
	float curVol = 0.0;
	
	// samples are "interleaved"
	int numCounted = 0;
    
	//lets go through each sample and calculate the root mean square which is a rough way to calculate volume
	for (int i = 0; i < bufferSize; i++){
		left[i]		= input[i*2]*0.5;
		right[i]	= input[i*2+1]*0.5;
        
		curVol += left[i] * left[i];
		curVol += right[i] * right[i];
		numCounted+=2;
	}
	
	//this is how we get the mean of rms :)
	curVol /= (float)numCounted;
	
	// this is how we get the root of rms :)
	curVol = sqrt( curVol );
	
	smoothedVol *= 0.93;
	smoothedVol += 0.07 * curVol;
	
	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
