FROM ubuntu:22.04

# Установка зависимостей
RUN apt-get update && apt-get install -y \
    cmake \
    build-essential \
    libboost-all-dev \
    libpq-dev \
    libpqxx-dev

WORKDIR /app
# Копируем исходный код
COPY ./server /app/server

# Сборка
WORKDIR /app/server
RUN mkdir build && cd build && cmake .. && make -j

# Копируем скомпилированный файл
RUN cp ./build/libhv-http /usr/local/bin/http-server

# Устанавливаем рабочую директорию
WORKDIR /usr/local/bin
CMD ["http-server"]