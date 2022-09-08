const std = @import("std");
// const sdl = @cImport(
// @cInclude("SDL.h") );
const sdlttf = @cImport(
@cInclude("SDL_ttf.h") );
const sdl = sdlttf; 
const SCREEN_WIDTH = 800;
const SCREEN_HEIGHT = 600;
const ComicSans = @embedFile("comic.ttf");

const tau = 6.283185307179586;


var freq : f64 = 440.0;
var have : sdl.SDL_AudioSpec = undefined;
export  fn MyAudioCallback(  userdata:?*anyopaque,
                       stream8:[*c]u8,
                       len:c_int) void
{    
    
    
    std.log.info("callback {}" ,.{len});
    const stream =  @ptrCast([*]i16, stream8);
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
            sdl.SDL_WINDOW_SHOWN
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
        const Sans = sdlttf.TTF_OpenFontRW(rw, 1, 24);
//        const Sans = sdlttf.TTF_OpenFont("C:\\Windows\\Fonts\\comic.ttf", 24);//C:\\Windows\\WinSxS\\amd64_microsoft-windows-font-truetype-corbel_31bf3856ad364e35_10.0.19041.1_none_889a9b699e1310d9\\corbell.ttf", 24);
        std.debug.print("{*}", .{Sans});

        // this is the color in rgb format,
        // maxing out all would give you the color white,
        // and it will be your text's color
        const White:sdl.SDL_Color = .{.r = 255, .g = 255, .b = 255, .a = 255};

        // as TTF_RenderText_Solid could only be used on
        // SDL_Surface then you have to create the surface first
        const surfaceMessage =
            sdlttf.TTF_RenderText_Solid(Sans, "put your text here", White); 

        // now you can convert it into a texture
        const Message = sdl.SDL_CreateTextureFromSurface(renderer, surfaceMessage);

        var Message_rect: sdl.SDL_Rect = .{.x = 50,  //controls the rect's x coordinate 
        .y = 50, // controls the rect's y coordinte
        .w = 500, // controls the width of the rect
        .h = 100}; // controls the height of the rect

        // (0,0) is on the top left of the window/screen,
        // think a rect as the text's box,
        // that way it would be very simple to understand

        // Now since it's a texture, you have to put RenderCopy
        // in your game loop area, the area where the whole code executes

        // you put the renderer's name first, the Message,
        // the crop size (you can ignore this if you don't want
        // to dabble with cropping), and the rect which is the size
        // and coordinate of your texture
        _ = sdl.SDL_RenderCopy(renderer, Message, 0, &Message_rect);

        // Don't forget to free your surface and texture
        sdl.SDL_FreeSurface(surfaceMessage);
        sdl.SDL_DestroyTexture(Message);
        
        
        sdl.SDL_RenderPresent(renderer);
        var e: sdl.SDL_Event = undefined;
        while(true) {
          _ = sdl.SDL_PollEvent(&e);
          
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
