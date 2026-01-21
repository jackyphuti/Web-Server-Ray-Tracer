# C++ Projects

Two comprehensive C++ projects demonstrating advanced programming concepts.

## Project 1: Web Server

A high-performance multi-threaded HTTP/HTTPS server with advanced features.

**Key Features:**
- **HTTP/1.1 Protocol Support** - Full request parsing and response generation
- **HTTPS/TLS Ready** - OpenSSL integration for secure connections
- **Multi-threaded** - Thread pool for handling concurrent connections
- **HTTP Methods** - GET, POST, PUT, DELETE, HEAD, OPTIONS
- **Response Caching** - Built-in cache with TTL support
- **REST API** - JSON responses for `/api/data`, `/api/submit`, `/api/update`, `/api/remove`
- **Smart Routing** - Dedicated handlers for different endpoints
- **Error Handling** - Graceful error responses with proper HTTP status codes

**Building:**
```bash
cd /home/jacky-mpoka/Documents/new/server
mkdir -p build
cd build
cmake ..
make web_server
```

**Running:**
```bash
./web_server 8080
```

Then visit `http://localhost:8080` in your browser.

**API Examples:**
```bash
# GET request
curl http://localhost:8080/

# GET JSON
curl http://localhost:8080/api/data

# POST request
curl -X POST -d "key=value" http://localhost:8080/api/submit

# PUT request
curl -X PUT -d "update=data" http://localhost:8080/api/update

# DELETE request
curl -X DELETE http://localhost:8080/api/remove

# OPTIONS request
curl -X OPTIONS http://localhost:8080/
```

## Project 2: Ray Tracer

A high-performance 3D graphics renderer with advanced ray tracing techniques.

**Key Features:**
- **Advanced Lighting Model** - Ambient, diffuse, and specular shading (Blinn-Phong)
- **Reflections** - Recursive ray tracing for mirror-like surfaces
- **Soft Shadows** - Area lights with multi-sampling for realistic shadows
- **Textures** - Procedural textures (solid, checkerboard, gradient)
- **Materials** - Customizable material properties (ambient, diffuse, specular, reflection)
- **Anti-aliasing** - Multi-sampling per pixel
- **Parallelization** - Multi-threaded rendering using all CPU cores
- **PPM Output** - Standard image format with gamma correction
- **Performance** - Hardware-aware thread count detection

**Building:**
```bash
cd /home/jacky-mpoka/Documents/new/server
mkdir -p build
cd build
cmake ..
make ray_tracer
```

**Running:**
```bash
# Basic rendering (800x600, 10 samples, auto-detect CPU threads)
./ray_tracer

# Custom resolution and samples
./ray_tracer 1024 768 20

# Custom thread count
./ray_tracer 800 600 10 8
```

**Scene Description:**
The default scene includes:
- Checkerboard ground plane (reflective)
- Red matte sphere
- Green reflective sphere
- Blue highly reflective sphere
- Mirror sphere (95% reflective)
- 2 area lights with soft shadow sampling

**Output:**
- `output.ppm` - Portable Pixmap format image
- Convert to PNG using ImageMagick: `convert output.ppm output.png`

## Project Structure

```
.
├── CMakeLists.txt              # Root CMake configuration
├── web_server/                 # HTTP Server project
│   ├── CMakeLists.txt
│   ├── include/
│   │   ├── server.h            # HTTP server
│   │   ├── request_handler.h   # Request parsing & routing
│   │   ├── thread_pool.h       # Thread pool implementation
│   │   └── cache.h             # Response caching with TTL
│   └── src/
│       ├── main.cpp
│       ├── server.cpp
│       ├── request_handler.cpp
│       ├── thread_pool.cpp
│       └── cache.cpp
├── ray_tracer/                 # Ray Tracer project
│   ├── CMakeLists.txt
│   ├── include/
│   │   ├── ray_tracer.h        # Main rendering engine
│   │   ├── vector3.h           # 3D vector math
│   │   ├── scene.h             # Scene definition
│   │   ├── texture.h           # Texture support
│   │   └── ppm_writer.h        # Image output
│   └── src/
│       ├── main.cpp
│       ├── ray_tracer.cpp      # Advanced rendering with reflections/soft shadows
│       ├── vector3.cpp
│       ├── scene.cpp
│       ├── texture.cpp
│       └── ppm_writer.cpp
├── build/                      # Build output directory
└── include/                    # Shared headers
```

