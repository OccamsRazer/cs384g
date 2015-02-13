//
// impressionistUI.cpp
//
// The user interface part for the program.
//


#include <FL/fl_ask.H>

#include <algorithm>

#include <math.h>

#include "impressionistUI.h"
#include "impressionistDoc.h"

/*
//------------------------------ Widget Examples -------------------------------------------------
Here is some example code for all of the widgets that you may need to add to the 
project.  You can copy and paste these into your code and then change them to 
make them look how you want.  Descriptions for all of the widgets here can be found 
in links on the fltk help session page.

//---------Window/Dialog and Menubar-----------------------------------
	
	//----To install a window--------------------------
	Fl_Window* myWindow = new Fl_Window(600, 300, "MyWindow");
		myWindow->user_data((void*)(this));	// record self to be used by static callback functions
		
		// install menu bar
		myMenubar = new Fl_Menu_Bar(0, 0, 600, 25);
		Fl_Menu_Item ImpressionistUI::myMenuItems[] = {
			{ "&File",		0, 0, 0, FL_SUBMENU },
				{ "&Load...",	FL_ALT + 'l', (Fl_Callback *)ImpressionistUI::cb_load },
				{ "&Save...",	FL_ALT + 's', (Fl_Callback *)ImpressionistUI::cb_save }.
				{ "&Quit",			FL_ALT + 'q', (Fl_Callback *)ImpressionistUI::cb_exit },
				{ 0 },
			{ "&Edit",		0, 0, 0, FL_SUBMENU },
				{ "&Copy",FL_ALT + 'c', (Fl_Callback *)ImpressionistUI::cb_copy, (void *)COPY },
				{ "&Cut",	FL_ALT + 'x', (Fl_Callback *)ImpressionistUI::cb_cut, (void *)CUT },
				{ "&Paste",	FL_ALT + 'v', (Fl_Callback *)ImpressionistUI::cb_paste, (void *)PASTE },
				{ 0 },
			{ "&Help",		0, 0, 0, FL_SUBMENU },
				{ "&About",	FL_ALT + 'a', (Fl_Callback *)ImpressionistUI::cb_about },
				{ 0 },
			{ 0 }
		};
		myMenubar->menu(myMenuItems);
	myWindow->end();

	//----The window callback--------------------------
	// One of the callbacks
	void ImpressionistUI::cb_load(Fl_Menu_* o, void* v) 
	{	
		ImpressionistDoc *pDoc=whoami(o)->getDocument();

		char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName() );
		if (newfile != NULL) {
			pDoc->loadImage(newfile);
		}
	}


//------------Slider---------------------------------------

	//----To install a slider--------------------------
	Fl_Value_Slider * mySlider = new Fl_Value_Slider(10, 80, 300, 20, "My Value");
	mySlider->user_data((void*)(this));	// record self to be used by static callback functions
	mySlider->type(FL_HOR_NICE_SLIDER);
	mySlider->labelfont(FL_COURIER);
	mySlider->labelsize(12);
	mySlider->minimum(1);
	mySlider->maximum(40);
	mySlider->step(1);
	mySlider->value(m_nMyValue);
	mySlider->align(FL_ALIGN_RIGHT);
	mySlider->callback(cb_MyValueSlides);

	//----The slider callback--------------------------
	void ImpressionistUI::cb_MyValueSlides(Fl_Widget* o, void* v)
	{
		((ImpressionistUI*)(o->user_data()))->m_nMyValue=int( ((Fl_Slider *)o)->value() ) ;
	}
	

//------------Choice---------------------------------------
	
	//----To install a choice--------------------------
	Fl_Choice * myChoice = new Fl_Choice(50,10,150,25,"&myChoiceLabel");
	myChoice->user_data((void*)(this));	 // record self to be used by static callback functions
	Fl_Menu_Item ImpressionistUI::myChoiceMenu[3+1] = {
	  {"one",FL_ALT+'p', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void *)ONE},
	  {"two",FL_ALT+'l', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void *)TWO},
	  {"three",FL_ALT+'c', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void *)THREE},
	  {0}
	};
	myChoice->menu(myChoiceMenu);
	myChoice->callback(cb_myChoice);
	
	//-----The choice callback-------------------------
	void ImpressionistUI::cb_myChoice(Fl_Widget* o, void* v)
	{
		ImpressionistUI* pUI=((ImpressionistUI *)(o->user_data()));
		ImpressionistDoc* pDoc=pUI->getDocument();

		int type=(int)v;

		pDoc->setMyType(type);
	}


//------------Button---------------------------------------

	//---To install a button---------------------------
	Fl_Button* myButton = new Fl_Button(330,220,50,20,"&myButtonLabel");
	myButton->user_data((void*)(this));   // record self to be used by static callback functions
	myButton->callback(cb_myButton);

	//---The button callback---------------------------
	void ImpressionistUI::cb_myButton(Fl_Widget* o, void* v)
	{
		ImpressionistUI* pUI=((ImpressionistUI*)(o->user_data()));
		ImpressionistDoc* pDoc = pUI->getDocument();
		pDoc->startPainting();
	}


//---------Light Button------------------------------------
	
	//---To install a light button---------------------
	Fl_Light_Button* myLightButton = new Fl_Light_Button(240,10,150,25,"&myLightButtonLabel");
	myLightButton->user_data((void*)(this));   // record self to be used by static callback functions
	myLightButton->callback(cb_myLightButton);

	//---The light button callback---------------------
	void ImpressionistUI::cb_myLightButton(Fl_Widget* o, void* v)
	{
		ImpressionistUI *pUI=((ImpressionistUI*)(o->user_data()));

		if (pUI->myBool==TRUE) pUI->myBool=FALSE;
		else pUI->myBool=TRUE;
	}

//----------Int Input--------------------------------------

	//---To install an int input-----------------------
	Fl_Int_Input* myInput = new Fl_Int_Input(200, 50, 5, 5, "&My Input");
	myInput->user_data((void*)(this));   // record self to be used by static callback functions
	myInput->callback(cb_myInput);

	//---The int input callback------------------------
	void ImpressionistUI::cb_myInput(Fl_Widget* o, void* v)
	{
		((ImpressionistUI*)(o->user_data()))->m_nMyInputValue=int( ((Fl_Int_Input *)o)->value() );
	}

//------------------------------------------------------------------------------------------------
*/

