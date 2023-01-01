#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <stdbool.h>
#include <errno.h>
SDL_Window* rootWindow;
SDL_Renderer* rootWindowRenderer;
void clean() {
    fprintf(stderr, "How are you");

}

#define XDND_PROTOCOL_VERSION 5
static Atom XdndAware, /*XA_ATOM,*/ XdndEnter, XdndPosition, XdndActionCopy, XdndLeave, XdndStatus, XdndDrop,
XdndSelection, XDND_DATA, XdndTypeList, XdndFinished, WM_PROTOCOLS, SDL_WAKEUP, WM_DELETE_WINDOW, typesWeAccept[6];

// This function prints the contents of ClientMessage events
static void printClientMessage(Display *disp, XClientMessageEvent *message)
{
    // Get atom type as string
    if(message->message_type == SDL_WAKEUP) return;
    static int r = 0;
    char *messageTypeStr = XGetAtomName(disp, message->message_type);
    fprintf(stderr,"Message type: %s %d %d\n", messageTypeStr, message->message_type, r++);
    XFree(messageTypeStr);

    // Handle format
    fprintf(stderr,"Message word size: %d bits\n", message->format);
    fprintf(stderr,"Message ");
    switch (message->format) {
    case 8:
        fprintf(stderr,"bytes: ");
        for (int i = 0; i < 20; ++i)
            fprintf(stderr,"%d ", message->data.b[i]);
        break;
    case 16:
        fprintf(stderr,"16-bit shorts: ");
        for (int i = 0; i < 10; ++i)
            fprintf(stderr,"%d ", message->data.s[i]);
        break;
    case 32:
        fprintf(stderr,"32-bit longs: ");
        for (int i = 0; i < 5; ++i)
            fprintf(stderr,"%d ", (int32_t)message->data.l[i]);
        break;
    }
    fprintf(stderr,"\n");
}

/* This turns events into strings */
const char *getEventType(XEvent *event)
{
    const char *retVal;
    switch (event->type) {
    case 2:
        retVal = "KeyPress";
        break;
    case 3:
        retVal = "KeyRelease";
        break;
    case 4:
        retVal = "ButtonPress";
        break;
    case 5:
        retVal = "ButtonRelease";
        break;
    case 6:
        retVal = "MotionNotify";
        break;
    case 7:
        retVal = "EnterNotify";
        break;
    case 8:
        retVal = "LeaveNotify";
        break;
    case 9:
        retVal = "FocusIn";
        break;
    case 10:
        retVal = "FocusOut";
        break;
    case 11:
        retVal = "KeymapNotify";
        break;
    case 12:
        retVal = "Expose";
        break;
    case 13:
        retVal = "GraphicsExpose";
        break;
    case 14:
        retVal = "NoExpose";
        break;
    case 15:
        retVal = "VisibilityNotify";
        break;
    case 16:
        retVal = "CreateNotify";
        break;
    case 17:
        retVal = "DestroyNotify";
        break;
    case 18:
        retVal = "UnmapNotify";
        break;
    case 19:
        retVal = "MapNotify";
        break;
    case 20:
        retVal = "MapRequest";
        break;
    case 21:
        retVal = "ReparentNotify";
        break;
    case 22:
        retVal = "ConfigureNotify";
        break;
    case 23:
        retVal = "ConfigureRequest";
        break;
    case 24:
        retVal = "GravityNotify";
        break;
    case 25:
        retVal = "ResizeRequest";
        break;
    case 26:
        retVal = "CirculateNotify";
        break;
    case 27:
        retVal = "CirculateRequest";
        break;
    case 28:
        retVal = "PropertyNotify";
        break;
    case 29:
        retVal = "SelectionClear";
        break;
    case 30:
        retVal = "SelectionRequest";
        break;
    case 31:
        retVal = "SelectionNotify";
        break;
    case 32:
        retVal = "ColormapNotify";
        break;
    case 33:
        retVal = "ClientMessage";
        break;
    case 34:
        retVal = "MappingNotify";
        break;
    case 35:
        retVal = "GenericEvent";
        break;
    default:
        retVal = "Unknown";
        break;
    }

    return retVal;
}


static void philErrorCore(const char *str, va_list args)
{
    /* Call vfprintf etc. */
    vfprintf(stderr, str, args);
    fprintf(stderr, ": %s\n", strerror(errno));
}

