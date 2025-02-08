{
  description = "Raylib development environment";
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };
  outputs = { self, nixpkgs, flake-utils, ... }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };
        toolchain = with pkgs; [
          libGL
          clang-tools
          clang
          raylib
          wayland
          wayland-scanner
          gnumake
          emscripten
          python3
        ];
      in
      {
        devShells.default = pkgs.mkShell {
          buildInputs = toolchain;
      C_INCLUDE_PATH = "${pkgs.emscripten}/share/emscripten/cache/sysroot/include/";
          LD_LIBRARY_PATH = pkgs.lib.makeLibraryPath (with pkgs; [
            wayland
            emscripten
            libxkbcommon
          ]);

        };
      }
    );
}