const std = @import("std");
// const sdl = @cImport(
// @cInclude("SDL.h") );
const sdlttf = @cImport(
@cInclude("SDL_ttf.h") );
//usingnamespace @cImport(
//@cInclude("SDL_ttf.h") );
//const gpa: std.heap.GeneralPurposeAllocator(.{}){};
const allocator = std.heap.page_allocator;
const sdl = sdlttf; 
const SCREEN_WIDTH = 800;
const SCREEN_HEIGHT = 600;
const ComicSans = @embedFile("comic.ttf");
var windowWidth :i32= SCREEN_WIDTH;
var windowHeight :i32= SCREEN_HEIGHT;
const tau = 6.283185307179586;
var Sans : ?*sdl.TTF_Font = undefined;
var textTextures = std.StringHashMap(*sdl.SDL_Texture).init(allocator);
fn drawText(text:[*c]const u8, renderer: ?*sdl.SDL_Renderer,  x:i32, y:i32) !void {
  std.log.info("drawText {s}" ,.{text});
  var zigs = std.mem.span(text);
  var value = textTextures.get(zigs);
  if (value == null) {
    const White:sdl.SDL_Color = .{.r = 255, .g = 255, .b = 255, .a = 255};
    const surfaceMessage =
            sdlttf.TTF_RenderUTF8_Solid(Sans, text, White); 
    const textext = sdl.SDL_CreateTextureFromSurface(renderer, surfaceMessage).?;
    try textTextures.put(zigs, textext);
    value = textext;
    sdl.SDL_FreeSurface(surfaceMessage);
  }  
  const texture = value.?;
  var w : c_int=undefined; var h : c_int=undefined ;
  _ = sdl.SDL_QueryTexture(texture, null, null, &w, &h);
  var Message_rect: sdl.SDL_Rect = .{.x = x,  //controls the rect's x coordinate 
                                    .y = y, // controls the rect's y coordinte
                                    .w = w, // controls the width of the rect
                                    .h = h}; // controls the height of the rect
  // (0,0) is on the top left of the window/screen,
  // think a rect as the text's box,
  // that way it would be very simple to understand

  // Now since it's a texture, you have to put RenderCopy
  // in your game loop area, the area where the whole code executes

  // you put the renderer's name first, the Message,
  // the crop size (you can ignore this if you don't want
  // to dabble with cropping), and the rect which is the size
  // and coordinate of your texture
  _ = sdl.SDL_RenderCopy(renderer, texture, 0, &Message_rect);

}


var freq : f64 = 440.0;
var have : sdl.SDL_AudioSpec = undefined;
export  fn MyAudioCallback(  userdata:?*anyopaque,
                       stream8:[*c]u8,
                       len:c_int) void
{    
    
    
  //  std.log.info("callback {}" ,.{len});
    const stream =  @ptrCast([*]i16, @alignCast(2, stream8));
     _ = userdata;
    const ph = struct {
        var ase: i32 = 0;
    };
    var i : u32 = 0;
    while(i <  @divFloor(len, 2)) {
      stream[i] = @floatToInt(i16,@sin(@intToFloat(f64 , ph.ase) * freq * tau / @intToFloat(f64 ,have.freq))*3000);
      ph.ase = ph.ase+1;
      i+=1;
    } 
    
}


pub fn audio() anyerror!void {
  var want:sdl.SDL_AudioSpec = std.mem.zeroes(sdl.SDL_AudioSpec );
  //var dev: sdl.SDL_AudioDeviceID ;

  // inline for (want) |i| {
        // i = 0;
    // }
//  @memset(&want, 0, sizeof(want)); /* or SDL_zero(want) */
  want.freq = 44100;
  want.format = sdl.AUDIO_S16;
  want.channels = 1;
  want.samples = 4096;
  want.callback = MyAudioCallback;  // you wrote this function elsewhere.
//  var dev: sdl.SDL_AudioDeviceID  = sdl.SDL_OpenAudioDevice(null, 0,  SDL_AUDIO_ALLOW_FORMAT_CHANGE);


  const ret =  sdl.SDL_OpenAudioDevice(
                          null,
                          sdl.SDL_FALSE,
                          &want, &have,
                          0);
  std.log.info("returend {} {}", .{ret, have});
  
  sdl.SDL_PauseAudioDevice(ret, 0);
}

