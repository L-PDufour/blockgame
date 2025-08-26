{
  description = "SDL3 C/C++ development environment with Emscripten WASM build";
  inputs.nixpkgs.url = "https://flakehub.com/f/NixOS/nixpkgs/0.1";
  outputs =
    inputs:
    let
      supportedSystems = [
        "x86_64-linux"
        "aarch64-linux"
        "x86_64-darwin"
        "aarch64-darwin"
      ];
      forEachSupportedSystem =
        f:
        inputs.nixpkgs.lib.genAttrs supportedSystems (
          system:
          f {
            pkgs = import inputs.nixpkgs { inherit system; };
          }
        );
    in
    {
      devShells = forEachSupportedSystem (
        { pkgs }:
        {
          default =
            pkgs.mkShell.override
              {
                # Override stdenv in order to change compiler:
                # stdenv = pkgs.clangStdenv;
              }
              {
                packages =
                  with pkgs;
                  [
                    clang-tools
                    cmake
                    codespell
                    conan
                    cppcheck
                    doxygen
                    gtest
                    lcov
                    vcpkg
                    vcpkg-tool
                    sdl3
                    sdl3-image
                    sdl3-ttf
                    # Build tools
                    entr
                    emscripten
                    # Web server for testing WASM
                    python3
                  ]
                  ++ (if system == "aarch64-darwin" then [ ] else [ gdb ]);

                shellHook = ''
                  echo "SDL3 + Emscripten development environment loaded!"
                  echo ""
                  echo "Available commands:"
                  echo "  nix run .#build-wasm    - Build WASM version"
                  echo "  nix run .#serve         - Serve WASM build locally"
                  echo "  nix run .#build-native  - Build native version"
                  echo ""
                '';
              };
        }
      );

      apps = forEachSupportedSystem (
        { pkgs }:
        {
          build-wasm = {
            type = "app";
            program = toString (pkgs.writeShellScript "build-wasm" ''
              set -e
              echo "Building WASM version with Emscripten..."

              # Create build directory
              mkdir -p build/wasm
              cd build/wasm

              # Use emscripten to build
              emcc ../../src/main.c \
                -o index.html \
                -s USE_SDL=3 \
                -s WASM=1 \
                -s ASYNCIFY \
                -s TOTAL_MEMORY=67108864 \
                --preload-file ../../assets@assets \
                -O2


              echo "✅ WASM build complete! Files in build/wasm/"
              echo "Run 'nix run .#serve' to test locally"
            '');
          };

          serve = {
            type = "app";
            program = toString (pkgs.writeShellScript "serve-wasm" ''
              set -e
              if [ ! -d "build/wasm" ] || [ ! -f "build/wasm/index.html" ]; then
                echo "❌ No WASM build found. Run 'nix run .#build-wasm' first"
                exit 1
              fi

              echo "🌐 Serving WASM build at http://localhost:8000"
              echo "Press Ctrl+C to stop"
              cd build/wasm
              ${pkgs.python3}/bin/python -m http.server 8080
            '');
          };

          build-native = {
            type = "app";
            program = toString (pkgs.writeShellScript "build-native" ''
              set -e
              echo "Building native version..."

              # Create build directory
              mkdir -p build/native
              cd build/native

              # Use cmake if CMakeLists.txt exists, otherwise direct compilation
              if [ -f "../../CMakeLists.txt" ]; then
                cmake ../.. -DCMAKE_BUILD_TYPE=Debug
                make -j$(nproc)
              else
                # Direct compilation
                clang src/main.c \
                  -lSDL3 -lSDL3_image -lSDL3_ttf \
                  -o game \
                  -g -O0
              fi

              echo "✅ Native build complete! Executable in build/native/"
            '');
          };
        }
      );

      # Optional: Package the WASM build
      packages = forEachSupportedSystem (
        { pkgs }:
        {
          wasm = pkgs.stdenv.mkDerivation {
            pname = "sdl3-wasm-game";
            version = "0.1.0";

            src = ./.;

            nativeBuildInputs = with pkgs; [
              emscripten
            ];

            buildPhase = ''
              mkdir -p $out/www

              emcc src/*.c \
                -o $out/www/index.html \
                -s USE_SDL=3 \
                -s WASM=1 \
                -s ASYNCIFY \
                -s TOTAL_MEMORY=67108864 \
                --preload-file assets@assets \
                -O2
            '';

            installPhase = ''
              # Files are already in $out/www from buildPhase
              echo "WASM build packaged in $out/www"
            '';
          };
        }
      );
    };
}
