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
COPY ./client /app/client

# Сборка
WORKDIR /app/client
RUN mkdir build && cd build && cmake .. && make -j

# Копируем скомпилированный файл
RUN cp ./build/libhv-http /usr/local/bin/http-client

# Устанавливаем рабочую директорию
WORKDIR /usr/local/bin
CMD ["http-client"]