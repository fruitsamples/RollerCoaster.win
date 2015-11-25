/*
	File:		MacApplication.c
	
	Contains:	Contains standard mac event loop & initialization code
	
	Written by:	Scott Kuechle, based on original Gerbils code by Brian Greenstone

	Copyright:	� 1998 by Apple Computer, Inc. All rights reserved
	
	Change History (most recent first)
	
		<1>		9/1/98		srk		first file


*/

/************************************************************
*                                                           *
*    INCLUDE FILES                                          *
*                                                           *
*************************************************************/


#include "MacApplication.h"


/************************************************************
*                                                           *
*    FUNCTION PROTOTYPES                                    *
*                                                           *
*************************************************************/

static void 		InitToolbox( void ) ;
static void 		MainEventLoop( void ) ;
static void 		HandleKeyPress(EventRecord *event) ;
static void 		HandleOSEvent(EventRecord *event) ;
static short 		HiWrd(long aLong);
static short 		LoWrd(long aLong);

/************************************************************
*                                                           *
*    GLOBAL VARIABLES FOR THIS SOURCE FILE                  *
*                                                           *
*************************************************************/

static DocumentRec	gDocument ;

/************************************************************
*                                                           *
*    FUNCTION:  Main                                        *
*                                                           *
*    PURPOSE:   Main routine for Macintosh sample program   *
*                                                           *
*************************************************************/

void main(void)
{
	Rect		rBounds = { 50, 50, 450, 450 } ;
	Str255		title = "\pRollerCoaster" ;

		InitToolbox() ;
		
			/* set up our globals */
		gDocument.fMainWindow = NewCWindow(nil,&rBounds,title,true,noGrowDocProc,(WindowPtr)-1,true,0) ;
		if (gDocument.fMainWindow == NULL)
		{
			Utils_DisplayFatalErrorMsg("Fatal Error: Unable to allocate a window!");
		}

		#if TARGET_OS_MAC
			QD3DSupport_InitDoc3DDTARGET_OS_WIN32ata( gDocument.fMainWindow, &gDocument );
		#else if 
			QD3DSupport_InitDoc3DData( HWND window, &gDocument );
		#endif
		
		MainEventLoop();
		
		QD3DSupport_DisposeDoc3DData( &gDocument ) ;
	
}






/************************************************************
*                                                           *
*    FUNCTION:  HiWrd                                       *
*                                                           *
*    PURPOSE:   Given a long value, return the high word    *
*                                                           *
*************************************************************/

static short HiWrd(long aLong)
{
	return	(short)(((aLong) >> 16) & 0xFFFF) ;
}

/************************************************************
*                                                           *
*    FUNCTION:  LoWrd                                       *
*                                                           *
*    PURPOSE:   Given a long value, return the low word     *
*                                                           *
*************************************************************/

static short LoWrd(long aLong)
{
	return	(short)((aLong) & 0xFFFF) ;

}

/************************************************************
*                                                           *
*    FUNCTION:  InitToolbox                                 *
*                                                           *
*    PURPOSE:   Initialize the various Macintosh Toolbox    *
*               Managers                                    *
*                                                           *
*************************************************************/

static void InitToolbox()
{
	Handle		menuBar = nil;

	MaxApplZone() ;
	MoreMasters() ; MoreMasters() ; MoreMasters() ; 
	
	InitGraf( &qd.thePort );
	InitFonts();
	InitWindows();
	InitCursor();

	FlushEvents( everyEvent, 0 ) ;
	
}


/************************************************************
*                                                           *
*    FUNCTION:  MainEventLoop                               *
*                                                           *
*    PURPOSE:   Standard Macintosh event loop               *
*                                                           *
*************************************************************/
static void MainEventLoop()
{
	EventRecord 	event;
	WindowPtr   	window;
	short       	thePart;
	Rect        	screenRect;
	Point			aPoint = {100, 100};
	Boolean 		quitFlag = false;
	GrafPtr			oldPort;

	while( !quitFlag )
	{
		if (WaitNextEvent( everyEvent, &event, 0, nil ))
		{

			switch (event.what)
			{
				case mouseDown:
				
					thePart = FindWindow( event.where, &window );
					
					switch( thePart )
					{
						case inMenuBar: 
							break;
						
						case inDrag:
					
							screenRect = (**GetGrayRgn()).rgnBBox;
							DragWindow( window, event.where, &screenRect );
							break ;
					
						case inContent:
					
							if (window != FrontWindow())
								SelectWindow( window );
							break ;
					
						case inGoAway:
							if (TrackGoAway( window, event.where ))
							{
								DisposeWindow ( window );
								quitFlag = true;

							}
							break ;
							
						default:
							break ;
					}
					break ;
							
						
				case updateEvt:
				
					window = (WindowPtr)event.message;
					
					GetPort(&oldPort);
					SetPort( window ) ;
					
					BeginUpdate( window );
					QD3DSupport_DocDraw3DData( &gDocument ) ;
					EndUpdate( window );
					
					SetPort(oldPort);
					break ;
					
				case keyDown:
				case autoKey:
					HandleKeyPress(&event);
					break;
					
				case diskEvt:
					if ( HiWrd(event.message) != noErr ) 
						(void) DIBadMount(aPoint, event.message);
					break;
					
				case osEvt:
				case activateEvt:
					break;
			}
		}
		else
		{
				/* we received a null event */
			Rect theRect = ((GrafPtr)gDocument.fMainWindow)->portRect ;
			
				SetPort((GrafPtr)gDocument.fMainWindow) ;
				InvalRect( &theRect ) ;
		}
	}
}


/************************************************************
*                                                           *
*    FUNCTION:  HandleKeyPress                              *
*                                                           *
*    PURPOSE:   Handle any keypress events                  *
*                                                           *
*************************************************************/
static void HandleKeyPress(EventRecord *event)
{
#pragma unused (event)
}