//------------------------------------- Help Functions --------------------------------------------

//------------------------------------------------------------
// This returns the UI, given the menu item.  It provides a
// link from the menu items to the UI
//------------------------------------------------------------
ImpressionistUI* ImpressionistUI::whoami(Fl_Menu_* o)	
{
	return ( (ImpressionistUI*)(o->parent()->user_data()) );
}


//--------------------------------- Callback Functions --------------------------------------------

//------------------------------------------------------------------
// Brings up a file chooser and then loads the chosen image
// This is called by the UI when the load image menu item is chosen
//------------------------------------------------------------------
void ImpressionistUI::cb_load_image(Fl_Menu_* o, void* v) 
{
	ImpressionistDoc *pDoc=whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName() );
	if (newfile != NULL) {
		pDoc->loadImage(newfile);
	}
}


//------------------------------------------------------------------
// Brings up a file chooser and then saves the painted image
// This is called by the UI when the save image menu item is chosen
//------------------------------------------------------------------
void ImpressionistUI::cb_save_image(Fl_Menu_* o, void* v) 
{
	ImpressionistDoc *pDoc=whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Save File?", "*.bmp", "save.bmp" );
	if (newfile != NULL) {
		pDoc->saveImage(newfile);
	}
}

//-------------------------------------------------------------
// Brings up the paint dialog
// This is called by the UI when the brushes menu item
// is chosen
//-------------------------------------------------------------
void ImpressionistUI::cb_brushes(Fl_Menu_* o, void* v) 
{
	whoami(o)->m_brushDialog->show();
}

//-------------------------------------------------------------
// Brings up the paint dialog
// This is called by the UI when the brushes menu item
// is chosen
//-------------------------------------------------------------
void ImpressionistUI::cb_filter_kernel(Fl_Menu_* o, void* v)
{
	whoami(o)->m_kernelDialog->show();
}


