{
  description = "Raylib development environment";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
      ...
    }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = import nixpkgs { inherit system; };
        toolchain = with pkgs; [
          libGL
          clang-tools
          clang
          raylib
          wayland-scanner
          libxkbcommon
          wayland
          glfw-wayland
          gnumake
          cmake
          zig
        ];
      in
      {
        devShells.default = pkgs.mkShell {
          buildInputs = toolchain ++ [
          ];
          LD_LIBRARY_PATH = pkgs.lib.makeLibraryPath (
            with pkgs;
            [
              wayland
              emscripten
              libxkbcommon
              # any other libraries that need to be dynamically linked to
            ]
          );
          # Environment variables and shell hooks
          shellHook = '''';
        };
      }
    );
}
