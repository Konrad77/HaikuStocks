/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "MainWindow.h"
#include "StockRequester.h"
#include "Company.h"
#include "MessageConstants.h"
#include "CompanyListItem.h"

#include <Application.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <Locale.h>

#include <StringItem.h>
#include <LayoutBuilder.h>
#include <stdio.h>
#include <ListView.h>
#include <StringItem.h>

#include <GroupLayout.h>
#include <GridLayout.h>
#include <LayoutBuilder.h>
#include <GridLayoutBuilder.h>
#include <GroupLayoutBuilder.h>

MainWindow::MainWindow(BRect rect) 
	:BWindow(rect, "MainWindow", B_TITLED_WINDOW, B_NOT_RESIZABLE | B_QUIT_ON_WINDOW_CLOSE )
	,fMenuBar(NULL)
	,fStockRequester(NULL) 
	,fStockListView(NULL) {
	
	SetupViews();
}

MainWindow::~MainWindow() {
	delete fStockRequester;
}

StockRequester *
MainWindow::Requester() {
	if (fStockRequester == NULL) {
		fStockRequester = new StockRequester(this);
	}
	return fStockRequester;
}

void 
MainWindow::AddSymbol(const char *symbol) {
	Requester()->AddStockSymbol(symbol);
	Requester()->RequestData();
}

void
MainWindow::SetupViews() {

	BGroupLayout *layout = new BGroupLayout(B_VERTICAL);
	SetLayout(layout);
	
	BLayoutBuilder::Menu<>(fMenuBar = new BMenuBar(Bounds(), "Menu"))
		.AddMenu("Main")
			.AddItem("About...", B_ABOUT_REQUESTED, 'A')
			.AddSeparator()
			.AddItem("Quit", B_QUIT_REQUESTED, 'Q')
		.End()
		.AddMenu("Settings")
			.AddItem("Add symbol...", kShowSearchWindowMessage, 'S')
		.End();
	
	//AddChild(fMenuBar);	

	fStockListView = new BListView();

	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.Add(fMenuBar)
		.Add(fStockListView);
	
	//AddChild(fStockListView);	
}

void
MainWindow::MessageReceived(BMessage *message) {
	switch (message->what) {
		
		case kShowSearchWindowMessage: {
			BMessage *showSearchWindowMessage = new BMessage(kShowSearchWindowMessage);
			be_app_messenger.SendMessage(showSearchWindowMessage);
			delete showSearchWindowMessage;
		}
		break;
		
		case B_ABOUT_REQUESTED:
			printf("Hello world\n");
			break;
		case kUpdateCompanyMessage: {
			
			BMessage companyMessage;
			message->FindMessage("Company", &companyMessage);
			Company *company = new Company(companyMessage);
			printf("Company %s\n", company->name.String());
			fStockListView->AddItem(new BStringItem(company->name.String()));
			break;
		}
		default:
			break;
	}
}

