FROM gcc:13
WORKDIR /app
COPY . .
RUN apt-get update && apt-get install -y cmake
RUN mkdir build && cd build && cmake .. && cmake --build .