//------------------------------------------------------------
// Clears the paintview canvas.
// Called by the UI when the clear canvas menu item is chosen
//------------------------------------------------------------
void ImpressionistUI::cb_clear_canvas(Fl_Menu_* o, void* v)
{
	ImpressionistDoc* pDoc=whoami(o)->getDocument();

	pDoc->clearCanvas();
}

//------------------------------------------------------------
// Causes the Impressionist program to exit
// Called by the UI when the quit menu item is chosen
//------------------------------------------------------------
void ImpressionistUI::cb_exit(Fl_Menu_* o, void* v) 
{
	whoami(o)->m_mainWindow->hide();
	whoami(o)->m_brushDialog->hide();

}



//-----------------------------------------------------------
// Brings up an about dialog box
// Called by the UI when the about menu item is chosen
//-----------------------------------------------------------
void ImpressionistUI::cb_about(Fl_Menu_* o, void* v) 
{
	fl_message("Impressionist FLTK version 1.1.9 for CS 384G, Fall 2008");
}

//------- UI should keep track of the current for all the controls for answering the query from Doc ---------
//-------------------------------------------------------------
// Sets the type of brush to use to the one chosen in the brush 
// choice.  
// Called by the UI when a brush is chosen in the brush choice
//-------------------------------------------------------------
void ImpressionistUI::cb_brushChoice(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI=((ImpressionistUI *)(o->user_data()));
	ImpressionistDoc* pDoc=pUI->getDocument();

	//	int type=(int)v;
	long long tmp = reinterpret_cast<long long>(v);
	int type = static_cast<int>(tmp);

	pDoc->setBrushType(type);

	if(type == BRUSH_LINES || type == BRUSH_SCATTERED_LINES){
		pUI->m_BrushWeightSlider->activate();
		pUI->m_BrushAngleSlider->activate();
	}
	else{
		pUI->m_BrushWeightSlider->deactivate();
		pUI->m_BrushAngleSlider->deactivate();
	}

}

//-------------------------------------------------------------
// Sets the type of stroke to use to the one chosen in the stroke 
// choice.  
// Called by the UI when a stroke is chosen in the stroke choice
//-------------------------------------------------------------
void ImpressionistUI::cb_strokeChoice(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI=((ImpressionistUI *)(o->user_data()));
	ImpressionistDoc* pDoc=pUI->getDocument();

	//	int type=(int)v;
	long long tmp = reinterpret_cast<long long>(v);
	int type = static_cast<int>(tmp);

	pDoc->setStrokeType(type);

}

//-------------------------------------------------------------
// Sets the type of stroke to use to the one chosen in the stroke 
// choice.  
// Called by the UI when a stroke is chosen in the stroke choice
//-------------------------------------------------------------
void ImpressionistUI::cb_dummyCallback(Fl_Widget* o, void* v)
{
	return;
}

//------------------------------------------------------------
// Clears the paintview canvas.
// Called by the UI when the clear canvas button is pushed
//------------------------------------------------------------
void ImpressionistUI::cb_clear_canvas_button(Fl_Widget* o, void* v)
{
	ImpressionistDoc * pDoc = ((ImpressionistUI*)(o->user_data()))->getDocument();

	pDoc->clearCanvas();
}


//------------------------------------------------------------
// Auto paints on canvas
// Called by the UI when the clear canvas button is pushed
//------------------------------------------------------------
void ImpressionistUI::cb_auto_draw_buton(Fl_Widget* o, void* v)
{
	ImpressionistDoc * pDoc = ((ImpressionistUI*)(o->user_data()))->getDocument();

	pDoc->autoDraw();
}

//------------------------------------------------------------
// Auto paints on canvas
// Called by the UI when the clear canvas button is pushed
//------------------------------------------------------------
void ImpressionistUI::cb_radial_draw_buton(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI=((ImpressionistUI *)(o->user_data()));
	ImpressionistDoc * pDoc = pUI->getDocument();

	float x_val = atof(pUI->m_radialX->value());
	float y_val = atof(pUI->m_radialY->value());

	pDoc->radialDraw(x_val, y_val);
}


