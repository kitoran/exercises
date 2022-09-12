const std = @import("std");

pub fn build(b: *std.build.Builder) void {
    // Standard target options allows the person running `zig build` to choose
    // what target to build for. Here we do not override the defaults, which
    // means any target is allowed, and the default is native. Other options
    // for restricting supported target set are available.
    const target = b.standardTargetOptions(.{});

    // Standard release options allow the person running `zig build` to select
    // between Debug, ReleaseSafe, ReleaseFast, and ReleaseSmall.
    const mode = b.standardReleaseOptions();
    const sdl = b.addSharedLibrary("sdl", "/src/SDL2-2.24.0/x86_64-w64-mingw32", std.build.LibExeObjStep.SharedLibKind.unversioned);
    _=sdl;
    const exe = b.addExecutable("iteration", "src/main.zig");
    exe.linkLibC();
//    exe.addLibraryPath(/src/SDL2-2.24.0/x86_64-w64-mingw32");
    exe.addIncludePath("/src/SDL2-2.24.0/x86_64-w64-mingw32/include/SDL2");
    exe.addIncludePath("/src/SDL2_ttf-devel-2.20.1-VC/SDL2_ttf-2.20.1/include");
    exe.addLibraryPath("/src/SDL2_ttf-devel-2.20.1-VC/SDL2_ttf-2.20.1/lib/x64");
    exe.addLibraryPath("C:\\src\\iteration");
    exe.linkSystemLibrary("SDL2_ttf");
    exe.linkSystemLibrary("SDL2");
    //exe.linkSystemLibraryName("SDL2");
    //exe.linkLibrary("libSDL2.a");
    exe.setTarget(target);
    exe.setBuildMode(mode);
    exe.install();

    const run_cmd = exe.run();
    run_cmd.step.dependOn(b.getInstallStep());
    if (b.args) |args| {
        run_cmd.addArgs(args);
    }

    const run_step = b.step("run", "Run the app");
    run_step.dependOn(&run_cmd.step);

    const exe_tests = b.addTest("src/main.zig");
    exe_tests.setTarget(target);
    exe_tests.setBuildMode(mode);

    const test_step = b.step("test", "Run unit tests");
    test_step.dependOn(&exe_tests.step);
}