void philErrorMsg(const char *str, ...)
{
    /* Variables */
    va_list list;

    /* Process arguments */
    va_start(list, str);
    philErrorCore(str, list);
    va_end(list);

    /* Reset errno */
    errno = 0;
}

void philError(const char *str, ...)
{
    /* Variables */
    va_list list;

    /* Process arguments */
    va_start(list, str);
    philErrorCore(str, list);
    va_end(list);

    /* Exit with error */
    exit(EXIT_FAILURE);
}







// State machine structure
typedef struct {
    bool xdndExchangeStarted;
    bool xdndPositionReceived;
    bool xdndStatusReceived;
    bool xdndStatusSent;
    bool xdndDropReceived;
    Time xdndDropTimestamp;
    Time xdndLastPositionTimestamp;
    bool amISource;
    int p_rootX;
    int p_rootY;
    Window otherWindow;
    Atom proposedAction;
    Atom proposedType;
} XDNDStateMachine;
// Atom definitions

// XDND global state machine
static XDNDStateMachine xdndState;

// This somewhat naively calculates what window we are over by drilling down
// to its children and so on using recursion
static Window getWindowPointerIsOver(Display *disp, Window startingWindow,
                                     int p_rootX, int p_rootY, int originX, int originY)
{
    // Window we are returning
    Window returnWindow = None;

    // Get stacked list of children in stacked order
    Window rootReturn, parentReturn, childReturn, *childList;
    unsigned int numOfChildren;
    if (XQueryTree(disp, startingWindow, &rootReturn, &parentReturn,
                   &childList, &numOfChildren) != 0) {
        // Search through children
        for (int i = numOfChildren - 1; i >= 0; --i) {
            // Get window attributes
            XWindowAttributes childAttrs;
            XGetWindowAttributes(disp, childList[i], &childAttrs);

            // Check if cursor is in this window
            if (p_rootX >= originX + childAttrs.x &&
                    p_rootX < originX + childAttrs.x + childAttrs.width &&
                    p_rootY >= originY + childAttrs.y &&
                    p_rootY < originY + childAttrs.y + childAttrs.height) {
                returnWindow = getWindowPointerIsOver(disp, childList[i],
                                                      p_rootX, p_rootY, originX + childAttrs.x, originY + childAttrs.y);
                break;
            }
        }
        XFree(childList);
    }

    // We are are bottom of recursion stack, set correct window to be returned up through each level
    if (returnWindow == None)
        returnWindow = startingWindow;

    return returnWindow;
}
// This checks if the supplied window has the XdndAware property
static int hasCorrectXdndAwareProperty(Display *disp, Window wind) {
    // Try to get property
    int retVal = 0;
    Atom actualType = None;
    int actualFormat;
    unsigned long numOfItems, bytesAfterReturn;
    unsigned char *data = NULL;
    if (XGetWindowProperty(disp, wind, XdndAware, 0, 1024, False, AnyPropertyType,
                           &actualType, &actualFormat, &numOfItems, &bytesAfterReturn, &data) == Success) {
        if (actualType != None) {
            // Assume architecture is little endian and just read first byte for
            // XDND protocol version
            if (data[0] <= XDND_PROTOCOL_VERSION) {
                retVal = data[0];
            }

            XFree(data);
        }
    }

    return retVal;
}

// This sends the XdndEnter message which initiates the XDND protocol exchange
static void sendXdndEnter(Display *disp, int xdndVersion, Window source, Window target)
{
    // Only send if we are not already in an exchange
    if (!xdndState.xdndExchangeStarted) {
        // Declare message struct and populate its values
        XEvent message;
        memset(&message, 0, sizeof(message));
        message.xclient.type = ClientMessage;
        message.xclient.display = disp;
        message.xclient.window = target;
        message.xclient.message_type = XdndEnter;
        message.xclient.format = 32;
        message.xclient.data.l[0] = source;
        message.xclient.data.l[1] = xdndVersion << 24;
        message.xclient.data.l[2] = typesWeAccept[0];
        message.xclient.data.l[3] = None;
        message.xclient.data.l[4] = None;

        // Send it to target window
        if (XSendEvent(disp, target, False, 0, &message) == 0)
            philError("XSendEvent");
    }
}