//------------------------------------------------------------
// Preview the kernel in the paintview canvas.
// Called by the UI when the preview kernel button is pushed
//------------------------------------------------------------
void ImpressionistUI::cb_previewKernelButton(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI=((ImpressionistUI *)(o->user_data()));
	ImpressionistDoc * pDoc = pUI->getDocument();

	pUI->divisor = atof(pUI->m_divisor->value());
	pUI->offset = atof(pUI->m_offset->value());
	int image = pUI->m_srcImageChoice->value();

	if(pUI->divisor == 0)
		return;
	// grab the values from kernel
	for(int r = 0; r < FLT_HEIGHT; r++){
		for(int c = 0; c < FLT_WIDTH; c++){
			pUI->fltKernel[r*FLT_WIDTH+c] = atof(pUI->m_kernel[r*FLT_WIDTH+c]->value());
		}
	}

	if(image == ORIG_IMAGE){
		pDoc->previewFilter(pDoc->m_ucBitmap, pDoc->m_nPaintWidth, pDoc->m_nPaintHeight, pDoc->m_ucPainting, pUI->fltKernel, FLT_WIDTH, FLT_HEIGHT, pUI->divisor, pUI->offset);
	}
	else{
		pDoc->previewFilter(pDoc->m_ucPainting, pDoc->m_nPaintWidth, pDoc->m_nPaintHeight, pDoc->m_ucPainting, pUI->fltKernel, FLT_WIDTH, FLT_HEIGHT, pUI->divisor, pUI->offset);	
	}
}

//------------------------------------------------------------
// Apply the kernel in the paintview canvas.
// Called by the UI when the apply kernel button is pushed
//------------------------------------------------------------
void ImpressionistUI::cb_applyKernelButton(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI=((ImpressionistUI *)(o->user_data()));
	ImpressionistDoc * pDoc = pUI->getDocument();

	pUI->divisor = atof(pUI->m_divisor->value());
	pUI->offset = atof(pUI->m_offset->value());
	int image = pUI->m_srcImageChoice->value();

	if(pUI->divisor == 0)
		return;
	// grab the values from kernel
	for(int r = 0; r < FLT_HEIGHT; r++){
		for(int c = 0; c < FLT_WIDTH; c++){
			pUI->fltKernel[r*FLT_WIDTH+c] = atof(pUI->m_kernel[r*FLT_WIDTH+c]->value());
		}
	}

	if(image == ORIG_IMAGE){
		pDoc->applyFilter(pDoc->m_ucBitmap, pDoc->m_nPaintWidth, pDoc->m_nPaintHeight, pDoc->m_ucPainting, pUI->fltKernel, FLT_WIDTH, FLT_HEIGHT, pUI->divisor, pUI->offset);
	}
	else{
		pDoc->applyFilter(pDoc->m_ucPainting, pDoc->m_nPaintWidth, pDoc->m_nPaintHeight, pDoc->m_ucPainting, pUI->fltKernel, FLT_WIDTH, FLT_HEIGHT, pUI->divisor, pUI->offset);	
	}
}

//------------------------------------------------------------
// Cancel the kernel in the paintview canvas.
// Called by the UI when the cancel kernel button is pushed
//------------------------------------------------------------
void ImpressionistUI::cb_cancelKernelButton(Fl_Widget* o, void* v)
{
	// ImpressionistDoc * pDoc = ((ImpressionistUI*)(o->user_data()))->getDocument();

	// clear any preview
	// hide the window
	// pDoc->clearCanvas();
	ImpressionistUI* pUI=((ImpressionistUI *)(o->user_data()));
	ImpressionistDoc * pDoc = pUI->getDocument();

	pDoc->cancelFilter(pDoc->m_ucPainting, pDoc->m_nPaintWidth, pDoc->m_nPaintHeight);
	pUI->m_kernelDialog->hide();
	// cancel filter
}

