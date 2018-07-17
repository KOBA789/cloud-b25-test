FROM alpine:3.8

WORKDIR /root

RUN apk add --no-cache \
    git \
    cmake \
    make \
    gcc \
    g++ \
    libc-dev \
    musl-dev \
    pcsc-lite-dev

COPY ./libarib25 /root/libarib25

RUN ln -s /usr/lib /usr/lib64 && \
    cd /root/libarib25 && \
    cmake . -DCMAKE_BUILD_TYPE=release -DCMAKE_INSTALL_PREFIX=/usr && \
    make install

COPY ./b25s.c /root/

RUN cd /root && \
    gcc -o b25s b25s.c -Wl,-dn -larib25 -Wl,-dy -lpcsclite

FROM alpine:3.8

RUN apk add --no-cache \
    pcsc-lite-libs \
    socat

COPY --from=0 /root/b25s /usr/bin/b25s
COPY b25.sh /root/

ENTRYPOINT ["/root/b25.sh"]
