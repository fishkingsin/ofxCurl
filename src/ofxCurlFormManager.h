//
//  ofxCurlFormManager.h
//  emptyExample
//
//  Created by Nick Hardeman on 11/7/12.
//
//

#pragma once
#include "ofMain.h"
#include "ofxCurl.h"

enum OFX_CURL_POST_EVENTS {
    OFX_CURL_POST_EVENT_SUCCESS = 1,
    OFX_CURL_POST_EVENT_ERROR = 2
};

class ofxCurlFormEntry {
public:
    ofxCurlFormEntry() {
        ID          = -1;
        name        = "";
        timestamp   = ofGetElapsedTimeMillis();
        status      = -1;
        form        = NULL;
    }
    ~ofxCurlFormEntry() {
        if(form != NULL) delete form; form = NULL;
    }
    
    int ID;
    string name;
    unsigned int timestamp;
    ofxCurl curl;
    ofxCurlForm* form;
    long status;
};

class ofxCurlFormEvent {
public:
    
    ofxCurlFormEvent() {}
    ofxCurlFormEvent( int a_ID, string a_name, long a_status, unsigned int a_timestamp, string a_response) {
        ID  = a_ID;
        name = a_name;
        status = a_status;
        timestamp = a_timestamp;
        response = a_response;
    }
    
    ~ofxCurlFormEvent() {}
    string getAsString() {
        stringstream ss;
        string stype = type == OFX_CURL_POST_EVENT_SUCCESS ? "SUCCESS" : "ERROR";
        ss << "CurlFormEvent "<<stype<<" id: "<<ofToString(ID,0)<<" name: "<< name <<endl;
        ss << "status: " << ofToString(status,0) << " | timestamp: " << timestamp << endl;
        ss << "Response: -- " << endl;
        ss << response << endl;
        ss << "--------------- " << endl;
        return ss.str().c_str();
    }
    
    int type;
    int ID; string name; long status; unsigned int timestamp;
    string response;
};

class ofxCurlFormManager : public ofThread {
public:
    
    void start(bool _blocking = true, bool _verbose = true);
    void stop();
    void clear();
    
    void exit( ofEventArgs& args );
    void update( ofEventArgs& args );
    
    void threadedFunction();
    void postNextForm();
    
    bool hasUnpostedForms();
    void addForm( ofxCurlFormEntry* a_entry );
    void addForm( ofxCurlFormEntry* a_entry, int a_id );
    void addForm( ofxCurlFormEntry* a_entry, string a_name );
    void addForm( ofxCurlFormEntry* a_entry, int a_id, string a_name );
    
    vector<ofxCurlFormEntry*> entries;
    vector<ofxCurlFormEvent> events;
    
    ofEvent<ofxCurlFormEvent> SuccessEvent;
    ofEvent<ofxCurlFormEvent> ErrorEvent;
};