void ImpressionistUI::cb_showYGradient(Fl_Menu_* o, void* v) 
{
	ImpressionistDoc* pDoc=whoami(o)->getDocument();

	pDoc->loadGradient(pDoc->m_ucGradientY, pDoc->m_nPaintWidth, pDoc->m_nPaintHeight, pDoc->m_ucPainting);
}

void ImpressionistUI::cb_showXGradient(Fl_Menu_* o, void* v) 
{
	ImpressionistDoc* pDoc=whoami(o)->getDocument();

	pDoc->loadGradient(pDoc->m_ucGradientX, pDoc->m_nPaintWidth, pDoc->m_nPaintHeight, pDoc->m_ucPainting);
}


void ImpressionistUI::cb_unshowGradient(Fl_Menu_* o, void* v) 
{
	ImpressionistDoc* pDoc=whoami(o)->getDocument();

	pDoc->unloadGradient(pDoc->m_ucPainting, pDoc->m_nPaintWidth, pDoc->m_nPaintHeight);
}

//-----------------------------------------------------------
// Updates the brush size to use from the value of the size
// slider
// Called by the UI when the size slider is moved
//-----------------------------------------------------------
void ImpressionistUI::cb_sizeSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nSize=int( ((Fl_Slider *)o)->value() ) ;
}

//-----------------------------------------------------------
// Updates the brush weight to use from the value of the weight
// slider
// Called by the UI when the weight slider is moved
//-----------------------------------------------------------
void ImpressionistUI::cb_weightSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nWeight=int( ((Fl_Slider *)o)->value() ) ;
}

//-----------------------------------------------------------
// Updates the brush angle to use from the value of the angle
// slider
// Called by the UI when the angle slider is moved
//-----------------------------------------------------------
void ImpressionistUI::cb_angleSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nAngle=int( ((Fl_Slider *)o)->value() ) ;
}

//-----------------------------------------------------------
// Updates the brush alpha to use from the value of the alpha
// slider
// Called by the UI when the alpha slider is moved
//-----------------------------------------------------------
void ImpressionistUI::cb_alphaSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nAlpha=float( ((Fl_Slider *)o)->value() ) ;
}

//---------------------------------- per instance functions --------------------------------------

//------------------------------------------------
// Return the ImpressionistDoc used
//------------------------------------------------
ImpressionistDoc* ImpressionistUI::getDocument()
{
	return m_pDoc;
}

//------------------------------------------------
// Draw the main window
//------------------------------------------------
void ImpressionistUI::show() {
	m_mainWindow->show();
	m_paintView->show();
	m_origView->show();
}

//------------------------------------------------
// Change the paint and original window sizes to 
// w by h
//------------------------------------------------
void ImpressionistUI::resize_windows(int w, int h) {
	m_paintView->size(w,h);
	m_origView->size(w,h);
}

//------------------------------------------------ 
// Set the ImpressionistDoc used by the UI to 
// communicate with the brushes 
//------------------------------------------------
void ImpressionistUI::setDocument(ImpressionistDoc* doc)
{
	m_pDoc = doc;

	m_origView->m_pDoc = doc;
	m_paintView->m_pDoc = doc;
}

//------------------------------------------------
// Return the brush size
//------------------------------------------------
int ImpressionistUI::getSize()
{
	return m_nSize;
}

//-------------------------------------------------
// Set the brush size
//-------------------------------------------------
void ImpressionistUI::setSize( int size )
{
	m_nSize=size;

	if (size<=40) 
		m_BrushSizeSlider->value(m_nSize);
}

//------------------------------------------------
// Return the brush weight
//------------------------------------------------
int ImpressionistUI::getWeight()
{
	return m_nWeight;
}

//-------------------------------------------------
// Set the brush weight
//-------------------------------------------------
void ImpressionistUI::setWeight( int weight )
{
	m_nWeight=weight;

	if (weight<=40) 
		m_BrushWeightSlider->value(m_nWeight);
}

//------------------------------------------------
// Return the brush angle
//------------------------------------------------
int ImpressionistUI::getAngle()
{
	return m_nAngle;
}