// This sends the XdndPosition messages, which update the target on the state of the cursor
// and selected action
static void sendXdndPosition(Display *disp, Window source, Window target, int time, int p_rootX, int p_rootY)
{
    if (xdndState.xdndExchangeStarted && xdndState.amISource) {
        // Declare message struct and populate its values
        XEvent message;
        memset(&message, 0, sizeof(message));
        message.xclient.type = ClientMessage;
        message.xclient.display = disp;
        message.xclient.window = target;
        message.xclient.message_type = XdndPosition;
        message.xclient.format = 32;
        message.xclient.data.l[0] = source;
        //message.xclient.data.l[1] reserved
        message.xclient.data.l[2] = p_rootX << 16 | p_rootY;
        message.xclient.data.l[3] = time;
        message.xclient.data.l[4] = XdndActionCopy;

        // Send it to target window
        if (XSendEvent(disp, target, False, 0, &message) == 0)
            philError("XSendEvent");
    }
}
// This is sent by the source when the exchange is abandoned
static void sendXdndLeave(Display *disp, Window source, Window target)
{
    if (xdndState.xdndExchangeStarted && xdndState.amISource) {
        // Declare message struct and populate its values
        XEvent message;
        memset(&message, 0, sizeof(message));
        message.xclient.type = ClientMessage;
        message.xclient.display = disp;
        message.xclient.window = target;
        message.xclient.message_type = XdndLeave;
        message.xclient.format = 32;
        message.xclient.data.l[0] = source;
        // Rest of array members reserved so not set

        // Send it to target window
        if (XSendEvent(disp, target, False, 0, &message) == 0)
            philError("XSendEvent");
    }
}
// This is sent by the target when the exchange has completed
static void sendXdndFinished(Display *disp, Window source, Window target)
{
    if (xdndState.xdndExchangeStarted && !xdndState.amISource) {
        // Declare message struct and populate its values
        XEvent message;
        memset(&message, 0, sizeof(message));
        message.xclient.type = ClientMessage;
        message.xclient.display = disp;
        message.xclient.window = target;
        message.xclient.message_type = XdndFinished;
        message.xclient.format = 32;
        message.xclient.data.l[0] = source;
        message.xclient.data.l[1] = 1;
        message.xclient.data.l[2] = XdndActionCopy;

        // Send it to target window
        if (XSendEvent(disp, target, False, 0, &message) == 0)
            philError("XSendEvent");
    }
}
// This is sent by the target to the source to say whether or not it will accept the drop
static void sendXdndStatus(Display *disp, Window source, Window target, Atom action)
{
    if (xdndState.xdndExchangeStarted && !xdndState.amISource) {
        // Declare message struct and populate its values
        XEvent message;
        memset(&message, 0, sizeof(message));
        message.xclient.type = ClientMessage;
        message.xclient.display = disp;
        message.xclient.window = target;
        message.xclient.message_type = XdndStatus;
        message.xclient.format = 32;
        message.xclient.data.l[0] = source;
        message.xclient.data.l[1] = 1; // Sets accept and want position flags

        // Send back window rectangle coordinates and width
        message.xclient.data.l[2] = 0;
        message.xclient.data.l[3] = 0;

        // Specify action we accept
        message.xclient.data.l[4] = action;

        // Send it to target window
        if (XSendEvent(disp, target, False, 0, &message) == 0)
            philError("XSendEvent");
    }
}
// This is sent by the source to the target to say it can call XConvertSelection
static void sendXdndDrop(Display *disp, Window source, Window target)
{
    if (xdndState.xdndExchangeStarted && xdndState.amISource) {
        // Declare message struct and populate its values
        XEvent message;
        memset(&message, 0, sizeof(message));
        message.xclient.type = ClientMessage;
        message.xclient.display = disp;
        message.xclient.window = target;
        message.xclient.message_type = XdndDrop;
        message.xclient.format = 32;
        message.xclient.data.l[0] = source;
        //message.xclient.data.l[1] reserved
        message.xclient.data.l[2] = xdndState.xdndLastPositionTimestamp;

        // Send it to target window
        if (XSendEvent(disp, target, False, 0, &message) == 0)
            philError("XSendEvent");
    }
}
// This is sent by the source to the target to say the data is ready
static void sendSelectionNotify(Display *disp, XSelectionRequestEvent *selectionRequest, const char *pathStr)
{
    if (xdndState.xdndExchangeStarted && xdndState.amISource) {
        // Allocate buffer (two bytes at end for CR/NL and another for null byte)
        size_t sizeOfPropertyData = strlen("file://") + strlen(pathStr) + 3;
        char *propertyData = malloc(sizeOfPropertyData);
        if (!propertyData)
            philError("malloc");

        // Copy data to buffer
        strcpy(propertyData, "file://");
        strcat(propertyData, pathStr);
        propertyData[sizeOfPropertyData-3] = 0xD;
        propertyData[sizeOfPropertyData-2] = 0xA;
        propertyData[sizeOfPropertyData-1] = '\0';

        // Set property on target window - do not copy end null byte
        XChangeProperty(disp, selectionRequest->requestor, selectionRequest->property,
                        typesWeAccept[0], 8, PropModeReplace, (unsigned char *)propertyData, sizeOfPropertyData-1);

        // Free property buffer
        free(propertyData);

        // Declare message struct and populate its values
        XEvent message;
        memset(&message, 0, sizeof(message));
        message.xselection.type = SelectionNotify;
        message.xselection.display = disp;
        message.xselection.requestor = selectionRequest->requestor;
        message.xselection.selection = selectionRequest->selection;
        message.xselection.target = selectionRequest->target;
        message.xselection.property = selectionRequest->property;
        message.xselection.time = selectionRequest->time;

        // Send it to target window
        if (XSendEvent(disp, selectionRequest->requestor, False, 0, &message) == 0)
            philError("XSendEvent");
    }
}

