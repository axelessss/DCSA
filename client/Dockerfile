FROM ubuntu:22.04

RUN RUN apt-get update && \
    apt-get install -y \
        libboost-all-dev \
        libpq-dev \
        libpqxx-dev \
        cmake \
        build-essential


COPY ./client/build/libhv-http /usr/local/bin/client

WORKDIR /usr/local/bin/

EXPOSE 7777

CMD ["client"]
