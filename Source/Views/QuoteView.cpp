/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include "QuoteView.h"
#include "Quote.h"
#include <Messenger.h>
#include <StringView.h>
#include <Button.h>
#include <string>
#include <sstream>
#include <iostream>
#include "Constants.h"

#include <GroupLayout.h>
#include <GridLayout.h>
#include <LayoutBuilder.h>
#include <GridLayoutBuilder.h>
#include <GroupLayoutBuilder.h>
#include <Box.h>
#include <Catalog.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "QuoteView"

QuoteView::QuoteView()
	:BBox("QuoteView")
	,fMessenger(NULL)
	,fPortfolioButton(NULL)
	,fTitle(NULL)
	,f52High(NULL)
	,f52Low(NULL)
	,fChangePercent(NULL)
	,fQuote(NULL)
	,fHasQuote(false) {	
	
	fTitle = new BStringView("Title", "");
	f52High = new BStringView("52High", "");
	f52Low = new BStringView("52Low", "");
	fChangePercent = new BStringView("ChangePercent", "");
}

QuoteView::~QuoteView() {
	delete fQuote;
	delete fMessenger;
}

void
QuoteView::AttachedToWindow() {
	InitLayout();
}

void
QuoteView::MessageReceived(BMessage *message) {
		
	switch (message->what) {
		case kPortfolioButtonPressedMessage: {
			BMessage message(kPortfolioButtonPressedMessage);
			message.AddBool("removeQuote", fHasQuote);
			message.AddString("symbol", fQuote->symbol);
			fMessenger->SendMessage(&message);
			break;
		}
		default:
			BView::MessageReceived(message);
			break;
	}
}

const char * 
QuoteView::MakeText(const char * title, double value) {
	BString str;
	str << title << value;
	return str.String();
}

void 
QuoteView::SetChange(float percent, float dollars) {
	
	std::ostringstream text;

	if (percent < 0 ) {
		fChangePercent->SetHighColor(stockMinusColor);
		text << percent * 100 << "%" << " (" << "$" << dollars << ")";

	} else {
		fChangePercent->SetHighColor(stockPlusColor);
		text << "+" << percent * 100 << "%" << " (" << "$" << dollars << ")";

	}
	BFont font(be_plain_font);
	font.SetSize(14.0);
	fChangePercent->SetAlignment(B_ALIGN_RIGHT);
	fChangePercent->SetFont(&font);
	fChangePercent->SetText(text.str().c_str());
}

void 
QuoteView::SetTarget(BHandler *handler) {
	delete fMessenger;
	fMessenger = new BMessenger(handler);
}

void
QuoteView::SetQuote(Quote *quote, bool hasQuote) {
	
	fHasQuote = hasQuote;
	
	SetupButton(hasQuote);
	
	delete fQuote;
	fQuote = quote;
	
	fTitle->SetText(MakeText("$", quote->latestPrice));
	
	f52High->SetText(MakeText("$",  quote->week52High));
	f52Low->SetText(MakeText("$",  quote->week52Low));
	SetChange(quote->changePercent, quote->change);
}

void
QuoteView::SetupButton(bool hasQuote) {
	fPortfolioButton->SetLabel(hasQuote ? B_TRANSLATE("Remove from portfolio") : B_TRANSLATE("Add to portfolio"));
}

BBox *
QuoteView::MakeSeparator() const {
	BBox *separator = new BBox("Separator");
	separator->SetExplicitMinSize(BSize(100, 1));
	separator->SetExplicitMaxSize(BSize(B_SIZE_UNLIMITED, 1));
	return separator;
}

BView *
QuoteView::MakeTitleGroup(const char *title, BStringView *right) {
	
	if (right == NULL) 
		return NULL;
		
	right->SetAlignment(B_ALIGN_RIGHT);
	BFont font(be_bold_font);
	right->SetFont(&font);
	
	BView *group = BGroupLayoutBuilder(B_HORIZONTAL, 0)
		.Add(new BStringView("title", title))
		.AddGlue()
		.Add(right)
		.TopView();
	return group;
}

void
QuoteView::InitLayout() {

	SetExplicitMinSize(BSize(180, 150));
	SetExplicitMaxSize(BSize(180, B_SIZE_UNSET));
	
	BGroupLayout *groupLayout = new BGroupLayout(B_HORIZONTAL);
	SetLayout(groupLayout);

	if (fPortfolioButton == NULL) {
		fPortfolioButton = new BButton("Portfolio", B_TRANSLATE("Add to portfolio"), new BMessage(kPortfolioButtonPressedMessage));
	}	
	fPortfolioButton->SetTarget(this);
	
	BView *buttonGroup = BGroupLayoutBuilder(B_HORIZONTAL, 0)
		.AddGlue()
		.Add(fPortfolioButton)
		.TopView();
	
	BView *group = BGroupLayoutBuilder(B_VERTICAL, 5)
		.Add(fChangePercent)
		.Add(MakeSeparator())
		.Add(MakeTitleGroup(B_TRANSLATE("Latest price"), fTitle))
		.Add(MakeSeparator())
		.Add(MakeTitleGroup(B_TRANSLATE("52 week high"), f52High))
		.Add(MakeTitleGroup(B_TRANSLATE("52 week low"), f52Low))
		.AddGlue()
		.Add(buttonGroup)
		.TopView();
	
	AddChild(group);
}