//-------------------------------------------------
// Set the brush angle
//-------------------------------------------------
void ImpressionistUI::setAngle( int angle )
{
	m_nAngle=angle;

	if (angle<=359)
		m_BrushAngleSlider->value(m_nAngle);
}

//------------------------------------------------
// Return the brush alpha
//------------------------------------------------
float ImpressionistUI::getAlpha()
{
	return m_nAlpha;
}

//-------------------------------------------------
// Set the brush alpha
//-------------------------------------------------
void ImpressionistUI::setAlpha( float alpha )
{
	m_nAlpha=alpha;

	if (alpha<=40) 
		m_BrushAlphaSlider->value(m_nAlpha);
}

// Main menu definition
Fl_Menu_Item ImpressionistUI::menuitems[] = {
	{ "&File",		0, 0, 0, FL_SUBMENU },
		{ "&Load Image...",	FL_ALT + 'l', (Fl_Callback *)ImpressionistUI::cb_load_image },
		{ "&Save Image...",	FL_ALT + 's', (Fl_Callback *)ImpressionistUI::cb_save_image },
		{ "&Brushes...",	FL_ALT + 'b', (Fl_Callback *)ImpressionistUI::cb_brushes },
		{ "&Kernel...",		FL_ALT + 'k', (Fl_Callback *)ImpressionistUI::cb_filter_kernel},
		{ "&Clear Canvas", FL_ALT + 'c', (Fl_Callback *)ImpressionistUI::cb_clear_canvas, 0, FL_MENU_DIVIDER },
		
		{ "&Quit",			FL_ALT + 'q', (Fl_Callback *)ImpressionistUI::cb_exit },
		{ 0 },
	{ "&Gradients", 0, 0, 0, FL_SUBMENU},
		{ "&X Gradient",	FL_ALT + 'x', (Fl_Callback *)ImpressionistUI::cb_showXGradient },
		{ "&Y Gradient",	FL_ALT + 'y', (Fl_Callback *)ImpressionistUI::cb_showYGradient },
		{ "&Return to Paint view",	FL_ALT + 'r', (Fl_Callback *)ImpressionistUI::cb_unshowGradient },
		{ 0 },

	{ "&Help",		0, 0, 0, FL_SUBMENU },
		{ "&About",	FL_ALT + 'a', (Fl_Callback *)ImpressionistUI::cb_about },
		{ 0 },

	{ 0 }
};

// Brush choice menu definition
Fl_Menu_Item ImpressionistUI::brushTypeMenu[NUM_BRUSH_TYPE+1] = {
  {"Points",			FL_ALT+'p', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_POINTS},
  {"Lines",				FL_ALT+'l', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_LINES},
  {"Circles",			FL_ALT+'c', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_CIRCLES},
  {"Triangles",			FL_ALT+'t', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_TRIANGLES},
  {"Scattered Points",	FL_ALT+'q', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_POINTS},
  {"Scattered Lines",	FL_ALT+'m', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_LINES},
  {"Scattered Circles",	FL_ALT+'d', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_CIRCLES},
  {0}
};

// Stroke chooser menu definition
Fl_Menu_Item ImpressionistUI::strokeMenu[NUM_STROKE_TYPE+1] = {
  {"Sliders/Right Click",	FL_ALT+'s', (Fl_Callback *)ImpressionistUI::cb_strokeChoice, (void *)STROKE_SLIDERS},
  {"Brush Direction",		FL_ALT+'b', (Fl_Callback *)ImpressionistUI::cb_strokeChoice, (void *)STROKE_BRUSH},
  {"Against Gradient",		FL_ALT+'g', (Fl_Callback *)ImpressionistUI::cb_strokeChoice, (void *)STROKE_GRAD},
  {0}
};

Fl_Menu_Item ImpressionistUI::imageMenu[3] = {
  {"Original",	FL_ALT+'o', (Fl_Callback *)ImpressionistUI::cb_dummyCallback, (void *)ORIG_IMAGE},
  {"Painting",	FL_ALT+'p', (Fl_Callback *)ImpressionistUI::cb_dummyCallback, (void *)PAINT_IMAGE},
  {0}
};