// Test whether the supplied atom is in our types list
static bool doWeAcceptAtom(Atom a)
{
    for (int i = 0; i < sizeof(typesWeAccept) / sizeof(Atom); ++i) {
        if (a == typesWeAccept[i]) {
            return true;
        }
    }

    return false;
}
// This gets the XdndTypeList from the source window when we need it, and then determines the
// type we will ask for
static Atom getSupportedType(Display *disp, Window source)
{
    // Try to get XdndTypeList property
    Atom retVal = None;
    Atom actualType = None;
    int actualFormat;
    unsigned long numOfItems, bytesAfterReturn;
    unsigned char *data = NULL;
    if (XGetWindowProperty(disp, source, XdndTypeList, 0, 1024, False, AnyPropertyType,
                           &actualType, &actualFormat, &numOfItems, &bytesAfterReturn, &data) == Success) {
        if (actualType != None) {
            Atom *supportedAtoms = (Atom *)data;
            for (int i = 0; i < numOfItems; ++i) {
                if (doWeAcceptAtom(supportedAtoms[i])) {
                    retVal = supportedAtoms[i];
                    break;
                }
            }

            XFree(data);
        }
    }

    return retVal;
}
// Read copied path string from our window property
static char *getCopiedData(Display *disp, Window source)
{
    // Declare return value
    char *retVal = NULL;

    // Try to get PRIMARY property
    Atom actualType = None;
    int actualFormat;
    unsigned long numOfItems, bytesAfterReturn;
    unsigned char *data = NULL;
    if (XGetWindowProperty(disp, source, XDND_DATA, 0, 1024, False, AnyPropertyType,
                           &actualType, &actualFormat, &numOfItems, &bytesAfterReturn, &data) == Success) {
        // Allocate temporary buffer
        char *tempBuffer = malloc(numOfItems + 1);
        if (!tempBuffer)
            philError("malloc");

        // Copy all data from X buffer then add null-byte to create proper string, then
        // dispose of X buffer
        memcpy(tempBuffer, data, numOfItems);
        tempBuffer[numOfItems] = '\0';
        XFree(data);

        // Copy from beyond 'file://' prefix if present
        char *tempPtr;
        if ((tempPtr = strstr(tempBuffer, "file://")) != NULL) {
            tempPtr = tempBuffer + 7;
        } else {
            tempPtr = tempBuffer;
        }

        // Check if cr/nl ending is present and terminate string
        // before this if so
        if (tempPtr[strlen(tempPtr)-2] == 0xD && tempPtr[strlen(tempPtr)-1] == 0xA)
            tempPtr[strlen(tempPtr)-2] = '\0';

        // Allocate return buffer
        retVal = malloc(strlen(tempPtr) + 1);
        if (!retVal)
            philError("malloc");

        // Copy data from temp buffer to it, then free temp buffer
        memcpy(retVal, tempPtr, strlen(tempPtr));
        retVal[strlen(tempPtr)] = '\0';
        free(tempBuffer);
    }

    // Return malloc allocated buffer - caller must free
    return retVal;
}
char procStr[] = "guiExperiments";
int main()
{
    if (SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO  ) < 0) {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s \n", SDL_GetError());
        abort();
    }

    rootWindow = SDL_CreateWindow(
                "experiments",
                700,0,700, 700,
                //       0, 0,
                SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
                );
    //    fprintf(stderr, "Hi there");
    //    {
    //        __attribute__((cleanup(clean))) int G = 3;

    //    }
    //    fprintf(stderr, "General Kenobi");


    rootWindowRenderer = SDL_CreateRenderer(rootWindow, -1,  0);
    SDL_Event e;

    bool continueEventLoop = true;
    bool clickedStillInWindow = false;
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version)
            SDL_GetWindowWMInfo(rootWindow, &wmInfo);
    Display* disp = wmInfo.info.x11.display;
    Window wind = wmInfo.info.x11.window;
    unsigned int xdndVersion = XDND_PROTOCOL_VERSION;
    Atom *propertyList;
    int numberOfProperties;

    XdndAware = XInternAtom(disp, "XdndAware", False);
    //    XA_ATOM = XInternAtom(disp, "XA_ATOM", False);
    XdndEnter = XInternAtom(disp, "XdndEnter", False);
    XdndPosition = XInternAtom(disp, "XdndPosition", False);
    XdndActionCopy = XInternAtom(disp, "XdndActionCopy", False);
    XdndLeave = XInternAtom(disp, "XdndLeave", False);
    XdndStatus = XInternAtom(disp, "XdndStatus", False);
    XdndDrop = XInternAtom(disp, "XdndDrop", False);
    XdndSelection = XInternAtom(disp, "XdndSelection", False);
    XDND_DATA = XInternAtom(disp, "XDND_DATA", False);
    XdndTypeList = XInternAtom(disp, "XdndTypeList", False);
    XdndFinished = XInternAtom(disp, "XdndFinished", False);
    WM_PROTOCOLS = XInternAtom(disp, "WM_PROTOCOLS", False);
    WM_DELETE_WINDOW = XInternAtom(disp, "WM_DELETE_WINDOW", False);
    SDL_WAKEUP = XInternAtom(disp, "_SDL_WAKEUP", False);

    // Define type atoms we will accept for file drop
    typesWeAccept[0] = XInternAtom(disp, "text/uri-list", False);
    typesWeAccept[1] = XInternAtom(disp, "UTF8_STRING", False);
    typesWeAccept[2] = XInternAtom(disp, "TEXT", False);
    typesWeAccept[3] = XInternAtom(disp, "STRING", False);
    typesWeAccept[4] = XInternAtom(disp, "text/plain;charset=utf-8", False);
    typesWeAccept[5] = XInternAtom(disp, "text/plain", False);


    //SDL_SysWMmsg

    //    for(int i = 0; i < SDL_LASTEVENT; i++) {
    //        printf("event %x is state %d\n", i, SDL_GetEventState(i));
    //    }
    XChangeProperty(disp, wind, XdndAware,
                    XA_ATOM, 32, PropModeReplace,
                    (void *)&xdndVersion, 1);

    SDL_EventState(0x201, SDL_ENABLE);
    while(    SDL_WaitEvent(&e) ) {
//        fprintf(stderr,"%d\n", clickedStillInWindow);
        SDL_RenderClear(rootWindowRenderer);
        SDL_RenderPresent(rootWindowRenderer);
        if(e.type == SDL_SYSWMEVENT) {
            XEvent xv = e.syswm.msg->msg.x11.event;
            switch (xv.type) {
            // We are being asked for X selection data by the target
            case SelectionRequest:
                fprintf(stderr,"%s: got selectionRequest\n", procStr);
                if (xdndState.xdndExchangeStarted && xdndState.amISource) {
                    // Add data to the target window
                    sendSelectionNotify(disp, &xv.xselectionrequest,
                                        "/tmp/square.state");
                }
                break;
                // We have received a selection notification
            case SelectionNotify:
                // Ignore if not XDND related
                if (xv.xselection.property != XDND_DATA)
                    break;

                // Temporary variables for XQueryPointer
                Window rootReturn, childReturn;
                int rootXReturn, rootYReturn, winXReturn, winYReturn;
                unsigned int maskReturn;

                // Read data out into path string
                char *pathStr = getCopiedData(disp, wind);

                // Delete property on window
                XDeleteProperty(disp, wind, XDND_DATA);

                // Set state on square
                //                    restoreSquareState(pathStr, &square);
                //                    square.visible = true;
                free(pathStr);
                //                    XSetForeground(disp, gContext, square.colour == RedSquare ? red : blue);

                // Set new square coordinate origin
                XQueryPointer(disp, wind, &rootReturn, &childReturn, &rootXReturn, &rootYReturn,
                              &winXReturn, &winYReturn, &maskReturn);
                //                    square.x = winXReturn - 25;
                //                    square.y = winYReturn - 25;
                //                    if (square.x < 0)
                //                        square.x = 0;
                //                    if (square.y < 0)
                //                        square.y = 0;
                //                    if (square.x > 150)
                //                        square.x = 150;
                //                    if (square.y > 150)
                //                        square.y = 150;

                // Send XdndFinished message
                fprintf(stderr,"%s: sending XdndFinished\n", procStr);
                sendXdndFinished(disp, wind, xdndState.otherWindow);
                memset(&xdndState, 0, sizeof(xdndState));
                //                    drawSquare(disp, wind, gContext, &square);
                break;
            case ClientMessage:
                if (xv.xclient.message_type != XdndEnter &&
                        xv.xclient.message_type != XdndPosition &&
                        xv.xclient.message_type != XdndLeave &&
                        xv.xclient.message_type != XdndStatus &&
                        xv.xclient.message_type != XdndDrop &&
                        xv.xclient.message_type != XdndFinished &&
                        xv.xclient.message_type != WM_PROTOCOLS
                        && xv.xclient.message_type != SDL_WAKEUP
                        ) {
                    fprintf(stderr,"%s: received %s message\n", procStr, getEventType(&xv));
                    printClientMessage(disp, &xv.xclient);
                }
                // Check if we are being closed
                if (xv.xclient.message_type == WM_PROTOCOLS) {
                    if (xv.xclient.data.l[0] == WM_DELETE_WINDOW) {
                        // End xv loop
                        continueEventLoop = false;
                        break;
                    }
                }

                // Check if already in XDND protocol exchange
                if (!xdndState.xdndExchangeStarted) {
                    // Only handle XdndEnter messages here
                    if (xv.xclient.message_type == XdndEnter) {
                        fprintf(stderr,"%s: receiving XdndEnter\n", procStr);

                        // Update state
                        xdndState.xdndExchangeStarted = true;
                        xdndState.amISource = false;
                        xdndState.otherWindow = xv.xclient.data.l[0];
                        fprintf(stderr,"data[1] = %ld\n", xv.xclient.data.l[1]);
                        // Determine type to ask for
                        if (xv.xclient.data.l[1] & 0x1) {
                            // More than three types, look in XdndTypeList
                            xdndState.proposedType =
                                    getSupportedType(disp, xdndState.otherWindow);
                        } else {
                            // Only three types, check three in turn and stop when we find
                            // one we support
                            xdndState.proposedType = None;
                            for (int i = 2; i < 5; ++i) {
                                if (doWeAcceptAtom(xv.xclient.data.l[i])) {
                                    fprintf(stderr,"checking atom %s...\n", XGetAtomName(disp,
                                                                                 xv.xclient.data.l[i]));
                                    fprintf(stderr,"success\n");
                                    xdndState.proposedType = xv.xclient.data.l[i];
                                    //								break;
                                }
                            }
                        }
                    }
                    break;
                } else {
                    // Check whether we are source or target
                    if (xdndState.amISource) {
                        // Check for XdndStatus message
                        if (xv.xclient.message_type == XdndStatus) {
                            xdndState.xdndStatusReceived = true;
                            fprintf(stderr,"%s: receiving XdndStatus message\n", procStr);

                            // Check if target will accept drop
                            if ((xv.xclient.data.l[1] & 0x1) != 1) {
                                // Won't accept, break exchange and wipe state
                                fprintf(stderr,"%s: sending XdndLeave message to target window "
                                       "as it won't accept drop\n", procStr);
                                sendXdndLeave(disp, wind, xdndState.otherWindow);
                                memset(&xdndState, 0, sizeof(xdndState));
                                break;
                            }
                        }
                        else if (xv.xclient.message_type == XdndFinished) {
                            fprintf(stderr,"%s: receiving XdndFinished message\n", procStr);
                            //                            square.visible = false;
                            memset(&xdndState, 0, sizeof(xdndState));
                            //                            drawSquare(disp, wind, gContext, &square);
                        }
                    } else {
                        // Check for XdndPosition message
                        if (xv.xclient.message_type == XdndPosition) {
                            fprintf(stderr,"%s: receiving XdndPosition\n", procStr);

                            // Ignore if not for our window and sent erroneously
                            if (xdndState.xdndPositionReceived &&
                                    xv.xclient.data.l[0] != xdndState.otherWindow) {
                                fprintf(stderr,"%s: receiving XdndPosition from erroneous "
                                       "window, ignoring\n", procStr);
                                break;
                            }

                            // Update state
                            xdndState.xdndPositionReceived = true;
                            xdndState.p_rootX = xv.xclient.data.l[2] >> 16;
                            xdndState.p_rootY = xv.xclient.data.l[2] & 0xFFFF;
                            xdndState.proposedAction = xv.xclient.data.l[4];
                            xdndState.xdndLastPositionTimestamp = xv.xclient.data.l[3];

                            // Now check if we should send XdndStatus message
                            if (!xdndState.xdndStatusSent) {
                                fprintf(stderr,"%s: sending XdndStatus\n", procStr);
                                xdndState.xdndStatusSent = true;
                                sendXdndStatus(disp, wind,
                                               xdndState.otherWindow, xdndState.proposedAction);
                            }
                        }

                        // Check for XdndLeave message
                        if (xv.xclient.message_type == XdndLeave) {
                            fprintf(stderr,"%s: receiving XdndLeave, clearing state\n", procStr);
                            memset(&xdndState, 0, sizeof(xdndState));
                        }

                        // Check for XdndDrop message
                        if (xv.xclient.message_type == XdndDrop) {
                            fprintf(stderr,"%s: receiving XdndDrop, processing selection\n", procStr);

                            // Ignore if not for our window and/or sent erroneously
                            if (!xdndState.xdndPositionReceived ||
                                    xv.xclient.data.l[0] != xdndState.otherWindow) {
                                fprintf(stderr,"%s: receiving XdndDrop from erroneous "
                                       "window, ignoring\n", procStr);
                                break;
                            }

                            // Update state
                            xdndState.xdndDropReceived = true;
                            xdndState.xdndDropTimestamp = xv.xclient.data.l[2];

                            // Call XConvertSelection
                            XConvertSelection(disp, XdndSelection, xdndState.proposedType,
                                              XDND_DATA, wind, xdndState.xdndDropTimestamp);
                        }
                    }
                }

            }
        }

        if(e.type == SDL_QUIT) {
            break;
        }
        if(e.type == SDL_MOUSEMOTION) {
            //            if (square.selected) {
            //                square.x += e.motion.x - square.mouse_x;
            //                square.y += e.motion.y - square.mouse_y;
            //                if (square.x < 0)
            //                    square.x = 0;
            //                if (square.y < 0)
            //                    square.y = 0;
            //                if (square.x > 150)
            //                    square.x = 150;
            //                if (square.y > 150)
            //                    square.y = 150;
            //                square.mouse_x = e.motion.x;
            //                square.mouse_y = e.motion.y;
            fprintf(stderr, "hi^): %d", clickedStillInWindow);
            if (!clickedStillInWindow) {
                // Find window cursor is over
                int x,y;
                SDL_GetWindowPosition(rootWindow, &x,&y);
                static Window lastTarget = None;
                Window targetWindow = getWindowPointerIsOver(disp, DefaultRootWindow(disp),
                                                             e.motion.x+x, e.motion.y+y, 0, 0);
                if(lastTarget != targetWindow) {
                    static char* name = NULL;
                    if(name!=NULL) XFree(name);
                    XGetWMName(disp,
                               targetWindow,
                               &name);
                    fprintf(stderr, "mouse over %s",
                            name);
                    lastTarget = targetWindow;
                }

                if (targetWindow == None)
                    break;


                // If cursor has moved out of previous window and cursor XDND
                // exchange is ongoing, cancel it and reset state
                if (xdndState.xdndExchangeStarted && targetWindow != xdndState.otherWindow) {
                    // Send XdndLeave message
                    fprintf(stderr,"%s: sending XdndLeave message to target window 0x%lx\n",
                           procStr, xdndState.otherWindow);
                    sendXdndLeave(disp, wind, xdndState.otherWindow);

                    // Wipe state back to default
                    memset(&xdndState, 0, sizeof(xdndState));
                }

                // Check state of window and engage XDND protocol exchange if needed
                if (!xdndState.xdndExchangeStarted) {
                    // Check it supports XDND
                    int supportsXdnd = hasCorrectXdndAwareProperty(disp, targetWindow);
                    if (supportsXdnd == 0)
                        break;

                    // Claim ownership of Xdnd selection
                    XSetSelectionOwner(disp, XdndSelection, wind, 0/*e.motion.time*/);
                    //XEvent.xmotion
                    // Send XdndEnter message
                    fprintf(stderr,"%s: sending XdndEnter to target window 0x%lx\n",
                           procStr, targetWindow);
                    sendXdndEnter(disp, supportsXdnd, wind, targetWindow);
                    xdndState.xdndExchangeStarted = true;
                    xdndState.amISource = true;
                    xdndState.otherWindow = targetWindow;
                }

                if (!xdndState.xdndStatusReceived) {
                    // Send XdndPosition message
                    fprintf(stderr,"%s: sending XdndPosition to target window 0x%lx\n",
                           procStr, targetWindow);
                    sendXdndPosition(disp, wind, targetWindow, 0,//e.motion.time,
                                     e.motion.x+x, e.motion.y+y);
                }
            }
            //            }
            //            drawSquare(disp, wind, gContext, &square);
        }

        if(e.type == SDL_KEYUP) {
            //            if (square.visible) {
            // If 'a' is pressed, alternate colour
            //                if (e.xkey.keycode == 38) {
            //                    square.colour = square.colour == RedSquare ? BlueSquare : RedSquare;
            //                    XSetForeground(disp, gContext, square.colour == RedSquare ? red : blue);
            //                    drawSquare(disp, wind, gContext, &square);
            //                }
            //            }

        }
        if(e.type == SDL_MOUSEBUTTONDOWN) {
            SDL_CaptureMouse(SDL_TRUE);

            /*   if (isPointerInsideSquare(e.button.x, e.button.y, &square)) {
                // Set square properties
                square.selected = true;
                square.mouse_x = e.xbutton.x;
                square.mouse_y = e.xbutton.y*/;
            clickedStillInWindow = true;
            //                XSetForeground(disp, gContext, green);
            //                drawSquare(disp, wind, gContext, &square);
        }

        if(e.type == SDL_MOUSEBUTTONUP) {

            SDL_CaptureMouse(SDL_FALSE);
            if (xdndState.xdndExchangeStarted && xdndState.amISource && xdndState.xdndStatusReceived) {
                // Send XdndDrop message
                fprintf(stderr,"%s: sending XdndDrop to target window\n", procStr);
                sendXdndDrop(disp, wind, xdndState.otherWindow);
            }
            //            if (square.selected) {
            //                // Set square properties
            //                square.selected = false;
            //                XSetForeground(disp, gContext, square.colour == RedSquare ? red : blue);
            //                drawSquare(disp, wind, gContext, &square);
            //            }
        }
//        if(e.type == SDL_WINDOWEVENT) {
//            SDL_WindowEvent we = e.window;
//            if(we.event == SDL_WINDOWEVENT_ENTER) {
//                fprintf(stderr, "\n\n\n!!!ENTERR!!! %d***\n\n\n", we.windowID);

//                //                    if (square.selected) {
//                clickedStillInWindow = true;
//                //                    }
//            }
//            if(we.event == SDL_WINDOWEVENT_LEAVE) {
////                if( SDL_GetMouseState(0,0) != 0) {
////                    SDL_CaptureMouse(SDL_TRUE);
////                }
//                fprintf(stderr, "\n\n\n**LEAVE %d***\n\n\n", we.windowID);
////                if (square.selected) {
//                    clickedStillInWindow = false;
////                }
//            }

//        }
    }

    return 0;



}