## Requirements

- **C++17 or later**
- **CMake 3.16+**
- **POSIX-compliant system** (Linux/macOS)
- **OpenSSL** (for HTTPS support in web server)
- **pthread** (usually included)

### Ubuntu/Debian Installation
```bash
sudo apt-get install build-essential cmake libssl-dev
```

### macOS Installation
```bash
brew install cmake openssl
```

## Technologies & Concepts

### Web Server
- Socket programming (Berkeley sockets)
- HTTP protocol implementation
- Thread pool design patterns
- Cache management with TTL
- OpenSSL/TLS integration
- Request/response parsing
- RESTful API design

### Ray Tracer
- Linear algebra (vectors, dot/cross products)
- 3D graphics algorithms
- Ray-sphere intersection testing
- Phong/Blinn-Phong shading models
- Reflection calculations
- Soft shadow implementation via area lights
- Procedural texture generation
- Multi-threaded rendering
- Gamma correction
- Anti-aliasing techniques

## Advanced Features

### Web Server Features
1. **Caching System**
   - Automatic TTL-based expiration
   - Thread-safe operations
   - Per-endpoint cache policies

2. **HTTP Method Support**
   - Full GET with routing
   - POST with request body parsing
   - PUT for updates
   - DELETE for resource removal
   - HEAD for headers-only responses
   - OPTIONS for CORS support

3. **HTTPS Ready**
   - OpenSSL context initialization
   - TLS 1.2/1.3 support (configure with certificates)
   - Production-ready architecture

### Ray Tracer Features
1. **Advanced Shading**
   - Ambient component (base lighting)
   - Diffuse component (matte surfaces)
   - Specular component (shininess)
   - Multiple light sources

2. **Soft Shadows**
   - Area light implementation
   - Multi-sample shadow calculation
   - Realistic penumbra effect

3. **Reflections**
   - Recursive ray tracing
   - Configurable reflection depth
   - Per-material reflection strength

4. **Textures**
   - Checkerboard pattern
   - Gradient mapping
   - Extensible texture system

5. **Performance**
   - Automatic CPU detection
   - Configurable thread count
   - Efficient parallel rendering

## Performance Tips

### Web Server
- Increase thread pool size for high concurrency: Modify `ThreadPool(4)` in `server.cpp`
- Adjust cache TTL based on content update frequency
- Enable HTTPS for production deployments

### Ray Tracer
- Use fewer samples per pixel for faster preview: `./ray_tracer 800 600 5`
- Reduce resolution for quick tests: `./ray_tracer 400 300 10`
- Increase threads for better utilization: `./ray_tracer 800 600 10 16`
- Lower reflection depth if performance is critical: `tracer.set_max_depth(2)`

## Development Notes

Both projects follow modern C++ practices:
- Header files in `include/` directories
- Implementation in `src/` directories
- Clear separation of concerns
- Exception-safe code
- Efficient resource management with smart pointers
- Thread-safe operations where needed
- RAII (Resource Acquisition Is Initialization) pattern
- Const-correctness throughout

## Future Enhancements

### Web Server
- Connection pooling for persistent HTTP/1.1 connections
- Gzip compression
- Static file serving with MIME types
- Session management
- WebSocket support
- Rate limiting

### Ray Tracer
- BVH acceleration structure for faster intersection testing
- Triangle mesh support
- Normal mapping
- Environment mapping
- Importance sampling for better soft shadows
- CUDA/GPU acceleration
- Tone mapping and post-processing
- Depth-of-field effects
