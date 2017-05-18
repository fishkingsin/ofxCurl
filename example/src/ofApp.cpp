#include "ofApp.h"
#include "ofxCurl.h"
//--------------------------------------------------------------
void ofApp::setup(){

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == 't') {
		// Create a form.
		ofxCurl curl;
		
		ofxCurlForm* form = curl.createForm("http://butterflies.apmhkonline.hk/lib/ajax/file_upload/");
		//form->setTimeout(99999);
		// Add some form entries.
		form->addFile("file_upload_file", ofToDataPath("apm-2017-03-20-21-43-03-489.mp4", true));

		// Perform the post.
		try {
			form->post();
		}
		catch (exception e) {
			cout << "OOPS.. something went wrong while posting "  <<endl;
		}

		// Do something with the response from the post.
		vector<char> response_buf = form->getPostResponseAsBuffer();
		string response_str = form->getPostResponseAsString();
		cout << "Size of response buffer: " << response_buf.size() << endl << endl;
		cout << "Response string:" << endl;
		cout << "-----------------" << endl;
		cout << response_str << endl;

		// Cleanup
		delete form;
	}
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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