//----------------------------------------------------
// Constructor.  Creates all of the widgets.
// Add new widgets here
//----------------------------------------------------
ImpressionistUI::ImpressionistUI() {
	int r,c;
	// Create the main window


	m_mainWindow = new Fl_Window(600, 300, "Impressionist");
		m_mainWindow->user_data((void*)(this));	// record self to be used by static callback functions
		// install menu bar
		m_menubar = new Fl_Menu_Bar(0, 0, 600, 25);
		m_menubar->menu(menuitems);

		// Create a group that will hold two sub windows inside the main
		// window
		Fl_Group* group = new Fl_Group(0, 25, 600, 275);

			// install paint view window
			m_paintView = new PaintView(300, 25, 300, 275, "This is the paint view");//0jon
			m_paintView->box(FL_DOWN_FRAME);

			// install original view window
			m_origView = new OriginalView(0, 25, 300, 275, "This is the orig view");//300jon
			m_origView->box(FL_DOWN_FRAME);
			m_origView->deactivate();

		group->end();
		Fl_Group::current()->resizable(group);
	m_mainWindow->end();

	// init values

	m_nSize = 10;
	m_nWeight = 1;
	m_nAngle = 0;
	m_nAlpha = 1.0;
	divisor = 1.0;
	offset = 0.0;

	// brush dialog definition
	m_brushDialog = new Fl_Window(400, 325, "Brush Dialog");
		// Add a brush type choice to the dialog
		m_BrushTypeChoice = new Fl_Choice(50,10,150,25,"&Brush");
		m_BrushTypeChoice->user_data((void*)(this));	// record self to be used by static callback functions
		m_BrushTypeChoice->menu(brushTypeMenu);
		m_BrushTypeChoice->callback(cb_brushChoice);

		m_ClearCanvasButton = new Fl_Button(240,10,150,25,"&Clear Canvas");
		m_ClearCanvasButton->user_data((void*)(this));
		m_ClearCanvasButton->callback(cb_clear_canvas_button);

		// Add a stroke type choice to the dialog
		m_strokeTypeChoice = new Fl_Choice(50,40,150,25,"&Stroke");
		m_strokeTypeChoice->user_data((void*)(this));	// record self to be used by static callback functions
		m_strokeTypeChoice->menu(strokeMenu);
		m_strokeTypeChoice->callback(cb_strokeChoice);

		m_AutoDrawButton = new Fl_Button(280,40,110,25,"&Auto Draw");
		m_AutoDrawButton->user_data((void*)(this));
		m_AutoDrawButton->callback(cb_auto_draw_buton);


		// Add brush size slider to the dialog 
		m_BrushSizeSlider = new Fl_Value_Slider(10, 80, 300, 20, "Size");
		m_BrushSizeSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_BrushSizeSlider->type(FL_HOR_NICE_SLIDER);
		m_BrushSizeSlider->labelfont(FL_COURIER);
		m_BrushSizeSlider->labelsize(12);
		m_BrushSizeSlider->minimum(1);
		m_BrushSizeSlider->maximum(40);
		m_BrushSizeSlider->step(1);
		m_BrushSizeSlider->value(m_nSize);
		m_BrushSizeSlider->align(FL_ALIGN_RIGHT);
		m_BrushSizeSlider->callback(cb_sizeSlides);

		// Add line thickness slider to the dialog
		m_BrushWeightSlider = new Fl_Value_Slider(10, 110, 300, 20, "Line Weight");
		m_BrushWeightSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_BrushWeightSlider->type(FL_HOR_NICE_SLIDER);
		m_BrushWeightSlider->labelfont(FL_COURIER);
		m_BrushWeightSlider->labelsize(12);
		m_BrushWeightSlider->minimum(1);
		m_BrushWeightSlider->maximum(40);
		m_BrushWeightSlider->step(1);
		m_BrushWeightSlider->value(m_nWeight);
		m_BrushWeightSlider->align(FL_ALIGN_RIGHT);
		m_BrushWeightSlider->callback(cb_weightSlides);
		m_BrushWeightSlider->deactivate();

		// Add brush angle slider to the dialog
		m_BrushAngleSlider = new Fl_Value_Slider(10, 140, 300, 20, "Line Angle");
		m_BrushAngleSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_BrushAngleSlider->type(FL_HOR_NICE_SLIDER);
		m_BrushAngleSlider->labelfont(FL_COURIER);
		m_BrushAngleSlider->labelsize(12);
		m_BrushAngleSlider->minimum(0);
		m_BrushAngleSlider->maximum(359);
		m_BrushAngleSlider->step(1);
		m_BrushAngleSlider->value(m_nAngle);
		m_BrushAngleSlider->align(FL_ALIGN_RIGHT);
		m_BrushAngleSlider->callback(cb_angleSlides);
		m_BrushAngleSlider->deactivate();

		// Add alpha slider to the dialog
		m_BrushAlphaSlider = new Fl_Value_Slider(10, 170, 300, 20, "Alpha");
		m_BrushAlphaSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_BrushAlphaSlider->type(FL_HOR_NICE_SLIDER);
		m_BrushAlphaSlider->labelfont(FL_COURIER);
		m_BrushAlphaSlider->labelsize(12);
		m_BrushAlphaSlider->minimum(0.0);
		m_BrushAlphaSlider->maximum(1.0);
		m_BrushAlphaSlider->step(0.01);
		m_BrushAlphaSlider->value(m_nAlpha);
		m_BrushAlphaSlider->align(FL_ALIGN_RIGHT);
		m_BrushAlphaSlider->callback(cb_alphaSlides);

		m_radialX = new Fl_Float_Input(20, 200, 50, 25, "X");
		m_radialX->user_data((void*)(this));
		m_radialX->value("0");

		m_radialY = new Fl_Float_Input(90, 200, 50, 25, "Y");
		m_radialY->user_data((void*)(this));
		m_radialY->value("0");

		m_RadialDrawButton = new Fl_Button(150,200,110,25,"&Radial Draw");
		m_RadialDrawButton->user_data((void*)(this));
		m_RadialDrawButton->callback(cb_radial_draw_buton);


	m_brushDialog->end();

	m_kernelDialog = new Fl_Window(270, 310, "Kernel Dialog");
		for(int r = 0; r < FLT_HEIGHT; r++){
			for(int c = 0; c < FLT_WIDTH; c++){
				m_kernel[r*FLT_WIDTH+c] = new Fl_Float_Input(10+c*50, 10+r*25, 50, 25, "");
				m_kernel[r*FLT_WIDTH+c]->user_data((void*)(this));
				if(r*FLT_WIDTH+c == FLT_HEIGHT*FLT_WIDTH/2){
					m_kernel[r*FLT_WIDTH+c]->value("1");
				}
				else{
					m_kernel[r*FLT_WIDTH+c]->value("0");	
				}
			}
		}
		m_divisor = new Fl_Float_Input(110, 145, 100, 25, "Divide By");
		m_divisor->user_data((void*)(this));
		m_divisor->value("1");

		m_offset = new Fl_Float_Input(110, 175, 100, 25, "Offset");
		m_offset->user_data((void*)(this));
		m_offset->value("0");

		m_srcImageChoice = new Fl_Choice(110,205,150,25,"&Source");
		m_srcImageChoice->user_data((void*)(this));
		m_srcImageChoice->value(ORIG_IMAGE);
		m_srcImageChoice->menu(imageMenu);

		m_PreviewKernel = new Fl_Button(60, 240,150,25,"&Preview");
		m_PreviewKernel->user_data((void*)(this));
		m_PreviewKernel->callback(cb_previewKernelButton);

		m_ApplyKernel = new Fl_Button(10,275,120,25,"&Apply");
		m_ApplyKernel->user_data((void*)(this));
		m_ApplyKernel->callback(cb_applyKernelButton);

		m_CancelKernel = new Fl_Button(140,275,120,25,"&Cancel");
		m_CancelKernel->user_data((void*)(this));
		m_CancelKernel->callback(cb_cancelKernelButton);

	m_kernelDialog->end();

}

ImpressionistUI::~ImpressionistUI()
{
}
