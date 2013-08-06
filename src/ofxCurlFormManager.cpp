//
//  ofxCurlFormManager.cpp
//  emptyExample
//
//  Created by Nick Hardeman on 11/7/12.
//
//

#include "ofxCurlFormManager.h"

//--------------------------------------------------------------
void ofxCurlFormManager::start(bool _blocking, bool _verbose) {
    ofAddListener(ofEvents.update, this, &ofxCurlFormManager::update );
    ofAddListener(ofEvents.exit, this, &ofxCurlFormManager::exit );
    startThread( _blocking, _verbose );
}

//--------------------------------------------------------------
void ofxCurlFormManager::stop() {
    ofRemoveListener(ofEvents.update, this, &ofxCurlFormManager::update );
    stopThread();
}

//--------------------------------------------------------------
void ofxCurlFormManager::clear() {
    lock();
    for(int i = 0; i < entries.size(); i++ ) {
        delete entries[i]; entries[i] = NULL;
    }
    entries.clear();
    unlock();
}

//--------------------------------------------------------------
void ofxCurlFormManager::exit( ofEventArgs& args ) {
    clear();
}

//--------------------------------------------------------------
void ofxCurlFormManager::update( ofEventArgs& args ) {
    lock();
    for(int i = 0; i < events.size(); i++) {
        if(events[i].type == OFX_CURL_POST_EVENT_SUCCESS) {
            ofNotifyEvent( SuccessEvent, events[i], this );
        } else if (events[i].type == OFX_CURL_POST_EVENT_ERROR ) {
            ofNotifyEvent( ErrorEvent, events[i], this);
        }
        events.erase(events.begin() + i);
        break;
    }
    unlock();
}

//--------------------------------------------------------------
void ofxCurlFormManager::threadedFunction() {
    while( isThreadRunning() != 0 ) {
        if(hasUnpostedForms()) {
            postNextForm();
            ofSleepMillis(50);
        } else {
            // chill for a bit //
            ofSleepMillis(500);
        }
    }
}

//--------------------------------------------------------------
void ofxCurlFormManager::postNextForm() {

    lock();
    ofxCurlFormEntry* entry = entries[0];

    // Perform the post.
    try {
        entry->form->post();
    }
    catch(...) {
        entry->status = -1;
        if(verbose) cout << "OOPS.. something went wrong while posting" << endl;
    }

    // Do something with the response from the post.
    vector<char> response_buf   = entry->form->getPostResponseAsBuffer();
    string response_str         = entry->form->getPostResponseAsString();
    int entryId                 = entry->ID;
    string entryName            = entry->name;
    unsigned int tstamp         = entry->timestamp;
    long entryStatus            = entry->form->getHTTPCode();

    unlock();

    if(verbose) {
        stringstream ss;
        ss << "------------------------------------------------------- " << endl;
        ss << "Form posted id: "<<ofToString(entryId,0)<<" | name: "<<entryName<<" | timestamp: "<<ofToString(tstamp,0)<<endl;
        ss << "Http Response: " +ofToString(entryStatus, 0) << endl;
        ss << "Size of response buffer: " << response_buf.size() << endl;
        ss << "Response string: -----------------" << endl;
        ss << response_str <<endl;
        ss << "------------------------------------------------------- " << endl;
        cout << ss.str().c_str();
    }




    ofxCurlFormEvent cevent(
                            entryId,
                            entryName,
                            entryStatus,
                            tstamp,
                            response_str
                        );

    lock();
    // Cleanup
    bool bError     = false;

    if(entryStatus != 200) {
        bError  = true;
    }

    if(bError) {
        cevent.type = OFX_CURL_POST_EVENT_ERROR;
        events.push_back(cevent);
    }
    if(!bError) {
        cevent.type = OFX_CURL_POST_EVENT_SUCCESS;
        events.push_back(cevent);
    }

    delete entries[0]; entries[0] = NULL;
    entries.erase( entries.begin() );

    unlock();
}

//--------------------------------------------------------------
bool ofxCurlFormManager::hasUnpostedForms() {
    lock();
    int size = entries.size();
    unlock();
    return (size > 0);
}

//--------------------------------------------------------------
void ofxCurlFormManager::addForm( ofxCurlFormEntry* a_entry ) {
    addForm( a_entry, a_entry->ID, a_entry->name );
}

//--------------------------------------------------------------
void ofxCurlFormManager::addForm( ofxCurlFormEntry* a_entry, int a_id ) {
    addForm( a_entry, a_id, "" );
}

//--------------------------------------------------------------
void ofxCurlFormManager::addForm( ofxCurlFormEntry* a_entry, string a_name ) {
    addForm( a_entry, -1, a_name );
}

//--------------------------------------------------------------
void ofxCurlFormManager::addForm( ofxCurlFormEntry* a_entry, int a_id, string a_name ) {
    a_entry->name  = a_name;
    a_entry->ID    = a_id;

    lock();
        entries.push_back( a_entry );
    unlock();
}
























