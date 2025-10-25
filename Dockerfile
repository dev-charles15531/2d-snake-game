# ──────────────────────────────────────────────
# Stage 1 – Build SFML 3 and your game (Ubuntu 24.04 | GLIBC 2.39)
# ──────────────────────────────────────────────
FROM ubuntu:24.04 AS build
ENV DEBIAN_FRONTEND=noninteractive

# Dependencies for SFML 3 and your game
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential cmake git wget ca-certificates \
    libglfw3-dev libglew-dev libopenal-dev libvorbis-dev libflac-dev \
    libx11-dev libxrandr-dev libxcursor-dev libxinerama-dev libxi-dev \
    libudev-dev libfreetype6-dev libgl1-mesa-dev libglu1-mesa-dev \
    libjpeg-dev libpng-dev libsndfile1-dev \
    && update-ca-certificates \
    && rm -rf /var/lib/apt/lists/*

# Build SFML 3 from source
WORKDIR /deps
RUN git clone --depth=1 -b 3.0.2 https://github.com/SFML/SFML.git SFML3 && \
    cmake -B SFML3/build -S SFML3 \
        -DWEB_BUILD=ON \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX=/usr/local \
        -DSFML_BUILD_AUDIO=ON \
        -DSFML_BUILD_GRAPHICS=ON \
        -DSFML_BUILD_WINDOW=ON \
        -DSFML_BUILD_NETWORK=ON \
        -DSFML_BUILD_SYSTEM=ON \
        -DSFML_USE_SYSTEM_DEPS=ON \
        -DBUILD_SHARED_LIBS=ON && \
    cmake --build SFML3/build -j$(nproc) && \
    cmake --install SFML3/build

# Build the game
WORKDIR /src
COPY . /src
RUN cmake -B build -S . -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=/usr/local && \
    cmake --build build -j$(nproc)


# ──────────────────────────────────────────────
# Stage 2 – Runtime (Debian bookworm-slim)
# ──────────────────────────────────────────────
FROM ubuntu:24.04 AS runtime
ENV DEBIAN_FRONTEND=noninteractive

# Minimal runtime + noVNC stack
RUN apt-get update && apt-get install -y --no-install-recommends \
    bash xvfb x11vnc fluxbox websockify procps ca-certificates git x11-utils \
    libgl1 libglu1-mesa libglew2.2 libglfw3 \
    libx11-6 libxcursor1 libxrandr2 libxinerama1 libxi6 \
    libopenal1 libvorbis0a libvorbisfile3 libvorbisenc2 libflac12 \
    && rm -rf /var/lib/apt/lists/*

# Install noVNC
WORKDIR /opt
RUN git clone --depth=1 https://github.com/novnc/noVNC.git && \
    git clone --depth=1 https://github.com/novnc/websockify /opt/noVNC/utils/websockify && \
    ln -s /opt/noVNC/vnc.html /opt/noVNC/index.html

# Copy SFML 3 libraries from build stage
COPY --from=build /usr/local/lib/ /usr/local/lib/
COPY --from=build /usr/local/include/ /usr/local/include/
RUN ldconfig

# Environment variables
ENV DISPLAY=:99 \
    RESOLUTION=1024x600 \
    VNC_PORT=5900 \
    NOVNC_PORT=8080 \
    LIBGL_ALWAYS_SOFTWARE=1 \
    GALLIUM_DRIVER=llvmpipe \
    LP_NUM_THREADS=2

# Copy the built game binary
WORKDIR /app/game
COPY --from=build /src/build/bin/my_game /app/game/my_game
COPY src/shaders /app/src/shaders
RUN chmod +x /app/game/my_game

# ──────────────────────────────────────────────
# Startup script – waits for Xvfb before starting Fluxbox + VNC
# ──────────────────────────────────────────────
RUN bash -c 'cat > /app/start.sh' <<'EOF'
#!/bin/bash
set -e
echo "🚀 Starting lightweight SFML 3 game container..."

# Remove stale X lock
if [ -f /tmp/.X99-lock ]; then
    echo "⚠️ Removing stale Xvfb lock..."
    rm -f /tmp/.X99-lock
fi

# Start Xvfb
Xvfb $DISPLAY -screen 0 ${RESOLUTION}x24 -ac +extension GLX +render -noreset &
XVFB_PID=$!

# Wait for display socket
for i in $(seq 1 20); do
    if xdpyinfo -display $DISPLAY >/dev/null 2>&1; then
        echo "✅ Xvfb is ready."
        break
    fi
    echo "⏳ Waiting for Xvfb ($i)..."
    sleep 0.5
done

if ! xdpyinfo -display $DISPLAY >/dev/null 2>&1; then
    echo "❌ Xvfb failed to start."
    exit 1
fi

# Start Fluxbox + VNC + noVNC
echo "Starting Fluxbox..."
fluxbox & sleep 1
echo "Starting x11vnc..."
x11vnc -display $DISPLAY -forever -shared -rfbport $VNC_PORT -nopw & sleep 1
echo "Starting noVNC..."
/opt/noVNC/utils/novnc_proxy --vnc localhost:$VNC_PORT --listen $NOVNC_PORT & sleep 2

echo "✅ Ready! Open http://localhost:$NOVNC_PORT/vnc.html"
cd /app/game
while true; do
  echo "🎮 Launching Snake Game..."
  ./my_game || echo "⚠️ Game exited unexpectedly."
  echo "🔄 Restarting Snake Game in 2 seconds..."
  sleep 2
done
EOF
RUN chmod +x /app/start.sh

# ──────────────────────────────────────────────
# Expose ports & start
# ──────────────────────────────────────────────
EXPOSE 8080 5900
CMD ["/app/start.sh"]
