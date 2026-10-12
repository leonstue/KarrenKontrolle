# Stage 1: Build

FROM ubuntu:24.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

# Install build dependencies
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    ninja-build \
    git \
    curl \
    zip \
    unzip \
    tar \
    pkg-config \
    python3 \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy project including Git metadata
COPY . .

# Initialize vcpkg submodule
RUN git submodule update --init --recursive \
    && ./external/vcpkg/bootstrap-vcpkg.sh -disableMetrics

# Configure CMake with vcpkg
RUN cmake -S . -B build \
    -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_TOOLCHAIN_FILE=/app/external/vcpkg/scripts/buildsystems/vcpkg.cmake

# Build executable
RUN cmake --build build --parallel 2











# Stage 2: Run

FROM ubuntu:24.04 AS run

# Install run dependencies
RUN apt-get update && apt-get install -y --no-install-recommends \
    libstdc++6 \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy compiled executable
COPY --from=builder /app/build/karrenkontrolle /app/karrenkontrolle


# Document default API port
EXPOSE 8080

# Start application and forward CLI arguments
ENTRYPOINT ["/app/karrenkontrolle", "--ip", "0.0.0.0", "--port", "8080"]