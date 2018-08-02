/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H


#include <SupportDefs.h>
#include <Window.h>

class BMessenger;
class BHandler;
class SettingsWindow : public BWindow {
public:
	SettingsWindow();
	~SettingsWindow();
	
	virtual void MessageReceived(BMessage *message);
	virtual bool QuitRequested();
			void SetTarget(BHandler *handler);
private:
	BMessenger *fMessenger;
};


#endif // _H
