# SDL Game for iOS Web

A simple SDL-based game optimized for older iOS devices running in web browsers.

## Features
- Touch/mouse controls
- Optimized for older iOS Safari
- Simple Pong-like gameplay
- Responsive design

## Building

### Desktop Version
```bash
nix develop
make build
make run
```

### Web Version (iOS Optimized)
```bash
nix develop
make web
make serve
```

Then visit http://localhost:8000

## Controls
- Desktop: Move mouse to control paddle
- Mobile: Touch and drag to control paddle
- Hit the ball with the paddle to score points

## Compatibility
Tested on:
- iOS 10+ Safari
- Modern desktop browsers
- Chrome/Firefox on Android

## Development
The game uses SDL2 with Emscripten for web compilation. All optimizations target older iOS devices while maintaining compatibility with modern browsers.