pub fn main() anyerror!void {
    // Note that info level log messages are by default printed only in Debug
    // and ReleaseSafe build modes.
    std.log.info("All your codebase are belong to us." ,.{});
    if (sdl.SDL_Init( sdl.SDL_INIT_VIDEO | sdl.SDL_INIT_AUDIO  ) < 0) {
        std.log.info("SDL could not initialize! SDL_Error: {s} \n", .{sdl.SDL_GetError()});
    } else {
        try audio();
        _ = sdl.SDL_SetHint( sdl.SDL_HINT_RENDER_SCALE_QUALITY, "2" );
        const wind = sdl.SDL_CreateWindow(
            "SDL2 Demo",
            sdl.SDL_WINDOWPOS_CENTERED, sdl.SDL_WINDOWPOS_CENTERED,
            SCREEN_WIDTH, SCREEN_HEIGHT,
            sdl.SDL_WINDOW_SHOWN | sdl.SDL_WINDOW_RESIZABLE
        );        _ = sdl.SDL_SetHint( sdl.SDL_HINT_RENDER_SCALE_QUALITY, "2" );

        const renderer = sdl.SDL_CreateRenderer(wind, -1,  0);
        _ = sdl.SDL_SetHint( sdl.SDL_HINT_RENDER_SCALE_QUALITY, "2" );
        _ = sdl.SDL_SetRenderDrawColor(renderer, 255,255,255, 255);
        _ = sdl.SDL_SetHint( sdl.SDL_HINT_RENDER_SCALE_QUALITY, "2" );
        _ = sdl.SDL_RenderDrawLineF(renderer,
                        10.5, 10.5, 105.5, 30.5);
//        SDL_RenderClear(renderer);
//        SDL_RenderCopy(renderer, bitmapTex, NULL, NULL);
        _ = sdl.SDL_SetHint( sdl.SDL_HINT_RENDER_SCALE_QUALITY, "2" );
        
        _ = sdlttf.TTF_Init();
        //this opens a font style and sets a size
        
        const rw = sdl.SDL_RWFromConstMem(ComicSans, ComicSans.len);
        Sans = sdlttf.TTF_OpenFontRW(rw, 1, 24);
//        const Sans = sdlttf.TTF_OpenFont("C:\\Windows\\Fonts\\comic.ttf", 24);//C:\\Windows\\WinSxS\\amd64_microsoft-windows-font-truetype-corbel_31bf3856ad364e35_10.0.19041.1_none_889a9b699e1310d9\\corbell.ttf", 24);
        std.debug.print("{*}", .{Sans});
        try drawText(@as([*c]const u8, "440"), renderer, 10, 10);
        try drawText("540", renderer, 10, 30);
  
        sdl.SDL_RenderPresent(renderer);
        var e: sdl.SDL_Event = undefined;
        while(true) {
          var pollres   = sdl.SDL_WaitEvent(&e);
          std.log.info("got event {x} res {}" ,.{e.type, pollres});
          if(e.type == sdl.SDL_WINDOWEVENT) {
            if(e.window.event == sdl.SDL_WINDOWEVENT_RESIZED  or e.window.event == sdl.SDL_WINDOWEVENT_SIZE_CHANGED) {
            std.log.info("resizeevent" ,.{});
            windowWidth = e.window.data1;
            windowHeight = e.window.data2;
            
            
            try drawText(@as([*c]const u8, "440"), renderer, 10, @divFloor(windowHeight, 2));
            // SDL_Log("Window %d resized to %dx%d",
                // event->window.windowID, ,
                // );
                
        sdl.SDL_RenderPresent(renderer);
            }
          }
          
          if(e.type == sdl.SDL_MOUSEMOTION) {
            freq = @intToFloat(f64, e.motion.y) + 140.0;
            std.log.info("motion {} {}", .{e.motion.y, @intToFloat(f64, e.motion.y) + 140.0});
          }
//          std.log.info("{}", .{@intToEnum(sdl.SDL_EventType, e.type)});
        // std.debug.print("{}", .{sdl.SDL_POLLSENTINEL          });
          if(e.type == sdl.SDL_QUIT) {
            return;
          }
        }
        sdl.SDL_Delay(2000);
    }
    
}


        
test "basic test" {
    try std.testing.expectEqual(10, 3 + 7);
}
